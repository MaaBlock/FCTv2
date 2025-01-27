#pragma once
#include "../Context/DataTypes.h"
#include "../MutilThreadBase/RefCount.h"
#include "../Context/IPipelineResource.h"
#include "../Context/Context.h"
#include "../Context/ConstBuffer.h"
#include <glad/glad.h>
#include <vector>
#include <atomic>

namespace FCT
{
    class Object : public RefCount
    {
    protected:
        std::vector<IPipelineResource*> m_resources;
        bool m_initialized = false;
        Vec3 m_position{0, 0, 0};
        Vec3 m_rotation{0, 0, 0};
        Vec3 m_scale{1, 1, 1};
        mutable Mat4 m_cachedTransform;
        mutable std::atomic<bool> m_transformDirty{true};
        Texture* m_transform;
        Object *m_parent = nullptr;
        std::vector<Object *> m_children;

        void updateTransform() const;

    public:
        Object(Context* ctx);
        virtual ~Object();

        void setPosition(const Vec3 &pos);
        void setRotation(const Vec3 &rotation);
        void setScale(const Vec3 &scale);
        void rotate(const Vec3 &rotation);

        const Mat4 &worldTransform() const;

        void setParent(Object *parent);
        void addChild(Object *child);
        void addResource(IPipelineResource* res);
        
        virtual void create() = 0;
        void draw() const;

    protected:
        void markDirty();
    };

} // namespace FCT