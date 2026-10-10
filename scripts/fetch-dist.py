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
    tag = sys.argv[1] if len(sys.argv) > 1 else "v0.1.7"
    DIST.mkdir(parents=True, exist_ok=True)
    print(f"Checking latest release and artifacts for {REPO} (tag: {tag})...")

    # Check if a release exists with assets
    code, out, _ = run(f"gh release view {tag} --repo {REPO} --json assets")
    if code == 0 and out:
        try:
            data = json.loads(out)
            assets = data.get("assets", [])
            if assets:
                print(f"Found {len(assets)} release assets in {tag}. Downloading to ./dist ...")
                has_aria2c = (subprocess.run("which aria2c", shell=True, capture_output=True).returncode == 0)
                for a in assets:
                    url = a.get("url")
                    name = a.get("name")
                    target = DIST / name
                    if has_aria2c:
                        print(f"Downloading {name} via aria2c...")
                        run(f"aria2c -x 16 -s 16 -k 1M --dir='{DIST}' -o '{name}' '{url}'")
                    else:
                        print(f"Downloading {name} via gh...")
                        run(f"gh release download {tag} --repo {REPO} -p '{name}' --dir '{DIST}' --clobber")
                print("Successfully downloaded release assets into ./dist:")
                for f in sorted(DIST.iterdir()):
                    print(f"  - {f.name} ({f.stat().st_size:,} bytes)")
                return 0
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
