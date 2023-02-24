fd_set fd_in, fd_out;
FD_ZERO(&fd_in);
FD_ZERO(&fd_out);

// monitor fd1 for input events and fd2 for output events
FD_SET(fd1, &fd_in);
FD_SET(fd2, &fd_out);

// find out which fd has the largest numeric value
int largest_fd = (fd1 > fd2) ? fd1 : fd2;

// wait up to 5 seconds
struct timeval tv = {
    .tv_sec  = 5,
    .tv_usec = 0,
};

// wait for events
int ret = select(largest_fd + 1, &fd_in, &fd_out, NULL, &tv);

// check if select actually succeed
if (ret == ‐1) {
    // report error and abort
} else if (ret == 0) {
    // timeout; no event detected
} else {
    if (FD_ISSET(fd1, &fd_in)) {
        // input event on sock1
    }
    if (FD_ISSET(fd2, &fd_out)) {
        // output event on sock2
    }
}