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

#include "Font.h"

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

using namespace GX;

Font::Font(Graphics* gfx,const String& fn,int h,Shader* s) {
    graphics = gfx;
    filename = fn;
    height = h;
    shader = s;

    FT_New_Face(*graphics->getFreeTypeLibrary(),
                filename.cstr(),
                0,
                &freeTypeFace);

    FT_Set_Pixel_Sizes(freeTypeFace,
                       0,
                       height);

    glyphData.clear();
    atlases.clear();

    renderAtlas(0);
}

Font::~Font() {
    for (int i=0;i<atlases.size();i++) {
        delete atlases[i].mesh;
        delete atlases[i].material;
        delete atlases[i].texture;
    }

    delete shader;

    FT_Done_Face(freeTypeFace);
}

void Font::renderAtlas(long chr) {
    bool needsNewAtlas = false;

    long startChr = chr-1024;
    if (startChr<0) { startChr = 0; }
    long endChr = startChr+2048;

    uint8_t* buffer = nullptr;
    int x = -1; int y = -1;
    int maxHeight = -1;
    for (long i=startChr;i<endChr;i++) {
        std::map<long,GlyphData>::iterator it = glyphData.find(i);
        if (it==glyphData.end()) {
            long glyphIndex = FT_Get_Char_Index(freeTypeFace,i);
            FT_Load_Glyph(freeTypeFace,
                          glyphIndex,
                          FT_LOAD_DEFAULT);
            if (glyphIndex != 0) {
                FT_Render_Glyph(freeTypeFace->glyph,
                                FT_RENDER_MODE_NORMAL);
                unsigned char* glyphBuffer = freeTypeFace->glyph->bitmap.buffer;
                int glyphWidth = freeTypeFace->glyph->bitmap.width;
                int glyphHeight = freeTypeFace->glyph->bitmap.rows;

                if (glyphWidth>0 && glyphHeight>0) {
                    if (buffer==nullptr) {
                        buffer = new uint8_t[atlasDims*atlasDims*4];
                        for (int j=0;j<atlasDims*atlasDims;j++) {
                            buffer[(j*4)+0]=255;
                            buffer[(j*4)+1]=255;
                            buffer[(j*4)+2]=255;
                            buffer[(j*4)+3]=0;
                        }
                        x=1; y=1; maxHeight=0;
                    }

                    if (x+glyphWidth+1>atlasDims-1) {
                        x=1; y+=maxHeight+1;
                        maxHeight = 0;
                    }
                    if (y+glyphHeight+1>atlasDims-1) {
                        needsNewAtlas = true;
                        break;
                    }
                    if (glyphHeight>maxHeight) { maxHeight = glyphHeight; }

                    for (int j=0;j<glyphWidth*glyphHeight;j++) {
                        int bufferPos = x+y*atlasDims;
                        bufferPos += (j%glyphWidth)+((j/glyphWidth)*atlasDims);
                        buffer[(bufferPos*4)+3]=glyphBuffer[j];
                    }

                    GlyphData gd;
                    gd.atlasIndex = atlases.size();
                    gd.horizontalAdvance = freeTypeFace->glyph->metrics.horiAdvance>>6;
                    gd.drawOffset = Vector2f(-freeTypeFace->glyph->bitmap_left,freeTypeFace->glyph->bitmap_top-height*10/14);
                    gd.srcRect = Rectanglef((float)x/(float)atlasDims,(float)y/(float)atlasDims,(float)(x+glyphWidth)/(float)atlasDims,(float)(y+glyphHeight)/(float)atlasDims);

                    x+=glyphWidth+1;

                    glyphData.emplace(i,gd);
                } else {
                    GlyphData gd;
                    gd.atlasIndex = -1;
                    gd.horizontalAdvance = freeTypeFace->glyph->metrics.horiAdvance>>6;
                    glyphData.emplace(i,gd);
                }
            } else {
                GlyphData gd;
                gd.atlasIndex = -1;
                gd.horizontalAdvance = freeTypeFace->glyph->metrics.horiAdvance>>6;
                glyphData.emplace(i,gd);
            }
        }
    }

    if (buffer!=nullptr) {
        Atlas newAtlas;
        newAtlas.texture = new Texture(graphics,atlasDims,atlasDims,false,buffer);
        newAtlas.material = new Material(shader,newAtlas.texture);
        newAtlas.mesh = new Mesh(graphics,Primitive::TYPE::TRIANGLE);
        newAtlas.mesh->setMaterial(newAtlas.material);
        atlases.push_back(newAtlas);
        delete[] buffer;
    }

    if (needsNewAtlas) {
        renderAtlas(chr);
    }
}

