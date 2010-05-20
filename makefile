# This makefile compiles the project listed in the PROJ macro
#
PROJ = myBIH
OBJS = main.o vec.o vertex.o rect.o face.o object.o scene.o image.o ray.o bihnode.o bih.o color.o 

# Configuration:
#
MODEL = s
CC = g++

# include directories
INCLUDES = -I./include

# library paths
LIBS = -L./lib -L/usr/lib -lFreeImage


# Compiler-dependent section
#
#%if $(CC) == g++
  #CFLAGS = –m$(MODEL)		# $(CFLAGS) is –ms
  #LDSTART = c0$(MODEL)		# the start-up object file
  #LDLIBS = c$(MODEL)		# the library
  #LDFLAGS = /Lf:\bc\lib		# f:\bc\lib is library directory
#%else				# else
#% abort Unsupported CC==$(CC)	# compiler is not supported
#%endif				# endif 

# The project to be built
#
$(PROJ).out : $(OBJS)
	$(CC) $(INCLUDES) -g -o ./bin/myBIH.out $(OBJS) $(LIBS)

main.o: main.cpp
	$(CC) -g -c main.cpp

#classes

vec.o: vec.cpp vec.h
	$(CC) -g -c vec.cpp

vertex.o: vertex.cpp vertex.h
	$(CC) -g -c vertex.cpp

rect.o: rect.cpp rect.h
	$(CC) -g -c rect.cpp

face.o: face.cpp face.h
	$(CC) -g -c face.cpp

object.o: object.cpp object.h
	$(CC) -g -c object.cpp

scene.o: scene.cpp scene.h
	$(CC) -g -c scene.cpp

image.o: image.cpp image.h
	$(CC) $(INCLUDES) -g -c image.cpp $(OBJS) $(LIBS)

ray.o: ray.cpp ray.h
	$(CC) -g -c ray.cpp

bihnode.o: bihnode.cpp bihnode.h
	$(CC) -g -c bihnode.cpp

bih.o: bih.cpp bih.h
	$(CC) -g -c bih.cpp

color.o: color.cpp color.h
	$(CC) -g -c color.cpp

clean:
	rm *.o ./bin/myBIH.out

