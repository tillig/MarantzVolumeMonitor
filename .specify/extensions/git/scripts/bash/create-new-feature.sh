#!/usr/bin/env bash
# Git extension: create-new-feature.sh
# Adapted from core scripts/bash/create-new-feature.sh for extension layout.
# Sources common.sh from the project's installed scripts, falling back to
# git-common.sh for minimal git helpers.

set -e

JSON_MODE=false
DRY_RUN=false
ALLOW_EXISTING=false
SHORT_NAME=""
BRANCH_NUMBER=""
USE_TIMESTAMP=false
ARGS=()
i=1
while [ $i -le $# ]; do
    arg="${!i}"
    case "$arg" in
        --json)
            JSON_MODE=true
            ;;
        --dry-run)
            DRY_RUN=true
            ;;
        --allow-existing-branch)
            ALLOW_EXISTING=true
            ;;
        --short-name)
            if [ $((i + 1)) -gt $# ]; then
                echo 'Error: --short-name requires a value' >&2
                exit 1
            fi
            i=$((i + 1))
            next_arg="${!i}"
            if [[ "$next_arg" == --* ]]; then
                echo 'Error: --short-name requires a value' >&2
                exit 1
            fi
            SHORT_NAME="$next_arg"
            ;;
        --number)
            if [ $((i + 1)) -gt $# ]; then
                echo 'Error: --number requires a value' >&2
                exit 1
            fi
            i=$((i + 1))
            next_arg="${!i}"
            if [[ "$next_arg" == --* ]]; then
                echo 'Error: --number requires a value' >&2
                exit 1
            fi
            BRANCH_NUMBER="$next_arg"
            if [[ ! "$BRANCH_NUMBER" =~ ^[0-9]+$ ]]; then
                echo 'Error: --number must be a non-negative integer' >&2
                exit 1
            fi
            ;;
        --timestamp)
            USE_TIMESTAMP=true
            ;;
        --help|-h)
            echo "Usage: $0 [--json] [--dry-run] [--allow-existing-branch] [--short-name <name>] [--number N] [--timestamp] <feature_description>"
            echo ""
            echo "Options:"
            echo "  --json              Output in JSON format"
            echo "  --dry-run           Compute branch name without creating the branch"
            echo "  --allow-existing-branch  Switch to branch if it already exists instead of failing"
            echo "  --short-name <name> Provide a custom short name (2-4 words) for the branch"
            echo "  --number N          Specify branch number manually (overrides auto-detection)"
            echo "  --timestamp         Use timestamp prefix (YYYYMMDD-HHMMSS) instead of sequential numbering"
            echo "  --help, -h          Show this help message"
            echo ""
            echo "Environment variables:"
            echo "  GIT_BRANCH_NAME     Use this exact branch name, bypassing all prefix/suffix generation"
            echo ""
            echo "Examples:"
            echo "  $0 'Add user authentication system' --short-name 'user-auth'"
            echo "  $0 'Implement OAuth2 integration for API' --number 5"
            echo "  $0 --timestamp --short-name 'user-auth' 'Add user authentication'"
            echo "  GIT_BRANCH_NAME=my-branch $0 'feature description'"
            exit 0
            ;;
        *)
            ARGS+=("$arg")
            ;;
    esac
    i=$((i + 1))
done

FEATURE_DESCRIPTION="${ARGS[*]}"
if [ -z "$FEATURE_DESCRIPTION" ]; then
    echo "Usage: $0 [--json] [--dry-run] [--allow-existing-branch] [--short-name <name>] [--number N] [--timestamp] <feature_description>" >&2
    exit 1
fi

# Trim whitespace and validate description is not empty
FEATURE_DESCRIPTION=$(echo "$FEATURE_DESCRIPTION" | sed -E 's/^[[:space:]]+|[[:space:]]+$//g')
if [ -z "$FEATURE_DESCRIPTION" ]; then
    echo "Error: Feature description cannot be empty or contain only whitespace" >&2
    exit 1
fi

