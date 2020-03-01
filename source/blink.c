#include "include/libgpio.h"

unsigned int getSleepTime(int);

int blinkLED(struct gpio *gpio, int repeat, int speed)
{
	int i = 0 ;
	int value = -1;
	unsigned int sleeptime = 0;
	errno = 0;

	sleeptime = getSleepTime(speed);

	while(i < repeat) {
		if((value = getValue(gpio)) != -1) {
			switch(value) {
				case 0:
					if(setValue(gpio, 1) < 0) {
						return -1;
					}
					break;
				case 1:
					if(setValue(gpio, 0) < 0) {
						return -1;
					}
			}
		}
		if(usleep(sleeptime) != 0) {
			perror("sleep timer failed");
			return -1;
		}
		i++;
	}	
	return 0;
}

unsigned int getSleepTime(int speed)
{
	switch(speed) {
		case 0:
			return 0;
		case 1:
			return 1000000;
		case 2:
			return 500000;
		case 3:
			return 250000;
		case 4:
			return 125000;
		case 5:
			return 50000;
	}
	return -1;
}
