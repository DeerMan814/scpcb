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

#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include "StringType.h"
#include "Graphics.h"

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>

#include <FreeImage.h>

namespace GX {

struct Texture {
    public:
        Texture(Graphics* gfx, int width, int height, bool renderTarget = false, const void* buffer = nullptr);
        Texture(Graphics* gfx, const String& fn);
        ~Texture();

        void useTexture(int layer);

        bool isRenderTarget() const;
    protected:
        static FIBITMAP* loadFIBuffer(String filename,int& width,int& height,int& realWidth,int& realHeight);

        int width; int height;
        int realWidth; int realHeight;

        String filename; String name;

        Texture(){};
        
        Graphics* graphics;

        D3D11_TEXTURE2D_DESC dxTextureDesc;
        D3D11_SUBRESOURCE_DATA dxTextureData;
        ID3D11Texture2D* dxTexture;
        D3D11_SHADER_RESOURCE_VIEW_DESC dxShaderResourceViewDesc;
        ID3D11ShaderResourceView* dxShaderResourceView;

        bool isRT;
};

}

#endif
