#!/usr/bin/env python3

import argparse
import subprocess
import os

MKDOCS_DOCKER_IMAGE = "ghcr.io/heia-fr/mkdocs-edu:v0.1.3"

parser = argparse.ArgumentParser(description="Mkdocs Local Server")
parser.add_argument('--select', '-c', type=int, default=999)
parser.add_argument('--solution', '-s', type=int, default=999)
parser.add_argument('--pdf', '-p', action='store_true', default=False)
parser.add_argument('--build', '-b', help="Build instead of serve", action='store_true', default=False)

args = parser.parse_args()

print()
print("-" * 78)
if args.build:
    print("Building site...")
else:
    print("Serving using docker. Please, connect to http://127.0.0.1:8000")
    print("Press Ctrl+C to stop server")
print("-" * 78)
print()

cmd = ["docker", "run", "--rm"]
cmd.extend(["--mount", f"type=bind,\"src={os.getcwd()}\",dst=/site"])
cmd.extend(["--workdir", "/site"])
cmd.extend(["--env", f"SELECT_FILE_CONDITION={args.select}"])
cmd.extend(["--env", f"SHOW_SOLUTION={args.solution}"])

if args.pdf:
    cmd.extend(["--env", "ENABLE_PDF_EXPORT=1"])

if args.build:
    cmd.extend([MKDOCS_DOCKER_IMAGE, "build", "-f", f"config/mkdocs.yml", "-d", f"public"])
else:
    cmd.extend(["--publish", "8000:8000"])
    cmd.extend([MKDOCS_DOCKER_IMAGE, "serve", "-f", f"config/mkdocs.yml", "-a", "0.0.0.0:8000"])

p = subprocess.Popen(cmd)
try:
    p.wait()
except KeyboardInterrupt:
    try:
       p.terminate()
    except OSError:
       pass
    p.wait()

print("DONE")
