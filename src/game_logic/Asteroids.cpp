#include "Asteroids.h"

Asteroids::Asteroids(int x, int y, int id, bool big) : Object(x, y, 32, 32, id, 2)
{
    vel_x = 0;
    vel_y = 0;
    // Defina o tamanho do asteroide com base no parâmetro "big"
    if (big) {
        width = 64;
        height = 64;
        type = 2;
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
