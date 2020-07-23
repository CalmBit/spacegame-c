CC=gcc
CFLAGS=-std=c99 -g -Wall -Wextra -Wpedantic \
          -Wformat=2 -Wno-unused-parameter -Wshadow \
          -Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
          -Wredundant-decls -Wnested-externs -Wmissing-include-dirs \
		  -Wno-format-nonliteral \
		  -I/usr/local/opt/openal-soft/include
LDFLAGS=-L/usr/local/opt/openal-soft/lib
LIBS=-lglfw -framework OpenGL -lglew -lopenal
UNITS=main memory error glfw_window shader file audio \
 	  str_util list obj wav
OBJ_DIR=obj/
OBJS=$(addprefix $(OBJ_DIR), $(addsuffix .o, $(UNITS)))

MKDIR_P = mkdir -p

.PHONY: directories clean

all: directories spacegame

spacegame: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) $(OBJS)  -o $@

$(OBJ_DIR)%.o: %.c flags.h
	$(CC) -c $(CFLAGS) -o $@ $<

directories: ${OBJ_DIR}

clean:
	rm -f $(OBJS) spacegame

$(OBJ_DIR):
	$(MKDIR_P) $(OBJ_DIR)