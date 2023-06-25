// MyClass.cpp
#include "Space.h"
#include <stdio.h>

// Constructor definition
Space::Space() {
    nextId = 1;
    gameOver = 0;
    score = 0;

    create();
}

// Cria os primeiros objetos
void Space::create() {
    // TODO
}

std::vector<int> Space::update() {
    // Tira todos os inputs e:
        // Se tem comando de atirar, chama função Spaceship.shot() que vai retornar uma Bullet com x,y setados. Adiciona a bullet no vetor de objetos
        // Se tem comando de virar nave, usa função Spaceship.rotate(angle)

    // Atualiza posição dos objetos

    // Verifica colisões asteroides/projetil e asteroides/nave

    // Atualiza pontuação

    // Verifica se o jogo acabou ou não

    // Retorna objetos no formato que é utilizado na fila gráfica (id/tipo/x/y ou id/tipo/angulo)
    score++;
    if(score > 100)
        gameOver = 1;

    return objects;
}

void Space::setInputs(int input) {
    inputs.push_back(input);
}

int Space::getGameOver() {
    return gameOver;
}

int Space::getScore() {
    return score;
}
