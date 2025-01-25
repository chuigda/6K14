ifndef CC
	CC = gcc
endif
ifndef AR
	AR = ar
endif
ifndef RANLIB
	RANLIB = ranlib
endif
ifndef CFLAGS
	CFLAGS = -Wall -Wextra \
		-Wno-pedantic \
		-Wno-cast-function-type \
		-Wno-char-subscripts \
		-g \
		$(EXTRA_CFLAGS)
endif

ifndef WIN32
	SHARED_LIB_NAME = lib6k14.so
	EXECUTABLE_NAME = 6k14
else
	SHARED_LIB_NAME = lib6k14.dll
	EXECUTABLE_NAME = 6k14.exe
endif

define LOG
	@printf '\t%s\t%s\n' $1 $2
endef

define COMPILE
	$(call LOG,CC,$1)
	@$(CC) $(CFLAGS) $1 \
		-Iconfig -Iinclude -Isrc/include \
		-fPIC -c -o $2
endef

HEADER_FILES = $(wildcard include/*.h) $(wildcard src/include/*.h)
SOURCE_FILES = $(wildcard src/*.c)
OBJECT_FILES := $(patsubst src/%.c,%.o,$(SOURCE_FILES))

.PHONY: all
all: lib6k14-phony 6k14-phony

.PHONY: lib6k14-phony lib6k14-log
lib6k14-phony: lib6k14-log $(SHARED_LIB_NAME)

lib6k14-long:
	@echo Building shared library $(SHARED_LIB_NAME)

.PHONY: 6k14-phony 6k14-log
6k14-phony: 6k14-log $(EXECUTABLE_NAME)

6k14-log:
	@echo Building executable $(EXECUTABLE_NAME)

$(EXECUTABLE_NAME): $(HEADER_FILES) main.o
	$(call LOG,LINK,$(EXECUTABLE_NAME))
	@$(CC) $(CFLAGS) main.o -L. -l6k14 -o $(EXECUTABLE_NAME)

main.o: main.c $(HEADER_FILES)
	$(call COMPILE,$<,$@)

$(SHARED_LIB_NAME): $(OBJECT_FILES)
	$(call LOG,LINK,$(SHARED_LIB_NAME))
	@$(CC) $(CFLAGS) $(OBJECT_FILES) -fPIC -shared -o $(SHARED_LIB_NAME)

%.o: src/%.c $(HEADER_FILES)
	$(call COMPILE,$<,$@)

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so
	rm -f *.dll
	rm -f *.exe
	rm -f *.core
	rm -f 6k14
	rm -f vgcore*
	rm -f core*
