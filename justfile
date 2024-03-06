prefix := "/"

serve-now:
    #!/usr/bin/env bash
    poetry run mkdocs serve

serve-all:
    #!/usr/bin/env bash
    set -euxo pipefail
    CALENDAR_TODAY="today in 10 year" poetry run mkdocs serve

archive YEAR:
    #!/usr/bin/env bash
    set -euxo pipefail
    target="csel-website-{{ YEAR }}.wzip"
    rm -Rf public
    rm $target || true
    CALENDAR_TODAY="today in 10 year" poetry run mkdocs build -d public -f mkdocs.yml
    echo {{ prefix }} >public/.prefix
    cd public && zip -FS -r ../$target . && cd ..
