#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	struct termios sav_conf;
	struct termios raw_conf;
	char	c;

	tcgetattr(STDOUT_FILENO, &sav_conf);
	raw_conf = sav_conf;
	raw_conf.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDOUT_FILENO, TCSANOW, &raw_conf);
	read(STDIN_FILENO, &c, 1);
	//write(STDOUT_FILENO, &c, 1);
	//printf("c is %d\n", c);
	tcsetattr(STDOUT_FILENO, TCSANOW, &sav_conf);
	return (0);
}
