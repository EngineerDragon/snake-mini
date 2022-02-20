CC := clang
CFLAGS := -std=c99
LDLIBS := -lglfw -ldl
GLAD_PATH := ~/C++/gravity-game/src/glad.c
RELEASE := snake-mini
.PHONY := clean all release

all : $(RELEASE)
release : $(RELEASE)


$(RELEASE) : main.c
	$(CC) $(CFLAGS) main.c $(GLAD_PATH) -o $(RELEASE) $(LDLIBS)

clean : 
	rm -rvf snake-mini
