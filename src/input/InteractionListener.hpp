#ifndef INTERACTIONLISTENER_HPP
#define INTERACTIONLISTENER_HPP

class InteractionListener {
    public:
        //InteractionListener();

        virtual void moveUp(float speed);
        virtual void moveDown(float speed);
        virtual void moveLeft(float speed);
        virtual void moveRight(float speed);

        virtual void zoomIn(float speed);
        virtual void zoomOut(float speed);
};

#endif // INTERACTIONLISTENER_HPP
