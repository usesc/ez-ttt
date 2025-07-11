#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// emt = empty
#define EMT 0x2D
#define PLX 0x78
#define PLO 0x6F
#define TIE 0x03

#define u8 unsigned char

u8 check_winner(u8 b[9]) {
  // columns
  if (b[0] != EMT && b[0] == b[3] && b[0] == b[6]) return b[0];
  if (b[1] != EMT && b[1] == b[4] && b[1] == b[7]) return b[1];
  if (b[2] != EMT && b[2] == b[5] && b[2] == b[8]) return b[2];
  // rows
  if (b[0] != EMT && b[0] == b[1] && b[0] == b[2]) return b[0];
  if (b[3] != EMT && b[3] == b[4] && b[3] == b[5]) return b[3];
  if (b[6] != EMT && b[6] == b[7] && b[6] == b[8]) return b[6];
  // diagonal
  if (b[0] != EMT && b[0] == b[4] && b[0] == b[8]) return b[0];
  if (b[2] != EMT && b[2] == b[4] && b[2] == b[6]) return b[2];

  for (int i = 0; i < 9; i++) if (b[i] == EMT) return EMT;

  return TIE;
}

void render_board(int fd, u8 tboard[9]) {
  char buf[20], *p = buf;
  for (int i = 0; i < 9; i++) {
    *p++ = tboard[i];
    *p++ = (i % 3 == 2) ? '\n' : ' ';
  } *p = 0x00;
  write(fd, buf, p-buf);
}

void add_to_board(u8 board[9], u8 player, u8 pos) {
  board[pos] = player;
}

void t_getchar(int fd) {
  char c;
  read(fd, &c, 1);
}

void t_flush(int fd) {
  char c;
  while (read(fd, &c, 1) == 1 && c != '\n');
}

int main() {
  u8 board[9] = {EMT, EMT, EMT, EMT, EMT, EMT, EMT, EMT, EMT};

  char pos[2];
  u8 turn = 0;

  for (;;) {
    render_board(STDOUT_FILENO, board);

    u8 winner = check_winner(board);
    if (winner != EMT) {
      if (winner == TIE) write(STDOUT_FILENO, "its a tie!\n", 11);
      else write(STDOUT_FILENO, (winner == PLX) ? "player X wins!" : "player O wins!", 14);
      t_getchar(STDIN_FILENO);
      break;
    }

    write(STDOUT_FILENO, (turn & 1) ? "pos(o): " : "pos(x): ", 8);
    ssize_t n = read(STDIN_FILENO, pos, sizeof(pos) - 1);
    t_flush(STDOUT_FILENO);

    if (pos[0] == 'q') break;

    write(STDOUT_FILENO, "\n", 1);

    short index = atoi(pos) - 1;
    if (index >= 0 && index < 9 && board[index] == EMT) {
      add_to_board(board, (turn & 1) ? PLO : PLX, index);
    }
    else {
      write(STDOUT_FILENO, "invalid\n\n", 9);
      continue;
    }
    turn++;
  }

  return 0;
}
