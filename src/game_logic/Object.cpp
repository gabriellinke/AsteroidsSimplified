#include "Object.h"

Object::Object(float x, float y, int tam_x, int tam_y, int id, int type)
    : pos_x(x), pos_y(y), width(tam_x), height(tam_y), id(id), type(type), isAlive(true)
{
}

Object::~Object()
{
    // Destrutor vazio
}

#include "Object.h"

// Getters
float Object::getPosX() const {
    return pos_x;
}

float Object::getPosY() const {
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
void Object::setPosX(float x) {
    pos_x = x;
}

void Object::setPosY(float y) {
    pos_y = y;
}

void Object::setWidth(int w) {
    width = w;
}

void Object::setHeight(int h) {
    height = h;
}

void Object::setId(int id) {
    this->id = id;
}

void Object::setType(int type) {
    this->type = type;
}

void Object::setIsAlive(bool alive) {
    isAlive = alive;
}
