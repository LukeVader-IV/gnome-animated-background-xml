FILENAME = main.c
OUT = main
SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c, objects/%.o, $(SOURCES))
COMPILER = gcc
GLIBARGS = $(shell pkg-config --cflags --libs glib-2.0)

$(OUT) : $(FILENAME) $(OBJECTS)
	gcc $(FILENAME) $(OBJECTS) -o $(OUT) $(GLIBARGS)

objects/%.o : src/%.c
	mkdir -p objects
	gcc -g -c $^ -o $@ $(GLIBARGS)

run : $(OUT)
	 ./$(OUT)

debug : $(FILENAME) $(HEADER_OBJECT)
	gcc -g $(FILENAME) $(OBJECTS) $(HEADER_OBJECT) -o $(OUT) $(GLIBARGS)
	gdb --args $(OUT) ~/Pictures/Gorilla.jpg

clean :
	rm -f $(OUT) $(OBJECTS)

install: run

test: $(OUT)
	./$(OUT)
