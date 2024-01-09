#include "Vec2.h"

Vec2::Vec2() {}

Vec2::Vec2(float x_in, float y_in)
    : x(x_in)
    , y(y_in)
{}

float Vec2::length()
{
    return sqrt((x*x) + (y*y));
}

void Vec2::normalize()
{
    float len = length();
    x /= len;
    y /= len;
}

// Returns the distance from self to rhs
float Vec2::dist(const Vec2& rhs) const
{
    return sqrtf((x - rhs.x)*(x - rhs.x) + (y - rhs.y)*(y - rhs.y));
}

// Returns a Vec2 with the absolute value
Vec2 Vec2::abs() const
{
    return Vec2(x > 0 ? x : -x, y > 0 ? : y);
}

Vec2 Vec2::operator +(const Vec2& rhs) const { return Vec2(x + rhs.x, y + rhs.y); }
Vec2 Vec2::operator -(const Vec2& rhs) const { return Vec2(x - rhs.x, y - rhs.y); }
Vec2 Vec2::operator *(const float val) const { return Vec2(x * val, y * val); }
Vec2 Vec2::operator /(const float val) const { return Vec2(x / val, y / val); }

bool Vec2::operator ==(const Vec2& rhs) const { return ((x == rhs.x) && (y == rhs.y)); }
bool Vec2::operator !=(const Vec2& rhs) const { return ((x != rhs.x) || (y != rhs.y)); }

void Vec2::operator +=(const Vec2& rhs) { x += rhs.x; y += rhs.y; }
void Vec2::operator -=(const Vec2& rhs) { x -= rhs.x; y -= rhs.y; }
void Vec2::operator *=(const float val) { x *= val; y *= val; }
void Vec2::operator /=(const float val) { x /= val; y /= val; }