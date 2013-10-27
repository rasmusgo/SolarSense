#ifndef SHADOWMAPGENERATOR_HPP
#define SHADOWMAPGENERATOR_HPP

#include "Commons.hpp"
#include "FrameBuffer.hpp"

class ShadowMapGenerator : public GameObject {
public:
    ShadowMapGenerator();

    void update(float deltaTime);

    const Texture* getShadowMap() const;

private:
    void createTexture();

    void drawShadowMap(GameObject* o);

    GLvoid* shadowMapTexture;
    FramebufferObject* pShadowFBO;
    Texture* tShadowMap;

    float shadowMapBias;
    mat4f shadowMVP;
};

#endif // SHADOWMAPGENERATOR_HPP
