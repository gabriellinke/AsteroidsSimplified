#ifndef OBJECT_H
#define OBJECT_H

class Object
{
protected:
    int pos_x;
    int pos_y;
    int width;
    int height;
    int id;
    int type;
    bool isAlive;

public:
    Object(int x, int y, int size_x, int size_y, int id, int type);
    virtual ~Object();
    virtual void update() = 0;

    // Getters
    int getPosX() const;
    int getPosY() const;
    int getWidth() const;
    int getHeight() const;
    int getId() const;
    int getType() const;
    bool getIsAlive() const;

    // Setters
    void setPosX(int x);
    void setPosY(int y);
    void setWidth(int w);
    void setHeight(int h);
    void setId(int id);
    void setType(int type);
    void setIsAlive(bool alive);
};

#endif // OBJECT_H
