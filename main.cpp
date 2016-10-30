#include <iostream>
#include <list>
#include <ncurses.h>

using namespace std;



void print_col(int x, int y, char fill, int hight){
    for(int i = 0; i < hight; ++i){
        mvaddch(y+i, x, fill);
    }
}
void print_row(int x, int y, char fill, int hight){
    for(int i = 0; i < hight; ++i){
        mvaddch(y, x+i, fill);
    }
}



void print_x(int x, int y){
    mvprintw(y+0, x+1, "\\ \\ / /");
    mvprintw(y+1, x+1, " \\ V / ");
    mvprintw(y+2, x+1, "  > <");
    mvprintw(y+3, x+1, " / . \\ ");
    mvprintw(y+4, x+1, "/_/ \\_\\");
}

void print_o(int x, int y){
    mvprintw(y+0, x+1, "  ____  ");
    mvprintw(y+1, x+1, " / __ \\ ");
    mvprintw(y+2, x+1, "| |  | |");
    mvprintw(y+3, x+1, "| |__| |");
    mvprintw(y+4, x+1, " \\____/ ");
}

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
    mvprintw(18, 0, "Use the arrow keys to navigate, and hit enter to place. Reset at anytime with the space bar.");
    refresh();

}

int solutions(int board[3][3], int check_for, int& out_x, int& out_y){
  int sum;
  int count = 0;
  for(int x = 0; x < 3; ++x){
    sum = 0;
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
  return count;
}

bool force(int b[3][3], int p, int t, int& p_x, int& p_y){
  int x;
  int y;
  if(solutions(b, 2*p, x, y) > 0 && t == p){
    return true;
  }else if(solutions(b, -2*p, x, y) > 0 && t == p){
    b[x][y] = p;
    if(force(b, p, -1*t, p_x, p_y)){
      b[x][y] = 0;
      p_x = x;
      p_y = y;
      return true;
    }
    b[x][y] = 0;
  }else if(solutions(b, -2*p, x, y) > 0 && t == -1*p){
    return false;
  }else if(solutions(b, 2*p, x, y) > 0 && t == -1*p){
    b[x][y] = t;
    if(force(b, p, -1*t, p_x, p_y)){
      b[x][y] = 0;
      return true;
    }
    b[x][y] = 0;
  }else if(p == t){
    for(x = 0; x < 3; ++x){
      for(y = 0; y < 3; ++y){
        if(b[x][y] == 0){
          b[x][y] = p;
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


bool solve(int board[3][3], int player){
  int x;
  int y;
  if(solutions(board, 2*player, x, y) > 0){
    board[x][y] = 1;
    return true;
  }else if(solutions(board, -2*player, x, y) > 0){
    board[x][y] = 1;
    return false;
  }else if(force(board, player, player, x, y)){
    board[x][y] = 1;
    return false;
  }else if(board[1][1] == -1){
		board[2][2] = 1;
	}
  return false;
}

void reset(int board[3][3], int curs_x, int curs_y){
  clear();
  for(int x = 0; x < 3; ++x){
      for(int y = 0; y < 3; ++y){
          board[x][y] = 0;
      }
  }
  board[0][0] = 1;
  draw(board, curs_x, curs_y);
}

int main(int argc, char* argv[]){
    int board[3][3];
    initscr();
    int c;
    int curs_x = 0;
    int curs_y = 0;
    reset(board, curs_x, curs_y);
    keypad(stdscr, true);
    curs_set(0);
    noecho();
    draw(board, curs_x, curs_y);
    while((c=getch()) !=27){
      refresh();
      if(c == 259 && curs_y > 0){--curs_y;}
      else if(c == 258 && curs_y < 2){++curs_y;}
      else if(c == 260 && curs_x > 0){--curs_x;}
      else if(c == 261 && curs_x < 2){++curs_x;}
      else if(c == 10 && board[curs_x][curs_y] == 0){
        board[curs_x][curs_y] = -1;
        if(solve(board, 1)){
          mvprintw(19, 0, "Haha! You suck!");
          draw(board, curs_x, curs_y);
          c = getch();
          if(c != 32){
            break;
          }else{
            reset(board, curs_x, curs_y);
          }
        }
      }else if(c == 32){
        reset(board, curs_x, curs_y);
      }
      draw(board, curs_x, curs_y);
    }
    endwin();
    return 0;
}
