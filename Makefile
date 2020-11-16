CC = gcc
FLAGS :=  -g3

FILES := my_curl main

HEADERS = $(addprefix lib/,my_curl.h)

OFILES :=$(FILES:=.o)

SFILES :=$(FILES:=.c)

OBJ = $(addprefix obj/,$(OFILES))

SOURCE = $(addprefix source/,$(SFILES))

all: my_curl

my_curl: $(OBJ) $(HEADERS)
	$(CC) -I./lib/ -I./source/ -o my_curl $(OBJ) $(FLAGS)

obj/%.o: source/%.c
	$(CC) -I./lib/ -I./source/ -c $< $(FLAGS) && mv *.o obj/

clean:
	rm -rf obj/*.o my_curl

re: clean all