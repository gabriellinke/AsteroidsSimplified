#ifndef SPACE_H
#define SPACE_H

#include "Asteroids.h"
#include "Bullet.h"
#include "Spaceship.h"

#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

class Space
{
public:
    Space();
    ~Space();

    std::vector<int> update();
    void setInputs(std::vector<int> inputs);

    int getGameOver();
    int getScore();

private:
    int score;
    int asteroidCount;
    bool gameOver;
    Spaceship* spaceship;

    std::vector<Object*> objects;
    std::vector<int> inputs;

    void handleCollisions();
    bool checkCollision(Object* obj1, Object* obj2);
    void removeObject(Object* object);
    void generateAsteroids(int n);
};

#endif // SPACE_H
