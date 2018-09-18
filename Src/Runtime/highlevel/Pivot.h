#ifndef PIVOT_H_INCLUDED
#define PIVOT_H_INCLUDED

#include "Entity.h"

class Pivot : public Entity {
    public:
        GX::Vector2f getCollisionRadii() const;
        void setCollisionRadii(GX::Vector2f radii);

};

#endif
