#ifndef FIRSTAID_H_INCLUDE
#define FIRSTAID_H_INCLUDE

#include "Item.h"

namespace CBN {

class FirstAid : public Item {
private:
    bool prevCrouch;
    String type;
    FirstAid();

    const float DEFAULT_TIME = 10 * 70;
    float timer;
public:
    static FirstAid* spawn(float x, float y, float z);

    String getType();
    String getInvName();

    void onUse();
    void updateUse();
    void drawUse();
};

}

#endif // FIRSTAID_H_INCLUDE