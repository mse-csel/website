#!/bin/bash -l

set -o errexit
set -o pipefail
set -o nounset
# set -o xtrace

# Set magic variables for current file & dir
__dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
__file="${__dir}/$(basename "${BASH_SOURCE[0]}")"
__base="$(basename ${__file} .sh)"

site_dir="${1:-public}"
config="${2:-config/mkdocs.yml}"
week="${3:-999}"
solutions="${4:-999}"

python --version
mkdocs --version

for i in $(find docs -name .pages.j2); do
    jinja2 -D week=$week -o $(dirname $i)/$(basename $i .j2) $i
done

export SHOW_SOLUTION=$solutions
mkdocs build --clean --config-file ${config} --site-dir ${site_dir}
