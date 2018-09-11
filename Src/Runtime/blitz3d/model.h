
#ifndef MODEL_H
#define MODEL_H

#include <vector>

#include "../gxruntime/Texture.h"
#include "../gxruntime/Material.h"

#include "object.h"
#include "rendercontext.h"

class Sprite;
class MeshModel;

class Model : public Object {
public:
    enum {
        RENDER_SPACE_LOCAL = 0,
        RENDER_SPACE_WORLD = 1
    };
    enum {
        COLLISION_GEOMETRY_DEFAULT = 0,
        COLLISION_GEOMETRY_TRIS = 1,
        COLLISION_GEOMETRY_BOX = 2,
        COLLISION_GEOMETRY_SPHERE = 3
    };
    enum {
        QUEUE_OPAQUE = 0,
        QUEUE_TRANSPARENT = 1
    };

    Model();
    Model(const Model &m);

    //Entity interface
    Model *getModel() { return this; }

    //Object interface
    void capture();
    bool beginRender(float tween);

    //Model interface
    virtual bool render(const RenderContext &rc) { return false; }
    virtual void renderQueue(int type);

    virtual Sprite *getSprite() { return 0; }
    virtual MeshModel *getMeshModel() { return 0; }

    virtual void setMaterial(const GX::Material &b);
    virtual void setColor(const Vector &c);
    virtual void setAlpha(float a);
    virtual void setShininess(float t);
    virtual void setTexture(int i, const GX::Texture &t, int f);
    virtual void setBlend(int n);
    virtual void setFX(int n);

    const GX::Material &getMaterial() const;

    void setRenderSpace(int n) { space = n; }
    int getRenderSpace()const { return space; }

    void setAutoFade(float nr, float fr) { auto_fade_nr = nr; auto_fade_fr = fr; auto_fade = true; }

    bool doAutoFade(const Vector &eye);

    //void enqueue(gxMesh *mesh, int first_vert, int vert_cnt, int first_tri, int tri_cnt);
    //void enqueue(gxMesh *mesh, int first_vert, int vert_cnt, int first_tri, int tri_cnt, const Brush &b);

    int queueSize(int type)const { return queues[type].size(); }

private:
    class MeshQueue;

    int space;
    GX::Material material;
    //Brush brush, render_brush;

    mutable bool w_brush;
    float captured_alpha, tweened_alpha;

    bool auto_fade;
    float auto_fade_nr, auto_fade_fr;

    std::vector<MeshQueue*> queues[2];

    void enqueue(MeshQueue *q);
};

#endif
