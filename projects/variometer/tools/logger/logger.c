
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <stdint.h>

#define BAUDRATE B115200
#define MODEMDEVICE "/dev/ttyACM0"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1
        
volatile int STOP=FALSE; 
       
main()
{
    int fd,c, res;
    struct termios oldtio,newtio;
    char buf[255];
    FILE *fp;
        
    fp = fopen ("p_raw.dat", "w");
    fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY ); 

    if (fd <0) {perror(MODEMDEVICE); exit(-1); }
        
    tcgetattr(fd,&oldtio); /* save current port settings */
        
    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;
        
    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;
         
    newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
    newtio.c_cc[VMIN]     = 6;   /* blocking read until 6 chars received */
        
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd,TCSANOW,&newtio);
        
        
    while (1) {       /* loop for input */
        res = read(fd,buf,255);   /* returns after 6 chars have been input */
        buf[res]=0;               /* so we can printf... */
        
        fprintf (fp, "%s", buf);
        printf(".");
    }

    fclose (fp);
    tcsetattr (fd,TCSANOW,&oldtio);
}
