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
  srand(time(NULL));
  // playerInfo
  int playerInfo = 0;
  // p1 card and p2 card
  int cp1 = 0, cp2 = 0;
  // 0 is human / c1 turn
  // 1 is c2 turn
  int turn = 0;
  // computer select card 1 and 2
  int num1 = 0, num2 = 0;
  // 0 is human / c1 win
  // 1 is c2 win
  int winner = 0;

  // computer table
  int size = 52;
  int comtbl[52] = {};
  for(int i = 0; i < size; i++) {
    comtbl[i] = i+1;
  }

  // random the desktop
  //rd(desktop);

  // process game
  while(remainCard) {
    // 1st card and 2nd card row and col
    int row1, row2, col1, col2;
    if (gamemode == 1 || !turn ){
      printf("\n");
      showDesktop(desktop);
      printf("\n");
    }

    if(turn > 0) {
      printf("Flipped cards: %d; ", 52-remainCard);
      if(gamemode == 1) {
        printf("You: %d; Computer: %d;", cp1, cp2);
      } else if (gamemode == 2) {
        printf("Computer 1: %d; Computer 2: %d;", cp1, cp2);
      }
      printf("\n");
    }


    if(gamemode == 1 && turn%2 == 0) {
      // human select

      // 1 is ok
      // 0 is not ok
      int inputCorrect = 0; // input lock

      // get card 1
      while(!inputCorrect) {
        // set row1 and col1 to 0
        row1 = col1 = 0;

        printf("Please input 1st card: ");
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
      printf("You chose 1st card (%d, %d) and it is ", row1, col1);
      printCard(desktop[row1][col1]);
      printf("\n");

      // reset the lock
      inputCorrect = 0;

      //get card 2
      while(!inputCorrect) {
        // set row 2 and col2 to 0
        row2 = col2 = 0;

          printf("Please input 2nd card: ");
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
      printf("You chose 2nd card (%d, %d) and it is ", row2, col2);
      printCard(desktop[row2][col2]);
      printf("\n");

    } else if(gamemode == 2 || turn%2 == 1) {
      // computer select

      // get card 1 position
      num1 = rand() % size + 1;
      num2 = -1;
      do {
        // delay 1 second to advoid select 2 same card
        sleep(1);

        // get card 2 position
        num2 = rand() % size + 1;
      } while(num1 == num2);

      // get Card 1 row and col
      int rCard1 = comtbl[--num1];
      row1 = rCard1 / 13;
      col1 = rCard1 % 13;
      if(rCard1 == 52) {
        row1 = 3;
      }
      if(col1 == 0) {
        col1 = 13;
      }

      // set card status = 1
      desktop[row1][col1].status = 1;
      printf("Computer chose 1st card(%d, %d) and it is ", row1, col1);
      printCard(desktop[row1][col1]);
      printf("\n");

      // get Card 2 row and col
      int rCard2 = comtbl[--num2];
      row2 = rCard2 / 13;
      col2 = rCard2 % 13;
      if(rCard2 == 52) {
        row2 = 3;
      }
      if(col2 == 0) {
        col2 = 13;
      }

      desktop[row2][col2].status = 1;
      printf("Computer chose 1st card(%d, %d) and it is ", row2, col2);
      printCard(desktop[row2][col2]);
      printf("\n");

      num1 = rCard1;
      num2 = rCard2;

    }

    // if 2 card is match
    if( ( ((desktop[row1][col1].suit=='C' && desktop[row2][col2].suit=='S') ||
         (desktop[row1][col1].suit=='S' && desktop[row2][col2].suit=='C')) &&
         (desktop[row1][col1].rank == desktop[row2][col2].rank) ) ||

         (((desktop[row1][col1].suit=='D' && desktop[row2][col2].suit=='H') ||
         (desktop[row1][col1].suit=='H' && desktop[row2][col2].suit=='D')) &&
         (desktop[row1][col1].rank == desktop[row2][col2].rank)))
      {
      // printf("Correct\n");
      // change status
      desktop[row1][col1].status = desktop[row2][col2].status = -1;

      // comtbl delete element and minus size
      if(gamemode == 1 && turn%2 == 0) {
        // user correct, delete
        num1 = row1 * 13 + col1;
        num2 = row2 * 13 + col2;
      }

      if(size-2 > 0) {

        for(int i = 0; i < size; i++) {
          if(comtbl[i] == num1) {
            num1 = i;
            break;
          }
        }

        for(int i = num1; i < size-1; i++) {
          comtbl[i] = comtbl[i+1];
        }
        size--;

        for(int i = 0; i < size; i++) {
          if(comtbl[i] == num2) {
            num2 = i;
            break;
          }
        }

        for(int i = num2; i < size-1; i++) {
          comtbl[i] = comtbl[i+1];
        }
        size--;
      }

      // add play card
      if(!(turn%2)) {
          cp1 += 2;
      } else {
          cp2 += 2;
      }
      // decrease the remain Card
      remainCard -= 2;
    } else {
      // printf("Incorrect\n");
      // if not match, reset the card status to 0
      desktop[row1][col1].status = desktop[row2][col2].status = 0;
    }

    // finially chage turn
    turn++;
  }

  printf("\n");
  showDesktop(desktop);
  printf("\n");

  printf("Flipped cards: %d; ", 52-remainCard);
  if(gamemode == 1) {
    printf("You: %d; Computer: %d;", cp1, cp2);
  } else if (gamemode == 2) {
    printf("Computer 1: %d; Computer 2: %d;", cp1, cp2);
  }
  printf("\n");

  // print winning message


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

    while (gamemode){
      gamePlay(desktop, remainCard, gamemode);
    }

    /* last statement of this C program */

    return 0;
}
