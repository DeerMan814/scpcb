#include "Entity.h"

int Entity::getCollisionCategory() const {
    return 0;
}

GX::Vector3f Entity::getPosition() const {
    return position;
}

GX::Vector3f Entity::getRotation() const {
    return GX::Vector3f::zero;
}

void Entity::setCollisionCategory(int category) {
    //do nothing since this is not applicable to all entities
}

void Entity::setPosition(GX::Vector3f pos) {
    //TODO: add tweening
    position = pos;
}

void Entity::setRotation(GX::Vector3f rot) {
    //do nothing since this is not applicable to all entities
}

Entity::Entity() {}

Entity::~Entity() {}
