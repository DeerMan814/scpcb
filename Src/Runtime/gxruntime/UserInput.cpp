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

#include "UserInput.h"

using namespace GX;

UserInput::UserInput() {
    inputDown = false;
}

bool UserInput::isDown() const {
    return inputDown;
}

void UserInput::setDown(bool down) {
    inputDown = down;
}

KeyboardInput::KeyboardInput(SDL_Scancode inKeyCode) {
    keyCode = inKeyCode;
}

UserInput::DEVICE KeyboardInput::getDevice() const {
    return UserInput::DEVICE::KEYBOARD;
}

SDL_Scancode KeyboardInput::getButton() const {
    return keyCode;
}

MouseInput::MouseInput(MouseInput::BUTTON inMouseButton) {
    mouseButton = inMouseButton;
}

UserInput::DEVICE MouseInput::getDevice() const {
    return UserInput::DEVICE::MOUSE;
}

MouseInput::BUTTON MouseInput::getButton() const {
    return mouseButton;
}

GamepadInput::GamepadInput(SDL_GameControllerButton inGamepadButton) {
    gamepadButton = inGamepadButton;
}

UserInput::DEVICE GamepadInput::getDevice() const {
    return UserInput::DEVICE::GAMEPAD;
}

SDL_GameControllerButton GamepadInput::getButton() const {
    return gamepadButton;
}
