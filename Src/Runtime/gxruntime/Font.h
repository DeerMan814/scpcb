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

#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

#include <ft2build.h>
#include FT_FREETYPE_H
#include <vector>
#include <map>

#include "../../World/Components/Graphics.h"

#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include "Mesh.h"

#include "../Misc/String.h"
#include "../Math/Vector.h"
#include "../Math/Rectangle.h"

namespace GX {

struct Font {
    public:
        Font(Graphics* gfx,const String& fn,int h,Shader* s);
        ~Font();

        void draw(String text,Vector3f pos,Vector2f scale=Vector2f::one,Vector3f rotation=Vector3f::zero,Color color=Color(1.f,1.f,1.f,1.f));
        void draw(String text,Vector2f pos,Vector2f scale=Vector2f::one,float rotation=0.f,Color color=Color(1.f,1.f,1.f,1.f));

        struct Atlas {
            Texture* texture;
            Material* material;
            Mesh* mesh;
        };

        std::vector<Atlas> atlases;
    private:
        Font();

        void draw(String text,Vector3f pos,Vector2f scale,Vector3f rotation,Color color,bool inWorld);

        struct GlyphData {
            int atlasIndex;
            Vector2f drawOffset;
            float horizontalAdvance;
            Rectanglef srcRect;
        };

        Shader* shader;

        const int atlasDims = 32;

        void renderAtlas(long chr);

        int height;
        FT_Face freeTypeFace;

        std::map<long,GlyphData> glyphData;

        Graphics* graphics;

        String filename;
};

}

#endif
