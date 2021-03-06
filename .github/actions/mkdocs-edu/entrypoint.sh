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

# Install mkdocs from .devcontainer in a sub-shell
cp -a .devcontainer/mkdocs-edu /
(cd /mkdocs-edu && poetry config virtualenvs.create false && poetry install)

python --version
mkdocs --version

echo "Filtering pages"
for i in $(find docs -name pages.j2); do
    echo "Processing $i"
    jinja2 -D week=$week -o $(dirname $i)/.pages $i
done

echo "Building site"
export SHOW_SOLUTION=$solutions
mkdocs build --clean --config-file ${config} --site-dir ${site_dir}
