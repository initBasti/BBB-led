#include "include/pin_setting.h"

int setValue(char path[], char descriptor[], char value[])
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

int getValue(char path[], char descriptor[])
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
		return -1;
	}

	if(strncmp(result, "in", 5) == 0 || strncmp(result, "0", 5) == 0) {
		return_value = 0;	
	}
	if(strncmp(result, "out", 5) == 0 || strncmp(result, "1", 5) == 0) {
		return_value = 1;	
	}
	if(fclose(file) < 0) {
		snprintf(msg, MAX_COMMAND+15, "Error at %s", full_path);
		perror(msg);
		return -1;
	}
	return return_value;
}

int buildPath(char base_path[], char gpio_id[], char out_path[])
{
	struct stat buffer;
	if(snprintf(out_path, PATH_MAX, "%s/gpio%s", base_path, gpio_id) > 0) {
		fprintf(stderr, "path truncated: %s\n", out_path);
	}	

	return stat(out_path, &buffer);
}
