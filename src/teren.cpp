#include "teren.h"

// Constructor
Board::Board(int dimension_x,int dimension_y, int mode)
{
    m_dimension_x=dimension_x;
    m_dimension_y=dimension_y;
    setMode(mode);
    // create teren
    m_teren = new char*[m_dimension_y+2];
    for (int i=0; i<m_dimension_y+2; i++){
        m_teren[i]=new char[m_dimension_x+2];
    }

    // fill teren
    for( int x = 0 ; x < m_dimension_x + 2 ; x++){
        for( int y= 0 ; y < m_dimension_y + 2 ; y++){
            if ( (x == 0 || x == m_dimension_x + 1) || (y == 0 || y == m_dimension_y + 1) )
                m_teren[x][y] = BOARDER;
            else
                m_teren[x][y] = EMPTY;
        }
    }

    // create snake and put in the teren
    Point startingPoint( m_dimension_x/2 , m_dimension_y/2 );
    m_snake.push_front(startingPoint);
    // put snake inside board
    m_teren[startingPoint.x][startingPoint.y] = HEAD;

    // put food in an empty box inside the teren
    food_create();

}

Board::~Board()
{
    // create teren
    for (int i=0; i<m_dimension_y+2; i++){
        delete[] m_teren[i];
    }
    delete[] m_teren;
}

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
        if (m_mode==EASY){
            // x-axis boarder
            if (newHead.x == m_dimension_x+1){
                newHead.x-=m_dimension_x;
            }
            else if(newHead.x==0){
                newHead.x+=m_dimension_x;
            }
            // y-axis boarder
            if (newHead.y == m_dimension_y+1){
                newHead.y-=m_dimension_y;
            }
            else if(newHead.y==0){
                newHead.y+=m_dimension_y;
            }
        }
        else if (m_mode==HARD){
            return BOARDER;
        }
    }
    // new position is body.
    if (m_teren[newHead.x][newHead.y] == BODY && (newHead != oldTail || m_snake.size() == 2)){
        return BODY;
    }
    // new position is food.
    if (m_teren[newHead.x][newHead.y] == FOOD){
        if (m_snake.size() ==(m_dimension_x * m_dimension_y) - 1)
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
    return -1;
}

void Board::food_create()
{
    do{
        m_food.x = rand()%m_dimension_x + 1;
        m_food.y = rand()%m_dimension_y + 1;
    }while( m_teren[m_food.x][m_food.y] != EMPTY );

    m_teren[m_food.x][m_food.y] = FOOD;
}

bool Board::ending_move(int move){
    if (move==Board::BODY || move==Board::BOARDER || move==Board::COMPLETE)
        return true;
    return false;
}

void Board::print_teren()
{
    //clear screen
    std::cout << "\x1B[2J\x1B[H";

    for (int x = 0  ; x < m_dimension_x + 2 ; x++){
        for (int y = 0  ; y < m_dimension_y + 2 ; y++){
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

void Board::intro(){
    std::cout << "\x1B[2J\x1B[H";
    std::cout << std::setw(m_dimension_x/2+1) << "READY?" << std::endl;
    for (int i = 3 ; i > 0 ; i--){
        std::cout << std::setw(m_dimension_x/2+1) << i << "\r";
        std::cout << std::flush;
        sleep(1);
    }
    std::cout << std::endl << std::setw(m_dimension_x/2+1) << "GO!!" << std::endl;
    sleep(1);
}

int Board::play(int direction)
{
    int move = snake_bite(direction);
    print_teren();
    return move;
}

void Board::setMode(int mode)
{
    if (mode == EASY)
        m_mode=EASY;
    else
        m_mode=HARD;
}



























