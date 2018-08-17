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

#ifndef RECTANGLE_H_INCLUDED
#define RECTANGLE_H_INCLUDED

#include "Vector.h"

namespace GX {

struct Rectanglef; struct Rectanglei;

struct Rectanglef {
    public:
        Rectanglef();
        Rectanglef(const Vector2f& itl,const Vector2f& ibr);
        Rectanglef(float il,float it,float ir,float ib);
        Rectanglef(const Rectanglei& ri);

        void addPoint(float x,float y);
        void addPoint(const Vector2f& point);

        float width() const; float height() const; float area() const;
        Vector2f center() const;
        Vector2f topLeftCorner() const; Vector2f bottomRightCorner() const;
        Vector2f topRightCorner() const; Vector2f bottomLeftCorner() const;

        bool isPointInside(const Vector2f& p) const;
        bool intersects(const Rectanglef& other) const;
    private:
        Vector2f tl; Vector2f br;
        void repair();
};

struct Rectanglei {
    public:
        Rectanglei();
        Rectanglei(const Vector2i& itl,const Vector2i& ibr);
        Rectanglei(int il,int it,int ir,int ib);
        Rectanglei(const Rectanglef& rf);

        void addPoint(int x,int y);
        void addPoint(const Vector2i& point);

        int width() const; int height() const; int area() const;
        Vector2i topLeftCorner() const; Vector2i bottomRightCorner() const;
        Vector2i topRightCorner() const; Vector2i bottomLeftCorner() const;

        bool isPointInside(const Vector2i& p) const;
        bool intersects(const Rectanglei& other) const;
    private:
        Vector2i tl; Vector2i br;
        void repair();
};

}

#endif
