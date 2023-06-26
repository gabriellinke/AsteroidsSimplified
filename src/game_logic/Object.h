#ifndef OBJECT_H
#define OBJECT_H

class Object
{
protected:
    float pos_x;
    float pos_y;
    int width;
    int height;
    int id;
    int type;
    bool isAlive;

public:
    Object(float  x, float y, int size_x, int size_y, int id, int type);
    virtual ~Object();
    virtual void update() = 0;
    virtual int getObjectAsMessage() = 0;

    // Getters
    float getPosX() const;
    float getPosY() const;
    int getWidth() const;
    int getHeight() const;
    int getId() const;
    int getType() const;
    bool getIsAlive() const;

    // Setters
    void setPosX(float x);
    void setPosY(float y);
    void setWidth(int w);
    void setHeight(int h);
    void setId(int id);
    void setType(int type);
    void setIsAlive(bool alive);
};

#endif // OBJECT_H
