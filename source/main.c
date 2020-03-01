#include <inttypes.h>
#include "include/libgpio.h"
#include "../config.h"

int main(int argc, char*argv[])
{
	struct gpio input[2] = {
		{
			.path = {0},
			.expect_type = {0}
		}
	};
	struct gpio output[2] = {
		{
			.path = {0},
			.expect_type = {0}
		}
	};
	struct gpio *inputs[2] = {&input[0], &input[1]};
	struct gpio *outputs[2] = {&output[0], &output[1]};
	errno = 0;

	if(initGpio(&input[0], BASE_PATH, P9_PIN_11, "in") < 0) {
		return EXIT_FAILURE;
	}
	if(initGpio(&input[1], BASE_PATH, P9_PIN_13, "in") < 0) {
		return EXIT_FAILURE;
	}
	if(initGpio(&output[0], BASE_PATH, P9_PIN_12, "out") < 0) {
		return EXIT_FAILURE;
	}
	if(initGpio(&output[1], BASE_PATH, P9_PIN_27, "out") < 0) {
		return EXIT_FAILURE;
	}

	if(pollSwitch(inputs, outputs, 2, 2) < 0) {
		goto reset_fail;
	}

	goto reset_success;

reset_success:
	if(resetGpio(&input[0]) < 0) {
		fprintf(stderr, "reset of gpio values failed\n");
		return EXIT_FAILURE;
	}
	if(resetGpio(&input[1]) < 0) {
		fprintf(stderr, "reset of gpio values failed\n");
		return EXIT_FAILURE;
	}
	if(resetGpio(&output[0]) < 0) {
		fprintf(stderr, "reset of gpio values failed\n");
		return EXIT_FAILURE;
	}
	if(resetGpio(&output[1]) < 0) {
		fprintf(stderr, "reset of gpio values failed\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
reset_fail:
	if(resetGpio(&input[0]) < 0) {
		fprintf(stderr, "reset of gpio values failed\n");
		return EXIT_FAILURE;
	}
	if(resetGpio(&input[1]) < 0) {
		fprintf(stderr, "reset of gpio values failed\n");
		return EXIT_FAILURE;
	}
	if(resetGpio(&output[0]) < 0) {
		fprintf(stderr, "reset of gpio values failed\n");
		return EXIT_FAILURE;
	}
	if(resetGpio(&output[1]) < 0) {
		fprintf(stderr, "reset of gpio values failed\n");
		return EXIT_FAILURE;
	}
	return EXIT_FAILURE;
}
