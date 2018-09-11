#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <Vector.h>

class Entity {
    public:
        int getCollisionCategory() const;
        GX::Vector2f getCollisionRadii() const;
        GX::Vector3f getPosition() const;

        void setCollisionCategory(int coll);
        void setCollisionRadii(GX::Vector2f radii);
        void setPosition(GX::Vector3f pos);
    protected:
        Entity();
        ~Entity();

        int collisionCategory;
        GX::Vector3f position;
};

#endif
