TARGET = gnome-animation
WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O2
SOURCE = main.c
GLIBC = $(shell pkg-config --cflags --libs glib-2.0)
SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c, objects/%.o, $(SOURCES))


all: $(TARGET)

$(TARGET): $(SOURCE) $(OBJECTS)
	gcc $^ -o $@ $(WARNINGS) $(DEBUG) $(OPTIMIZE) -lm $(GLIBC)

objects/%.o: src/%.c
	mkdir -p objects
	gcc -c $^ -o $@


clean:
	rm -f $(TARGET) a.out objects/*

# Builder will call this to install the application before running.
install:
	echo "Installing is not supported"

# Builder uses this target to run your application.
run: $(TARGET)
	./$^

debug: $(SOURCE) src/*
	gcc -g $^ $(WARNINGS) $(GLIBC)
	gdb a.out
