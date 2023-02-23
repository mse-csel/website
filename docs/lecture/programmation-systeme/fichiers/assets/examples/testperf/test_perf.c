/**
 * Copyright 2014 University of Applied Sciences Western Switzerland / Fribourg
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
 * Project:	EIA-FR / Embedded Systems 4 Laboratory
 *
 * Abstract: 	P.03 - Exercice 2 - Regular File Handling
 *
 * Purpose:	This module will test performance differences between
 *		File-I/O and Standard-I/O operations.
 *
 * Autĥor:	Daniel Gachet
 * Date:	06.03.2015
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define TEST_TEXT "That's only a simple text line to test the performances"  \
		  "of buffered i/o operations versus standard i/o operations\n"

#define TEST_FILE "test_perf.temp"

/**
 * routine to test performance of Linux file-io services
 *
 *@param sz size of chunck of memory to read
 */
static int get_fio (size_t sz)
{
	char str[sz];
	int c = 0;
	struct timespec start_time;
	struct timespec stop_time;

	int fd = open (TEST_FILE, O_RDONLY);
	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &start_time);
	do {
		c = read (fd, str, sz);
	} while (c != 0);
	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &stop_time);
	close (fd);

	return	(stop_time.tv_nsec - start_time.tv_nsec) / 1000 +
		(stop_time.tv_sec - start_time.tv_sec) * 1000000;
}

/**
 * routine to test performance of Linux file-io services on write
 *
 *@param sz size of chunck of memory to write
 *@param len number of bytes to write
 */
static int set_fio (size_t sz, size_t len)
{
	struct timespec start_time;
	struct timespec stop_time;
    char block[sz];
    memset (block, 'a', sz);

	int fd = open (TEST_FILE"_fio", O_WRONLY | O_CREAT, 0666);
	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &start_time);
	while (len > 0) {
		write (fd, block, sz);
		len = len > sz ? len - sz : 0;
	}
	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &stop_time);
	close (fd);

	return	(stop_time.tv_nsec - start_time.tv_nsec) / 1000 +
		(stop_time.tv_sec - start_time.tv_sec) * 1000000;
}


/**
 * routine to test performance of standard-io services
 * this routine will test fgetc() method (character based)
 */
static int get_byte_sio()
{
	int c = 0;
	struct timespec start_time;
	struct timespec stop_time;

	FILE* stream = fopen (TEST_FILE, "r");
	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &start_time);
	do {
		c = fgetc (stream);
	} while (c > 0);
	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &stop_time);
	fclose (stream);

	return	(stop_time.tv_nsec - start_time.tv_nsec) / 1000 +
		(stop_time.tv_sec - start_time.tv_sec) * 1000000;
}


/**
 * routine to test performance of standard-io services
 * this routine will test fgetc() method (character based)
 *
 *@param len number of bytes to write
 */
static int set_byte_sio(size_t len)
{
	struct timespec start_time;
	struct timespec stop_time;

	FILE* stream = fopen (TEST_FILE"_sio", "w");
	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &start_time);
	while (len-- > 0) {
		fputc ('c', stream);
	}
	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &stop_time);
	fclose (stream);

	return	(stop_time.tv_nsec - start_time.tv_nsec) / 1000 +
		(stop_time.tv_sec - start_time.tv_sec) * 1000000;
}


/**
 * routine to test performance of standard-io services
 * this routine will test fgets() method (line based)
 */
static int get_line_sio()
{
	char str[1000];
	char* c = 0;
	struct timespec start_time;
	struct timespec stop_time;

	FILE* stream = fopen (TEST_FILE, "r");
	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &start_time);
	do {
		c = fgets (str, sizeof(str), stream);
	} while (c != 0);
	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &stop_time);
	fclose (stream);

	return	(stop_time.tv_nsec - start_time.tv_nsec) / 1000 +
		(stop_time.tv_sec - start_time.tv_sec) * 1000000;
}


/**
 * routine to test performance of standard-io services
 * this routine will test fputs() method (line based)
 *
 *@param len number of bytes to write
 */
static int set_line_sio(size_t len)
{
	struct timespec start_time;
	struct timespec stop_time;

	FILE* stream = fopen (TEST_FILE"_sio", "w");
	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &start_time);
	while (len > 0) {
		fputs (TEST_TEXT, stream);
		len = len > strlen(TEST_TEXT) ? len - strlen(TEST_TEXT) : 0;
	};
	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &stop_time);
	fclose (stream);

	return	(stop_time.tv_nsec - start_time.tv_nsec) / 1000 +
		(stop_time.tv_sec - start_time.tv_sec) * 1000000;
}


/**
 * routine to test performance of standard-io services
 * this routine will test fread() method (block based)
 *
 *@param sz size of chunck of memory to read
 */

static int get_block_sio (size_t sz)
{
	char str[sz];
	ssize_t c = 0;
	struct timespec start_time;
	struct timespec stop_time;

	FILE* stream = fopen (TEST_FILE, "r");
	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &start_time);
	do {
		c = fread (str, sz, 1, stream);
	} while (c != 0);
	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &stop_time);
	fclose (stream);

	return  (stop_time.tv_nsec - start_time.tv_nsec) / 1000 +
		(stop_time.tv_sec - start_time.tv_sec) * 1000000;
}