# Function to get highest number from specs directory
get_highest_from_specs() {
    local specs_dir="$1"
    local highest=0

    if [ -d "$specs_dir" ]; then
        for dir in "$specs_dir"/*; do
            [ -d "$dir" ] || continue
            dirname=$(basename "$dir")
            # Match sequential prefixes (>=3 digits), but skip timestamp dirs.
            if echo "$dirname" | grep -Eq '^[0-9]{3,}-' && ! echo "$dirname" | grep -Eq '^[0-9]{8}-[0-9]{6}-'; then
                number=$(echo "$dirname" | grep -Eo '^[0-9]+')
                number=$((10#$number))
                if [ "$number" -gt "$highest" ]; then
                    highest=$number
                fi
            fi
        done
    fi

    echo "$highest"
}

# Function to get highest number from git branches
get_highest_from_branches() {
    git branch -a 2>/dev/null | sed 's/^[* ]*//; s|^remotes/[^/]*/||' | _extract_highest_number
}

# Extract the highest sequential feature number from a list of ref names (one per line).
_extract_highest_number() {
    local highest=0
    while IFS= read -r name; do
        [ -z "$name" ] && continue
        if echo "$name" | grep -Eq '(^|/)[0-9]{3,}-' && ! echo "$name" | grep -Eq '(^|/)[0-9]{8}-[0-9]{6}-'; then
            number=$(echo "$name" | grep -Eo '(^|/)[0-9]+' | tail -1 | tr -d '/' || echo "0")
            number=$((10#$number))
            if [ "$number" -gt "$highest" ]; then
                highest=$number
            fi
        fi
    done
    echo "$highest"
}

# Function to get highest number from remote branches without fetching (side-effect-free)
get_highest_from_remote_refs() {
    local highest=0

    for remote in $(git remote 2>/dev/null); do
        local remote_highest
        remote_highest=$(GIT_TERMINAL_PROMPT=0 git ls-remote --heads "$remote" 2>/dev/null | sed 's|.*refs/heads/||' | _extract_highest_number)
        if [ "$remote_highest" -gt "$highest" ]; then
            highest=$remote_highest
        fi
    done

    echo "$highest"
}

# Function to check existing branches and return next available number.
check_existing_branches() {
    local specs_dir="$1"
    local skip_fetch="${2:-false}"

    if [ "$skip_fetch" = true ]; then
        local highest_remote=$(get_highest_from_remote_refs)
        local highest_branch=$(get_highest_from_branches)
        if [ "$highest_remote" -gt "$highest_branch" ]; then
            highest_branch=$highest_remote
        fi
    else
        git fetch --all --prune >/dev/null 2>&1 || true
        local highest_branch=$(get_highest_from_branches)
    fi

    local highest_spec=$(get_highest_from_specs "$specs_dir")

    local max_num=$highest_branch
    if [ "$highest_spec" -gt "$max_num" ]; then
        max_num=$highest_spec
    fi

    echo $((max_num + 1))
}

# Function to clean and format a branch name
clean_branch_name() {
    local name="$1"
    echo "$name" | tr '[:upper:]' '[:lower:]' | sed 's/[^a-z0-9]/-/g' | sed 's/-\+/-/g' | sed 's/^-//' | sed 's/-$//'
}

extract_yaml_scalar() {
    local file="$1"
    local key="$2"

    awk -v key="$key" '
        $0 ~ "^[[:space:]]*" key ":[[:space:]]*" {
            sub("^[[:space:]]*" key ":[[:space:]]*", "", $0)
            gsub(/^"/, "", $0)
            gsub(/"$/, "", $0)
            print $0
            exit
        }
    ' "$file"
}

extract_yaml_map_value() {
    local file="$1"
    local section="$2"
    local key="$3"

    awk -v section="$section" -v key="$key" '
        $0 ~ "^[[:space:]]*" section ":[[:space:]]*$" {
            in_section=1
            next
        }
        in_section && $0 ~ "^[^[:space:]]" {
            in_section=0
        }
        in_section && $0 ~ "^[[:space:]]+" key ":[[:space:]]*" {
            sub("^[[:space:]]*" key ":[[:space:]]*", "", $0)
            gsub(/^"/, "", $0)
            gsub(/"$/, "", $0)
            print $0
            exit
        }
    ' "$file"
}

# ---------------------------------------------------------------------------
# Source common.sh for resolve_template, json_escape, get_repo_root, has_git.
#
# Search locations in priority order:
#  1. .specify/scripts/bash/common.sh under the project root (installed project)
#  2. scripts/bash/common.sh under the project root (source checkout fallback)
#  3. git-common.sh next to this script (minimal fallback — lacks resolve_template)
# ---------------------------------------------------------------------------
SCRIPT_DIR="$(CDPATH="" cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Find project root by walking up from the script location
_find_project_root() {
    local dir="$1"
    while [ "$dir" != "/" ]; do
        if [ -d "$dir/.specify" ] || [ -d "$dir/.git" ]; then
            echo "$dir"
            return 0
        fi
        dir="$(dirname "$dir")"
    done
    return 1
}

_common_loaded=false
_PROJECT_ROOT=$(_find_project_root "$SCRIPT_DIR") || true

if [ -n "$_PROJECT_ROOT" ] && [ -f "$_PROJECT_ROOT/.specify/scripts/bash/common.sh" ]; then
    source "$_PROJECT_ROOT/.specify/scripts/bash/common.sh"
    _common_loaded=true
elif [ -n "$_PROJECT_ROOT" ] && [ -f "$_PROJECT_ROOT/scripts/bash/common.sh" ]; then
    source "$_PROJECT_ROOT/scripts/bash/common.sh"
    _common_loaded=true
elif [ -f "$SCRIPT_DIR/git-common.sh" ]; then
    source "$SCRIPT_DIR/git-common.sh"
    _common_loaded=true
fi

if [ "$_common_loaded" != "true" ]; then
    echo "Error: Could not locate common.sh or git-common.sh. Please ensure the Specify core scripts are installed." >&2
    exit 1
fi

# Resolve repository root
if type get_repo_root >/dev/null 2>&1; then
    REPO_ROOT=$(get_repo_root)
elif git rev-parse --show-toplevel >/dev/null 2>&1; then
    REPO_ROOT=$(git rev-parse --show-toplevel)
elif [ -n "$_PROJECT_ROOT" ]; then
    REPO_ROOT="$_PROJECT_ROOT"
else
    echo "Error: Could not determine repository root." >&2
    exit 1
fi

# Check if git is available at this repo root
if type has_git >/dev/null 2>&1; then
    if has_git "$REPO_ROOT"; then
        HAS_GIT=true
    else
        HAS_GIT=false
    fi
elif git -C "$REPO_ROOT" rev-parse --is-inside-work-tree >/dev/null 2>&1; then
    HAS_GIT=true
else
    HAS_GIT=false
fi

cd "$REPO_ROOT"

SPECS_DIR="$REPO_ROOT/specs"
BRANCH_CONVENTION_FILE="$REPO_ROOT/.specify/branch-convention.yml"
BRANCH_PATTERN=""
BRANCH_SEQ_PADDING="3"
BRANCH_DATE_FORMAT="YYYYMMDD"
BRANCH_SEPARATOR="-"
BRANCH_LOWERCASE="true"
BRANCH_DEFAULT_TYPE="feature"
BRANCH_TYPE_VALUE="feature"
BRANCH_TICKET_PATTERN="[A-Z]+-[0-9]+"
BRANCH_MAX_LENGTH=""
BRANCH_DATE_VALUE=""
BRANCH_TICKET_VALUE=""

load_branch_convention() {
    [ -f "$BRANCH_CONVENTION_FILE" ] || return 0

    BRANCH_PATTERN=$(extract_yaml_scalar "$BRANCH_CONVENTION_FILE" "branch_pattern")
    BRANCH_SEQ_PADDING=$(extract_yaml_scalar "$BRANCH_CONVENTION_FILE" "seq_padding")
    BRANCH_DATE_FORMAT=$(extract_yaml_scalar "$BRANCH_CONVENTION_FILE" "date_format")
    BRANCH_SEPARATOR=$(extract_yaml_scalar "$BRANCH_CONVENTION_FILE" "separator")
    BRANCH_LOWERCASE=$(extract_yaml_scalar "$BRANCH_CONVENTION_FILE" "lowercase")
    BRANCH_DEFAULT_TYPE=$(extract_yaml_scalar "$BRANCH_CONVENTION_FILE" "default_type")
    BRANCH_TICKET_PATTERN=$(extract_yaml_scalar "$BRANCH_CONVENTION_FILE" "ticket_pattern")
    BRANCH_MAX_LENGTH=$(extract_yaml_scalar "$BRANCH_CONVENTION_FILE" "max_length")

    [ -n "$BRANCH_SEQ_PADDING" ] || BRANCH_SEQ_PADDING="3"
    [ -n "$BRANCH_DATE_FORMAT" ] || BRANCH_DATE_FORMAT="YYYYMMDD"
    [ -n "$BRANCH_SEPARATOR" ] || BRANCH_SEPARATOR="-"
    [ -n "$BRANCH_LOWERCASE" ] || BRANCH_LOWERCASE="true"
    [ -n "$BRANCH_DEFAULT_TYPE" ] || BRANCH_DEFAULT_TYPE="feature"
    [ -n "$BRANCH_TICKET_PATTERN" ] || BRANCH_TICKET_PATTERN="[A-Z]+-[0-9]+"

    BRANCH_TYPE_VALUE=$(extract_yaml_map_value "$BRANCH_CONVENTION_FILE" "type_prefix" "$BRANCH_DEFAULT_TYPE")
    [ -n "$BRANCH_TYPE_VALUE" ] || BRANCH_TYPE_VALUE="$BRANCH_DEFAULT_TYPE"
}

normalize_branch_token() {
    local value="$1"
    local separator="${BRANCH_SEPARATOR:--}"

    if [ "$BRANCH_LOWERCASE" = "true" ]; then
        value=$(printf '%s' "$value" | tr '[:upper:]' '[:lower:]')
    fi

    value=$(printf '%s' "$value" | sed "s/[^[:alnum:]]/${separator}/g")
    value=$(printf '%s' "$value" | sed -E "s/${separator}{2,}/${separator}/g")
    value=$(printf '%s' "$value" | sed -E "s/^${separator}+//; s/${separator}+$//")

    printf '%s\n' "$value"
}

convert_date_format_to_strftime() {
    local format="$1"

    format="${format//YYYY/%Y}"
    format="${format//MM/%m}"
    format="${format//DD/%d}"
    format="${format//HH/%H}"
    format="${format//mm/%M}"
    format="${format//ss/%S}"

    printf '%s\n' "$format"
}

extract_ticket_value() {
    if [ -z "$BRANCH_TICKET_PATTERN" ]; then
        return 0
    fi

    printf '%s\n' "$FEATURE_DESCRIPTION" | grep -Eo "$BRANCH_TICKET_PATTERN" | head -1
}

compose_branch_name_from_convention() {
    local suffix_source="$1"
    local branch_name="$BRANCH_PATTERN"
    local kebab_value
    local summary_value
    local type_value

    kebab_value=$(normalize_branch_token "$suffix_source")
    summary_value=$(normalize_branch_token "$FEATURE_DESCRIPTION")
    type_value=$(normalize_branch_token "$BRANCH_TYPE_VALUE")

    branch_name="${branch_name//\{seq\}/$FEATURE_NUM}"
    branch_name="${branch_name//\{date\}/$BRANCH_DATE_VALUE}"
    branch_name="${branch_name//\{ticket\}/$BRANCH_TICKET_VALUE}"
    branch_name="${branch_name//\{type\}/$type_value}"
    branch_name="${branch_name//\{kebab\}/$kebab_value}"
    branch_name="${branch_name//\{summary\}/$summary_value}"

    printf '%s\n' "$branch_name"
}

# Function to generate branch name with stop word filtering
generate_branch_name() {
    local description="$1"

    local stop_words="^(i|a|an|the|to|for|of|in|on|at|by|with|from|is|are|was|were|be|been|being|have|has|had|do|does|did|will|would|should|could|can|may|might|must|shall|this|that|these|those|my|your|our|their|want|need|add|get|set)$"

    local clean_name=$(echo "$description" | tr '[:upper:]' '[:lower:]' | sed 's/[^a-z0-9]/ /g')

    local meaningful_words=()
    for word in $clean_name; do
        [ -z "$word" ] && continue
        if ! echo "$word" | grep -qiE "$stop_words"; then
            if [ ${#word} -ge 3 ]; then
                meaningful_words+=("$word")
            elif echo "$description" | grep -qw -- "${word^^}"; then
                meaningful_words+=("$word")
            fi
        fi
    done

    if [ ${#meaningful_words[@]} -gt 0 ]; then
        local max_words=3
        if [ ${#meaningful_words[@]} -eq 4 ]; then max_words=4; fi

        local result=""
        local count=0
        for word in "${meaningful_words[@]}"; do
            if [ $count -ge $max_words ]; then break; fi
            if [ -n "$result" ]; then result="$result-"; fi
            result="$result$word"
            count=$((count + 1))
        done
        echo "$result"
    else
        local cleaned=$(clean_branch_name "$description")
        echo "$cleaned" | tr '-' '\n' | grep -v '^$' | head -3 | tr '\n' '-' | sed 's/-$//'
    fi
}

# Check for GIT_BRANCH_NAME env var override (exact branch name, no prefix/suffix)
if [ -n "${GIT_BRANCH_NAME:-}" ]; then
    BRANCH_NAME="$GIT_BRANCH_NAME"
    # Extract FEATURE_NUM from the branch name if it starts with a numeric prefix
    # Check timestamp pattern first (YYYYMMDD-HHMMSS-) since it also matches the simpler ^[0-9]+ pattern
    if echo "$BRANCH_NAME" | grep -Eq '(^|/)[0-9]{8}-[0-9]{6}-'; then
        FEATURE_NUM=$(echo "$BRANCH_NAME" | grep -Eo '(^|/)[0-9]{8}-[0-9]{6}' | tail -1 | tr -d '/')
        BRANCH_SUFFIX="${BRANCH_NAME##*/}"
        BRANCH_SUFFIX="${BRANCH_SUFFIX#${FEATURE_NUM}-}"
    elif echo "$BRANCH_NAME" | grep -Eq '(^|/)[0-9]+-'; then
        FEATURE_NUM=$(echo "$BRANCH_NAME" | grep -Eo '(^|/)[0-9]+' | tail -1 | tr -d '/')
        BRANCH_SUFFIX="${BRANCH_NAME##*/}"
        BRANCH_SUFFIX="${BRANCH_SUFFIX#${FEATURE_NUM}-}"
    else
        FEATURE_NUM="$BRANCH_NAME"
        BRANCH_SUFFIX="$BRANCH_NAME"
    fi
else
    load_branch_convention

    # Generate branch name
    if [ -n "$SHORT_NAME" ]; then
        BRANCH_SUFFIX=$(clean_branch_name "$SHORT_NAME")
    else
        BRANCH_SUFFIX=$(generate_branch_name "$FEATURE_DESCRIPTION")
    fi

    # Warn if --number and --timestamp are both specified
    if [ "$USE_TIMESTAMP" = true ] && [ -n "$BRANCH_NUMBER" ]; then
        >&2 echo "[specify] Warning: --number is ignored when --timestamp is used"
        BRANCH_NUMBER=""
    fi

    if [ -n "$BRANCH_PATTERN" ]; then
        if [[ "$BRANCH_PATTERN" == *"{seq}"* ]]; then
            if [ -z "$BRANCH_NUMBER" ]; then
                if [ "$DRY_RUN" = true ] && [ "$HAS_GIT" = true ]; then
                    BRANCH_NUMBER=$(check_existing_branches "$SPECS_DIR" true)
                elif [ "$DRY_RUN" = true ]; then
                    HIGHEST=$(get_highest_from_specs "$SPECS_DIR")
                    BRANCH_NUMBER=$((HIGHEST + 1))
                elif [ "$HAS_GIT" = true ]; then
                    BRANCH_NUMBER=$(check_existing_branches "$SPECS_DIR")
                else
                    HIGHEST=$(get_highest_from_specs "$SPECS_DIR")
                    BRANCH_NUMBER=$((HIGHEST + 1))
                fi
            fi

            FEATURE_NUM=$(printf "%0${BRANCH_SEQ_PADDING}d" "$((10#$BRANCH_NUMBER))")
        fi

        if [[ "$BRANCH_PATTERN" == *"{date}"* ]]; then
            BRANCH_DATE_VALUE=$(date +"$(convert_date_format_to_strftime "$BRANCH_DATE_FORMAT")")
            if [ -z "${FEATURE_NUM:-}" ]; then
                FEATURE_NUM="$BRANCH_DATE_VALUE"
            fi
        fi

        if [[ "$BRANCH_PATTERN" == *"{ticket}"* ]]; then
            BRANCH_TICKET_VALUE=$(extract_ticket_value)
            if [ -z "$BRANCH_TICKET_VALUE" ]; then
                echo "Error: Branch convention requires a ticket matching '$BRANCH_TICKET_PATTERN' in the feature description or GIT_BRANCH_NAME override." >&2
                exit 1
            fi
            if [ -z "${FEATURE_NUM:-}" ]; then
                FEATURE_NUM="$BRANCH_TICKET_VALUE"
            fi
        fi

        [ -n "${FEATURE_NUM:-}" ] || FEATURE_NUM="$BRANCH_SUFFIX"
        BRANCH_NAME=$(compose_branch_name_from_convention "$BRANCH_SUFFIX")
    elif [ "$USE_TIMESTAMP" = true ]; then
        FEATURE_NUM=$(date +%Y%m%d-%H%M%S)
        BRANCH_NAME="${FEATURE_NUM}-${BRANCH_SUFFIX}"
    else
        if [ -z "$BRANCH_NUMBER" ]; then
            if [ "$DRY_RUN" = true ] && [ "$HAS_GIT" = true ]; then
                BRANCH_NUMBER=$(check_existing_branches "$SPECS_DIR" true)
            elif [ "$DRY_RUN" = true ]; then
                HIGHEST=$(get_highest_from_specs "$SPECS_DIR")
                BRANCH_NUMBER=$((HIGHEST + 1))
            elif [ "$HAS_GIT" = true ]; then
                BRANCH_NUMBER=$(check_existing_branches "$SPECS_DIR")
            else
                HIGHEST=$(get_highest_from_specs "$SPECS_DIR")
                BRANCH_NUMBER=$((HIGHEST + 1))
            fi
        fi

        FEATURE_NUM=$(printf "%03d" "$((10#$BRANCH_NUMBER))")
        BRANCH_NAME="${FEATURE_NUM}-${BRANCH_SUFFIX}"
    fi
fi

# GitHub enforces a 244-byte limit on branch names
MAX_BRANCH_LENGTH=244
if [ -n "$BRANCH_MAX_LENGTH" ] && [ "$BRANCH_MAX_LENGTH" -lt "$MAX_BRANCH_LENGTH" ] 2>/dev/null; then
    MAX_BRANCH_LENGTH="$BRANCH_MAX_LENGTH"
fi
_byte_length() { printf '%s' "$1" | LC_ALL=C wc -c | tr -d ' '; }
BRANCH_BYTE_LEN=$(_byte_length "$BRANCH_NAME")
if [ -n "${GIT_BRANCH_NAME:-}" ] && [ "$BRANCH_BYTE_LEN" -gt $MAX_BRANCH_LENGTH ]; then
    >&2 echo "Error: GIT_BRANCH_NAME must be 244 bytes or fewer in UTF-8. Provided value is ${BRANCH_BYTE_LEN} bytes."
    exit 1
elif [ "$BRANCH_BYTE_LEN" -gt $MAX_BRANCH_LENGTH ]; then
    TRUNCATED_SUFFIX="$BRANCH_SUFFIX"
    ORIGINAL_BRANCH_NAME="$BRANCH_NAME"

    while [ -n "$TRUNCATED_SUFFIX" ]; do
        TRUNCATED_SUFFIX="${TRUNCATED_SUFFIX%?}"
        TRUNCATED_SUFFIX="${TRUNCATED_SUFFIX%-}"

        if [ -n "$BRANCH_PATTERN" ]; then
            BRANCH_NAME=$(compose_branch_name_from_convention "$TRUNCATED_SUFFIX")
        else
            BRANCH_NAME="${FEATURE_NUM}-${TRUNCATED_SUFFIX}"
        fi

        BRANCH_BYTE_LEN=$(_byte_length "$BRANCH_NAME")
        if [ "$BRANCH_BYTE_LEN" -le "$MAX_BRANCH_LENGTH" ]; then
            break
        fi
    done

    >&2 echo "[specify] Warning: Branch name exceeded GitHub's 244-byte limit"
    >&2 echo "[specify] Original: $ORIGINAL_BRANCH_NAME ($(_byte_length "$ORIGINAL_BRANCH_NAME") bytes)"
    >&2 echo "[specify] Truncated to: $BRANCH_NAME ($(_byte_length "$BRANCH_NAME") bytes)"
fi

if [ "$DRY_RUN" != true ]; then
    if [ "$HAS_GIT" = true ]; then
        branch_create_error=""
        if ! branch_create_error=$(git checkout -q -b "$BRANCH_NAME" 2>&1); then
            current_branch="$(git rev-parse --abbrev-ref HEAD 2>/dev/null || true)"
            if git branch --list "$BRANCH_NAME" | grep -q .; then
                if [ "$ALLOW_EXISTING" = true ]; then
                    if [ "$current_branch" = "$BRANCH_NAME" ]; then
                        :
                    elif ! switch_branch_error=$(git checkout -q "$BRANCH_NAME" 2>&1); then
                        >&2 echo "Error: Failed to switch to existing branch '$BRANCH_NAME'. Please resolve any local changes or conflicts and try again."
                        if [ -n "$switch_branch_error" ]; then
                            >&2 printf '%s\n' "$switch_branch_error"
                        fi
                        exit 1
                    fi
                elif [ "$USE_TIMESTAMP" = true ]; then
                    >&2 echo "Error: Branch '$BRANCH_NAME' already exists. Rerun to get a new timestamp or use a different --short-name."
                    exit 1
                else
                    >&2 echo "Error: Branch '$BRANCH_NAME' already exists. Please use a different feature name or specify a different number with --number."
                    exit 1
                fi
            else
                >&2 echo "Error: Failed to create git branch '$BRANCH_NAME'."
                if [ -n "$branch_create_error" ]; then
                    >&2 printf '%s\n' "$branch_create_error"
                else
                    >&2 echo "Please check your git configuration and try again."
                fi
                exit 1
            fi
        fi
    else
        >&2 echo "[specify] Warning: Git repository not detected; skipped branch creation for $BRANCH_NAME"
    fi

    printf '# To persist: export SPECIFY_FEATURE=%q\n' "$BRANCH_NAME" >&2
fi

if $JSON_MODE; then
    if command -v jq >/dev/null 2>&1; then
        if [ "$DRY_RUN" = true ]; then
            jq -cn \
                --arg branch_name "$BRANCH_NAME" \
                --arg feature_num "$FEATURE_NUM" \
                '{BRANCH_NAME:$branch_name,FEATURE_NUM:$feature_num,DRY_RUN:true}'
        else
            jq -cn \
                --arg branch_name "$BRANCH_NAME" \
                --arg feature_num "$FEATURE_NUM" \
                '{BRANCH_NAME:$branch_name,FEATURE_NUM:$feature_num}'
        fi
    else
        if type json_escape >/dev/null 2>&1; then
            _je_branch=$(json_escape "$BRANCH_NAME")
            _je_num=$(json_escape "$FEATURE_NUM")
        else
            _je_branch="$BRANCH_NAME"
            _je_num="$FEATURE_NUM"
        fi
        if [ "$DRY_RUN" = true ]; then
            printf '{"BRANCH_NAME":"%s","FEATURE_NUM":"%s","DRY_RUN":true}\n' "$_je_branch" "$_je_num"
        else
            printf '{"BRANCH_NAME":"%s","FEATURE_NUM":"%s"}\n' "$_je_branch" "$_je_num"
        fi
    fi
else
    echo "BRANCH_NAME: $BRANCH_NAME"
    echo "FEATURE_NUM: $FEATURE_NUM"
    if [ "$DRY_RUN" != true ]; then
        printf '# To persist in your shell: export SPECIFY_FEATURE=%q\n' "$BRANCH_NAME"
    fi
fi
