#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include <vector>

#include <StringType.h>
#include <Mesh.h>

#include "Entity.h"

class Model : public Entity {
    public:
        Model(const String& filepath,GX::Material* material);
        virtual ~Model();

        GX::Vector3f getScale() const;
        GX::Vector3f getRotation() const;

        void setScale(GX::Vector3f scl);
        void setRotation(GX::Vector3f rot);
    protected:
        Model();
        
        std::vector<GX::Mesh*> surfaces;

        GX::Vector3f scale;
        GX::Vector3f rotation;
};

#endif
