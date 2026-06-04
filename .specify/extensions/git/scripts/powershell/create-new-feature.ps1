#!/usr/bin/env pwsh
# Git extension: create-new-feature.ps1
# Adapted from core scripts/powershell/create-new-feature.ps1 for extension layout.
# Sources common.ps1 from the project's installed scripts, falling back to
# git-common.ps1 for minimal git helpers.
[CmdletBinding()]
param(
    [switch]$Json,
    [switch]$AllowExistingBranch,
    [switch]$DryRun,
    [string]$ShortName,
    [Parameter()]
    [long]$Number = 0,
    [switch]$Timestamp,
    [switch]$Help,
    [Parameter(Position = 0, ValueFromRemainingArguments = $true)]
    [string[]]$FeatureDescription
)
$ErrorActionPreference = 'Stop'

if ($Help) {
    Write-Host "Usage: ./create-new-feature.ps1 [-Json] [-DryRun] [-AllowExistingBranch] [-ShortName <name>] [-Number N] [-Timestamp] <feature description>"
    Write-Host ""
    Write-Host "Options:"
    Write-Host "  -Json               Output in JSON format"
    Write-Host "  -DryRun             Compute branch name without creating the branch"
    Write-Host "  -AllowExistingBranch  Switch to branch if it already exists instead of failing"
    Write-Host "  -ShortName <name>   Provide a custom short name (2-4 words) for the branch"
    Write-Host "  -Number N           Specify branch number manually (overrides auto-detection)"
    Write-Host "  -Timestamp          Use timestamp prefix (YYYYMMDD-HHMMSS) instead of sequential numbering"
    Write-Host "  -Help               Show this help message"
    Write-Host ""
    Write-Host "Environment variables:"
    Write-Host "  GIT_BRANCH_NAME     Use this exact branch name, bypassing all prefix/suffix generation"
    Write-Host ""
    exit 0
}

if (-not $FeatureDescription -or $FeatureDescription.Count -eq 0) {
    Write-Error "Usage: ./create-new-feature.ps1 [-Json] [-DryRun] [-AllowExistingBranch] [-ShortName <name>] [-Number N] [-Timestamp] <feature description>"
    exit 1
}

$featureDesc = ($FeatureDescription -join ' ').Trim()

if ([string]::IsNullOrWhiteSpace($featureDesc)) {
    Write-Error "Error: Feature description cannot be empty or contain only whitespace"
    exit 1
}

function Get-HighestNumberFromSpecs {
    param([string]$SpecsDir)

    [long]$highest = 0
    if (Test-Path $SpecsDir) {
        Get-ChildItem -Path $SpecsDir -Directory | ForEach-Object {
            if ($_.Name -match '^(\d{3,})-' -and $_.Name -notmatch '^\d{8}-\d{6}-') {
                [long]$num = 0
                if ([long]::TryParse($matches[1], [ref]$num) -and $num -gt $highest) {
                    $highest = $num
                }
            }
        }
    }
    return $highest
}

function Get-HighestNumberFromNames {
    param([string[]]$Names)

    [long]$highest = 0
    foreach ($name in $Names) {
        if ($name -match '(^|/)(\d{3,})-' -and $name -notmatch '(^|/)\d{8}-\d{6}-') {
            [long]$num = 0
            if ([long]::TryParse($matches[2], [ref]$num) -and $num -gt $highest) {
                $highest = $num
            }
        }
    }
    return $highest
}

function Get-HighestNumberFromBranches {
    param()

    try {
        $branches = git branch -a 2>$null
        if ($LASTEXITCODE -eq 0 -and $branches) {
            $cleanNames = $branches | ForEach-Object {
                $_.Trim() -replace '^\*?\s+', '' -replace '^remotes/[^/]+/', ''
            }
            return Get-HighestNumberFromNames -Names $cleanNames
        }
    } catch {
        Write-Verbose "Could not check Git branches: $_"
    }
    return 0
}

