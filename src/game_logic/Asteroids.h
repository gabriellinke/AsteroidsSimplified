#ifndef ASTEROIDS_H
#define ASTEROIDS_H

#include "Object.h"

class Asteroids : public Object
{
public:
    Asteroids(int x, int y, int id, bool big);
    virtual ~Asteroids();
    void update();

private:
    int vel_x, vel_y;
};

#endif // ASTEROIDS_H
