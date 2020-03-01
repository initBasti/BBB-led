SUFFIXES += .d
CLEANUP = rm -f
MKDIR = mkdir -p
TARGET_EXTENSION=out
LIB_EXTENSION=so

.PHONY: clean
.PHONY: test

BIN_NAME = gpioprog
LIB_NAME = libgpio
CASCADE_0 = 1.0
CASCADE_1 = 1

PATHS = source/
PATHB = build/
PATHD = build/depends/
PATHO = build/objs/
PATHL = build/libs/
PATHI = source/include/
PATHBIN = bin/
LIB_PATH = /usr/lib/

LIB_SOURCES := $(filter-out $(PATHS)main.c, $(wildcard $(PATHS)*.c))
SOURCES = $(PATHS)main.c
LIB_OBJECTS := $(LIB_SOURCES:$(PATHS)%.c=$(PATHO)%.o)
OBJECTS := $(SOURCES:$(PATHS)%.c=$(PATHO)%.o)
DEPS := $(SOURCES:$(PATHS)%.c=$(PATHD)%.d)
LIB_DEPS := $(LIB_SOURCES:$(PATHS)%.c=$(PATHD)%.d)

COMPILE=gcc -c -g -Wall -pedantic -Wextra -std=c99 -fPIC
LINK=gcc
SHARED=gcc -shared
DEPEND=gcc -MM -MT $(@:$(PATHD)%.d=$(PATHO)%.o) >$@
INCLUDES = -I$(PATHS) -I$(PATHI)

all: $(PATHBIN)$(BIN_NAME)
	@echo "Making symlink: $(BIN_NAME) -> $<"
	@$(RM) $(BIN_NAME)
	@ln -s $(PATHBIN)/$(BIN_NAME) $(BIN_NAME)

lib: $(PATHL)$(LIB_NAME).$(LIB_EXTENSION).$(CASCADE_0)
	@echo "copy lib to $(LIB_PATH)"
	cp $(PATHL)$(LIB_NAME).$(LIB_EXTENSION).$(CASCADE_0) $(LIB_PATH)

$(PATHL)$(LIB_NAME).$(LIB_EXTENSION).$(CASCADE_0): $(LIB_OBJECTS)
	@echo "Create shared library: $@"
	$(SHARED) $< -o $@

$(PATHBIN)$(BIN_NAME): $(OBJECTS)
	@echo "Linking: $@"
	$(LINK) $(OBJECTS) -o $@ -lgpio

$(LIB_OBJECTS):: $(LIB_SOURCES)
	@echo "Compiling: $< -> $@"
	$(COMPILE) $(INCLUDES) $< -o $@

$(OBJECTS): $(SOURCES)
	@echo "Compiling: $< -> $@"
	$(COMPILE) $(INCLUDES) $< -o $@ -lgpio

$(PATHD)%.d:: $(PATHS)%.c | $(PATHD)
	$(DEPEND) -o $@ $<

$(PATHB):
	$(MKDIR) $(PATHB)

$(PATHD):
	$(MKDIR) $(PATHD)

$(PATHO):
	$(MKDIR) $(PATHO)

$(PATHL):
	$(MKDIR) $(PATHL)

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f $(PATHBIN)$(BIN_NAME) $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/$(BIN_NAME)
	ln -sf $(LIB_PATH)$(LIB_NAME).$(LIB_EXTENSION).$(CASCADE_0) $(LIB_PATH)$(LIB_NAME).$(LIB_EXTENSION).$(CASCADE_1)
	ln -sf $(LIB_PATH)$(LIB_NAME).$(LIB_EXTENSION).$(CASCADE_1) $(LIB_PATH)$(LIB_NAME).$(LIB_EXTENSION)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(BIN_NAME)

clean:
	$(CLEANUP) $(PATHO)*.o
	$(CLEANUP) $(PATHB)*.$(TARGET_EXTENSION)
	$(CLEANUP) $(PATHD)*.d

.PRECIOUS: $(PATHD)%.d
.PRECIOUS: $(PATHO)%.o

-include $(DEPS)
