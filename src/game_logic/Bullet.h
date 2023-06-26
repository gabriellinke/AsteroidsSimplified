#ifndef BULLET_H
#define BULLET_H

#include "Object.h"
#include "constants.h"

class Bullet : public Object
{
public:
    Bullet(float pos_x, float pos_y, float velocity_x, float velocity_y, int id);
    virtual ~Bullet();
    void update();
    int getObjectAsMessage();

private:
    float velocity_x;
    float velocity_y;
};

#endif // BULLET_H
