#ifndef LIB_GPIO
#define LIB_GPIO

#include <linux/limits.h>
#include "pin_setting.h"
#include "types.h"

#define MAX_TYPE 4
#define MAX_VALUE_LEN 4
#define BASE_PATH_MAX PATH_MAX-100

#define DIRECT_IN 0
#define DIRECT_OUT 1
#define OFF 0
#define ON 1

struct gpio {
	char path[PATH_MAX+9];
	char expect_type[MAX_TYPE];
};

extern int usleep(useconds_t usec);

extern int setDirection(struct gpio*, char[]);
extern int setValue(struct gpio*, int);
extern int getDirection(struct gpio*);
extern int getValue(struct gpio*);
extern int toggleValue(struct gpio*);
extern int initGpio(struct gpio*, char[], char[], char[]);
extern int resetGpio(struct gpio*);
extern int blinkLED(struct gpio*, int, int);
extern int pollSwitch(struct gpio*[], struct gpio*[], int, int);
#endif
