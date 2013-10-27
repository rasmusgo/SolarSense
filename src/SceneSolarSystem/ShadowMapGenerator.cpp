#include "ShadowMapGenerator.hpp"
#include "ShadowCaster.hpp"

ShadowMapGenerator::ShadowMapGenerator() {
    this->setUpdatePriority(900000);

    shadowMapBias = 1e-10;
    shadowMVP = mat4f(1.0f);

    createTexture();
}

void ShadowMapGenerator::createTexture(){
    tShadowMap = new Texture(7);
    tShadowMap->loadRawDepthTexture(shadowMapTexture, SCRWIDTH, SCRHEIGHT);
    std::vector<GLuint> attachments;
    std::vector<Texture*> textures;
    textures.push_back(tShadowMap);
    pShadowFBO = new FramebufferObject(attachments, textures, true);
}

void ShadowMapGenerator::update(float deltaTime) {
    (void) deltaTime;

    VBE_LOG("Drawing shadow map");

    glViewport( 0, 0, SCRWIDTH, SCRHEIGHT);
    pShadowFBO->bind();

    // //glEnable( GL_DEPTH_TEST )
    glDisable(GL_TEXTURE_2D);
    //glDisable(GL_CULL_FACE);
    glClearColor(1.f,1.f,1.f,0.5f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float FOV = 90.0f; //degrees
    float ZNEAR = 1.0f;
    float ZFAR = 400.0f;

    mat4f lightViewMatrix = glm::lookAt(vec3f(1.0,0.0,0.0),vec3f(0.0,1.0,0.0),vec3f(0.0,1.0,0.0));
    mat4f lightProjectionMatrix = glm::perspective(FOV,(float) SCRWIDTH/SCRHEIGHT,ZNEAR,ZFAR);
    shadowMVP = lightProjectionMatrix*lightViewMatrix;

    drawShadowMap(getGame()->getRoot());

    pShadowFBO->unbind();
}

void ShadowMapGenerator::drawShadowMap(GameObject* o) {

    ShadowCaster* sc = dynamic_cast<ShadowCaster*>(o);
    if (sc)
        sc->castShadow(shadowMVP, shadowMapBias);

    for(std::list<GameObject*>::const_iterator it = o->getChildren().begin(); it != o->getChildren().end(); ++it)
        drawShadowMap((*it));
}

const Texture* ShadowMapGenerator::getShadowMap() const {
    return tShadowMap;
}
