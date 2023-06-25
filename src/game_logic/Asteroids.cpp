#include "Asteroids.h"

Asteroids::Asteroids(int x, int y, int id, bool big) : Object(x, y, SMALL_ASTEROID_SIZE, SMALL_ASTEROID_SIZE, id, SMALL_ASTEROID)
{
    vel_x = 0;
    vel_y = 0;
    // Defina o tamanho do asteroide com base no parâmetro "big"
    if (big) {
        width = BIG_ASTEROID_SIZE;
        height = BIG_ASTEROID_SIZE;
        type = BIG_ASTEROID;
    }
}

Asteroids::~Asteroids()
{
    // Implementação do destrutor
}

void Asteroids::update()
{
    // Atualize a posição e outras propriedades do asteroide
    // Implementação vai aqui
}

int Asteroids::getObjectAsMessage()
{
    int messageObj = id << SHIFT_ID | type << SHIFT_TYPE | pos_x << SHIFT_COORDS_X | pos_y << SHIFT_COORDS_Y;
    return messageObj;
}
