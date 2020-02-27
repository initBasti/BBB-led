SUFFIXES += .d
CLEANUP = rm -f
MKDIR = mkdir -p
TARGET_EXTENSION=out

.PHONY: clean
.PHONY: test

BIN_NAME = blinkled

PATHS = source/
PATHB = build/
PATHD = build/depends/
PATHO = build/objs/
PATHI = source/include/
PATHBIN = bin/

SOURCES = $(wildcard $(PATHS)*.c)
OBJECTS = $(SOURCES:$(PATHS)%.c=$(PATHO)%.o)
DEPS = $(SOURCES:$(PATHS)%.c=$(PATHD)%.d)

COMPILE=gcc -c -g -Wall -pedantic -Wextra -std=c99 -fPIC
LINK=gcc
DEPEND=gcc -MM -MT $(@:$(PATHD)%.d=$(PATHO)%.o) >$@
INCLUDES = -I$(PATHS) -I$(PATHI)

all: $(PATHBIN)$(BIN_NAME)
	@echo "Making symlink: $(BIN_NAME) -> $<"
	@$(RM) $(BIN_NAME)
	@ln -s $(PATHBIN)/$(BIN_NAME) $(BIN_NAME)

$(PATHBIN)$(BIN_NAME): $(OBJECTS)
	@echo "Linking: $@"
	$(LINK) $(OBJECTS) -o $@

$(PATHO)%.o:: $(PATHS)%.c
	@echo "Compiling: $< -> $@"
	$(COMPILE) $(INCLUDES) $< -o $@

$(PATHD)%.d:: $(PATHS)%.c | $(PATHD)
	$(DEPEND) -o $@ $<

$(PATHB):
	$(MKDIR) $(PATHB)

$(PATHD):
	$(MKDIR) $(PATHD)

$(PATHO):
	$(MKDIR) $(PATHO)

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f $(PATHBIN)$(BIN_NAME) $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/$(BIN_NAME)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(BIN_NAME)

clean:
	$(CLEANUP) $(PATHO)*.o
	$(CLEANUP) $(PATHB)*.$(TARGET_EXTENSION)
	$(CLEANUP) $(PATHD)*.d

.PRECIOUS: $(PATHD)%.d
.PRECIOUS: $(PATHO)%.o

-include $(DEPS)