void Font::draw(String text,Vector3f pos,Vector2f scale,Vector3f rotation,Color color) {
    draw(text,pos,scale,rotation,color,true);
}

void Font::draw(String text,Vector2f pos,Vector2f scale,float rotation,Color color) {
    draw(text,Vector3f(pos.x,pos.y,0.1f),scale,Vector3f(0.f,0.f,rotation),color,false);
}

void Font::draw(String text,Vector3f pos,Vector2f scale,Vector3f rotation,Color color,bool inWorld) {
    Matrix4x4f oldViewMat = graphics->viewMatrix;
    Matrix4x4f oldProjMat = graphics->projectionMatrix;
    if (!inWorld) {
        graphics->viewMatrix = Matrix4x4f::identity;
        graphics->projectionMatrix = Matrix4x4f::identity;
        float w = (float)graphics->viewport.width();
        float h = (float)graphics->viewport.height();
        graphics->projectionMatrix.elements[0][0] = 2.f/w;
        graphics->projectionMatrix.elements[1][1] = -2.f/h;
        pos.x -= w*0.5f; pos.y -= h*0.5f;
    }
    Matrix4x4f worldMatrix = Matrix4x4f::constructWorldMat(pos,Vector3f(scale.x,scale.y,1.f),rotation);

    Vector3f currPos = Vector3f::zero;

    for (int i=0;i<atlases.size();i++) {
        atlases[i].mesh->clear();
        atlases[i].mesh->worldMatrix = worldMatrix;
    }
    for (int i=0;i<text.size();i++) {
        long chr = (long)text.wstr()[i];
        std::map<long,GlyphData>::iterator it = glyphData.find(chr);
        if (it==glyphData.end()) {
            renderAtlas(chr);
            it = glyphData.find(chr);
        }
        if (it!=glyphData.end()) {
            if (it->second.atlasIndex>=0) {
                GlyphData gd = it->second;
                Atlas atlas = atlases[gd.atlasIndex];
                Vector3f glyphPos = Vector3f(currPos.x-gd.drawOffset.x,
                                             currPos.y-gd.drawOffset.y,
                                             currPos.z);
                Vector3f glyphPos2 = glyphPos.add(Vector3f(gd.srcRect.width()*atlasDims,gd.srcRect.height()*atlasDims,0.f));
                Vector3f glyphNormal = Vector3f(0.f,0.f,-1.f);
                Vector2f glyphUv = Vector2f(gd.srcRect.topLeftCorner().x,gd.srcRect.topLeftCorner().y);
                Vector2f glyphUv2 = Vector2f(gd.srcRect.bottomRightCorner().x,gd.srcRect.bottomRightCorner().y);

                int vertCount = atlas.mesh->getVertices().size();

                atlas.mesh->addVertex(Vertex(glyphPos,glyphNormal,glyphUv,color));
                atlas.mesh->addVertex(Vertex(Vector3f(glyphPos2.x,glyphPos.y,glyphPos.z),glyphNormal,Vector2f(glyphUv2.x,glyphUv.y),color));
                atlas.mesh->addVertex(Vertex(Vector3f(glyphPos.x,glyphPos2.y,glyphPos.z),glyphNormal,Vector2f(glyphUv.x,glyphUv2.y),color));
                atlas.mesh->addVertex(Vertex(glyphPos2,glyphNormal,glyphUv2,color));

                if (inWorld) {
                    atlas.mesh->addPrimitive(Primitive(vertCount+0,vertCount+1,vertCount+2));
                    atlas.mesh->addPrimitive(Primitive(vertCount+2,vertCount+1,vertCount+3));
                }
                atlas.mesh->addPrimitive(Primitive(vertCount+0,vertCount+2,vertCount+1));
                atlas.mesh->addPrimitive(Primitive(vertCount+1,vertCount+2,vertCount+3));
            }
            currPos = currPos.add(Vector3f(it->second.horizontalAdvance,0.f,0.f));
        }
    }
    for (int i=0;i<atlases.size();i++) {
        if (atlases[i].mesh->getVertices().size()>0) {
            atlases[i].mesh->render();
        }
    }

    if (!inWorld) {
        graphics->viewMatrix = oldViewMat;
        graphics->projectionMatrix = oldProjMat;
    }
}
