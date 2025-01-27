#include "../headers.h"

namespace FCT
{
    Object::Object(Context* ctx) {
        m_transform = ctx->createTexture();
        m_transform->setSlot(0);
        m_transform->create(4, 1, FCT::Texture::Format::RGBA32F);
    }

    Object::~Object() {
        setParent(nullptr);
        for (auto child : m_children)
        {
            child->release();
        }
        for (auto res : m_resources) {
            res->release();
        }
    }

    void Object::updateTransform() const
    {
        if (m_transformDirty)
        {
            m_cachedTransform.identity();
            m_cachedTransform.translate(m_position.x, m_position.y, m_position.z);
            m_cachedTransform.rotateX(m_rotation.x);
            m_cachedTransform.rotateY(m_rotation.y);
            m_cachedTransform.rotateZ(m_rotation.z);
            m_cachedTransform.scale(m_scale.x, m_scale.y, m_scale.z);

            if (m_parent)
            {
                m_cachedTransform = m_parent->worldTransform() * m_cachedTransform;
            }

            m_transformDirty = false;
        }
    }

    void Object::setPosition(const Vec3 &pos)
    {
        m_position = pos;
        markDirty();
    }

    void Object::setRotation(const Vec3 &rotation)
    {
        m_rotation = rotation;
        markDirty();
    }

    void Object::setScale(const Vec3 &scale)
    {
        m_scale = scale;
        markDirty();
    }

    void Object::rotate(const Vec3 &rotation) {
        m_rotation += rotation;
        markDirty();
    }

    const Mat4 &Object::worldTransform() const
    {
        updateTransform();
        return m_cachedTransform;
    }

    void Object::setParent(Object *parent)
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

    void Object::addChild(Object *child)
    {
        m_children.push_back(child);
        child->addRef();
    }

    void Object::addResource(IPipelineResource* res) {
        m_resources.push_back(res);
        res->addRef();
    }

    void Object::draw() const {
        updateTransform();
        m_transform->setData(&m_cachedTransform, sizeof(m_cachedTransform));
        m_transform->bind();
        for (auto res : m_resources) {
            res->bind();
        }
    }

    void Object::markDirty()
    {
        m_transformDirty = true;
        for (auto child : m_children)
        {
            child->markDirty();
        }
    }

} // namespace FCT