#include "shape.h"

/////////////////////////////////////////////////////////////

Shape::~Shape()
{
}

/////////////////////////////////////////////////////////////

Rect::Rect (int _x, int _y)
{
    x = _x;
    y = _y;
    w = 50 + rand() % 50;
    h = 50 + rand() % 50;
    r = rand() % 256;
    g = rand() % 256;
    b = rand() % 256;
}

void Rect::Draw(HDC hdc)
{
    HBRUSH hBrush = CreateSolidBrush (RGB (r, g, b)) ;
    SelectObject (hdc, hBrush) ;
    Rectangle (hdc, x-w, y-h, x+w, y+h) ;
    DeleteObject (hBrush) ;
}

/////////////////////////////////////////////////////////////

Square::Square (int _x, int _y)
{
    x = _x;
    y = _y;
    w = 50 + rand() % 50;
    r = rand() % 256;
    g = rand() % 256;
    b = rand() % 256;
}

void Square::Draw(HDC hdc)
{
    HBRUSH hBrush = CreateSolidBrush (RGB (r, g, b)) ;
    SelectObject (hdc, hBrush) ;
    Rectangle (hdc, x-w, y-w, x+w, y+w) ;
    DeleteObject (hBrush) ;
}

/////////////////////////////////////////////////////////////

Circle::Circle (int _x, int _y)
{
    x = _x;
    y = _y;
    w = 50 + rand() % 50;
    r = rand() % 256;
    g = rand() % 256;
    b = rand() % 256;
}

void Circle::Draw(HDC hdc)
{
    HBRUSH hBrush = CreateSolidBrush (RGB (r, g, b)) ;
    SelectObject (hdc, hBrush) ;
    Ellipse (hdc, x-w, y-w, x+w, y+w) ;
    DeleteObject (hBrush) ;
}

/////////////////////////////////////////////////////////////

ShapeGame::ShapeGame()
{
    numShapes = 0;
}

void ShapeGame::AddDrawShape(HDC hdc, int x, int y)
{
    int type = rand()%3;
    if(type==0) shapes[numShapes] = new Rect(x, y);
    else if(type==1) shapes[numShapes] = new Square(x, y);
    else if(type==2) shapes[numShapes] = new Circle(x, y);
    shapes[numShapes]->Draw(hdc);
    numShapes++;
}

void ShapeGame::DrawShapes(HDC hdc)
{
    int i;
    for(i=0;i<numShapes;i++) shapes[i]->Draw(hdc);
}

void ShapeGame::DestroyShapes()
{
    int i;
    for(i=0;i<numShapes;i++) delete shapes[i];
    numShapes = 0;
}

ShapeGame::~ShapeGame()
{
    DestroyShapes();
}

/////////////////////////////////////////////////////////////
