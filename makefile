# output file and main

myBIH.out : main.o vec.o vertex.o rect.o face.o object.o scene.o image.o ray.o
	g++ -o ./bin/myBIH.out main.o vec.o vertex.o rect.o face.o object.o scene.o image.o ray.o

main.o: main.cpp
	g++ -c main.cpp

#classes

vec.o: vec.cpp vec.h
	g++ -c vec.cpp

vertex.o: vertex.cpp vertex.h
	g++ -c vertex.cpp

rect.o: rect.cpp rect.h
	g++ -c rect.cpp

face.o: face.cpp face.h
	g++ -c face.cpp

object.o: object.cpp object.h
	g++ -c object.cpp

scene.o: scene.cpp scene.h
	g++ -c scene.cpp

image.o: image.cpp image.h
	g++ -c image.cpp

ray.o: ray.cpp ray.h
	g++ -c ray.cpp

clean:
	rm *.o ./bin/myBIH.out

