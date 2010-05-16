# output file and main

myBIH.out : main.o vec.o vertex.o rect.o face.o object.o scene.o image.o ray.o bihnode.o bih.o color.o
	g++ -g -o ./bin/myBIH.out main.o vec.o vertex.o rect.o face.o object.o scene.o image.o ray.o bihnode.o bih.o color.o

main.o: main.cpp
	g++ -g -c main.cpp

#classes

vec.o: vec.cpp vec.h
	g++ -g -c vec.cpp

vertex.o: vertex.cpp vertex.h
	g++ -g -c vertex.cpp

rect.o: rect.cpp rect.h
	g++ -g -c rect.cpp

face.o: face.cpp face.h
	g++ -g -c face.cpp

object.o: object.cpp object.h
	g++ -g -c object.cpp

scene.o: scene.cpp scene.h
	g++ -g -c scene.cpp

image.o: image.cpp image.h
	g++ -g -c image.cpp

ray.o: ray.cpp ray.h
	g++ -g -c ray.cpp

bihnode.o: bihnode.cpp bihnode.h
	g++ -g -c bihnode.cpp

bih.o: bih.cpp bih.h
	g++ -g -c bih.cpp

color.o: color.cpp color.h
	g++ -g -c color.cpp

clean:
	rm *.o ./bin/myBIH.out

