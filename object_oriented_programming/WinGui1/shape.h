#ifndef SHAPE_H
#define SHAPE_H

#include <windows.h>

class Shape
{
public:
    virtual void Draw(HDC hdc)=0;
    virtual ~Shape();
};

class Rect : public Shape
{
protected:
    int x, y;
    int w, h;
    int r, g, b;
public:
    Rect (int _x, int _y);
    void Draw(HDC hdc);
};

class Square : public Shape
{
protected:
    int x, y;
    int w;
    int r, g, b;
public:
    Square (int _x, int _y);
    void Draw(HDC hdc);
};

class Circle : public Shape
{
protected:
    int x, y;
    int w;
    int r, g, b;
public:
    Circle (int _x, int _y);
    void Draw(HDC hdc);
};

class ShapeGame
{
protected:
    Shape* shapes[1000];
    int numShapes;
public:
    ShapeGame();
    void AddDrawShape(HDC hdc, int x, int y);
    void DrawShapes(HDC hdc);
    void DestroyShapes();
    ~ShapeGame();
};

#endif // SHAPE_H
