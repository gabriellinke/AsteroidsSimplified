#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "Object.h"
#include "constants.h"

class Spaceship : public Object
{
public:
    Spaceship(int x, int y, int id);
    virtual ~Spaceship();
    void update();
    int getObjectAsMessage();

private:
    int aim_x;
    int aim_y;
};

#endif // SPACESHIP_H
