#ifndef PIN_SETTING_H
#define PIN_SETTING_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/limits.h>
#define __USE_XOPEN
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include "types.h"

extern FILE *popen(const char *command, const char *type);
extern int pclose(FILE *stream); 
extern size_t strnlen(const char *s, size_t maxlen);

int setValue(char[], char[], char[]);
int getValue(char[], char[]);
int buildPath(char[], char[], char[]);
#endif
