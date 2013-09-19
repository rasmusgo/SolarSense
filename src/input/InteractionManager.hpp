#ifndef INTERACTIONMANAGER_HPP
#define INTERACTIONMANAGER_HPP

#include "tools.hpp"
#include "InteractionListener.hpp"

class InteractionManager {
    public:
        static InteractionManager& getInstance() {
            static InteractionManager instance;
            return instance;
        }

        void registerListener(InteractionListener* il);

    private:
        InteractionManager() {}
        InteractionManager(InteractionManager const&);
        void operator=(InteractionManager const&);

        std::list<InteractionListener*> listeners;
};

#endif // INTERACTIONMANAGER_HPP
