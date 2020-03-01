#include "include/libgpio.h"

int setValue_generic(char path[], char descriptor[], char value[])
{
	char command[MAX_COMMAND] = {0};
	FILE *process = NULL;
	char msg[MAX_COMMAND+15] = {0};
	errno = 0;

	snprintf(command, MAX_COMMAND, "echo %s > %s/%s",
			value, path, descriptor);

	process = popen(command, "r");
	if(!process) {
		fprintf(stderr, "Couldn't execute %s\n", command);
		return -1;
	}
	if(pclose(process) < 0) {
		snprintf(msg, MAX_COMMAND+15, "Error at %s", command);
		perror(msg);
		return -1;
	}
	return 0;
}

int setValue(struct gpio* gpio, int value)
{
	char str_value[MAX_VALUE_LEN] = {0};
	snprintf(str_value, MAX_VALUE_LEN, "%d", value);
	if(setValue_generic(gpio->path, "value", str_value) == -1) {
		return -1;
	}
	return 0;
}

int setDirection(struct gpio* gpio, char value[])
{
	if(setValue_generic(gpio->path, "direction", value) == -1) {
		return -1;
	}
	return 0;
}

int getValue_generic(char path[], char descriptor[])
{
	char full_path[MAX_COMMAND] = {0};
	FILE *file = NULL;
	char msg[MAX_COMMAND+15] = {0};
	char result[5] = {0};
	int return_value = -1;
	errno = 0;

	snprintf(full_path, MAX_COMMAND, "%s/%s", path, descriptor);

	file = fopen(full_path, "r");
	if(!file) {
		fprintf(stderr, "Couldn't open %s\n", full_path);
		return -1;
	}
	if(fgets(result, 5, file) == NULL) {
		fprintf(stderr, "No output\n");
		return -1;
	}
	if(result[strnlen(result, 5)-1] == '\n') {
		result[strnlen(result, 5)-1] = '\0';
	}

	if(strncmp(result, "in", MAX_TYPE) == 0 || strncmp(descriptor, "direction", 10) == 0) {
		return_value = 0;	
	}
	if(strncmp(result, "0", 2) == 0 || strncmp(descriptor, "value", 7) == 0) {
		return_value = 0;	
	}
	if(strncmp(result, "out", MAX_TYPE) == 0 || strncmp(descriptor, "direction", 10) == 0) {
		return_value = 0;	
	}
	if(strncmp(result, "1", 2) == 0 || strncmp(descriptor, "value", 7) == 0) {
		return_value = 0;	
	}
	if(fclose(file) < 0) {
		snprintf(msg, MAX_COMMAND+15, "Error at %s", full_path);
		perror(msg);
		return -1;
	}
	return return_value;
}

int getValue(struct gpio* gpio)
{
	return getValue_generic(gpio->path, "value");
}

int getDirection(struct gpio* gpio)
{
	return getValue_generic(gpio->path, "direction");
}

int toggleValue(struct gpio* gpio)
{
	int value = -1;
	if((value=getValue(gpio)) == -1) {
		fprintf(stderr, "Error: get value from %s\n", gpio->path);
		return -1;
	}
	if(value == 0) {
		if(setValue(gpio, 1) == -1) {
			goto permission_invalid;
		}
	}
	else if(value == 1) {
		if(setValue(gpio, 0) == -1) {
			goto permission_invalid;
		}
	}
	return 0;

permission_invalid:
			fprintf(stderr, "Error: set value at %s\n", gpio->path);
			return -1;
}

int buildPath(char base_path[], char gpio_id[], char out_path[])
{
	struct stat buffer;
	snprintf(out_path, PATH_MAX, "%s/gpio%s", base_path, gpio_id);

	return stat(out_path, &buffer);
}

int initGpio(struct gpio* gpio, char base_path[], char id[], char expect_type[])
{
	if(!gpio) {
		fprintf(stderr, "Gpio struct pointer to NULL\n");
		return -1;
	}
	if(strnlen(base_path, BASE_PATH_MAX) < 2 || strnlen(id, MAX_TYPE) < 1 ||
			strnlen(expect_type, MAX_TYPE) <1) {
		fprintf(stderr, "invalid base path: %s\n", base_path);
		return -1;
	}
	if(buildPath(base_path, id, gpio->path) < 0) {
		fprintf(stderr, "invalid path: %s\n", gpio->path);
		return -1;
	}

	if(!strncmp(expect_type, "in", MAX_TYPE) &&
			!strncmp(expect_type, "out", MAX_TYPE)) {
		fprintf(stderr, "invalid expect direction: %s\n", expect_type);
		return -1;
	}
	strncpy(gpio->expect_type, expect_type, MAX_TYPE);
	if(setDirection(gpio, gpio->expect_type) < 0) {
		return -1;
	}
	if(strncmp(gpio->expect_type, "out", 4) == 0) {
		if(setValue(gpio, 0) < 0) {
			return -1;
		}
	}
	return 0;
}

int resetGpio(struct gpio* gpio)
{
	if(!gpio) {
		fprintf(stderr, "Gpio struct pointer to NULL\n");
		return -1;
	}		

	if(setValue(gpio, 0) < 0) {
		return -1;
	}
	if(setDirection(gpio, "in") < 0) {
		return -1;
	}
	return 0;
}
