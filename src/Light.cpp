#include "Light.h"

Light::Light(void) : position(), color()
{
}

Light::~Light(void)
{
}

Light::Light(glm::vec3 pos, glm::vec3 col) : position(pos), color(col) {}
