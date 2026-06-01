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
