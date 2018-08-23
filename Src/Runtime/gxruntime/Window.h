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

#ifndef GXWINDOW_H
#define GXWINDOW_H

#include <SDL.h>
#include <SDL_syswm.h>

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>

#include "StringType.h"
#include "SysEvents.h"

namespace GX {

    class Window {
    public:
        Window(String c, int w, int h, bool fs);
        ~Window();

        virtual void update();
        virtual void swap(bool vsyncEnabled = true);

        ID3D11Device* getDxDevice() const;
        ID3D11DeviceContext* getDxContext() const;
        ID3D11RenderTargetView* getBackBufferRtv() const;

        virtual bool isOpen() const;

        int getWidth() const;
        int getHeight() const;

        SDL_Window* getSdlWindow() const;
    private:
        Window(){};

        String caption;
        int width; int height; bool fullscreen;

        SDL_Window* sdlWindow;
        SysEvents::Subscriber eventSubscriber;

        bool open;
        bool vsync;

        IDXGIFactory1* dxgiFactory;
        DXGI_SWAP_CHAIN_DESC dxSwapChainDesc;
        IDXGISwapChain* dxSwapChain;
        ID3D11Device* dxDevice;
        ID3D11DeviceContext* dxContext;
        ID3D11RenderTargetView* dxBackBufferRtv;
        D3D11_RASTERIZER_DESC dxRasterizerStateDesc;
        ID3D11RasterizerState* dxRasterizerState;
        D3D11_BLEND_DESC dxBlendStateDesc;
        ID3D11BlendState* dxBlendState;
        D3D11_VIEWPORT dxViewport;
    };

}
#endif
