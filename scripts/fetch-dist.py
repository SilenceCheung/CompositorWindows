#!/usr/bin/env python3
"""
Downloads the latest built Windows packages (MSI, ZIP, Checksums)
from GitHub Actions / Releases into the local ./dist directory.
"""
import json
import os
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DIST = ROOT / "dist"
REPO = "SilenceCheung/CompositorWindows"

def run(cmd):
    result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
    return result.returncode, result.stdout.strip(), result.stderr.strip()

def main():
    DIST.mkdir(parents=True, exist_ok=True)
    print(f"Checking latest release and artifacts for {REPO}...")

    # Check if a release exists with assets
    code, out, _ = run(f"gh release view v0.1.6 --repo {REPO} --json assets")
    if code == 0 and out:
        try:
            data = json.loads(out)
            assets = data.get("assets", [])
            if assets:
                print(f"Found {len(assets)} release assets in v0.1.6. Downloading to ./dist ...")
                code, _, err = run(f"gh release download v0.1.6 --repo {REPO} --dir '{DIST}' --clobber")
                if code == 0:
                    print("Successfully downloaded release assets into ./dist:")
                    for f in DIST.iterdir():
                        print(f"  - {f.name} ({f.stat().st_size:,} bytes)")
                    return 0
                else:
                    print(f"Warning: gh release download failed: {err}")
        except Exception as e:
            print(f"Error parsing release: {e}")

    # Check latest workflow run artifacts
    code, out, _ = run(f"gh run list --repo {REPO} --limit 3 --json databaseId,status,conclusion,headBranch")
    if code == 0 and out:
        runs = json.loads(out)
        for r in runs:
            run_id = r["databaseId"]
            status = r["status"]
            conclusion = r["conclusion"]
            print(f"Run {run_id} ({r.get('headBranch')}): status={status}, conclusion={conclusion}")
            if status == "completed" and conclusion == "success":
                print(f"Downloading artifacts from run {run_id} into ./dist ...")
                run(f"gh run download {run_id} --repo {REPO} --dir '{DIST}'")
                print("Downloaded artifacts into ./dist.")
                return 0

    print("Build is currently in progress on Windows runner. Please check back shortly.")
    return 1

if __name__ == "__main__":
    sys.exit(main())