/**
 * routine to test performance of standard-io services
 * this routine will test fread() method (block based)
 *
 *@param sz size of chunck of memory to read
 */

static int set_block_sio (size_t sz, size_t len)
{
	char str[sz];
	struct timespec start_time;
	struct timespec stop_time;

	memset (str, 'a', sz);
	FILE* stream = fopen (TEST_FILE"_sio", "w");
	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &start_time);
	while (len > 0) {
		fwrite (str, sz, 1, stream);
		len = len > sz ? len - sz : 0;
	};
	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &stop_time);
	fclose (stream);

	return  (stop_time.tv_nsec - start_time.tv_nsec) / 1000 +
		(stop_time.tv_sec - start_time.tv_sec) * 1000000;
}


/**
 * print application usage
 */
static void print_usage()
{
	printf ("Usage: test_perf [OPTIONs]\n"
		"Test performances between standard-i/o and buffered-i/o calls\n"
		"Options:\n"
		"  -h   this message\n"
		"  -f   create test file (" TEST_FILE ")\n"
		"  -c   test byte access\n"
		"  -l   test line access\n"
		"  -b <size> test block (block size)\n"
	);
}

/**
 * main program...
 */
int main(int argc, char* argv[])
{
	printf ("File-I/O vs Standard-I/O performance test...\n");

	// parse fonction arguments
	int sz = -1;
	bool byte = false;
	bool line = false;
	bool file = false;
	while(1) {
		int op = getopt (argc, argv, "hfclb:");
		if (op == -1) break;
		switch (op) {
		case 'h':
			print_usage();
			return 0;
		case 'c':
			byte = true;
			break;
		case 'l':
			line = true;
			break;
		case 'f':
			file = true;
			break;
		case 'b':
			errno = 0;
			sz = strtol (optarg, 0, 10);
			if (errno != 0) {
				fprintf (stderr, "ERROR: invalid block size value.\n");
				exit (1);
			}
			break;
		default:
			fprintf (stderr, "ERROR: invalid option.\n");
			exit(1);
		}
	}
	printf ("options: c=%s, l=%s, b=%d\n",
		byte ? "true" : "false", line ? "true" : "false", sz);

	/* create test file on request */
	if (file) {
		printf ("create test file...\n");
		int fd = open (TEST_FILE, O_WRONLY|O_CREAT|O_TRUNC, 0664);
		for (int i=10000; i>0; i--)
			write (fd, TEST_TEXT, strlen (TEST_TEXT));
		write (fd, "", 1);
		close (fd);
	}

	/* test character access */
	if (byte) {
		printf ("test byte access:\n");
		int elapsed_sio = get_byte_sio();
		int elapsed_fio = get_fio (1);
		printf ("  --> elapsed-r: sio=%6dus, fio=%8dus, factor=%3d\n",
			elapsed_sio, elapsed_fio, elapsed_fio / elapsed_sio);

		elapsed_sio = set_byte_sio(1000000);
		elapsed_fio = set_fio (1, 1000000);
		printf ("  --> elapsed-w: sio=%6dus, fio=%8dus, factor=%3d\n",
			elapsed_sio, elapsed_fio, elapsed_fio / elapsed_sio);
	}

	/*test line access */
	if (line) {
		printf ("test line access:\n");
		int elapsed_sio = get_line_sio();
		int elapsed_fio = get_fio (strlen(TEST_TEXT));
		printf ("  --> elapsed-r: sio=%6dus, fio=%8dus, factor=%3d\n",
			elapsed_sio, elapsed_fio, elapsed_fio / elapsed_sio);

		elapsed_sio = set_line_sio(1000000);
		elapsed_fio = set_fio (strlen(TEST_TEXT), 1000000);
		printf ("  --> elapsed-w: sio=%6dus, fio=%8dus, factor=%3d\n",
			elapsed_sio, elapsed_fio, elapsed_fio / elapsed_sio);
	}


	/* test block access */
	if (sz > 0) {
		printf ("test block access:\n");
		int elapsed_sio = get_block_sio(sz);
		int elapsed_fio = get_fio (sz);
		int u=elapsed_fio / elapsed_sio;
		int f=(elapsed_fio*100) / elapsed_sio;
		printf ("  --> elapsed-r: sio=%6dus, fio=%8dus, factor=%2d.%02d\n",
			elapsed_sio, elapsed_fio, u,f);

		elapsed_sio = set_block_sio(sz, 1000000);
		elapsed_fio = set_fio (sz, 1000000);
		u=elapsed_fio / elapsed_sio;
		f=(elapsed_fio*100) / elapsed_sio;
		printf ("  --> elapsed-w: sio=%6dus, fio=%8dus, factor=%2d.%02d\n",
			elapsed_sio, elapsed_fio, u,f);
	}

	return 0;
}
