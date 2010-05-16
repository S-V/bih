#include "image.h"

Image::Image(const int& w, const int& h): m_width(w),m_height(h)
{
    m_rgb = new unsigned char[w*h*3];
}

Image::~Image()
{
    delete[] m_rgb;
}

int Image::width()
{
    return m_width;
}

int Image::height()
{
    return m_height;
}

void Image::setPixel(const int& index, const unsigned char& r, const unsigned char& g, const unsigned char& b)
{
    m_rgb[index]  =r;
    m_rgb[index+1]=g;
    m_rgb[index+2]=b;
}

void Image::setPixel(const int& x, const int& y, const unsigned char& r, const unsigned char& g, const unsigned char& b)
{
    int k = 3*(m_width*y+x);
    m_rgb[k]   = r;
    m_rgb[k+1] = g;
    m_rgb[k+2] = b;
}

void Image::setPixel(const int& x, const int& y, const Color& color)
{
    int k = 3*(m_width*y+x);
    m_rgb[k]   = color[0]*255.0f;
    m_rgb[k+1] = color[1]*255.0f;
    m_rgb[k+2] = color[2]*255.0f;
}

unsigned char* Image::getPixel(const int& index)
{
    return &(m_rgb[index]);
}

