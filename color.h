#ifndef _COLOR_H
#define _COLOR_H

class Color 
{
public:
    Color();
    Color(const float& xx, const float& yy, const float& zz);
    virtual ~Color();

    const float& x();
    const float& y();
    const float& z();

    //float mag(void) const;                  // returns magnitude

    Color& operator= (const Color& another);
    float& operator[] (const int& index);
    const float& operator[] (const int& index) const;
    
    //Color operator - (void) const;            // Negate
    //Color operator ! (void) const;            // Normalize: returns unit Colortor

    Color operator + (const Color& v) const;    // Add to
    //Color operator - (const Color& v) const;	// Subtract from
    Color operator * (float num) const;         // Scale
    //float operator * (const Color& v) const;  // DotProd
    //Color operator % (const Color& v) const;  // CrossProd
    
    //Color operator & (const Color& v) const;  // Element-wise multiplication
    
private:
    float* m_val;
};

#endif
