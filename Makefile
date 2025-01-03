RAYLIB ?= ./external/raylib-5.5/src/

all:
	gcc src/main.c -I $(RAYLIB) -L $(RAYLIB) -lraylib -lGL -lm -lpthread -ldl -lrt -lwayland-client

rerun: all run

run:
	./a.out

clean:
	rm a.out compile_commands.json -f

