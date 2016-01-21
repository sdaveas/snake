#include <iostream>
#include <stdio.h>
#include <sys/ioctl.h>
#include "teren.h"
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <iomanip>

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


int read_move( Board &board, char move ){
    switch(move){
    case 'd': return Board::UP;
    case 'a': return Board::DOWN;
    case 'w': return Board::LEFT;
    case 's': return Board::RIGHT;
    }
}

int sleep_from_to_for( int start_time, int end_time, int time_quantom){
    for( ; start_time < end_time ; start_time++){
        if (kbhit())
            break;
        usleep(time_quantom);
    }
    return start_time;
}

bool ending_move(int move){
    if (move==Board::BODY || move==Board::BOARDER || move==Board::COMPLETE)
        return true;
    return false;
}

void intro(){
    std::cout << "\x1B[2J\x1B[H";
    std::cout << std::setw(X_DIMENTION/2+1) << "READY?" << std::endl;
    for (int i = 3 ; i > 0 ; i--){
        std::cout << std::setw(X_DIMENTION/2+1) << i << "\r";
        std::cout << std::flush;
        sleep(1);
    }
    std::cout << std::endl << std::setw(X_DIMENTION/2+1) << "GO!!" << std::endl;
//    std::cout << std::flush;
    sleep(1);
}

int play(){
    Board board;
    int move_outcome;
    char move = Board::UP;
    int curr_sleeping_time = 0;
    int remaining_sleeping_time = 0;
    int max_sleeping_time = 100;
    int sleeping_quantom = 10000;

    intro();

    while(1){
        do{
            move_outcome = board.play(read_move(board, move));
            remaining_sleeping_time = sleep_from_to_for(curr_sleeping_time, max_sleeping_time, sleeping_quantom);
            sleep_from_to_for(remaining_sleeping_time, max_sleeping_time, sleeping_quantom);
            sleeping_quantom = sleeping_quantom - sleeping_quantom*0.001;
        }while(!kbhit() && !ending_move(move_outcome));
        if (ending_move(move_outcome)) break;
        std::cin >> move;
    }
    printf("outcome:%d\n", move_outcome);
    return move_outcome;
}

int main( int argc, char* argv[]){

    srand(time(0));

    int result;
    char answer;

    do{
        result = play();
        std::cout << "wanna play again? [y/n]" << std::endl;
        answer = getch();
    }while(answer != 'n' );

    printf("Ok! Bye Bye!\n");

    return 0;
}
