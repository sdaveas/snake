#include "teren.h"

// Constructor
Board::Board()
{
    // create teren
    for( int x = 0 ; x < X_DIMENTION + 2 ; x++){
        for( int y= 0 ; y < Y_DIMENTION + 2 ; y++){
            if ( (x == 0 || x == X_DIMENTION + 1) || (y == 0 || y == Y_DIMENTION + 1) )
                m_teren[x][y] = BOARDER;
            else
                m_teren[x][y] = EMPTY;
        }
    }
    // create snake and put in the teren
    Point startingPoint( X_DIMENTION/2 , Y_DIMENTION/2 );
    m_snake.push_front(startingPoint);
    // put snake inside board
    m_teren[startingPoint.x][startingPoint.y] = HEAD;

    // put food in an empty box inside the teren
    food_create();

}

Board::~Board()
{}

int Board::snake_bite(int direction)
{
    Point oldHead = m_snake.front();
    Point newHead(oldHead);
    Point oldTail = m_snake.back();

    // declare new position of the head.
    switch(direction){
        case UP : newHead.y++; break;
        case DOWN : newHead.y--; break;
        case LEFT : newHead.x--; break;
        case RIGHT : newHead.x++; break;
    }
    // new position is a wall.
    if (m_teren[newHead.x][newHead.y] == BOARDER){
        return BOARDER;
    }
    // new position is body.
    if (m_teren[newHead.x][newHead.y] == BODY && (newHead != oldTail || m_snake.size() == 2)){
        return BODY;
    }
    // new position is food.
    if (m_teren[newHead.x][newHead.y] == FOOD){
        if (m_snake.size() == X_DIMENTION * Y_DIMENTION - 1)
            return COMPLETE;
        m_teren[oldHead.x][oldHead.y] = BODY;
        m_teren[newHead.x][newHead.y] = HEAD;
        m_snake.push_front(newHead);
        food_create();
        return FOOD;
    }
    // new position is empty area.
    if (m_teren[newHead.x][newHead.y] == EMPTY || (newHead == oldTail && m_snake.size() != 2)){
        m_teren[oldHead.x][oldHead.y] = BODY;
        m_teren[newHead.x][newHead.y] = HEAD;
        m_snake.push_front(newHead);
        m_teren[oldTail.x][oldTail.y] = EMPTY;
        m_snake.pop_back();
        return EMPTY;
    }
}

Point Board::food_create()
{
    do{
        m_food.x = rand()%X_DIMENTION + 1;
        m_food.y = rand()%Y_DIMENTION + 1;
    }while( m_teren[m_food.x][m_food.y] != EMPTY );

    m_teren[m_food.x][m_food.y] = FOOD;
}

void Board::print_teren()
{
    //clear screen
    std::cout << "\x1B[2J\x1B[H";

    for (int x = 0  ; x < X_DIMENTION + 2 ; x++){
        for (int y = 0  ; y < Y_DIMENTION + 2 ; y++){
            switch(m_teren[x][y]){
            case BOARDER: std::cout << "x"; break;
            case HEAD: std::cout << "o"; break;
            case BODY: std::cout << "*"; break;
            case FOOD: std::cout << "O"; break;
            case EMPTY: std::cout << " "; break;
            }
        }
        std::cout << std::endl;
    }
}

int Board::play(int direction)
{
    int move = snake_bite(direction);
    print_teren();
    return move;
}



























