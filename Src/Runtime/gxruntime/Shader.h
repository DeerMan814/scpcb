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

#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>

#include <inttypes.h>
#include <vector>

#include "Graphics.h"
#include "StringType.h"

namespace GX {

struct Shader {
    public:
        Shader::Shader(Graphics* gfx, const String& path);
        ~Shader();

        void useShader();

        uint8_t* getDxVsCode(); int getDxVsCodeLen() const;
        uint8_t* getDxFsCode(); int getDxFsCodeLen() const;
    protected:
        Shader(){};
        String filepath;

        std::vector<uint8_t> vertexShaderBytecode;
        std::vector<uint8_t> fragmentShaderBytecode;

        ID3D11VertexShader* dxVertexShader;
        ID3D11PixelShader* dxFragmentShader;

        Graphics* graphics;
};

}

#endif
