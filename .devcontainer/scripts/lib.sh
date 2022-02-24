filter_pages() {
    for i in $(find docs -name .pages.j2); do
        jinja2 -D week=$1 -o $(dirname $i)/$(basename $i .j2) $i
    done
}
