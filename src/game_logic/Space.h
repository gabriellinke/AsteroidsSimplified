// MyClass.h
#ifndef SPACE_H
#define SPACE_H
#include <vector>

class Space {
private:
    int nextId;
    int gameOver;
    int score;
    std::vector<int> objects;
    std::vector<int> inputs;

    void create();
public:
    // Constructor
    Space();

    std::vector<int> update();
    void setInputs(int input);
    int getGameOver();
    int getScore();
};

#endif // MYCLASS_H
