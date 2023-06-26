#include "Spaceship.h"
#include <cmath>

Spaceship::Spaceship(float x, float y, int id) : Object(x, y, SPACESHIP_SIZE, SPACESHIP_SIZE, id, SPACESHIP)
{
    angle = 0;
}

Spaceship::~Spaceship()
{
    // dtor
}

void Spaceship::update()
{
    // implementação do método update da Spaceship
}

int Spaceship::getObjectAsMessage()
{
    //TODO: Atualizar para passar angulo ao invés de x e y
    int messageObj = id << SHIFT_ID | type << SHIFT_TYPE | angle;
    return messageObj;
}

int Spaceship::setAngle(int newAngle) {
    angle = newAngle;
}

Bullet* Spaceship::shoot(int new_bullet_id) {
    float bullet_x = X_MAX/2;
    float bullet_y = Y_MAX/2;

    int realAngle = (360 + ((int)(angle) - 90))%360;

    float bullet_velocity_x = BULLET_VELOCITY*cos(realAngle * PI / 180.0);
    float bullet_velocity_y = BULLET_VELOCITY*sin(realAngle * PI / 180.0);

    return new Bullet(bullet_x, bullet_y, bullet_velocity_x, bullet_velocity_y, new_bullet_id);
}
