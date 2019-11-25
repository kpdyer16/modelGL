#ifndef RGB_H
#define RGB_H

struct RGB
{
    float R,B,G;
    RGB() : RGB(1.0,1.0,1.0) {}
    RGB(float r, float b, float g) : R(r), B(b), G(g) {}
};

#endif // RGB_H
