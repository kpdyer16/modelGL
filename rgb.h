#ifndef RGB_H
#define RGB_H

struct Color
{
    float R,B,G;
    Color() : Color(1.0,1.0,1.0) {}
    Color(float r, float b, float g) : R(r), B(b), G(g) {}
};

#endif // RGB_H
