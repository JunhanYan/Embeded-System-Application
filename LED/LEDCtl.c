#include<stdlib.h>
#include<unistd.h>

#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#define CMD_ON 0
#define CMD_OFF 1
int main()
{
	int fd;
	fd=open("/dev/leds",O_WRONLY);
	if(fd<0){
		perror("open device leds");
		exit(1);
	}
	int i;
	for(i=0;i<4;i++){
		ioctl(fd,CMD_ON,i);
	}
	sleep(3);
	for(i=0;i<4;i++){
		ioctl(fd,CMD_OFF,i);
	}
	sleep(3);
	
	for(i=0;i<4;i++){
		ioctl(fd,CMD_ON,i);
		sleep(1);
		ioctl(fd,CMD_OFF,i);
	}

	close(fd);
}
