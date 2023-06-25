#include "Spaceship.h"

Spaceship::Spaceship(int x, int y, int id) : Object(x, y, 32, 32, id, 1)
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
