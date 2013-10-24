#ifndef WORLDOBJECT_HPP
#define WORLDOBJECT_HPP

#include "Commons.hpp"

class WorldObject : public GameObject {
public:
    WorldObject();

    virtual ~WorldObject();

    virtual void update(float deltaTime);
    virtual void draw() const;

    const vec3f getPosition() const;
    const vec3f getLocalPosition() const;

    const quat getRotation() const;
    const quat getLocalRotation() const;

protected:
    vec3f position;
    quat rotation;
};

#endif // WORLDOBJECT_HPP
