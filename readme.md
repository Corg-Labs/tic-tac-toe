# Tic-Tac-Toe

Unbeatable Tic-Tac-Toe with a minimax AI, in C.

A small, self-contained demo written in **pure C** — no external libraries,
just the standard library and POSIX. Part of the [Corg-Labs](https://github.com/Corg-Labs)
collection of single-file C programs.

---

## How It Works

1. The board is a flat 9-cell array
2. Minimax recursively scores every reachable position
3. The AI always picks the move with the best guaranteed score
4. Optimal play means the AI can never lose

---

# Tutorial

This tutorial walks through every moving part of `ttt.c` — from how the board
is stored to how the AI picks an unbeatable move.

## 1. The Board — One Global Array

The entire game state lives in a single 9-element `char` array:

```c
static char b[9];
```

Each cell holds `'X'`, `'O'`, or `' '` (a space meaning empty). Cells are
numbered 0–8 and map to the 3×3 grid in row-major order:

```
0 | 1 | 2
-----------
3 | 4 | 5
-----------
6 | 7 | 8
```

At the start of `main`, every cell is initialised to a space:

```c
for(int i=0;i<9;i++) b[i]=' ';
```

## 2. Win Detection

`winner()` checks all eight lines (three rows, three columns, two diagonals)
against a hard-coded lookup table:

```c
static int winner(void){
    int L[8][3]={{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    for(int i=0;i<8;i++){ char a=b[L[i][0]];
        if(a!=' '&&a==b[L[i][1]]&&a==b[L[i][2]]) return a=='O'?1:-1; }
    return 0;
}
```

It returns `1` if O has won, `-1` if X has won, and `0` if nobody has won yet.
The sign convention matters: the minimax search uses positive scores for the AI
(O) and negative scores for the human (X).

A companion helper `full()` returns `1` when all nine cells are occupied:

```c
static int full(void){ for(int i=0;i<9;i++) if(b[i]==' ') return 0; return 1; }
```

Together, `winner()` and `full()` define the three terminal states: AI win,
human win, and draw.

## 3. The Minimax Algorithm

`minimax(int ai)` explores the complete game tree recursively. The `ai` flag
indicates whose turn it is — `1` for the AI (maximiser), `0` for the human
(minimiser):

```c
static int minimax(int ai){
    int w=winner(); if(w||full()) return w;
    int best = ai?-10:10;
    for(int i=0;i<9;i++) if(b[i]==' '){
        b[i]=ai?'O':'X';
        int v=minimax(!ai);
        b[i]=' ';
        if(ai){ if(v>best)best=v; } else { if(v<best)best=v; }
    }
    return best;
}
```

Key points:

- **Base case**: if `winner()` returns a non-zero value, or the board is full,
  the score is returned immediately — no further recursion.
- **Maximiser (AI)**: starts `best` at `-10` and keeps the highest child score.
- **Minimiser (human)**: starts `best` at `10` and keeps the lowest child score.
- The board is mutated in-place (`b[i] = ...`) and then restored (`b[i] = ' '`)
  after the recursive call — no copying needed.

Because the Tic-Tac-Toe game tree has at most 9! = 362,880 leaves, the full
search completes instantly.

## 4. Picking the AI's Best Move

`aimove()` wraps `minimax` to find the index of the move with the highest score
from O's perspective:

```c
static int aimove(void){
    int bi=-1,best=-10;
    for(int i=0;i<9;i++) if(b[i]==' '){ b[i]='O'; int v=minimax(0); b[i]=' ';
        if(v>best){best=v;bi=i;} }
    return bi;
}
```

It tries every empty cell, calls `minimax(0)` (human's turn next), undoes the
move, and records the index `bi` of whichever cell yielded the best score. The
returned index is then written directly into `b`:

```c
b[aimove()]='O';
```

## 5. Rendering the Board

`show()` clears the terminal with an ANSI escape sequence and then prints the
3×3 grid, substituting the digit label (1–9) for empty cells so the player
always sees which numbers to type:

```c
static void show(void){
    printf("\033[2J\033[H");
    for(int i=0;i<9;i++){ printf(" %c ", b[i]==' '?('1'+i):b[i]); if(i%3!=2)printf("|"); else if(i!=8)printf("\n-----------\n"); }
    printf("\n\n");
}
```

- `\033[2J` erases the screen; `\033[H` moves the cursor to the top-left.
- The ternary `b[i]==' ' ? ('1'+i) : b[i]` maps index `i` to the character
  `'1'`–`'9'` when the cell is empty, or shows `'X'`/`'O'` otherwise.
- `i%3!=2` inserts a `|` separator between cells in the same row.
- `i!=8` suppresses the divider after the last row.

## 6. The Main Game Loop

`main` initialises the board, then alternates between reading the human's move
and computing the AI's response:

```c
while(1){
    show();
    if(winner()|| full()) break;
    int m; printf("your move: ");
    if(scanf("%d",&m)!=1) break;
    if(m<1||m>9||b[m-1]!=' ') continue;
    b[m-1]='X';
    if(winner()||full()){ show(); break; }
    b[aimove()]='O';
}
```

- The loop renders the board first so the player always sees the current state
  before being prompted.
- Invalid input (out of range or a cell already taken) is silently skipped with
  `continue` — the player is prompted again.
- After each human move the game checks for a terminal state before asking the
  AI to respond, preventing the AI from moving on a finished board.
- The final `winner()` call after the loop prints the outcome:
  `"AI (O) wins."`, `"You win!"`, or `"Draw."`.

---

## Build

```
gcc ttt.c -o ttt
```

## Run

```
./ttt
```

## Controls

Enter a cell number **1-9** on your turn (you are X).
