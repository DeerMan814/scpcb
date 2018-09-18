#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <Vector.h>

class Entity {
    public:
        virtual int getCollisionCategory() const;
        GX::Vector3f getPosition() const;
        virtual GX::Vector3f getRotation() const;

        virtual void setCollisionCategory(int category);
        virtual void setPosition(GX::Vector3f pos);
        virtual void setRotation(GX::Vector3f rot);
    protected:
        Entity();
        ~Entity();

        GX::Vector3f position;
};

#endif
