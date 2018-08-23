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

#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include <SDL.h>

namespace GX {

class UserInput {
    public:
        enum class DEVICE {
            KEYBOARD,
            MOUSE,
            GAMEPAD
        };

        virtual DEVICE getDevice() const = 0;

        bool isDown() const;
        void setDown(bool down);
    protected:
        UserInput();

        bool inputDown;
};

class KeyboardInput : public UserInput {
    public:
        virtual DEVICE getDevice() const;

        SDL_Scancode getButton() const;

        KeyboardInput(SDL_Scancode inKeyCode);
    private:
        SDL_Scancode keyCode;
};

class MouseInput : public UserInput {
    public:
        enum class BUTTON {
            LEFT,
            RIGHT,
            MIDDLE,
            SIDE1,
            SIDE2
        };

        virtual DEVICE getDevice() const;

        BUTTON getButton() const;

        MouseInput(BUTTON inMouseButton);
    private:
        BUTTON mouseButton;
};

class GamepadInput : public UserInput {
    public:
        virtual DEVICE getDevice() const;

        SDL_GameControllerButton getButton() const;

        GamepadInput(SDL_GameControllerButton inGamepadButton);
    private:
        SDL_GameControllerButton gamepadButton;
};

}

#endif

