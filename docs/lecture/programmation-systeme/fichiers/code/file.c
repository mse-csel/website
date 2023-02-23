char buf[100];
while (1) {
    ssize_t nr = read(fd, buf, sizeof(buf));
    if (nr == 0) break;  // --> all data have been read
    if (nr == -1) {
        if (errno == EINTR) continue;  // --> continue reading
        char estr[100] = {
            [0] = 0,
        };
        strerror_r(errno, estr, sizeof(estr) - 1);
        fprintf(stderr, "ERROR: %s\n", estr);
        break;  // --> error: stop reading
    }
    /* process read data */
}