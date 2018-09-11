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

#include "Texture.h"
#include <FreeImage.h>
#include <inttypes.h>

using namespace GX;

Texture::Texture(Graphics* gfx, int w, int h, bool renderTarget, const void* buffer) {
    graphics = gfx;
    ID3D11Device* dxDevice = graphics->getWindow()->getDxDevice();
    ID3D11DeviceContext* dxContext = graphics->getWindow()->getDxContext();

    filename = "<n/a>";

    void* newBuffer = nullptr;

    if (renderTarget) {
        name = "RenderTarget";
        buffer = nullptr;
    }
    else {
        name = "Static";
        if (buffer == nullptr) {
            name = String(name, "_Blank");
            newBuffer = new uint8_t[width*height * 4];
            memset(newBuffer, 0, width*height * 4 * sizeof(uint8_t));
            buffer = newBuffer;
        }
        else {
            name = String(name, "_Buffer");
        }
    }

    width = w; height = h;
    realWidth = width; realHeight = height;

    ZeroMemory(&dxTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
    dxTextureDesc.Width = (UINT)realWidth;
    dxTextureDesc.Height = (UINT)realHeight;
    dxTextureDesc.MipLevels = (UINT)1;
    dxTextureDesc.ArraySize = 1;
    dxTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    dxTextureDesc.SampleDesc.Count = 1;
    dxTextureDesc.SampleDesc.Quality = 0;
    dxTextureDesc.Usage = D3D11_USAGE_IMMUTABLE;
    dxTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    dxTextureDesc.CPUAccessFlags = 0;

    ZeroMemory(&dxTextureData, sizeof(D3D11_SUBRESOURCE_DATA));
    dxTextureData.pSysMem = buffer;
    dxTextureData.SysMemPitch = realWidth * 4;
    dxTextureData.SysMemSlicePitch = realWidth * realHeight * 4;

    HRESULT hr = 0;

    hr = dxDevice->CreateTexture2D(&dxTextureDesc, &dxTextureData, &dxTexture);
    if (FAILED(hr)) {
        SDL_Log("1. %d %d %d\n", realWidth, realHeight, hr);
    }

    ZeroMemory(&dxShaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    dxShaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    dxShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    dxShaderResourceViewDesc.Texture2D.MipLevels = 1;
    hr = dxDevice->CreateShaderResourceView(dxTexture, &dxShaderResourceViewDesc, &dxShaderResourceView);
    if (FAILED(hr)) {
        SDL_Log("2. %d\n", hr);
    }

    isRT = renderTarget;
    /*if (isRT) {
    glGenFramebuffers(1,&glFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER,glFramebuffer);
    TODO: FINISH THIS
    }*/

    if (newBuffer != nullptr) { delete[] newBuffer; }
}

Texture::Texture(Graphics* gfx, const String& fn) {
    graphics = gfx;
    ID3D11Device* dxDevice = graphics->getWindow()->getDxDevice();
    ID3D11DeviceContext* dxContext = graphics->getWindow()->getDxContext();

    filename = fn;
    name = fn;

    FIBITMAP* fiBuffer = loadFIBuffer(filename, width, height, realWidth, realHeight);

    ZeroMemory(&dxTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
    dxTextureDesc.Width = (UINT)realWidth;
    dxTextureDesc.Height = (UINT)realHeight;
    dxTextureDesc.MipLevels = (UINT)1;
    dxTextureDesc.ArraySize = 1;
    dxTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    dxTextureDesc.SampleDesc.Count = 1;
    dxTextureDesc.SampleDesc.Quality = 0;
    dxTextureDesc.Usage = D3D11_USAGE_IMMUTABLE;
    dxTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    dxTextureDesc.CPUAccessFlags = 0;

    ZeroMemory(&dxTextureData, sizeof(D3D11_SUBRESOURCE_DATA));
    dxTextureData.pSysMem = FreeImage_GetBits(fiBuffer);
    dxTextureData.SysMemPitch = realWidth * 4;
    dxTextureData.SysMemSlicePitch = realWidth * realHeight * 4;

    HRESULT hr = 0;

    hr = dxDevice->CreateTexture2D(&dxTextureDesc, &dxTextureData, &dxTexture);
    if (FAILED(hr)) {
        SDL_Log("1. %d %d %d\n", realWidth, realHeight, hr);
    }

    ZeroMemory(&dxShaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    dxShaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    dxShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    dxShaderResourceViewDesc.Texture2D.MipLevels = 1;
    hr = dxDevice->CreateShaderResourceView(dxTexture, &dxShaderResourceViewDesc, &dxShaderResourceView);
    if (FAILED(hr)) {
        SDL_Log("2. %d\n", hr);
    }

    isRT = false;

    FreeImage_Unload(fiBuffer);
}

Texture::~Texture() {
    dxShaderResourceView->Release();
    dxTexture->Release();
}

void Texture::useTexture(int layer) {
    ID3D11DeviceContext* dxContext = graphics->getWindow()->getDxContext();
    dxContext->PSSetShaderResources(layer, 1, &dxShaderResourceView);
}

bool Texture::isRenderTarget() const {
    return isRT;
}

FIBITMAP* Texture::loadFIBuffer(String filename,int& width,int& height,int& realWidth,int& realHeight) {
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename.cstr(),0);

    FIBITMAP* temp = FreeImage_Load(format, filename.cstr());
    FIBITMAP* image = FreeImage_ConvertTo32Bits(temp);
    FreeImage_Unload(temp);

    int w = FreeImage_GetWidth(image);
    int h = FreeImage_GetHeight(image);

    for (int x=0;x<w;x++) {
        for (int y=0;y<h;y++) {
            RGBQUAD rgba;
            FreeImage_GetPixelColor(image,x,y,&rgba);
            int red = rgba.rgbRed;
            int blue = rgba.rgbBlue;
            rgba.rgbRed = blue; rgba.rgbBlue = red;
            FreeImage_SetPixelColor(image,x,y,&rgba);
        }
    }

    width = w; height = h;

    int po2w = 1; int po2h = 1;
    while (po2w<w) { po2w*=2; }
    while (po2h<h) { po2h*=2; }

    if (po2w!=w || po2h!=h) {
        temp = image;
        image = FreeImage_Rescale(temp,po2w,po2h,FILTER_BILINEAR);
        FreeImage_Unload(temp);
    }

    realWidth = po2w; realHeight = po2h;

    return image;
}


