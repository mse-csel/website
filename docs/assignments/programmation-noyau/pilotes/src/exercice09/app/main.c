#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "../drv/skeleton.h"

int main(int argc, char* argv[])
{
    if (argc <= 1) return 0;
    char* f_name = argv[1];

    /* open memory file descriptor */
    int fd = open(f_name, O_RDWR);
    if (fd < 0) {
        printf("Could not open /dev/mymodule: error=%i\n", fd);
        return -1;
    }

    int io;
    struct skeleton_config w = {
        .id    = 10,
        .ref   = 20,
        .name  = "a name",
        .descr = "a description",
    };
    struct skeleton_config r;

    io = ioctl(fd, SKELETON_IO_RESET);
    printf("  SKELETON_IO_RESET   io=%d\n", io);
    io = ioctl(fd, SKELETON_IO_RD_REF, &r);
    printf("  SKELETON_IO_RD_REF  io=%d, r=%s,%s,%d,%ld\n",
           io,
           r.name,
           r.descr,
           r.id,
           r.ref);
    io = ioctl(fd, SKELETON_IO_WR_REF, &w);
    printf("  SKELETON_IO_WR_REF  io=%d, r=%s,%s,%d,%ld\n",
           io,
           w.name,
           w.descr,
           w.id,
           w.ref);
    io = ioctl(fd, SKELETON_IO_RD_REF, &r);
    printf("  SKELETON_IO_RD_REF  io=%d, r=%s,%s,%d,%ld\n",
           io,
           r.name,
           r.descr,
           r.id,
           r.ref);
    io = ioctl(fd, SKELETON_IO_RD_VAL);
    printf("  SKELETON_IO_RD_VAL  io=%d, r=%d\n", io, io);
    io = ioctl(fd, SKELETON_IO_WR_VAL, 10);
    printf("  SKELETON_IO_WR_VAL  io=%d, r=%d\n", io, 10);
    io = ioctl(fd, SKELETON_IO_RD_VAL);
    printf("  SKELETON_IO_RD_VAL  io=%d, r=%d\n", io, io);

    io = ioctl(fd, SKELETON_IO_RESET);
    printf("  SKELETON_IO_RESET   io=%d\n", io);
    io = ioctl(fd, SKELETON_IO_RD_REF, &r);
    printf("  SKELETON_IO_RD_REF  io=%d, r=%s,%s,%d,%ld\n",
           io,
           r.name,
           r.descr,
           r.id,
           r.ref);
    io = ioctl(fd, SKELETON_IO_RD_VAL);
    printf("  SKELETON_IO_RD_VAL  io=%d, r=%d\n", io, io);

    io = ioctl(fd, 10);
    printf("  10                  io=%d\n", io);

    close(fd);

    return 0;
}
