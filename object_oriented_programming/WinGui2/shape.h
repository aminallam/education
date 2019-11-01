#ifndef SHAPE_H
#define SHAPE_H

#include <windows.h>

class Shape
{
public:
    virtual void Draw(HDC hdc)=0; // do not change
    virtual bool IsInside(int x, int y)=0; // return true if point (x,y) is inside the shape
    virtual void MoveTo(int x, int y)=0; // move the center of the shape to point (x,y)
    virtual double Area()=0; // return the area of the shape
    virtual ~Shape();
};

class Rect : public Shape
{
protected:
    int x, y; // (x,y)=center of the rectangle
    int w, h; // w=half width, h=half height
    int r, g, b; // red, green, blue
public:
    Rect (int _x, int _y); // do not change
    void Draw(HDC hdc); // do not change
    bool IsInside(int x, int y); // [0.5 mark]
    void MoveTo(int x, int y);
    double Area(); // [0.5 mark]
};

class Square : public Shape
{
protected:
    int x, y; // (x,y)=center of the square
    int w; // w=half width=half height
    int r, g, b;
public:
    Square (int _x, int _y); // do not change
    void Draw(HDC hdc); // do not change
    bool IsInside(int x, int y); // [0.5 mark]
    void MoveTo(int x, int y);
    double Area(); // [0.5 mark]
};

class Circle : public Shape
{
protected:
    int x, y; // (x,y)=center of the circle
    int w; // w=radius
    int r, g, b;
public:
    Circle (int _x, int _y); // do not change
    void Draw(HDC hdc); // do not change
    bool IsInside(int x, int y); // [0.5 mark]
    void MoveTo(int x, int y);
    double Area(); // [0.5 mark]
};

class ShapeGame
{
protected:
    Shape* shapes[1000];
    int numShapes;
    int stx; int sty;
public:
    ShapeGame();
    void AddDrawShape(HDC hdc, int x, int y); // do not change
    void DrawShapes(HDC hdc); // do not change
    void DestroyShapes(); // do not change
    void StartMove(int x, int y); // set stx=x, sty=y
    void EndMove(int x, int y); // [2 marks] Move the center of the last shape containing point (stx,sty) to point (x,y)
                                //   and make this shape the last one (on top of all shapes)
    double GetArea(); // Get the area of the last shape
    ~ShapeGame();
};

#endif // SHAPE_H
