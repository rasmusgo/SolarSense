#ifndef INTERACTIONLISTENER_HPP
#define INTERACTIONLISTENER_HPP

class InteractionListener {
    public:
        //InteractionListener();

        virtual void zoomIn(float speed);
        virtual void zoomOut(float speed);
};

#endif // INTERACTIONLISTENER_HPP
