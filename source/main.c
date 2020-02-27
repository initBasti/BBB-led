#include <inttypes.h>
#include "include/pin_setting.h"
#include "include/blink.h"

int main(int argc, char*argv[])
{
	char gpio_id[3] = {0};
	char gpio_path[PATH_MAX-PATH_ADDITION] = {0};
	unsigned long repeats = 0;
	unsigned long speed = 0;
	int value = -1;
	errno = 0;
	if(argc == 4) {
		if(strnlen(argv[1], 5) > 3) {
			fprintf(stderr,
					"valid gpio ID is shorter or equal to 3 letters\n");
			return EXIT_FAILURE;	
		}
		strncpy(gpio_id, argv[1], 3);
		repeats = strtoumax(argv[2], NULL, 10);
		if(repeats == UINTMAX_MAX && errno == ERANGE) {
			fprintf(stderr, "Conversion of %s to integer failed", argv[2]);
		}
		speed = strtoumax(argv[3], NULL, 10);
		if(speed == UINTMAX_MAX && errno == ERANGE) {
			fprintf(stderr, "Conversion of %s to integer failed", argv[3]);
		}
	}
	else {
		fprintf(stderr, "Usage %s <gpio_id> <repeats> <speed>\n", argv[0]);
		return EXIT_FAILURE;
	}

	printf("GPIO: %s\t\tREPEATS: %ld\t\tSPEED: %ld\n", gpio_id, repeats, speed);

	if(buildPath("/sys/class/gpio", gpio_id, gpio_path) < 0) {
		fprintf(stderr, "Invalid path: %s\n", gpio_path);
		return EXIT_FAILURE;	
	}

	if((value=getValue(gpio_path, "direction")) != -1) {
		if(value == 0) {
			if(setValue(gpio_path, "direction", "out") < 0) {
				EXIT_FAILURE;
			}
		}
	}
	
	if(blinkLED(gpio_path, repeats, speed) < 0) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
