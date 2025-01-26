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
    class Shape : public RefCount
    {
    protected:
        std::vector<IPipelineResource*> m_resources;
        bool m_initialized = false;
        Vec2 m_position{0, 0};
        float m_rotation = 0;
        Vec2 m_scale{1, 1};
        mutable Mat4 m_cachedTransform;
        mutable std::atomic<bool> m_transformDirty{true};
        Texture* m_transform;
        Shape *m_parent = nullptr;
        std::vector<Shape *> m_children;

        void updateTransform() const
        {
            if (m_transformDirty)
            {
                m_cachedTransform.identity();
                m_cachedTransform.translate(m_position.x, m_position.y, 0);
                m_cachedTransform.rotateZ(m_rotation);
                m_cachedTransform.scale(m_scale.x, m_scale.y, 1);

                if (m_parent)
                {
                    m_cachedTransform = m_parent->worldTransform() * m_cachedTransform;
                }

                m_transformDirty = false;
            }
        }

    public:
        Shape(Context* ctx) {
			m_transform = ctx->createTexture();
            m_transform->setSlot(0);
            m_transform->create(4, 1, FCT::Texture::Format::RGBA32F);
        }
        virtual ~Shape() {
            setParent(nullptr);
            for (auto child : m_children)
            {
                child->release();
            }
            for (auto res : m_resources) {
                res->release();
            }
        }

        void setPosition(const Vec2 &pos)
        {
            m_position = pos;
            markDirty();
        }

        void setRotation(float degrees)
        {
            m_rotation = degrees;
            markDirty();
        }

        void setScale(const Vec2 &scale)
        {
            m_scale = scale;
            markDirty();
        }

        void rotate(float degrees) {
			setRotation(m_rotation + degrees);
        }

        const Mat4 &worldTransform() const
        {
            updateTransform();
            return m_cachedTransform;
        }

        void setParent(Shape *parent)
        {
            if (m_parent)
            {
                m_parent->release();
            }
            m_parent = parent;
            if (m_parent)
            {
                m_parent->addRef();
            }
        }
        void addChild(Shape *child)
        {
            m_children.push_back(child);
            child->addRef();
        }
        void addResource(IPipelineResource* res) {
            m_resources.push_back(res);
            res->addRef();
        }
        
        virtual void create() = 0;
        void draw() const {
            updateTransform();
			m_transform->setData(&m_cachedTransform,sizeof(m_cachedTransform));
            m_transform->bind();
            for (auto res : m_resources) {
                res->bind();
            }
        }
        

    protected:
        void markDirty()
        {
            m_transformDirty = true;
            for (auto child : m_children)
            {
                child->markDirty();
            }
        }
    };

} // namespace FCT
