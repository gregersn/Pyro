#!/usr/bin/env python3
"""Create Python compatible version string from git."""
import os
from datetime import datetime
import subprocess

GIT_COMMAND = ["git", "describe", "--dirty", "--tags", "--long", "--match", "*[0-9]*"]


def bump_tag(tag: str):
    """Increase patch part of version."""
    major, minor, patch = [int(t, 10) for t in tag[1:].split(".")]

    return ".".join(str(p) for p in (major, minor, patch + 1))


def format_version(input_version: str) -> str:
    """Format output from git."""
    parts = input_version.split("-")
    tag, steps, commit = parts[0:3]
    next_tag = bump_tag(tag)

    dirty = "dirty" in parts

    if int(steps, 10) > 0:
        formatted_string = f"{next_tag}.dev{steps}+{commit}.d{datetime.now().strftime("%Y%m%d")}"
    else:
        formatted_string = f"{tag}"

    if dirty:
        return formatted_string + "+dirty"
    return formatted_string


def get_git_version():
    """Get a version string from git."""
    p = subprocess.Popen(
        GIT_COMMAND,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        cwd=os.path.dirname(__file__),
    )

    out, _ = p.communicate()
    version_string = out.decode("utf-8").strip()

    return version_string


version = format_version(get_git_version())
print(version)
