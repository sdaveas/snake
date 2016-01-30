#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include "teren.h"

// get a character without expecting enter after.
char getch() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}

// this function is called only if a key is hitten.
int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}

// translates the keyboard input to snake movement.
int read_move( char move ){
    switch(move){
    case 'd': return Board::UP;
    case 'a': return Board::DOWN;
    case 'w': return Board::LEFT;
    case 's': return Board::RIGHT;
    case 'A': return Board::LEFT;
    case 'B': return Board::RIGHT;
    case 'C': return Board::UP;
    case 'D': return Board::DOWN;
    }
    return -1;
}

// sleeps for a certain time. If sleep was not complete, returns the remaining sleep time.
int sleep_from_to_for( int start_time, int end_time, int time_quantom){
    for( ; start_time < end_time ; start_time++){
        if (kbhit())
            break;
        usleep(time_quantom);
    }
    return start_time;
}

// plays a round
int play(int x_dimension, int y_dimension){

    Board board(x_dimension, y_dimension);
    int move_outcome;
    char move = Board::UP;
    int curr_sleeping_time = 0;
    int remaining_sleeping_time = 0;
    int max_sleeping_time = 10;
    int sleeping_quantom = 100000;

    board.intro();

    while(1){
        do{
            move_outcome = board.play(read_move(move));
            remaining_sleeping_time = sleep_from_to_for(curr_sleeping_time, max_sleeping_time, sleeping_quantom);
            sleep_from_to_for(remaining_sleeping_time, max_sleeping_time, sleeping_quantom);
            sleeping_quantom = sleeping_quantom - sleeping_quantom*0.001;
        }while(!kbhit() && !board.ending_move(move_outcome));
        if (board.ending_move(move_outcome)) break;
        std::cin >> move;
    }
    printf("outcome:%d\n", move_outcome);
    return move_outcome;
}

int main( int argc, char* argv[]){

    int dimension_x=8;
    int dimension_y=8;

    if (argc == 3){
        dimension_x=atoi(argv[1]);
        dimension_y=atoi(argv[2]);
    }
    else if(argc == 2){
        dimension_x=atoi(argv[1]);
        dimension_y=dimension_x;
    }

    srand(time(0));

    int result;
    char answer;

    do{
        result = play(dimension_x, dimension_y);
        std::cout << "wanna play again? [y/n]" << std::endl;
        answer = getch();
    }while(answer != 'n' );

    printf("Ok! Bye Bye!\n");

    return 0;
}
