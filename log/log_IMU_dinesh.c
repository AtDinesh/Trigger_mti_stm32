
#include <stdlib.h>
#include <stdio.h>
//~ #include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <termios.h>
//~ #include <errno.h>
#include <sys/ioctl.h>
//~ #include <stdint.h>
#include <sys/time.h>

//#define PRINT_RAW_IMU_HL
//#define PRINT_NORMALIZED_HL
//#define PRINT_RAW_IMU_LH
//#define PRINT_NORMALIZED_LH
//#define RAW_IMU_HL
#define NORMALIZED_HL
//#define RAW_IMU_LH
//#define NORMALIZED_LH

typedef int bool;
#define true 1
#define false 0

int main(int argc, char *argv[])
{
  bool save_file = true;
  struct timeval tv, t_loop;
  struct timeval start_tv, start_loop;
  int fd,n;
  int i;
  FILE * logFile;

  double current_time;
  double process_time;
  struct termios toptions;
  int time_stop = 10;
  /* open serial port */
  printf("open port...\n");
  fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (fd != -1)
	printf("open ok\n");
  else printf("open() unsuccessful\n");


  tcgetattr(fd, &toptions);

  //cfsetispeed(&toptions, B230400);
  //cfsetospeed(&toptions, B230400);
  cfsetispeed(&toptions, B1000000);
  cfsetospeed(&toptions, B1000000);
  toptions.c_cflag     |= (CLOCAL | CREAD);		// turn on READ & ignore ctrl lines
  toptions.c_lflag     &= ~(ICANON | ECHO | ECHOE | ISIG);
  toptions.c_oflag     &= ~OPOST;
  toptions.c_cc[VMIN]  = 0;			// read doesn't block if 1
  toptions.c_cc[VTIME] = 0;		// 0.n tenth of a second elapses between bytes
/*VMIN = 0 and VTIME = 0
This is a completely non-blocking read - the call is satisfied immediately directly from the driver's input queue. If data are available, it's transferred to the caller's buffer up to nbytes and returned. Otherwise zero is immediately returned to indicate "no data". We'll note that this is "polling" of the serial port, and it's almost always a bad idea. If done repeatedly, it can consume enormous amounts of processor time and is highly inefficient.
*/ 
  tcsetattr(fd, TCSANOW, &toptions);
 printf("begin acquisition\n");
 if(fd != -1){
  gettimeofday(&start_tv, NULL);
  while(1)
  {
		unsigned char buf[1] = {0};
		gettimeofday(&start_loop, NULL);
   	read(fd, buf, 1);
		printf("%c", buf[0]);
      
    gettimeofday(&tv, NULL);
    current_time = (tv.tv_sec - start_tv.tv_sec) +  (tv.tv_usec - start_tv.tv_usec) / 1000000.0;
		//fflush(stdin);
     if (current_time > time_stop) break;
  }
	printf("\n");
  close(fd);
 }
else printf("could not connect to arduino. Leaving the program.\n");

  return 0;  
}
