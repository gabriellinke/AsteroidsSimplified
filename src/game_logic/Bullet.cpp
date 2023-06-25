#include "Bullet.h"

Bullet::Bullet(int pos_x, int pos_y, int pos_final_x, int pos_final_y, int id)
    : Object(pos_x, pos_y, 0, 0, id, 0), m(0), n(0)
{
    calculateLine(pos_x, pos_y, pos_final_x, pos_final_y);
}

Bullet::~Bullet()
{
    // Destructor code here
}

void Bullet::update()
{
    // Update logic for bullet
}

void Bullet::calculateLine(int x1, int y1, int x2, int y2)
{
    // Logic to calculate the line equation for the bullet
}
