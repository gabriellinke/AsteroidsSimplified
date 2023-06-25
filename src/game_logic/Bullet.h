#ifndef BULLET_H
#define BULLET_H

#include "Object.h"

class Bullet : public Object
{
public:
    Bullet(int pos_x, int pos_y, int pos_final_x, int pos_final_y, int id);
    virtual ~Bullet();
    void update();
    void calculateLine(int x1, int y1, int x2, int y2);

private:
    int m; // y = mx + n
    int n;
};

#endif // BULLET_H
