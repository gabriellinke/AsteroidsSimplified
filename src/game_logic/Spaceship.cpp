#include "Spaceship.h"

Spaceship::Spaceship(int x, int y, int id) : Object(x, y, SPACESHIP_SIZE, SPACESHIP_SIZE, id, SPACESHIP)
{
    aim_x = 0;
    aim_y = 0;
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
    int messageObj = id << SHIFT_ID | type << SHIFT_TYPE | pos_x << SHIFT_COORDS_X | pos_y << SHIFT_COORDS_Y;
    return messageObj;
}
