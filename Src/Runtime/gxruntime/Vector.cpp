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

#include <math.h>
#include "Vector.h"

using namespace GX;

const Vector2f Vector2f::zero = Vector2f(0.f,0.f);
const Vector2f Vector2f::one = Vector2f(1.f,1.f);

Vector2f::Vector2f() {
    x=0.f; y=0.f;
}

Vector2f::Vector2f(float s) {
    x = s; y = s;
}

Vector2f::Vector2f(float ix,float iy) {
    x = ix; y = iy;
}

Vector2f::Vector2f(const Vector2i& vi) {
    x = (float)vi.x; y = (float)vi.y;
}

float Vector2f::lengthSquared() const {
    return x*x+y*y;
}

float Vector2f::length() const {
    return (float)sqrt(x*x+y*y);
}

float Vector2f::distanceSquared(const Vector2f& b) const {
    return subtract(b).lengthSquared();
}

float Vector2f::distance(const Vector2f& b) const {
    return subtract(b).length();
}

bool Vector2f::equals(const Vector2f& b,float epsilon) const {
    return distanceSquared(b)<=(epsilon*epsilon);
}

Vector2f Vector2f::add(const Vector2f& b) const {
    return Vector2f(x+b.x,y+b.y);
}

Vector2f Vector2f::subtract(const Vector2f& b) const {
    return Vector2f(x-b.x,y-b.y);
}

Vector2f Vector2f::multiply(float s) const {
    return Vector2f(x*s,y*s);
}

Vector2f Vector2f::normalize() const {
    return multiply(1.f/length());
}

Vector2f Vector2f::reflect(const Vector2f& n) const {
    Vector2f reflectedVector = normalize();
    reflectedVector = n.multiply(2.f*reflectedVector.dotProduct(n)).subtract(reflectedVector);
    return reflectedVector.normalize();
}

float Vector2f::dotProduct(const Vector2f& b) const {
    return x*b.x+y*b.y;
}

const Vector3f Vector3f::zero = Vector3f(0.f,0.f,0.f);
const Vector3f Vector3f::one = Vector3f(1.f,1.f,1.f);

Vector3f::Vector3f() {
    x=0.f; y=0.f; z=0.f;
}

Vector3f::Vector3f(float s) {
    x = s; y = s; z = s;
}

Vector3f::Vector3f(float ix,float iy,float iz) {
    x=ix; y=iy; z=iz;
}

float Vector3f::lengthSquared() const {
    return x*x+y*y+z*z;
}

float Vector3f::length() const {
    return (float)sqrt(x*x+y*y+z*z);
}

float Vector3f::distanceSquared(const Vector3f& b) const {
    return subtract(b).lengthSquared();
}

float Vector3f::distance(const Vector3f& b) const {
    return subtract(b).length();
}

Vector3f Vector3f::add(const Vector3f& b) const {
    return Vector3f(x+b.x,y+b.y,z+b.z);
}

Vector3f Vector3f::subtract(const Vector3f& b) const {
    return Vector3f(x-b.x,y-b.y,z-b.z);
}

Vector3f Vector3f::multiply(float s) const {
    return Vector3f(x*s,y*s,z*s);
}

Vector3f Vector3f::normalize() const {
    return multiply(1.f/length());
}

float Vector3f::dotProduct(const Vector3f& b) const {
    return x*b.x+y*b.y+z*b.z;
}

Vector3f Vector3f::crossProduct(const Vector3f& b) const {
    return Vector3f(y*b.z-z*b.y,z*b.x-x*b.z,x*b.y-y*b.x);
}

const Vector2i Vector2i::zero = Vector2i(0,0);
const Vector2i Vector2i::one = Vector2i(1,1);

Vector2i::Vector2i() {
    x=0; y=0;
}

Vector2i::Vector2i(int s) {
    x = s; y = s;
}

Vector2i::Vector2i(int ix,int iy) {
    x = ix; y = iy;
}

Vector2i::Vector2i(const Vector2f& vf) {
    x = (int)vf.x; y = (int)vf.y;
}

int Vector2i::lengthSquared() const {
    return x*x+y*y;
}

float Vector2i::length() const {
    return (float)sqrt((int)(x*x+y*y));
}

int Vector2i::distanceSquared(const Vector2i& b) const {
    return subtract(b).lengthSquared();
}

float Vector2i::distance(const Vector2i& b) const {
    return subtract(b).length();
}

Vector2i Vector2i::add(const Vector2i& b) const {
    return Vector2i(x+b.x,y+b.y);
}

Vector2i Vector2i::subtract(const Vector2i& b) const {
    return Vector2i(x-b.x,y-b.y);
}

Vector2i Vector2i::multiply(int s) const {
    return Vector2i(x*s,y*s);
}