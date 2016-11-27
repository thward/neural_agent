#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<fcntl.h>
#include<termios.h>

int main() {    

    int STATE_OK=0;
    int STATE_WARNING=1;
    int STATE_CRITICAL=2; 
    char tempbuf[30];
    int w = 0;
    int n = 0;
    int iterator = 0;
    
    struct termios tty;

    int fd=open("/dev/rfcomm0",O_RDWR | O_NOCTTY);
    if(fd == -1)
    {
      printf("Unable to open /dev/rfcomm0\n");
      return STATE_WARNING;
    }
    else
    {
      if(tcgetattr(fd, &tty)!=0){
	perror("tcgetatt() error");
      }
      else{
	
	cfsetospeed(&tty, B9600);
	cfsetispeed(&tty, B9600);

	tty.c_cflag &= ~PARENB;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CSIZE;
	tty.c_cflag |= CS8;
	tty.c_cflag &= ~CRTSCTS; 
	tty.c_cflag |= CLOCAL | CREAD;

	tty.c_iflag |= IGNPAR | IGNCR;
	tty.c_iflag &= ~(IXON | IXOFF | IXANY);
	tty.c_lflag |= ICANON;
	tty.c_oflag &= ~OPOST;
	tcsetattr(fd, TCSANOW, &tty);

	while (iterator < 100){
	
	tcflush(fd,TCIOFLUSH);
	w=write(fd, "36", 2);/*printf("%d\n",w);
	fprintf(stderr, "fd = %d.\n", fd);*/
	usleep(500000);
	//sleep(1);
               
	strcpy(tempbuf, "                             ");
	n=read(fd,tempbuf,29);/*printf("%d \n",n);*/
	tempbuf[29]= '\0';
	printf("tempbuf: %s ",tempbuf);
		
	strcpy(tempbuf, "                             ");
	n=read(fd,tempbuf,29);/*printf("%d \n",n);*/
	tempbuf[29]= '\0';
	printf("tempbuf: %s ",tempbuf);
                
	strcpy(tempbuf, "                             ");
	n=read(fd,tempbuf,29);/*printf("%d \n",n);*/
	tempbuf[29]= '\0';
	printf("tempbuf: %s ",tempbuf);
	
	
	//sleep(1);
	
		
	tcflush(fd,TCIOFLUSH);
	w=write(fd, "25", 2);/*printf("%d\n",w);
	fprintf(stderr, "fd = %d.\n", fd);*/
	usleep(500000);
	//sleep(1);
		
	strcpy(tempbuf, "                             ");
	n=read(fd,tempbuf,29);/*printf("%d \n",n);*/
	tempbuf[29]= '\0';
	printf("tempbuf: %s ",tempbuf);
            
	strcpy(tempbuf, "                             ");
	n=read(fd,tempbuf,29);/*printf("%d \n",n);*/
	tempbuf[29]= '\0';
	printf("tempbuf: %s ",tempbuf);
		
	strcpy(tempbuf, "                             ");
	n=read(fd,tempbuf,29);/*printf("%d \n",n);*/
	tempbuf[29]= '\0';
	printf("tempbuf: %s ",tempbuf);
	
	//sleep(1);
	
	iterator ++;
	}
		
		
	close(fd);
		
	//printf("finished \n");  
		
		
      }
    }
    close(fd);
    return 0;
}



