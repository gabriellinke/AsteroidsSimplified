#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "Object.h"
#include "constants.h"
#include "Bullet.h"

class Spaceship : public Object
{
public:
    Spaceship(float x, float y, int id);
    virtual ~Spaceship();
    void update();
    int getObjectAsMessage();

    int setAngle(int newAngle);
    Bullet* shoot(int new_bullet_id);

private:
    int angle;
};

#endif // SPACESHIP_H
