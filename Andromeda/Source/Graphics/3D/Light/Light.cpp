#include "Light.h"

namespace AD {

    Light::Light()
    {

    }

    Light::~Light()
    {

    }

    Light* Light::DeepCopy(Light* light)
    {
        Light* copy = new Light();
        return copy;
    }
}