import re
import os
from pathlib import Path


def cleanup(path):
    content = None
    with open(path, "rt") as f:
        content = f.read()
    if content is None:
        raise Exception("ERROR")
    orig = content
    content = re.sub(r"p\.\s*ex\.?", "par exemple", content)

    if orig != content:
        print(orig)


def main():
    for root, dirs, files in os.walk("docs"):
        for f in files:
            if not f.endswith(".md"):
                continue
            cleanup(Path(root) / f)


if __name__ == "__main__":
    main()
