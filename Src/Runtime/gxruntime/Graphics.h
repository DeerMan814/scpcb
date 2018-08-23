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

#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Window.h"
#include "Rectangle.h"
#include "Matrix.h"
#include "Color.h"

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>

namespace GX {

    class Shader;
    class Graphics {
    public:

        Graphics(int w = 1280, int h = 720, bool fs = false);
        ~Graphics();

        Window* getWindow() const;
        FT_Library* getFreeTypeLibrary();

        void update();
        void swap(bool vsync = true);

        Matrix4x4f projectionMatrix;
        Matrix4x4f viewMatrix;

        Rectanglei viewport;

        void updateViewport();
        void updateDxCBuffer(Matrix4x4f worldMatrix);
        void useVertexInputLayout();
        void useMatrixCBuffer();
        void useSampler();

        virtual void clear(Color color);

        ID3D11RenderTargetView* currentRenderTargetView;

        Shader* getDefaultShader();
    private:
        Rectanglei currentViewport;

        D3D11_INPUT_ELEMENT_DESC dxVertexInputElemDesc[4];
        ID3D11InputLayout* dxVertexInputLayout;

        D3D11_BUFFER_DESC dxMatrixCBufferDesc;
        D3D11_SUBRESOURCE_DATA dxMatrixCBufferData;
        ID3D11Buffer* dxMatrixCBuffer;

        D3D11_SAMPLER_DESC dxSamplerDesc;
        ID3D11SamplerState* dxSamplerState;

        Shader* defaultShader;

        FT_Library freeTypeLib;

        Window* window;
    };
}


#endif
