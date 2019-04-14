/* include header file */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/* given TypeCard structure definition */
typedef struct card {
    char suit;   // 'S'pade / 'H'eart / 'C'lub / 'D'iamond
    int  rank;   // A (1) / 2 - 10 / J (11) / Q (12) / K (13);
    int  status; // covered (0) or flipped (1) or chosen (-1), etc.
} TypeCard;

void printCard(TypeCard aCard) {
    if (aCard.status == 0)
        printf("XXX");
    else {
        // display card suit
        switch (aCard.suit) {
            case 'S':
            case 'H':
            case 'C':
            case 'D':
                printf("%c", aCard.suit);
                break;
        }
        // display card rank
        switch (aCard.rank) {
            case 1:
                printf(" A");
                break;
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
                printf("%2d", aCard.rank);
                break;
            case 11:
                printf(" J");
                break;
            case 12:
                printf(" Q");
                break;
            case 13:
                printf(" K");
                break;
        }
    }
    return;
}

void showDesktop(TypeCard desktop[4][14]) {
    /* show all 4 x 13 cards in the desktop 2D TypeCard array */
    int r,c;
    for(r=0;r<4;r++) {
        for(c=1;c<14;c++) {
            printCard(desktop[r][c]);
            printf("\t");
        }
        printf("\n");
    }
}

void rd(TypeCard desktop[3][14]) {
 srand(time(NULL));
 for(int i=0; i<4; i++) {
     for(int j = 1; j < 14; j++) {
         int colR=rand()%12+1;
         int rowT=rand()%4+1;
         int rowR=rowT-1;
         TypeCard Tem = desktop[i][j];
         desktop[i][j]=desktop[rowR][colR];
         desktop[rowR][colR]=Tem;
  }
 }
}

int gameInfo() {
  printf("*****************************\n");
  printf("* Welcome to Concentration! *\n");
  printf("*****************************\n");
  printf("Please choose a mode of the game:\n");
  printf("1. Human vs. Computer\n");
  printf("2. Computer vs. Computer\n");
  printf("0. Quit\n");
  printf("Your choice:");
  int mode;
  // human vs computer (1)
  // computer vs computer (2)
  // quit (0)
  // check table 999
  scanf("%d", &mode);
  return mode;
}

void gamePlay(TypeCard desktop[4][14], int remainCard, int gamemode) {
  // p1 card and p2 card
  int cp1 = 0, cp2 = 0;
  // 0 is human / c1 turn
  // 1 is c2 turn
  int turn = 0;
  // 0 is human / c1 win
  // 1 is c2 win
  int winner = 0;

  // computer table
  int size = 52;
  int comtbl[52] = {};
  for(int i = 0; i < size; i++) {
    comtbl[i] = i;
  }

  // random the desktop
  rd(desktop);

  // process game
  while(remainCard) {
    // 1st card and 2nd card row and col
    int row1, row2, col1, col2;
    printf("\n");
    showDesktop(desktop);
    printf("\n");
    if(gamemode == 1 && turn == 0) {
      // human select

      // 1 is ok
      // 0 is not ok
      int inputCorrect = 0; // input lock

      // get card 1
      while(!inputCorrect) {
        // set row1 and col1 to 0
        row1 = col1 = 0;

        printf("Please input 1st card:\n");
        scanf("%d %d", &row1, &col1);

        if((row1 < 0 || row1 > 3) || (col1 < 1 || col1 > 13)) {
          // check for condition 1
          printf("Row and column must be within 0 - 3 and 1 - 13\n");
        } else if(desktop[row1][col1].status == -1 ) {
          // check for condition 2
          printf("The card must not be flipped already\n");
        } else {
          // input success
          inputCorrect = 1;
          desktop[row1][col1].status = 1;
        }
      }
      printf("You chose 1st card (%d, %d) and it is \n", row1, col1);
      printCard(desktop[row1][col1]);
      printf("\n");

      sleep(1);

      // reset the lock
      inputCorrect = 0;

      //get card 2
      while(!inputCorrect) {
        // set row 2 and col2 to 0
        row2 = col2 = 0;

          printf("Please input 2nd card:\n");
          scanf("%d %d", &row2, &col2);

          if((row2 < 0 || row2 > 3) || (col2 < 1 || col2 > 13)) {
            // check for condition 1
              printf("Row and column must be within 0 - 3 and 1 - 13\n");
          } else if(desktop[row2][col2].status == -1 ) {
            // check for condition 2
              printf("The card must not be flipped already\n");
          } else if((row2 == row1) && (col2 == col1)) {
            // check for condition 3
              printf("The second card cannot be the same as first\n");
          } else {
            // input success
              inputCorrect = 1;
              desktop[row2][col2].status = 1;
          }
      }
      printf("You chose 2nd card (%d, %d) and it is \n", row2, col2);
      printCard(desktop[row2][col2]);
      printf("\n");

    } else if(gamemode == 2 || turn == 1) {
      // computer select

      // get card 1
      // random get row 1 and col 1
      // still check condition 2, if fall into condition then random again


      // delay 1 second to advoid select 2 same card
      sleep(1);

      // get card 2
      // random get row 2 and col 2
      // still check condition 2 & 3, if fall into condition then random again


    }

    // if 2 card is match
      // change status
      // comtbl delete element and minus size
      // add play card
    // if not match, reset the card status to 0

    // finially chage turn



  }

  return;
}

/* starting point of this C program */
int main(void) {
    TypeCard desktop[4][14] = {
        /* the first element (column 0) of each row is UNUSED */
        /* row: 0 - 3; col: 1 - 13 */
        {{}, {'S', 1}, {'S', 2}, {'S', 3}, {'S', 4}, {'S', 5}, {'S', 6}, {'S', 7}, {'S', 8}, {'S', 9}, {'S', 10}, {'S', 11}, {'S', 12}, {'S', 13}},
        {{}, {'H', 1}, {'H', 2}, {'H', 3}, {'H', 4}, {'H', 5}, {'H', 6}, {'H', 7}, {'H', 8}, {'H', 9}, {'H', 10}, {'H', 11}, {'H', 12}, {'H', 13}},
        {{}, {'C', 1}, {'C', 2}, {'C', 3}, {'C', 4}, {'C', 5}, {'C', 6}, {'C', 7}, {'C', 8}, {'C', 9}, {'C', 10}, {'C', 11}, {'C', 12}, {'C', 13}},
        {{}, {'D', 1}, {'D', 2}, {'D', 3}, {'D', 4}, {'D', 5}, {'D', 6}, {'D', 7}, {'D', 8}, {'D', 9}, {'D', 10}, {'D', 11}, {'D', 12}, {'D', 13}}
    };

    int remainCard = 52;
    int gamemode = gameInfo();

    if(gamemode == 1) {
      // human vs computer
      gamePlay(desktop, remainCard, gamemode);
    } else if(gamemode == 2) {
      // computer vs computer
    } else if(gamemode == 999) {
      // show initial desktop
      printf("Initial desktop:\n");
      showDesktop(desktop);

      // flipped all cards
      for (int row = 0; row < 4; row++) {
        for (int col = 1; col <= 13; col++) {
          desktop[row][col].status = 1;
        }
      }

      // show desktop
      printf("\n\nRandom desktop\n");
      rd(desktop);
      showDesktop(desktop);
    } else if(gamemode == 0) {
    }

    /* last statement of this C program */

    return 0;
}
