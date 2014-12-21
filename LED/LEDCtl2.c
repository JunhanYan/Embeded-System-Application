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
	ioctl(fd,CMD_ON,2);
	sleep(30);
	int i;
	for(i=0;i<3;i++){
		
		ioctl(fd,CMD_OFF,2);
		sleep(1);

		ioctl(fd,CMD_ON,2);
		sleep(1);
	}
	
	ioctl(fd,CMD_OFF,2);

	ioctl(fd,CMD_ON,1);
	sleep(4);

	ioctl(fd,CMD_OFF,1);

	ioctl(fd,CMD_ON,0);
	sleep(30);
	for(i=0;i<3;i++){
		
		ioctl(fd,CMD_OFF,i);

	}
	close(fd);
}
