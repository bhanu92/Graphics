PROG = viewer

CC = g++
CFLAGS = -g -w -Iinclude -D_DEBUG -std=c++11
LIBS = -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -ldl
OBJS = $(PROG).o LoadShaders.o gl3w.o viewer.o sceneParse.o loadObj.o

all: $(PROG)

gl3w.o: gl3w.c
	gcc -c $(CFLAGS) -o $@ $<

.cpp.o:
	$(CC) -c $(CFLAGS) -o $@ $<

$(PROG): $(OBJS)
	$(CC) -g -o $@ $^ $(LIBS)

clean:
	rm -f *.o $(PROG)
