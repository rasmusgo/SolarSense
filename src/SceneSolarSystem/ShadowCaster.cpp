#include "ShadowCaster.hpp"

ShadowCaster::ShadowCaster() {
    shadowProgram = Programs.get("shadowMap");
}
