/**

Copyright (c) 2018 Juan Pablo Arce

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

**/

#include "Color.h"

using namespace GX;

Color::Color() {
    red = 1.f; green = 1.f; blue = 1.f; alpha = 1.f;
}

Color::Color(int r,int g,int b,int a) {
    red = ((float)r)/255.f; green = ((float)g)/255.f; blue = ((float)b)/255.f; alpha = ((float)a)/255.f;
}

Color::Color(float r,float g,float b,float a) {
    red = r; green = g; blue = b; alpha = a;
}

int Color::getRedInt() const {
    return (int)(red*255.f);
}

int Color::getGreenInt() const {
    return (int)(green*255.f);
}

int Color::getBlueInt() const {
    return (int)(blue*255.f);
}

int Color::getAlphaInt() const {
    return (int)(alpha*255.f);
}

void Color::setRedInt(int r) {
    red = ((float)r)/255.f;
}

void Color::setGreenInt(int g) {
    green = ((float)g)/255.f;
}

void Color::setBlueInt(int b) {
    blue = ((float)b)/255.f;
}

void Color::setAlphaInt(int a) {
    alpha = ((float)a)/255.f;
}
