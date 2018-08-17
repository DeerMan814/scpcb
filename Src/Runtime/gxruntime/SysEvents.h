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

#ifndef SYSEVENTS_H_INCLUDED
#define SYSEVENTS_H_INCLUDED

#include <SDL.h>
#include <set>
#include <vector>

namespace GX {

class SysEvents {
    public:
        struct Subscriber;
    private:
        static std::set<Subscriber*> subscribers;
        SysEvents(){};
    public:
        static void subscribe(Subscriber& sub);
        static void unsubscribe(Subscriber& sub);
        static void update();

        struct Subscriber {
            public:
                enum class EventType {
                    WINDOW,
                    KEYBOARD,
                    MOUSE,
                    GAMEPAD
                };
            private:
                SDL_Window* window;
                EventType eventType;
                std::vector<SDL_Event> events;
                bool receivedEvent;
            public:
                Subscriber();
                Subscriber(SDL_Window* w,EventType et);

                SDL_Window* getWindow() const;
                EventType getEventType() const;
                void pushEvent(SDL_Event e);
                bool popEvent(SDL_Event& e);
        };
};

}

#endif // SYSEVENTS_H_INCLUDED
