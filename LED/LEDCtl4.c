#include<stdlib.h>
#include<unistd.h>

#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#define CMD_ON 0
#define CMD_OFF 1
int main()
{
	int pwmfd;
	pwmfd=open("/dev/pwm",O_WRONLY);
	if(pwmfd<0){
		perror("open device key leds");
		exit(1);
	}
	ioctl(pwmfd,CMD_ON);
	sleep(1);
	int i;
	for(i=0;i<5;i++){
				
		ioctl(pwmfd,CMD_OFF,i*1000+1000);
		sleep(2);

		
		
	}
	
	ioctl(pwmfd,CMD_ON);



	close(pwmfd);
}
