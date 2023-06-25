#include "Object.h"

Object::Object(int x, int y, int tam_x, int tam_y, int id, int type)
    : pos_x(x), pos_y(y), width(tam_x), height(tam_y), id(id), type(type), isAlive(true)
{
}

Object::~Object()
{
    // Destrutor vazio
}

#include "Object.h"

// Getters
int Object::getPosX() const {
    return pos_x;
}

int Object::getPosY() const {
    return pos_y;
}

int Object::getWidth() const {
    return width;
}

int Object::getHeight() const {
    return height;
}

int Object::getId() const {
    return id;
}

int Object::getType() const {
    return type;
}

bool Object::getIsAlive() const {
    return isAlive;
}

// Setters
void Object::setPosX(int x) {
    pos_x = x;
}

void Object::setPosY(int y) {
    pos_y = y;
}

void Object::setWidth(int w) {
    width = w;
}

void Object::setHeight(int h) {
    height = h;
}

void Object::setId(int id) {
    id = id;
}

void Object::setType(int type) {
    type = type;
}

void Object::setIsAlive(bool alive) {
    isAlive = alive;
}
