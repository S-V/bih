#include "ray.h"
#include <stdio.h>

Ray::Ray(/*const Vec& position, */const Vec& direction):/*m_position(position),*/m_direction(direction)
{
}

/*
Vec& Ray::position()
{
    return m_position;
}*/

/*
const Vec& Ray::position() const
{
    return m_position;
}
*/

Ray& Ray::operator = (const Ray& another)
{
    //m_position = another.m_position;
    m_direction = another.m_direction;

    return *this;
}

void Ray::set(/*const Vec& position, */const Vec& direction)
{
    //m_position = position;
    m_direction = direction;
}

void Ray::toConsole()
{
    printf( "-----------------------------------------------\n");
    printf( "Ray\n\n" );
    printf( "-----------------------------------------------\n");

    //printf( "   position : %f %f %f \n",m_position[0],m_position[1],m_position[2]);
    printf( "   direction: %f %f %f \n\n",m_direction[0],m_direction[1],m_direction[2]);
}
