#include <iostream>
#include <list>
#include <ncurses.h>

using namespace std;


//allows printing of the fill carachter from (x,y) to (x,y+hight-1)
void print_col(int x, int y, char fill, int hight){
    for(int i = 0; i < hight; ++i){
        mvaddch(y+i, x, fill);//moving to y+i line, x column and add fill
    }
}

//allows printing of the fill carachter from (x,y) to (x+hight-1,y)
void print_row(int x, int y, char fill, int hight){
    for(int i = 0; i < hight; ++i){
        mvaddch(y, x+i, fill);//moving to y+i line, x column and add fill
    }
}



//print an X at x,y
void print_x(int x, int y){
    mvprintw(y+0, x+1, "\\ \\ / /");
    mvprintw(y+1, x+1, " \\ V / ");
    mvprintw(y+2, x+1, "  > <");
    mvprintw(y+3, x+1, " / . \\ ");
    mvprintw(y+4, x+1, "/_/ \\_\\");
}

//print an O at x,y
void print_o(int x, int y){
    mvprintw(y+0, x+1, "  ____  ");
    mvprintw(y+1, x+1, " / __ \\ ");
    mvprintw(y+2, x+1, "| |  | |");
    mvprintw(y+3, x+1, "| |__| |");
    mvprintw(y+4, x+1, " \\____/ ");
}

//draw the whole board
void draw(int board[3][3], int curs_x, int curs_y){
    int xpos[3] = {0, 11, 22};
    int ypos[3] = {0, 6, 12};
    print_col(10, 0, '*', 17);
    print_col(21, 0, '*', 17);
    print_row(0, 5, '*', 32);
    print_row(0, 11, '*', 32);
    for(int x = 0; x < 3; ++x){
        for(int y = 0; y < 3; ++y){
            if(board[x][y] == 1){
                print_x(xpos[x], ypos[y]);
            }else if(board[x][y] == -1){
                print_o(xpos[x], ypos[y]);
            }
            mvprintw(ypos[y],xpos[x], " ");
        }
    }
    mvprintw(ypos[curs_y], xpos[curs_x], "$");
    move(17, 0);
    mvprintw(18, 0, "You are the Os.");
    mvprintw(19, 0, "The \'$\' is the current positon.");
    mvprintw(20, 0, "Use the arrow keys to navigate.");
    mvprintw(21, 0, "Hit enter to place.");
    mvprintw(22, 0, "Reset anytime with space.");
    mvprintw(23, 0, "Leave with Esc.");
    mvprintw(24, 0, "Try to tie.");
    refresh();

}

//check to see if an imediate solution is available
int solutions(int board[3][3], int check_for, int& out_x, int& out_y){
  int sum;
  int count = 0;

  //go through each row
  for(int x = 0; x < 3; ++x){
    sum = 0;
    //add up all the peices
    //if can only be 2 or -2 if it is about to be solved
    //that is two 1s or two -1s and a zero
    for(int y = 0; y < 3; ++y){
      sum += board[x][y];
    }
    if(sum == check_for){
      for(int y = 0; y < 3; ++y){
        if(board[x][y] == 0){
          out_x = x;
          out_y = y;
          ++count;
        }
      }
    }
  }
  //go through each column and do the same
  for(int y = 0; y < 3; ++y){
    sum = 0;
    for(int x = 0; x < 3; ++x){
      sum += board[x][y];
    }
    if(sum == check_for){
      for(int x = 0; x < 3; ++x){
        if(board[x][y] == 0){
          out_x = x;
          out_y = y;
          ++count;
        }
      }
    }
  }
  //go along one diagonal
  sum = 0;
  for(int i = 0; i < 3; ++i){
    sum += board[i][i];
  }
  if(sum == check_for){
    for(int i = 0; i < 3; ++i){
      if(board[i][i] == 0){
        out_x = i;
        out_y = i;
        ++count;
      }
    }
  }
  //check the other diagonal
  sum = 0;
  for(int i = 0; i < 3; ++i){
    sum += board[i][2-i];
  }
  if(sum == check_for){
    for(int i = 0; i < 3; ++i){
      if(board[i][2-i] == 0){
        out_x = i;
        out_y = 2-i;
        ++count;
      }
    }
  }

  //return how many solutions are available
  //out_x and out_y will be set to the most recently found solution.
  return count;
}

