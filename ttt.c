/* ttt.c - Unbeatable Tic-Tac-Toe via minimax. You are X, AI is O.
 * Build: gcc ttt.c -o ttt */
#include <stdio.h>

static char b[9];

static int winner(void){
    int L[8][3]={{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    for(int i=0;i<8;i++){ char a=b[L[i][0]];
        if(a!=' '&&a==b[L[i][1]]&&a==b[L[i][2]]) return a=='O'?1:-1; }
    return 0;
}
static int full(void){ for(int i=0;i<9;i++) if(b[i]==' ') return 0; return 1; }

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
static int aimove(void){
    int bi=-1,best=-10;
    for(int i=0;i<9;i++) if(b[i]==' '){ b[i]='O'; int v=minimax(0); b[i]=' ';
        if(v>best){best=v;bi=i;} }
    return bi;
}
static void show(void){
    printf("\033[2J\033[H");
    for(int i=0;i<9;i++){ printf(" %c ", b[i]==' '?('1'+i):b[i]); if(i%3!=2)printf("|"); else if(i!=8)printf("\n-----------\n"); }
    printf("\n\n");
}
int main(void){
    for(int i=0;i<9;i++) b[i]=' ';
    printf("Tic-Tac-Toe: you are X. Enter 1-9.\n");
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
    int w=winner();
    printf(w==1?"AI (O) wins.\n":w==-1?"You win!\n":"Draw.\n");
    return 0;
}
