#include "Space.h"
#include <algorithm>
#include <constants.h>
#include <ctime>
#include <cstdlib>

Space::Space()
{
    asteroidCount = 4;
    score = 0;
    gameOver = 0;

    // Criando a spaceship
    int center_x = X_MAX/2 - SPACESHIP_SIZE/2;
    int center_y = Y_MAX/2 - SPACESHIP_SIZE/2;
    Spaceship* spaceship = new Spaceship(center_x, center_y, 1);
    objects.push_back(spaceship);

    // Criando os asteroids
    objects.push_back(new Asteroids(10, 10, 2, true));
    objects.push_back(new Asteroids(20, 20, 3, true));
    objects.push_back(new Asteroids(30, 30, 4, true));
    objects.push_back(new Asteroids(40, 40, 5, true));

}

Space::~Space()
{
    for (Object* object : objects) {
        delete object;
    }
}
void Space::handleCollisions() {
    for (auto it1 = objects.begin(); it1 != objects.end(); ++it1) {
        Object* obj1 = *it1;

        for (auto it2 = std::next(it1); it2 != objects.end(); ++it2) {
            Object* obj2 = *it2;

            // Check collision between obj1 and obj2
            if (checkCollision(obj1, obj2)) {
                if (obj1->getId() == 1 && obj2->getId() == 3 ||
                    obj1->getId() == 1 && obj2->getId() == 4 ||
                    obj1->getId() == 4 && obj2->getId() == 1 ||
                    obj1->getId() == 3 && obj2->getId() == 1)
                {
                    gameOver = true;
                }
                else if(obj1->getId() == 2 && obj2->getId() == 3 ||
                    obj1->getId() == 2 && obj2->getId() == 4 ||
                    obj1->getId() == 4 && obj2->getId() == 2 ||
                    obj1->getId() == 3 && obj2->getId() == 2)
                {
                    if(obj1->getId()== 3){
                        objects.push_back(new Asteroids(obj1->getPosX()+40, obj1->getPosY()+40, 2, false));
                        objects.push_back(new Asteroids(obj1->getPosX()-40, obj1->getPosY()-40, 2, false));
                    }
                    else if (obj2->getId()== 3){
                        objects.push_back(new Asteroids(obj2->getPosX()+40, obj1->getPosY()+40, 2, false));
                        objects.push_back(new Asteroids(obj2->getPosX()-40, obj2->getPosY()-40, 2, false));
                    }
                    if(obj1->getIsAlive() == true)
                        score++;
                    obj1->setIsAlive(false);
                    obj2->setIsAlive(false);
                }
            }
        }
    }
}

bool Space::checkCollision(Object* obj1, Object* obj2) {
    int obj1_left = obj1->getPosX();
    int obj1_right = obj1->getPosX() + obj1->getWidth();
    int obj1_top = obj1->getPosY();
    int obj1_bottom = obj1->getPosY() + obj1->getHeight();

    int obj2_left = obj2->getPosX();
    int obj2_right = obj2->getPosX() + obj2->getWidth();
    int obj2_top = obj2->getPosY();
    int obj2_bottom = obj2->getPosY() + obj2->getHeight();

    if (obj1_right >= obj2_left && obj1_left <= obj2_right &&
        obj1_bottom >= obj2_top && obj1_top <= obj2_bottom) {
        return true; // Collision detected
    }

    return false; // No collision
}


std::vector<int> Space::update() {
    // Update all objects in the space
/*
    for (auto it = objects.begin(); it != objects.end();) {
        Object* object = *it;
        object->update();

        // Check if the object is no longer alive
        if (!object->getIsAlive()) {
            // Remove the object from the space
            if (object->getId()==4)
                asteroidCount--;
            removeObject(object);
            it = objects.erase(it);
        } else {
            ++it;
        }
    }
*/
    std::vector<int> objectsToReturn;
    for (auto it = objects.begin(); it != objects.end(); it++) {
        Object* object = *it;
        int pos_x = object->getPosX();
        int pos_y = object->getPosY();
        int id = object->getId();
        int type = object->getType();

        // TODO: não vai levar em conta a nave, que tem que mandar o ângulo
        int messageObj = id << SHIFT_ID | type << SHIFT_TYPE | pos_x << SHIFT_COORDS_X | pos_y << SHIFT_COORDS_Y;
        objectsToReturn.push_back(messageObj);
    }

    return objectsToReturn;
}

void Space::removeObject(Object* object) {
    // Find the object in the vector
    auto it = std::find(objects.begin(), objects.end(), object);
    if (it != objects.end()) {
        // Remove the object from the vector
        objects.erase(it);
        // Free the memory allocated for the object
        delete object;
    }
}

void Space::generateAsteroids(int n)
{
    while (n > 0){
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        int posx = std::rand() % 320;

        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        int posy = std::rand() % 240;

        objects.push_back(new Asteroids(posx, posy, 2, true));
        n--;
    }
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
