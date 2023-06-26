#ifndef ASTEROIDS_H
#define ASTEROIDS_H

#include "Object.h"
#include "constants.h"

class Asteroids : public Object
{
public:
    Asteroids(float x, float y, int id, bool big);
    virtual ~Asteroids();
    void update();
    int getObjectAsMessage();

private:
    float velocity_x;
    float velocity_y;
};

#endif // ASTEROIDS_H
