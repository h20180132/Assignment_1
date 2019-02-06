#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main()
{
int fd=0;
char buf[10];
int t;
char axis;
printf("Enter the axis: x or y or z");
scanf("%c",&axis);
switch(axis)
{
case 'x':   
        fd=open("/dev/adxl_x",O_RDONLY);
	break;
case 'y':
	fd=open("/dev/adxl_y",O_RDONLY);
	break;
case 'z':
	fd=open("/dev/adxl_z",O_RDONLY);
	break;
default: 
	printf("Choose a valid axis");
}
read(fd, buf,sizeof(buf));
for (t=0;t<10;t++)  //printing array
{
	printf("%u",buf[t]);
}
close(fd);
return 0;
}
