#include "Bullet.h"

Bullet::Bullet(float pos_x, float pos_y, float velocity_x, float velocity_y, int id)
    : Object(pos_x, pos_y, BULLET_SIZE, BULLET_SIZE, id, BULLET)
{
    this->velocity_x = velocity_x;
    this->velocity_y = velocity_y;
}

Bullet::~Bullet()
{
    // Destructor code here
}

void Bullet::update()
{
    // Update logic for bullet
    pos_x += velocity_x;
    pos_y += velocity_y;

    if(pos_x > X_MAX || pos_y > Y_MAX || pos_x < 0 || pos_y < 0){
        isAlive = false;
    }
}


int Bullet::getObjectAsMessage()
{
    int messageObj = id << SHIFT_ID | type << SHIFT_TYPE | (int)pos_x << SHIFT_COORDS_X | (int)pos_y << SHIFT_COORDS_Y;
    return messageObj;
}
