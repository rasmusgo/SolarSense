#ifndef WORLDOBJECT_HPP
#define WORLDOBJECT_HPP

#include "Commons.hpp"

class WorldObject : public GameObject {
public:
    WorldObject();

    virtual ~WorldObject();

    virtual void update(float deltaTime, float time);
    virtual void draw() const;

    const vec3f getPosition() const;
    const vec3f getLocalPosition() const;

    const quat getRotation() const;
    const quat getLocalRotation() const;

    const vec3f getScale() const;
    const vec3f getLocalScale() const;

protected:
    vec3f position;
    quat rotation;
    vec3f scale;

    void onObjectAdd(GameObject* object);
};

#endif // WORLDOBJECT_HPP
