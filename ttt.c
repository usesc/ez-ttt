/*
board positions: 
1 2 3 
4 5 6
7 8 9
*/

// much simpler than my previous c++ oop garbage

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <ncurses.h>

// This is for the board drawing and for the winner
typedef enum {
  EMPTY,
  PLAYER_X,
  PLAYER_O,
  TIE
} board_t;

board_t board[9] = {
  EMPTY, EMPTY, EMPTY,
  EMPTY, EMPTY, EMPTY,
  EMPTY, EMPTY, EMPTY
};

// this is for me to test ttt board format (visuals)
board_t testing_board[9] = {
  PLAYER_X, PLAYER_O, PLAYER_X,
  PLAYER_O, PLAYER_X, PLAYER_O,
  PLAYER_O, PLAYER_X, PLAYER_O
};

// turns board member into a char (X, O)
char board_to_char(board_t sh) {
  switch (sh) {
    case EMPTY:
      return '-';
    case PLAYER_X:
      return 'X';
    case PLAYER_O:
      return 'O';
    default:
      return '~';
  }
}

// checks winner, no idea how it works, it just does
board_t check_winner(board_t b[9]) {
  for (int i = 0; i < 3; i++) {
    if (b[i] && b[i] == b[i+3] && b[i] == b[i+6]) return b[i];
    if (b[i*3] && b[i*3] == b[i*3+1] && b[i*3] == b[i*3+2]) return b[i*3];
  }
  if (b[0] && b[0] == b[4] && b[0] == b[8]) return b[0];
  if (b[2] && b[2] == b[4] && b[2] == b[6]) return b[2];
  for (int i = 0; i < 9; i++) if (b[i] == EMPTY) return EMPTY;
  return TIE;
}

// draws tictactoe board simply
void render_board(board_t tttboard[9]) {
  for (int i = 0; i < 9; i++) {
    printw("%c ", board_to_char(tttboard[i]));
    if (i % 3 == 2) printw("\n");
  }
}

// updates board with a new player spot
void add_to_board(board_t player, short pos) {
  board[pos] = player;
}

int main() {
  char pos[3];
  bool player_x = true;
  initscr();
  while (1) {
    erase();

    render_board(board);

    board_t winner = check_winner(board);
    if (winner != EMPTY) {
      curs_set(0);
      if (winner == TIE) printw("it's a tie!\n");
      else printw("player %c wins!\n", (winner == PLAYER_X) ? 'X' : 'O');
      getch();
      curs_set(1);
      break;
    }

    printw(player_x ? "pos(x): " : "pos(o): ");
    getnstr(pos, sizeof(pos));

    // means strcmp() == 0
    if (!strcmp(pos, "q")) break;

    short index = atoi(pos) - 1;
    if (index >= 0 && index < 9 && board[index] == EMPTY) {
      add_to_board(player_x ? PLAYER_X : PLAYER_O, index);
    } 
    else {
      curs_set(0);
      printw("invalid");
      getch();
      curs_set(1);
      continue;
    }
    player_x = !player_x;
  }

  endwin();
  return 0;
}
