#ifndef BLINK_H
#define BLINK_H

#ifndef PIN_SETTING_H
#include "pin_setting.h"
#endif

extern int usleep(useconds_t usec);

int blinkLED(char[], int, int);

#endif
