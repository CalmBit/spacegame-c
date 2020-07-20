CC=gcc
CFLAGS=-std=c99 -g -Wpedantic
LIBS=-lglfw -framework OpenGL -lglew
UNITS=main memory error glfw_window shader file
OBJ_DIR=obj/
OBJS=$(addprefix $(OBJ_DIR), $(addsuffix .o, $(UNITS)))

MKDIR_P = mkdir -p

.PHONY: directories clean

all: directories spacegame

spacegame: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS)  -o $@

$(OBJ_DIR)%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

directories: ${OBJ_DIR}

clean:
	rm -f $(OBJS) spacegame

$(OBJ_DIR):
	$(MKDIR_P) $(OBJ_DIR)