#include "InteractionManager.hpp"

void InteractionManager::registerListener(InteractionListener *il) {
    listeners.push_back(il);
}
