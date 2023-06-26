#include "Asteroids.h"
#include <ctime>
#include <cstdlib>
#include <math.h>

Asteroids::Asteroids(float x, float y, int id, bool big) : Object(x, y, SMALL_ASTEROID_SIZE, SMALL_ASTEROID_SIZE, id, SMALL_ASTEROID)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int aux = std::rand() % 2;
    if(aux == 1)
        velocity_x = -1;
    else
        velocity_x =1;

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    aux = std::rand() % 2;
    if(aux == 1)
        velocity_y = -1;
    else
        velocity_y = 1;

    // Defina o tamanho do asteroide com base no parâmetro "big"
    if (big) {
        width = BIG_ASTEROID_SIZE;
        height = BIG_ASTEROID_SIZE;
        type = BIG_ASTEROID;
        velocity_x *= BIG_ASTEROID_VELOCITY/sqrt(2);
        velocity_y *= BIG_ASTEROID_VELOCITY/sqrt(2);
    } else {
        velocity_x *= SMALL_ASTEROID_VELOCITY/sqrt(2);
        velocity_y *= SMALL_ASTEROID_VELOCITY/sqrt(2);
    }

}

Asteroids::~Asteroids()
{
    // Implementação do destrutor
}

void Asteroids::update()
{
    pos_x = pos_x + velocity_x;
    pos_y = pos_y + velocity_y;

    if(pos_x < 0)
        pos_x = X_MAX;
    else if(pos_x > X_MAX)
        pos_x =0;

    if(pos_y < 0)
        pos_y = Y_MAX;
    else if(pos_y > Y_MAX)
        pos_y =0;
}

int Asteroids::getObjectAsMessage()
{
    int messageObj = id << SHIFT_ID | type << SHIFT_TYPE | (int)pos_x << SHIFT_COORDS_X | (int)pos_y << SHIFT_COORDS_Y;
    return messageObj;
}
