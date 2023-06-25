#ifndef ASTEROIDS_H
#define ASTEROIDS_H

#include "Object.h"
#include "constants.h"

class Asteroids : public Object
{
public:
    Asteroids(int x, int y, int id, bool big);
    virtual ~Asteroids();
    void update();
    int getObjectAsMessage();

private:
    int vel_x, vel_y;
};

#endif // ASTEROIDS_H
