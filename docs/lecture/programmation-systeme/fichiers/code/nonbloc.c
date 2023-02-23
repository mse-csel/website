char buf[100];
ssize_t nr;
while (1) {
    nr = read(fd, buf, sizeof(buf));
    if (nr >= 0) break;
    if (errno == EINTR) continue;  // --> read again
    if (errno == EAGAIN) break;    // --> resubmit later
    perror("ERROR");
    break;  // --> error: stop reading
}
if (nr > 0)
// à process read data