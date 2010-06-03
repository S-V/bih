#include "image.h"
//#include "FreeImage.h" //No FreeImage library in Cell

//For writing to serialize file
#include <iostream>
#include <fstream>
using namespace std;


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
    m_rgb[k]   = (unsigned char)(color[0]*255.0f);
    m_rgb[k+1] = (unsigned char)(color[1]*255.0f);
    m_rgb[k+2] = (unsigned char)(color[2]*255.0f);
}

unsigned char* Image::getPixel(const int& index)
{
    return &(m_rgb[index]);
}

void Image::exportImage(const char* fileName)
{
	//Cell has no free image library
	
    //FIBITMAP* tmpImage = FreeImage_ConvertFromRawBits(m_rgb, m_width, m_height, 3*m_width, 24, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, false);
	//FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(fileName);
	//FreeImage_Save(fif,tmpImage,fileName);
	
	//Write out char array to serialized file
    ofstream myFile ("result.bin", ios::out | ios::binary);
    myFile.write ((char*)(m_rgb), (3*m_width*m_height));
    myFile.close();
}

