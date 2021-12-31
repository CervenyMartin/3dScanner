#include <stdio.h>
#include <string.h>
#include<fcntl.h>
#include<errno.h>
#include<termios.h>
#include<unistd.h>
int serial_port = open("/dev/ttyUSB0",0_RDWR);
