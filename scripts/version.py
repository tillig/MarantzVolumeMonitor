"""Inject the firmware version into the build as FIRMWARE_VERSION.

The release tag is the single source of truth. A tagged commit yields exactly the tag, for example "v2.1.0"; anything else gets a descriptive form such as "v2.1.0-3-gabc1234-dirty" so a hand-built image is never mistaken for a release. Falls back to "unknown" when git or the tags are unavailable.
"""

import subprocess

Import("env")


def firmware_version():
    try:
        described = subprocess.run(
            ["git", "describe", "--tags", "--always", "--dirty"],
            capture_output=True,
            text=True,
            timeout=10,
        )
    except (OSError, subprocess.SubprocessError):
        return "unknown"

    version = described.stdout.strip()
    if described.returncode != 0 or not version:
        return "unknown"
    return version


version = firmware_version()
print("Firmware version: %s" % version)
env.Append(CPPDEFINES=[("FIRMWARE_VERSION", env.StringifyMacro(version))])
