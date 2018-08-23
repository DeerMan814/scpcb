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

#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include "Graphics.h"

#include "Material.h"

#include <vector>
#include "Color.h"
#include "Vector.h"
#include "Matrix.h"

namespace GX {

struct Vertex {
    Vertex(const Vector3f& p,const Vector3f& n,const Vector2f& tc,const Color& c);

    Vector3f pos;
    Vector3f normal;
    Vector2f uv; //maybe have a second set of uv coords?
    Color color;
};

struct Primitive {
    enum class TYPE {
        TRIANGLE,
        LINE,
    };

    Primitive(long ia,long ib);
    Primitive(long ia,long ib,long ic);

    long a; long b; long c;
};

struct Mesh {
    public:
        Mesh(Graphics* gfx, Primitive::TYPE pt);
        ~Mesh();

        int addVertex(const Vertex& v);
        void removeVertex(int v);
        int addPrimitive(const Primitive& t);
        void removePrimitive(int t);
        void clear();
        void setMaterial(Material* m);

        Matrix4x4f worldMatrix;

        const std::vector<Vertex>& getVertices() const;
        const std::vector<Primitive>& getPrimitives() const;

        virtual void render();
    protected:
        Mesh(){};

        Graphics* graphics;

        bool isDirty = true;

        virtual void updateInternalData();

        Material* material;

        Primitive::TYPE primitiveType;

        std::vector<Vertex> vertices;
        std::vector<Primitive> primitives;

        std::vector<FLOAT> dxVertexData;
        std::vector<WORD> dxIndexData;

        D3D11_BUFFER_DESC dxVertexBufferDesc;
        D3D11_SUBRESOURCE_DATA dxVertexBufferData;
        ID3D11Buffer* dxVertexBuffer;

        D3D11_BUFFER_DESC dxIndexBufferDesc;
        D3D11_SUBRESOURCE_DATA dxIndexBufferData;
        ID3D11Buffer* dxIndexBuffer;
};

}

#endif
