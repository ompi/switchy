#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

void main() {
   int fd;
   fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);

   struct termios options;
   tcgetattr(fd, &options);
   cfsetispeed(&options, B115200);
   cfsetospeed(&options, B115200);
   tcsetattr(fd, TCSANOW, &options);
  
   int RTS_flag;
   int DTR_flag;
   RTS_flag = TIOCM_RTS;
   DTR_flag = TIOCM_DTR;
   ioctl(fd,TIOCMBIC,&DTR_flag);
   usleep(1000);
   ioctl(fd,TIOCMBIC,&RTS_flag);

   while(1) {
     char buf[256];
     int len;
     len = read(fd, buf, 256);
     if (len != -1) {
       write(STDOUT_FILENO, buf, len);
     }
   }

   close(fd);
}

