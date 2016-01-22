#ifndef _TEREN_H_
#define _TEREN_H_

#include <list>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <iomanip>

#define X_DIMENTION 8
#define Y_DIMENTION 8

struct Point {

    int x;
    int y;

    Point(){
        x = 0;
        y = 0;
    }

    Point(int x, int y){
        this->x = x;
        this->y = y;
    }

    Point(const Point &other)
        :x(other.x), y(other.y)
    {}

    Point operator=(const Point &other){
        if (this != &other){
            x = other.x;
            y = other.y;
        }
        return *this;
    }

    bool operator==(const Point &other){
        return (x == other.x && y == other.y);
    }

    bool operator!=(const Point &other){
        return (x != other.x || y != other.y);
    }

};

class Board {

typedef std::list<Point> Snake;
typedef Point Food;

private:
     char m_teren[X_DIMENTION+2][Y_DIMENTION+2];
     Snake m_snake;
     Food m_food;

public:
     enum {
         EMPTY=0, HEAD=1, BODY=2, FOOD=3, BOARDER=4, COMPLETE=5,
     } tableValues;

     enum {
         UP=0, DOWN=1, LEFT=2, RIGHT=3,
     }directionValues;

     // Constructor
     Board();
     // Destructor
     ~Board();
     
     ////Snake functions
     // snake bites a new point
     int snake_bite(int direction);
     
     ////Food functions
     // put a food point inside the teren
     void food_create();
     
     //// Generic fuctions
     bool ending_move(int move);
     void print_teren();
     void intro();
     int play(int direction);
};

#endif // _TEREN_H_
