#pragma once

#include <cmath>
//#include <iostream>

class Vec2
{
public:
    float x = 0;
    float y = 0;

    Vec2();
    Vec2(float x_in, float y_in);

    float length();
    void  normalize();
    float dist(const Vec2& rhs) const;
    Vec2  abs() const;

    Vec2 operator + (const Vec2& rhs) const;
    Vec2 operator - (const Vec2& rhs) const;
    Vec2 operator * (const float val) const;
    Vec2 operator / (const float val) const;

    bool operator == (const Vec2& rhs) const;
    bool operator != (const Vec2& rhs) const;

    void operator += (const Vec2& rhs);
    void operator -= (const Vec2& rhs);
    void operator *= (const float val);
    void operator /= (const float val);

    // For test purposes only
    // void print() const;
};