/**
 * Copyright 2015 University of Applied Sciences Western Switzerland / Fribourg
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Module:  MA-CSEL1 - Building Embedded Systems under Linux
 *
 * Abstract:    Introduction to the Embedded Linux Development Environment
 *
 * Purpose: Core dump demo program.
 *      Before to call this demo program don't forget set
 *              $ ulimit -c 10000
 *
 * Autĥor:  Daniel Gachet
 * Date:    28.08.2015
 */

#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/mman.h>
#include <unistd.h>

int main()
{
    /* open memory file descriptor */
    int fd = open("/dev/mymodule", O_RDWR);
    if (fd < 0) {
        printf("Could not open /dev/mem: error=%i\n", fd);
        return -1;
    }

    /* map to user space APF27 FPGA registers */
    volatile uint32_t* regs =
        mmap(0, 0x100, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (regs == (void*)-1) {
        printf("mmap failed, error: %i:%s \n", errno, strerror(errno));
        return -1;
    }

    uint32_t chipid[4] = {
        [0] = *(regs + 0x200 / sizeof(uint32_t)),
        [1] = *(regs + 0x204 / sizeof(uint32_t)),
        [2] = *(regs + 0x208 / sizeof(uint32_t)),
        [3] = *(regs + 0x20c / sizeof(uint32_t)),
    };

    printf("NanoPi NEO Plus2 chipid=%08x'%08x'%08x'%08x\n",
           chipid[0],
           chipid[1],
           chipid[2],
           chipid[3]);

    munmap((void*)chipid, 0x100);
    close(fd);

    return 0;
}
