#ifndef _VEC_H
#define _VEC_H

class Vec 
{
public:
    Vec();
    Vec(const float& xx, const float& yy, const float& zz);
    Vec(const Vec& another);
    virtual ~Vec();

    const float& x();
    const float& y();
    const float& z();

    float mag(void) const;                  // returns magnitude

    Vec& operator= (const Vec& another);
    float& operator[] (const int& index);
    const float& operator[] (const int& index) const;
    
    Vec operator - (void) const;            // Negate
    Vec operator ! (void) const;            // Normalize: returns unit vector

    Vec operator + (const Vec& v) const;    // Add to
    Vec operator - (const Vec& v) const;	// Subtract from
    Vec operator * (float num) const;       // Scale
    float operator * (const Vec& v) const;  // DotProd
    Vec operator % (const Vec& v) const;    // CrossProd
    
    Vec operator & (const Vec& v) const;    // Element-wise multiplication
    
private:
    float m_val[3];
};

#endif