//boolean function, check to see if there is a forcing move for the computer
//b is the board, p is the player trying to force
//t is the player who's turn it is
bool force(int b[3][3], int p, int t, int& p_x, int& p_y){
  int x;
  int y;
  //if we can solve the board right now, and it's our turn then just return
  if(solutions(b, 2*p, x, y) > 0 && t == p){
    return true;
  //block the next player if they are about to win
  }else if(solutions(b, -2*p, x, y) > 0 && t == p){
    b[x][y] = t;
    //after playing the block, recurse down, switching turns
    //if that force works, this x and y attempt works as a force
    //set p_x and p_y to be the x and y tried here
    if(force(b, p, -1*t, p_x, p_y)){
      b[x][y] = 0;
      p_x = x;
      p_y = y;
      return true;
    }
    b[x][y] = 0;

  //if the other play has their turn now, and they can win, return false
  }else if(solutions(b, -2*p, x, y) > 0 && t == -1*p){
    return false;
  //if it's their turn but we have a solution,
  }else if(solutions(b, 2*p, x, y) > 0 && t == -1*p){
    b[x][y] = t;
    if(force(b, p, -1*t, p_x, p_y)){
      b[x][y] = 0;
      return true;
    }
    b[x][y] = 0;
  //otherwise, if it's this players turn, loop through all positions
  //try placing a peice there and see if we are forcing.
  }else if(p == t){
    for(x = 0; x < 3; ++x){
      for(y = 0; y < 3; ++y){
        if(b[x][y] == 0){
          b[x][y] = t;
          if(force(b, p, -1*t, p_x, p_y)){
            b[x][y] = 0;
            p_x = x;
            p_y = y;
            return true;
          }
          b[x][y] = 0;
        }
      }
    }
  }
  return false;
}


//returns true if the player specified can win this turn
bool solve(int board[3][3], int player){
  int x;
  int y;
  //check to see if we have two in a row, just missing one peice
  //fill in a whole row, return true that we won
  if(solutions(board, 2*player, x, y) > 0){
    board[x][y] = player;
    return true;
  //check to see if the other player is about to win,
  //if so, block it
  }else if(solutions(board, -2*player, x, y) > 0){
    board[x][y] = player;
    return false;
  //check to see if we can force a win down the line
  //then play there
  }else if(force(board, player, player, x, y)){
    board[x][y] = player;
    return false;

  //special case: this is actually really bad practice
  //I should find a better way to choose something that isn't a "force"
  }else if(board[1][1] == -1){
		board[2][2] = 1;
	}
  return false;
}

//reset the board for a new game
void reset(int board[3][3], int curs_x, int curs_y){
  //clear the ncurses window
  clear();
  //empty all positions
  for(int x = 0; x < 3; ++x){
      for(int y = 0; y < 3; ++y){
          board[x][y] = 0;
      }
  }
  //set upper left to be a 1, that is strongest first play for the board
  board[0][0] = 1;
  draw(board, curs_x, curs_y);
}

int main(int argc, char* argv[]){
  //the board of peices
  //1 represents an X (computer)
  //-1 represents an O (player)
  int board[3][3];
  //initialize the ncurses screen
  initscr();

  //the character that we will read in from the user
  int c;

  //where the user is "Pointing"
  //this will be represented by a '$' character in the top left of the cell
  int curs_x = 0;
  int curs_y = 0;

  //clear the board, and draw it.
  reset(board, curs_x, curs_y);

  //ncurses initialization
  //we will just be using the stdscr window(default)
  keypad(stdscr, true);//allows using the arrow keys
  curs_set(0);//remove the cursor from the screen
  noecho();//stop user keypresses from appearing on the screen

  //draw the board
  draw(board, curs_x, curs_y);

  //main game loop, run once for each key press
  //if the escape keep (27) is pressed, escape
  while((c=getch()) !=27){
    //refresh the screen
    refresh();

    //Different arrow keys
    if(c == 259 && curs_y > 0){--curs_y;}
    else if(c == 258 && curs_y < 2){++curs_y;}
    else if(c == 260 && curs_x > 0){--curs_x;}
    else if(c == 261 && curs_x < 2){++curs_x;}
    //Enter key
    else if(c == 10 && board[curs_x][curs_y] == 0){
      //set the value at curs_x,curs_y to be -1 for an O
      board[curs_x][curs_y] = -1;
      //go through and try to win as the computer
      if(solve(board, 1)){
        mvprintw(25, 0, "Haha! You suck!");
        draw(board, curs_x, curs_y);
        c = getch();
        //break out of game if not spacebar
        if(c != 32){
          break;
        }else{
          reset(board, curs_x, curs_y);
        }
      }

    //if spacebar, reset the board
    }else if(c == 32){
      reset(board, curs_x, curs_y);
    }
    //draw what's changed
    draw(board, curs_x, curs_y);
  }

  //close the ncurses window
  endwin();
  return 0;
}
