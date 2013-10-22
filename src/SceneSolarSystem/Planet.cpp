#include "Planet.hpp"

Planet::Planet(vec3f scale, float orbRadius, float orbSpeed)
        : orbRadius(orbRadius), orbSpeed(orbSpeed), timeAcc(0.0f) {

}

Planet::~Planet(){
}