function Get-HighestNumberFromRemoteRefs {
    [long]$highest = 0
    try {
        $remotes = git remote 2>$null
        if ($remotes) {
            foreach ($remote in $remotes) {
                $env:GIT_TERMINAL_PROMPT = '0'
                $refs = git ls-remote --heads $remote 2>$null
                $env:GIT_TERMINAL_PROMPT = $null
                if ($LASTEXITCODE -eq 0 -and $refs) {
                    $refNames = $refs | ForEach-Object {
                        if ($_ -match 'refs/heads/(.+)$') { $matches[1] }
                    } | Where-Object { $_ }
                    $remoteHighest = Get-HighestNumberFromNames -Names $refNames
                    if ($remoteHighest -gt $highest) { $highest = $remoteHighest }
                }
            }
        }
    } catch {
        Write-Verbose "Could not query remote refs: $_"
    }
    return $highest
}

function Get-NextBranchNumber {
    param(
        [string]$SpecsDir,
        [switch]$SkipFetch
    )

    if ($SkipFetch) {
        $highestBranch = Get-HighestNumberFromBranches
        $highestRemote = Get-HighestNumberFromRemoteRefs
        $highestBranch = [Math]::Max($highestBranch, $highestRemote)
    } else {
        try {
            git fetch --all --prune 2>$null | Out-Null
        } catch { }
        $highestBranch = Get-HighestNumberFromBranches
    }

    $highestSpec = Get-HighestNumberFromSpecs -SpecsDir $SpecsDir
    $maxNum = [Math]::Max($highestBranch, $highestSpec)
    return $maxNum + 1
}

function ConvertTo-CleanBranchName {
    param([string]$Name)
    return $Name.ToLower() -replace '[^a-z0-9]', '-' -replace '-{2,}', '-' -replace '^-', '' -replace '-$', ''
}

