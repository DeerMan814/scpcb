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

#include "Shader.h"
#include <fstream>

using namespace GX;

Shader::Shader(Graphics* gfx,const String& path) {    
    graphics = gfx;

    filepath = path;

    char* buf = new char[512];

    vertexShaderBytecode.clear();
    std::ifstream vertexSourceFile; vertexSourceFile.open(String(path,"vertex.dxbc").cstr());
    SDL_Log("Shader file: %s %s",path.cstr(),String(path,"vertex.dxbc").cstr());
    while (!vertexSourceFile.eof()) {
        int writeInd = vertexShaderBytecode.size();
        vertexSourceFile.read(buf,512); int bytesRead = vertexSourceFile.gcount();
        if (bytesRead<=0) { break; }
        vertexShaderBytecode.resize(vertexShaderBytecode.size()+bytesRead);
        memcpy(&(vertexShaderBytecode[writeInd]),buf,bytesRead);
    }
    SDL_Log("Shader code size: %d",vertexShaderBytecode.size());
    vertexSourceFile.close();

    fragmentShaderBytecode.clear();
    std::ifstream fragmentSourceFile; fragmentSourceFile.open(String(path,"fragment.dxbc").cstr());
    while (!fragmentSourceFile.eof()) {
        int writeInd = fragmentShaderBytecode.size();
        fragmentSourceFile.read(buf,512); int bytesRead = fragmentSourceFile.gcount();
        if (bytesRead<=0) { break; }
        fragmentShaderBytecode.resize(fragmentShaderBytecode.size()+bytesRead);
        memcpy(&(fragmentShaderBytecode[writeInd]),buf,bytesRead);
    }
    fragmentSourceFile.close();

    ID3D11Device* dxDevice = graphics->getWindow()->getDxDevice();
    dxDevice->CreateVertexShader(vertexShaderBytecode.data(),sizeof(uint8_t)*vertexShaderBytecode.size(),NULL,&dxVertexShader);
    dxDevice->CreatePixelShader(fragmentShaderBytecode.data(),sizeof(uint8_t)*fragmentShaderBytecode.size(),NULL,&dxFragmentShader);

    delete[] buf;
}

Shader::~Shader() {
    dxVertexShader->Release();
    dxFragmentShader->Release();
}

uint8_t* Shader::getDxVsCode() {
    return vertexShaderBytecode.data();
}

int Shader::getDxVsCodeLen() const {
    return vertexShaderBytecode.size();
}

uint8_t* Shader::getDxFsCode() {
    return fragmentShaderBytecode.data();
}

int Shader::getDxFsCodeLen() const {
    return fragmentShaderBytecode.size();
}

void Shader::useShader() {
    ID3D11DeviceContext* dxContext = graphics->getWindow()->getDxContext();
    dxContext->VSSetShader(dxVertexShader,NULL,0);
    dxContext->PSSetShader(dxFragmentShader,NULL,0);
}
