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

#include "SysEvents.h"

using namespace GX;

std::set<SysEvents::Subscriber*> SysEvents::subscribers;

void SysEvents::subscribe(SysEvents::Subscriber& subscriber) {
    subscribers.emplace(&subscriber);
}

void SysEvents::unsubscribe(SysEvents::Subscriber& subscriber) {
    std::set<Subscriber*>::iterator it = subscribers.find(&subscriber);
    if (it != subscribers.end()) {
        subscribers.erase(it);
    }
}

void SysEvents::update() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        for (std::set<Subscriber*>::iterator it=subscribers.begin();it!=subscribers.end();it++) {
            Subscriber* subscriber = *it;
            bool takeEvent = false;
            if (subscriber->getEventType()==Subscriber::EventType::WINDOW) {
                if (event.type == SDL_WINDOWEVENT) {
                    takeEvent = event.window.windowID == SDL_GetWindowID(subscriber->getWindow());
                }
            } else if (subscriber->getEventType()==Subscriber::EventType::KEYBOARD) {
                if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                    takeEvent = SDL_GetKeyboardFocus()==subscriber->getWindow();
                }
            } else if (subscriber->getEventType()==Subscriber::EventType::MOUSE) {
                if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP ||
                    event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEWHEEL) {
                    takeEvent = SDL_GetMouseFocus()==subscriber->getWindow();
                }
            }
            //TODO: capture gamepad events
            if (takeEvent) {
                subscriber->pushEvent(event);
            }
        }
    }
}

SysEvents::Subscriber::Subscriber() {}

SysEvents::Subscriber::Subscriber(SDL_Window* w,EventType et) {
    window = w; eventType = et;
    events.clear();
}

SDL_Window* SysEvents::Subscriber::getWindow() const {
    return window;
}

SysEvents::Subscriber::EventType SysEvents::Subscriber::getEventType() const {
    return eventType;
}

bool SysEvents::Subscriber::popEvent(SDL_Event& e) {
    if (events.size()==0) { return false; }
    e = events[0]; events.erase(events.begin());
    return true;
}

void SysEvents::Subscriber::pushEvent(SDL_Event e) {
    events.push_back(e);
}
