
#ifndef SERIAL_H
#define SERIAL_H

// #include <linux/types.h>
// #include <linux/rtc.h>

#include <stdio.h>
// #include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#ifndef WIN32
#include <unistd.h>
#include <termios.h>
#endif
// #include <sys/types.h>
// #include <sys/ioctl.h>
#include <errno.h>
#include <string.h>


void serial_close(int fd);

int serial_open(unsigned char* dev, unsigned int baud);

int serial_read_data(int fd, unsigned char *val, int len);

int serial_write_data(int fd, unsigned char *val, int len);


#endif
