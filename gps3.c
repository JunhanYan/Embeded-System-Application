#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<malloc.h>
#include<time.h>


int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
	struct termios newtio,oldtio;
	if  ( tcgetattr( fd,&oldtio)  !=  0) { 
		perror("SetupSerial 1");
		return -1;
	}
	bzero( &newtio, sizeof( newtio ) );
	newtio.c_cflag  |=  CLOCAL | CREAD;
	newtio.c_cflag &= ~CSIZE;

	switch( nBits )
	{
	case 7:
		newtio.c_cflag |= CS7;
		break;
	case 8:
		newtio.c_cflag |= CS8;
		break;
	}

	switch( nEvent )
	{
	case 'O':
		newtio.c_cflag |= PARENB;
		newtio.c_cflag |= PARODD;
		newtio.c_iflag |= (INPCK | ISTRIP);
		break;
	case 'E': 
		newtio.c_iflag |= (INPCK | ISTRIP);
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
		break;
	case 'N':  
		newtio.c_cflag &= ~PARENB;
		break;
	}

	switch( nSpeed )
	{
	case 2400:
		cfsetispeed(&newtio, B2400);
		cfsetospeed(&newtio, B2400);
		break;
	case 4800:
		cfsetispeed(&newtio, B4800);
		cfsetospeed(&newtio, B4800);
		break;
	case 9600:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	case 115200:
		cfsetispeed(&newtio, B115200);
		cfsetospeed(&newtio, B115200);
		break;
	case 460800:
		cfsetispeed(&newtio, B460800);
		cfsetospeed(&newtio, B460800);
		break;
	default:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	}
	if( nStop == 1 )
		newtio.c_cflag &=  ~CSTOPB;
	else if ( nStop == 2 )
	newtio.c_cflag |=  CSTOPB;
	newtio.c_cc[VTIME]  = 0;
	newtio.c_cc[VMIN] = 0;
	tcflush(fd,TCIFLUSH);
	if((tcsetattr(fd,TCSANOW,&newtio))!=0)
	{
		perror("com set error");
		return -1;
	}
//	printf("set done!\n\r");
	return 0;
}

void createKML(double north,double east){
	char pre[]="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n<Document>\n<Placemark>\n<name>Place Name</name>\n<description>Place Description</description>\n<Point>\n<coordinates>";
	char end[]=",0</coordinates>\n</Point>\n</Placemark>\n</Document>\n</kml>\n";
	int length = 1+sizeof(north)+sizeof(east)+sizeof(pre)+sizeof(end);
	char kmlFile[length];

	sprintf(kmlFile,"%s%lf,%lf%s",pre,north,east,end);
   FILE *fp;
   if(fp = fopen("/home/junhan/Desktop/part4/myDate.kml","w"))
   {
		fwrite(kmlFile,sizeof(kmlFile),1,fp);		
   }
	fclose(fp);
}





time_t DateStrToSec(char* date, char* time)
{
	struct tm tmDate = {0};
	printf("%s\n", date);
	tmDate.tm_year = atoi(date+4)+100;
	printf("%d\n",tmDate.tm_year);
	tmDate.tm_mon = atoi(date+2)/100 - 1;    
	printf("%d\n", tmDate.tm_mon);
	tmDate.tm_mday = atoi(date)/10000;       
	printf("%d\n", tmDate.tm_mday);
	tmDate.tm_hour = atoi(time)/10000;
	printf("%d\n", tmDate.tm_hour);
	tmDate.tm_min = atoi(time+2)/100;
	printf("%d\n", tmDate.tm_min);
	tmDate.tm_sec = atoi(time+4);
	printf("%d\n", tmDate.tm_sec);
	return mktime(&tmDate);

}
int split(char *str, const char *del)
{
	char *s; 
	double north;
	double east;
	s=strtok(str,del);
	if(strcmp("$GPRMC",s)==0){
		strtok(NULL,del);
		strtok(NULL,del);
		north=atof(strtok(NULL,del))/100;
		strtok(NULL,del);
		east = atof(strtok(NULL,del))/100;
		createKML(north,east);
		return 0;
	}
	else
		return 1;

}


int	concat_command(char *cmd, char c)
{
	int i=0;
	if (c == '\n')
	{
		split(cmd,",");
		memset(cmd, '\0', 256);
		return (0);
	}
	while (cmd[i] != '\0')
	{
		i = i + 1;
	}
	cmd[i] = c;
	return (0);
}
int main(void)
{
	int fd1,fd2,nset1,nset2,nread;
	char buf;
	char buff[256];
	fd2 = open("/dev/ttySAC1", O_RDONLY);
//	fd2 = open( "/home/junhan/Desktop/part4/test1", O_RDWR);  //for ok6410-B GPS module
	if (fd2 == -1)
		exit(1);
	nset2 = set_opt(fd2, 4800, 8, 'N', 1);
	if (nset2 == -1)
		exit(1);  //使用的时候取消注释
	memset(buff, '\0', 256);
	while(1)
	{	
		nread = read(fd2, &buf, 1);
		if(nread > 0){
		concat_command(buff,buf);
		if(buf == 'q')
			break;
		}
	}


	close(fd2);

	return 0;

}