function Get-ConfigScalar {
    param(
        [string]$Path,
        [string]$Key
    )

    if (-not (Test-Path $Path)) {
        return $null
    }

    $match = Select-String -Path $Path -Pattern "^\s*$([regex]::Escape($Key))\s*:\s*`"?([^`"]+)`"?\s*$" | Select-Object -First 1
    if ($match) {
        return $match.Matches[0].Groups[1].Value
    }

    return $null
}

function Get-ConfigMapValue {
    param(
        [string]$Path,
        [string]$Section,
        [string]$Key
    )

    if (-not (Test-Path $Path)) {
        return $null
    }

    $lines = Get-Content $Path
    $inSection = $false

    foreach ($line in $lines) {
        if ($line -match "^\s*$([regex]::Escape($Section))\s*:\s*$") {
            $inSection = $true
            continue
        }

        if ($inSection -and $line -match '^\S') {
            break
        }

        if ($inSection -and $line -match "^\s+$([regex]::Escape($Key))\s*:\s*`"?([^`"]+)`"?\s*$") {
            return $matches[1]
        }
    }

    return $null
}

function ConvertTo-ConventionToken {
    param(
        [string]$Value,
        [string]$Separator,
        [bool]$Lowercase
    )

    if ($null -eq $Value) {
        return ''
    }

    $normalized = $Value
    if ($Lowercase) {
        $normalized = $normalized.ToLower()
    }

    $escapedSeparator = [regex]::Escape($Separator)
    $normalized = $normalized -replace '[^A-Za-z0-9]', $Separator
    $normalized = $normalized -replace "$escapedSeparator{2,}", $Separator
    $normalized = $normalized -replace "^$escapedSeparator+", ''
    $normalized = $normalized -replace "$escapedSeparator+$", ''

    return $normalized
}

function ConvertTo-StrftimeDateFormat {
    param([string]$Format)

    $resolved = if ([string]::IsNullOrWhiteSpace($Format)) { 'YYYYMMDD' } else { $Format }
    $resolved = $resolved.Replace('YYYY', 'yyyy')
    $resolved = $resolved.Replace('DD', 'dd')
    $resolved = $resolved.Replace('HH', 'HH')
    $resolved = $resolved.Replace('mm', 'mm')
    $resolved = $resolved.Replace('ss', 'ss')

    return $resolved
}

function Get-TicketValue {
    param(
        [string]$Description,
        [string]$TicketPattern
    )

    if ([string]::IsNullOrWhiteSpace($TicketPattern)) {
        return $null
    }

    $match = [regex]::Match($Description, $TicketPattern)
    if ($match.Success) {
        return $match.Value
    }

    return $null
}

function New-BranchConventionConfig {
    param([string]$Path)

    if (-not (Test-Path $Path)) {
        return $null
    }

    $branchPattern = Get-ConfigScalar -Path $Path -Key 'branch_pattern'
    if ([string]::IsNullOrWhiteSpace($branchPattern)) {
        return $null
    }

    $defaultType = Get-ConfigScalar -Path $Path -Key 'default_type'
    if ([string]::IsNullOrWhiteSpace($defaultType)) {
        $defaultType = 'feature'
    }

    $typeValue = Get-ConfigMapValue -Path $Path -Section 'type_prefix' -Key $defaultType
    if ([string]::IsNullOrWhiteSpace($typeValue)) {
        $typeValue = $defaultType
    }

    $separator = Get-ConfigScalar -Path $Path -Key 'separator'
    if ([string]::IsNullOrWhiteSpace($separator)) {
        $separator = '-'
    }

    $seqPadding = Get-ConfigScalar -Path $Path -Key 'seq_padding'
    [int]$resolvedSeqPadding = 3
    if (-not [string]::IsNullOrWhiteSpace($seqPadding)) {
        [int]::TryParse($seqPadding, [ref]$resolvedSeqPadding) | Out-Null
    }

    $lowercaseValue = Get-ConfigScalar -Path $Path -Key 'lowercase'
    $lowercase = $true
    if (-not [string]::IsNullOrWhiteSpace($lowercaseValue)) {
        [bool]::TryParse($lowercaseValue, [ref]$lowercase) | Out-Null
    }

    $maxLengthValue = Get-ConfigScalar -Path $Path -Key 'max_length'
    [int]$resolvedMaxLength = 0
    if (-not [string]::IsNullOrWhiteSpace($maxLengthValue)) {
        [int]::TryParse($maxLengthValue, [ref]$resolvedMaxLength) | Out-Null
    }

    [PSCustomObject]@{
        BranchPattern = $branchPattern
        SeqPadding = $resolvedSeqPadding
        DateFormat = Get-ConfigScalar -Path $Path -Key 'date_format'
        Separator = $separator
        Lowercase = $lowercase
        DefaultType = $defaultType
        TypeValue = $typeValue
        TicketPattern = Get-ConfigScalar -Path $Path -Key 'ticket_pattern'
        MaxLength = $resolvedMaxLength
    }
}

# ---------------------------------------------------------------------------
# Source common.ps1 from the project's installed scripts.
# Search locations in priority order:
#  1. .specify/scripts/powershell/common.ps1 under the project root
#  2. scripts/powershell/common.ps1 under the project root (source checkout)
#  3. git-common.ps1 next to this script (minimal fallback)
# ---------------------------------------------------------------------------
function Find-ProjectRoot {
    param([string]$StartDir)
    $current = Resolve-Path $StartDir
    while ($true) {
        foreach ($marker in @('.specify', '.git')) {
            if (Test-Path (Join-Path $current $marker)) {
                return $current
            }
        }
        $parent = Split-Path $current -Parent
        if ($parent -eq $current) { return $null }
        $current = $parent
    }
}

$projectRoot = Find-ProjectRoot -StartDir $PSScriptRoot
$commonLoaded = $false

if ($projectRoot) {
    $candidates = @(
        (Join-Path $projectRoot ".specify/scripts/powershell/common.ps1"),
        (Join-Path $projectRoot "scripts/powershell/common.ps1")
    )
    foreach ($candidate in $candidates) {
        if (Test-Path $candidate) {
            . $candidate
            $commonLoaded = $true
            break
        }
    }
}

if (-not $commonLoaded -and (Test-Path "$PSScriptRoot/git-common.ps1")) {
    . "$PSScriptRoot/git-common.ps1"
    $commonLoaded = $true
}

if (-not $commonLoaded) {
    throw "Unable to locate common script file. Please ensure the Specify core scripts are installed."
}

# Resolve repository root
if (Get-Command Get-RepoRoot -ErrorAction SilentlyContinue) {
    $repoRoot = Get-RepoRoot
} elseif ($projectRoot) {
    $repoRoot = $projectRoot
} else {
    throw "Could not determine repository root."
}

# Check if git is available
if (Get-Command Test-HasGit -ErrorAction SilentlyContinue) {
    # Call without parameters for compatibility with core common.ps1 (no -RepoRoot param)
    # and git-common.ps1 (has -RepoRoot param with default).
    $hasGit = Test-HasGit
} else {
    try {
        git -C $repoRoot rev-parse --is-inside-work-tree 2>$null | Out-Null
        $hasGit = ($LASTEXITCODE -eq 0)
    } catch {
        $hasGit = $false
    }
}

Set-Location $repoRoot

$specsDir = Join-Path $repoRoot 'specs'
$branchConventionFile = Join-Path $repoRoot '.specify/branch-convention.yml'
$branchConvention = New-BranchConventionConfig -Path $branchConventionFile

function Get-BranchName {
    param([string]$Description)

    $stopWords = @(
        'i', 'a', 'an', 'the', 'to', 'for', 'of', 'in', 'on', 'at', 'by', 'with', 'from',
        'is', 'are', 'was', 'were', 'be', 'been', 'being', 'have', 'has', 'had',
        'do', 'does', 'did', 'will', 'would', 'should', 'could', 'can', 'may', 'might', 'must', 'shall',
        'this', 'that', 'these', 'those', 'my', 'your', 'our', 'their',
        'want', 'need', 'add', 'get', 'set'
    )

    $cleanName = $Description.ToLower() -replace '[^a-z0-9\s]', ' '
    $words = $cleanName -split '\s+' | Where-Object { $_ }

    $meaningfulWords = @()
    foreach ($word in $words) {
        if ($stopWords -contains $word) { continue }
        if ($word.Length -ge 3) {
            $meaningfulWords += $word
        } elseif ($Description -match "\b$($word.ToUpper())\b") {
            $meaningfulWords += $word
        }
    }

    if ($meaningfulWords.Count -gt 0) {
        $maxWords = if ($meaningfulWords.Count -eq 4) { 4 } else { 3 }
        $result = ($meaningfulWords | Select-Object -First $maxWords) -join '-'
        return $result
    } else {
        $result = ConvertTo-CleanBranchName -Name $Description
        $fallbackWords = ($result -split '-') | Where-Object { $_ } | Select-Object -First 3
        return [string]::Join('-', $fallbackWords)
    }
}

function New-ConventionBranchName {
    param(
        [string]$Pattern,
        [string]$FeatureNum,
        [string]$DateValue,
        [string]$TicketValue,
        [string]$TypeValue,
        [string]$SuffixSource,
        [string]$Description,
        [string]$Separator,
        [bool]$Lowercase
    )

    $branchName = $Pattern
    $kebabValue = ConvertTo-ConventionToken -Value $SuffixSource -Separator $Separator -Lowercase $Lowercase
    $summaryValue = ConvertTo-ConventionToken -Value $Description -Separator $Separator -Lowercase $Lowercase
    $resolvedType = ConvertTo-ConventionToken -Value $TypeValue -Separator $Separator -Lowercase $Lowercase

    $branchName = $branchName.Replace('{seq}', $FeatureNum)
    $branchName = $branchName.Replace('{date}', $DateValue)
    $branchName = $branchName.Replace('{ticket}', $TicketValue)
    $branchName = $branchName.Replace('{type}', $resolvedType)
    $branchName = $branchName.Replace('{kebab}', $kebabValue)
    $branchName = $branchName.Replace('{summary}', $summaryValue)

    return $branchName
}

# Check for GIT_BRANCH_NAME env var override (exact branch name, no prefix/suffix)
if ($env:GIT_BRANCH_NAME) {
    $branchName = $env:GIT_BRANCH_NAME
    # Check 244-byte limit (UTF-8) for override names
    $branchNameUtf8ByteCount = [System.Text.Encoding]::UTF8.GetByteCount($branchName)
    if ($branchNameUtf8ByteCount -gt 244) {
        throw "GIT_BRANCH_NAME must be 244 bytes or fewer in UTF-8. Provided value is $branchNameUtf8ByteCount bytes; please supply a shorter override branch name."
    }
    # Extract FEATURE_NUM from the branch name if it starts with a numeric prefix
    # Check timestamp pattern first (YYYYMMDD-HHMMSS-) since it also matches the simpler ^\d+ pattern
    if ($branchName -match '(^|/)(\d{8}-\d{6})-') {
        $featureNum = $matches[2]
    } elseif ($branchName -match '(^|/)(\d+)-') {
        $featureNum = $matches[2]
    } else {
        $featureNum = $branchName
    }
} else {
    if ($ShortName) {
        $branchSuffix = ConvertTo-CleanBranchName -Name $ShortName
    } else {
        $branchSuffix = Get-BranchName -Description $featureDesc
    }

    if ($Timestamp -and $Number -ne 0) {
        Write-Warning "[specify] Warning: -Number is ignored when -Timestamp is used"
        $Number = 0
    }

    if ($branchConvention) {
        $dateValue = ''
        $ticketValue = ''

        if ($branchConvention.BranchPattern.Contains('{seq}')) {
            if ($Number -eq 0) {
                if ($DryRun -and $hasGit) {
                    $Number = Get-NextBranchNumber -SpecsDir $specsDir -SkipFetch
                } elseif ($DryRun) {
                    $Number = (Get-HighestNumberFromSpecs -SpecsDir $specsDir) + 1
                } elseif ($hasGit) {
                    $Number = Get-NextBranchNumber -SpecsDir $specsDir
                } else {
                    $Number = (Get-HighestNumberFromSpecs -SpecsDir $specsDir) + 1
                }
            }

            $featureNum = ('{0:d' + $branchConvention.SeqPadding + '}') -f $Number
        }

        if ($branchConvention.BranchPattern.Contains('{date}')) {
            $dateValue = Get-Date -Format (ConvertTo-StrftimeDateFormat -Format $branchConvention.DateFormat)
            if ([string]::IsNullOrWhiteSpace($featureNum)) {
                $featureNum = $dateValue
            }
        }

        if ($branchConvention.BranchPattern.Contains('{ticket}')) {
            $ticketValue = Get-TicketValue -Description $featureDesc -TicketPattern $branchConvention.TicketPattern
            if ([string]::IsNullOrWhiteSpace($ticketValue)) {
                throw "Branch convention requires a ticket matching '$($branchConvention.TicketPattern)' in the feature description or a GIT_BRANCH_NAME override."
            }
            if ([string]::IsNullOrWhiteSpace($featureNum)) {
                $featureNum = $ticketValue
            }
        }

        if ([string]::IsNullOrWhiteSpace($featureNum)) {
            $featureNum = $branchSuffix
        }

        $branchName = New-ConventionBranchName `
            -Pattern $branchConvention.BranchPattern `
            -FeatureNum $featureNum `
            -DateValue $dateValue `
            -TicketValue $ticketValue `
            -TypeValue $branchConvention.TypeValue `
            -SuffixSource $branchSuffix `
            -Description $featureDesc `
            -Separator $branchConvention.Separator `
            -Lowercase $branchConvention.Lowercase
    } elseif ($Timestamp) {
        $featureNum = Get-Date -Format 'yyyyMMdd-HHmmss'
        $branchName = "$featureNum-$branchSuffix"
    } else {
        if ($Number -eq 0) {
            if ($DryRun -and $hasGit) {
                $Number = Get-NextBranchNumber -SpecsDir $specsDir -SkipFetch
            } elseif ($DryRun) {
                $Number = (Get-HighestNumberFromSpecs -SpecsDir $specsDir) + 1
            } elseif ($hasGit) {
                $Number = Get-NextBranchNumber -SpecsDir $specsDir
            } else {
                $Number = (Get-HighestNumberFromSpecs -SpecsDir $specsDir) + 1
            }
        }

        $featureNum = ('{0:000}' -f $Number)
        $branchName = "$featureNum-$branchSuffix"
    }
}

$maxBranchLength = 244
if ($branchConvention -and $branchConvention.MaxLength -gt 0 -and $branchConvention.MaxLength -lt $maxBranchLength) {
    $maxBranchLength = $branchConvention.MaxLength
}
if ($branchName.Length -gt $maxBranchLength) {
    $truncatedSuffix = $branchSuffix
    $originalBranchName = $branchName

    while (-not [string]::IsNullOrWhiteSpace($truncatedSuffix)) {
        $truncatedSuffix = $truncatedSuffix.Substring(0, $truncatedSuffix.Length - 1)
        $truncatedSuffix = $truncatedSuffix -replace '[-_]$', ''

        if ($branchConvention) {
            $branchName = New-ConventionBranchName `
                -Pattern $branchConvention.BranchPattern `
                -FeatureNum $featureNum `
                -DateValue $dateValue `
                -TicketValue $ticketValue `
                -TypeValue $branchConvention.TypeValue `
                -SuffixSource $truncatedSuffix `
                -Description $featureDesc `
                -Separator $branchConvention.Separator `
                -Lowercase $branchConvention.Lowercase
        } else {
            $branchName = "$featureNum-$truncatedSuffix"
        }

        if ($branchName.Length -le $maxBranchLength) {
            break
        }
    }

    Write-Warning "[specify] Branch name exceeded GitHub's 244-byte limit"
    Write-Warning "[specify] Original: $originalBranchName ($($originalBranchName.Length) bytes)"
    Write-Warning "[specify] Truncated to: $branchName ($($branchName.Length) bytes)"
}

if (-not $DryRun) {
    if ($hasGit) {
        $branchCreated = $false
        $branchCreateError = ''
        try {
            $branchCreateError = git checkout -q -b $branchName 2>&1 | Out-String
            if ($LASTEXITCODE -eq 0) {
                $branchCreated = $true
            }
        } catch {
            $branchCreateError = $_.Exception.Message
        }

        if (-not $branchCreated) {
            $currentBranch = ''
            try { $currentBranch = (git rev-parse --abbrev-ref HEAD 2>$null).Trim() } catch {}
            $existingBranch = git branch --list $branchName 2>$null
            if ($existingBranch) {
                if ($AllowExistingBranch) {
                    if ($currentBranch -eq $branchName) {
                        # Already on the target branch
                    } else {
                        $switchBranchError = git checkout -q $branchName 2>&1 | Out-String
                        if ($LASTEXITCODE -ne 0) {
                            if ($switchBranchError) {
                                Write-Error "Error: Branch '$branchName' exists but could not be checked out.`n$($switchBranchError.Trim())"
                            } else {
                                Write-Error "Error: Branch '$branchName' exists but could not be checked out. Resolve any uncommitted changes or conflicts and try again."
                            }
                            exit 1
                        }
                    }
                } elseif ($Timestamp) {
                    Write-Error "Error: Branch '$branchName' already exists. Rerun to get a new timestamp or use a different -ShortName."
                    exit 1
                } else {
                    Write-Error "Error: Branch '$branchName' already exists. Please use a different feature name or specify a different number with -Number."
                    exit 1
                }
            } else {
                if ($branchCreateError) {
                    Write-Error "Error: Failed to create git branch '$branchName'.`n$($branchCreateError.Trim())"
                } else {
                    Write-Error "Error: Failed to create git branch '$branchName'. Please check your git configuration and try again."
                }
                exit 1
            }
        }
    } else {
        if ($Json) {
            [Console]::Error.WriteLine("[specify] Warning: Git repository not detected; skipped branch creation for $branchName")
        } else {
            Write-Warning "[specify] Warning: Git repository not detected; skipped branch creation for $branchName"
        }
    }

    $env:SPECIFY_FEATURE = $branchName
}

if ($Json) {
    $obj = [PSCustomObject]@{
        BRANCH_NAME = $branchName
        FEATURE_NUM = $featureNum
        HAS_GIT = $hasGit
    }
    if ($DryRun) {
        $obj | Add-Member -NotePropertyName 'DRY_RUN' -NotePropertyValue $true
    }
    $obj | ConvertTo-Json -Compress
} else {
    Write-Output "BRANCH_NAME: $branchName"
    Write-Output "FEATURE_NUM: $featureNum"
    Write-Output "HAS_GIT: $hasGit"
    if (-not $DryRun) {
        Write-Output "SPECIFY_FEATURE environment variable set to: $branchName"
    }
}
