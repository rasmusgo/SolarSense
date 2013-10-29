#ifndef RINGPLANET_HPP
#define RINGPLANET_HPP

#include "StandardPlanet.hpp"

class RingPlanet : public StandardPlanet {
public:
    RingPlanet(const std::string& name, float radius, float orbRadius, const std::string& shaderprogram,
               const std::string& texture, const std::string& ringtexture, const std::string& ringalphatexture, float distRingStart, float distRingEnd);
    RingPlanet(const std::string& name, float radius, float orbRadius, const std::string& shaderprogram, const std::string& bumpmap,
               const std::string& texture, const std::string& ringtexture, const std::string& ringalphatexture, float distRingStart, float distRingEnd);

    ~RingPlanet();

    void draw() const;
private:
    const std::string ringtexture;
    const std::string ringalphatexture;
    float distRingStart;
    float distRingEnd;
    Model ring;
};

#endif // RINGPLANET_HPP
