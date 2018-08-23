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

#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#include <set>

#include "SysEvents.h"
#include "UserInput.h"
#include "Window.h"

#include "Vector.h"

namespace GX {

class IO {
    private:
        SysEvents::Subscriber keyboardSubscriber;
        SysEvents::Subscriber mouseSubscriber;
        SysEvents::Subscriber gamepadSubscriber;

        std::set<UserInput*> inputs;
        Vector2i mousePos;
    public:
        IO(Window* window);
        ~IO();

        void update();

        void trackInput(UserInput* input);
        void untrackInput(UserInput* input);

        Vector2i getMousePosition() const;
};

}

#endif

