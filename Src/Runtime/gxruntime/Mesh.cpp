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

#include "Mesh.h"

using namespace GX;

Vertex::Vertex(const Vector3f& p,const Vector3f& n,const Vector2f& tc,const Color& c) {
    pos = p; normal = n; uv = tc; color = c;
}

Primitive::Primitive(long ia,long ib) {
    a = ia; b = ib; c = -1;
}

Primitive::Primitive(long ia,long ib,long ic) {
    a = ia; b = ib; c = ic;
}

Mesh::Mesh(Graphics* gfx, Primitive::TYPE pt) {
    graphics = gfx;

    primitiveType = pt;

    dxVertexData.clear();
    dxVertexBuffer = nullptr;
    dxIndexBuffer = nullptr;
}

Mesh::~Mesh() {
    if (dxVertexBuffer != nullptr) { dxVertexBuffer->Release(); }
    if (dxIndexBuffer != nullptr) { dxIndexBuffer->Release(); }
}

void Mesh::updateInternalData() {
    if (!isDirty) { return; }

    dxVertexData.clear();
    dxIndexData.clear();

    for (int i = 0; i < vertices.size(); i++) {
        dxVertexData.push_back(vertices[i].pos.x);
        dxVertexData.push_back(vertices[i].pos.y);
        dxVertexData.push_back(vertices[i].pos.z);

        dxVertexData.push_back(vertices[i].normal.x);
        dxVertexData.push_back(vertices[i].normal.y);
        dxVertexData.push_back(vertices[i].normal.z);

        dxVertexData.push_back(vertices[i].uv.x);
        dxVertexData.push_back(vertices[i].uv.y);

        dxVertexData.push_back(vertices[i].color.red);
        dxVertexData.push_back(vertices[i].color.green);
        dxVertexData.push_back(vertices[i].color.blue);
        dxVertexData.push_back(vertices[i].color.alpha);
    }

    for (int i = 0; i < primitives.size(); i++) {
        dxIndexData.push_back(primitives[i].a);
        dxIndexData.push_back(primitives[i].b);
        if (primitiveType == Primitive::TYPE::TRIANGLE) {
            dxIndexData.push_back(primitives[i].c);
        }
    }

    ID3D11Device* dxDevice = graphics->getWindow()->getDxDevice();
    ID3D11DeviceContext* dxContext = graphics->getWindow()->getDxContext();

    if (dxVertexBuffer != nullptr) {
        dxVertexBuffer->Release(); dxVertexBuffer = nullptr;
    }
    if (dxIndexBuffer != nullptr) {
        dxIndexBuffer->Release(); dxIndexBuffer = nullptr;
    }

    ZeroMemory(&dxVertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
    dxVertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    dxVertexBufferDesc.ByteWidth = sizeof(FLOAT)*dxVertexData.size();
    dxVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    dxVertexBufferDesc.CPUAccessFlags = 0;

    ZeroMemory(&dxVertexBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
    dxVertexBufferData.pSysMem = dxVertexData.data();

    dxDevice->CreateBuffer(&dxVertexBufferDesc, &dxVertexBufferData, &dxVertexBuffer);

    ZeroMemory(&dxIndexBufferDesc, sizeof(D3D11_BUFFER_DESC));
    dxIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    dxIndexBufferDesc.ByteWidth = sizeof(WORD)*dxIndexData.size();
    dxIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    dxIndexBufferDesc.CPUAccessFlags = 0;

    ZeroMemory(&dxIndexBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
    dxIndexBufferData.pSysMem = dxIndexData.data();

    dxDevice->CreateBuffer(&dxIndexBufferDesc, &dxIndexBufferData, &dxIndexBuffer);

    isDirty = false;
}

void Mesh::render() {
    ID3D11DeviceContext* dxContext = graphics->getWindow()->getDxContext();

    updateInternalData();

    graphics->updateDxCBuffer(worldMatrix);

    graphics->useVertexInputLayout();

    UINT stride = sizeof(FLOAT) * 12;
    UINT offset = 0;
    dxContext->IASetVertexBuffers(0, 1, &dxVertexBuffer, &stride, &offset);
    dxContext->IASetIndexBuffer(dxIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    D3D11_PRIMITIVE_TOPOLOGY dxPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    int dxIndexMultiplier = 3;
    if (primitiveType == Primitive::TYPE::LINE) {
        dxPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
        dxIndexMultiplier = 2;
    }

    dxContext->IASetPrimitiveTopology(dxPrimitiveTopology);

    material->getShader()->useShader();
    graphics->useMatrixCBuffer();
    graphics->useSampler();
    material->getTexture(0)->useTexture(0);

    dxContext->DrawIndexed(primitives.size()*dxIndexMultiplier, 0, 0);
}

int Mesh::addVertex(const Vertex& v) {
    isDirty = true;
    vertices.push_back(v);
    return vertices.size()-1;
}

void Mesh::removeVertex(int v) {
    isDirty = true;
    vertices.erase(vertices.begin()+v);
    for (int i=0;i<primitives.size();i++) {
        bool shouldRemove = primitives[i].a==v || primitives[i].b==v;
        if (primitiveType==Primitive::TYPE::TRIANGLE) {
            shouldRemove |= primitives[i].c==v;
        }
        if (shouldRemove) {
            primitives.erase(primitives.begin()+i); i--;
            continue;
        }
        if (primitives[i].a>v) { primitives[i].a--; }
        if (primitives[i].b>v) { primitives[i].b--; }
        if (primitiveType==Primitive::TYPE::TRIANGLE) {
            if (primitives[i].c>v) { primitives[i].c--; }
        }
    }
}

void Mesh::clear() {
    isDirty = true;
    vertices.clear();
    primitives.clear();
}

int Mesh::addPrimitive(const Primitive& t) {
    isDirty = true;
    primitives.push_back(t);
    return primitives.size()-1;
}

void Mesh::removePrimitive(int t) {
    isDirty = true;
    primitives.erase(primitives.begin()+t);
}

void Mesh::setMaterial(Material* m) {
    isDirty = true;
    material = m;
}

const std::vector<Vertex>& Mesh::getVertices() const {
    return vertices;
}

const std::vector<Primitive>& Mesh::getPrimitives() const {
    return primitives;
}
