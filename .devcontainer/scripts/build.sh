#!/usr/bin/env bash

set -o errexit
set -o pipefail
set -o nounset
# set -o xtrace

export SELECT_WEEK="999"
export SHOW_SOLUTION="999"
export ENABLE_PDF_EXPORT=0

# Set magic variables for current file & dir
__dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
__file="${__dir}/$(basename "${BASH_SOURCE[0]}")"
__base="$(basename ${__file} .sh)"
__root="$(cd "$(dirname "${__dir}")" && pwd)"

. $__dir/lib.sh

while getopts "w:s:" opt; do
    case ${opt} in
    w)
        SELECT_WEEK=$OPTARG
        ;;
    s)
        SHOW_SOLUTION=$OPTARG
        ;;
    \?)
        echo "Usage: serve [-c condition] [-s solution]" 1>&2
        exit 1
        ;;
    :)
        echo "Invalid option: $OPTARG requires an argument" 1>&2
        exit 2
        ;;
    esac
done

filter_pages $SELECT_WEEK
mkdocs build -f config/mkdocs.yml -d ../public
