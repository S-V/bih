#include <stdio.h>
#include "mloader.h"

using namespace std;

int main( int argc, const char* argv[] )
{
	printf( "\nRay Tracing - BIH\n\n" );

    Object* targetObject = new Object();
    MLoader::loadM("/home/yzong/Dissertation/Models/models/lion.m",targetObject);

    targetObject->toConsole();

    delete targetObject;
}

