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

#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED

#include "Vector.h"
#include "Rectangle.h"

namespace GX {

struct Line2f; struct Line2i;

struct Line2f {
    Vector2f pointA; Vector2f pointB;

    Line2f();
    Line2f(const Vector2f& a,const Vector2f& b);
    Line2f(float ax,float ay,float bx,float by);
    Line2f(const Line2i& li);

    Rectanglef boundingBox() const;
    bool intersects(const Line2f& other,Vector2f& point,bool segmentOnly=true) const;
    Vector2f closestPoint(const Vector2f& point,bool segmentOnly=true) const;
};

struct Line2i {
    Vector2i pointA; Vector2i pointB;

    Line2i();
    Line2i(const Vector2i& a,const Vector2i& b);
    Line2i(int ax,int ay,int bx,int by);
    Line2i(const Line2f& lf);

    Rectanglei boundingBox() const;
    bool intersects(const Line2i& other,Vector2f& point,bool segmentOnly=true) const;
};

}

#endif
