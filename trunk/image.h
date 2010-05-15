#ifndef _IMAGE_H
#define _IMAGE_H

#include "face.h"
#include "vertex.h"
#include "rect.h"
#include <vector>

using namespace std;

class Image
{
public:
    Image(const int& w, const int& height);
    virtual ~Image();

    int width();
    int height();

    void setPixel(const int& index, const unsigned char& r, const unsigned char& g, const unsigned char& b);
    void setPixel(const int& x, const int& y, const unsigned char& r, const unsigned char& g, const unsigned char& b);
    unsigned char* getPixel(const int& index);

private:
    int m_width;
    int m_height;
    unsigned char* m_rgb;

};

#endif
