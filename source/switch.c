#include "include/libgpio.h"

int stopSignal(int size, int on[size]);

/**
 * @brief	simple polling on equal amount of switches and leds
 *
 * On switch activation the assigned LED toggles
 * Stop execution when all input switches are activated
 *
 * @param[in]	input	array of gpio struct instance pointers(switches)
 * @param[in]	output	array of gpio struct instance pointers(LEDs)
 * @param[in]	ni	amount of input gpios
 * @param[in]	no	amount of output gpios
 *
 * @retval	0	successful execution
 * @retval	-1	Error encountered
 */
int pollSwitch(struct gpio *input[], struct gpio *output[], int ni, int no)
{
	int run = 1;
	int switch_on[ni];
	int value = 0;

	memset(switch_on, 0, ni);

	if(ni != no) {
		fprintf(stderr, "Amount switches must be equal to amount LEDs(%d:%d)\n",
				ni, no);
		return -1;
	}
	while(run) {
		for(int i = 0 ; i < ni ; i++) {
			if(input[i] == NULL || output[i] == NULL) {
				fprintf(stderr, "gpio number %d : Null pointer", i);
				return -1;
			}
			if((value=getValue(input[i])) == -1) {
				fprintf(stderr, "Error get value from %s\n", input[i]->path);
				return -1;
			}

			if(value == ON) {
				switch_on[i] = 1;
				//if(toggleValue(output[i]) == -1) {
				//	return -1;
				//}
				if(blinkLED(output[i], 8, 3) < 0) {
					return -1;
				}
			}
			else if(value == OFF) {
				switch_on[i] = 0;
			}
		}	
		if(stopSignal(ni, switch_on)) {
			run = 0;
		}
		memset(switch_on, 0, ni);

		if(usleep(250000) < 0) {
			return -1;
		}
	}
	return 0;
}

int stopSignal(int size, int on[size])
{
	for(int i = 0 ; i < size ; i++) {
		if(on[i] < 1) {
			return 0;
		}
	}
	return 1;
}
