#include "Light.h"

namespace AD {

    Light::Light()
        : m_Rotation(0.0f), m_Translation(0.0f)
    {

    }

    Light::~Light()
    {

    }

    void Light::SetTranslation(const glm::vec3& translation)
    {
        m_Translation = translation;
    }

    void Light::SetRotation(const glm::vec3& rotation)
    {
        m_Rotation = rotation;
    }

    const glm::vec3& Light::GetTranslation()
    {
        return m_Translation;
    }

    const glm::vec3& Light::GetRotation()
    {
        return m_Rotation;
    }

    glm::vec3& Light::GetTranslationRef()
    {
        return m_Translation;
    }

    glm::vec3& Light::GetRotationRef()
    {
        return m_Rotation;
    }

    Light* Light::DeepCopy(Light* light)
    {
        Light* copy = new Light();
        return copy;
    }
}