#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/poll.h>
#include <string.h>
#include <sys/types.h>

#define FILE_NAME "/dev/btn"

int main(int argc, char **argv)
{
	int fd,res;
	struct pollfd fds[2];

	fd = open(FILE_NAME, O_RDWR);
	if (fd < 0)
	{
		perror("open");
	 	exit(-1);
	}
	while (1)
	{
		memset(fds, 0, sizeof(struct pollfd));
		fds[0].fd = fd;
		fds[0].events = POLLIN;
		fds[0].revents=0;
		
		fds[1].fd = 0;
		fds[1].events = POLLIN;
		fds[1].revents = 0;

//		res = poll(fds, 1, 3000);
//		res = poll(fds, 1, 0);
		res = poll(fds, 2, -1);
		
		if (res == 0)
		{
			printf("app: poll timeout!\n");
		}
		else if(res < 0)
		{
			perror("app: poll");
			exit(-1);
		}
		else if(res > 0)
		{
			
			int i=0;
			int count=0;
			while(i!=2&&count!=res)
			{
				if((fds[i].revents & fds[i].events)==fds[i].events)
				{
					char key_value[1024];
					int recive_count=0;
					recive_count=read(fds[i].fd,key_value,sizeof(key_value));
					key_value[recive_count]='\0';
					switch(i)
					{
						case 0:
							printf("your pressed key is:%d\n",key_value[0]);
						break;
						
						case 1:
							if(!strncmp(key_value,"quit",4))
							{
								return 0;
							}
						break;
						
						default:
							
						break;
					}
					fds[i].revents=0;
					count++;
				}
				i++;
			}
			
			
		}
	}
	return 0;
}

