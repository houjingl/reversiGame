//
// Author: Jerry (Jingling) Hou
//

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// ASCII code: W 87 ; B 66 ; U 85 ; 32 between A and a

bool positionInBounds(int n, int row, int col) {
  if (row >= 0 && row < n && col >= 0 && col < n) {
    return true;
  } else {
    return false;
  }
}

void createBoard(char board[][26], int n) {
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      // initialize the table
      if ((row == n / 2 - 1 && col == n / 2 - 1) ||
          (row == n / 2 && col == n / 2)) {
        // printf("W");
        board[row][col] = (char)87;
      } else if ((row == n / 2 - 1 && col == n / 2) ||
                 (row == n / 2 && col == n / 2 - 1)) {
        // printf("B");
        board[row][col] = (char)66;
      } else {
        // printf("U");
        board[row][col] = (char)85;
      }
    }
    // printf("\n");
  }
}

void printBoard(char board[][26], int n) {
  char letter = 'a';

  // print header
  printf("  ");
  for (int i = 0; i < n; i++) {
    printf("%c", letter + i);
  }
  printf("\n");

  // print each row
  for (int row = 0; row < n; row++) {
    int col = 0;
    printf("%c ", letter + row);
    while (board[row][col] != '\0' && col < 26) {
      printf("%c", board[row][col]);
      col++;
    }
    printf("\n");
  }
}

bool checkLegalInDirection(char board[][26], int n, int row, int col,
                           char colour, int deltaRow, int deltaCol) {
  int tempR = row, tempC = col;
  bool encounterSpace = false;
  do {
    if (board[tempR][tempC] == 'U') {
      encounterSpace = true;
    }

    if (deltaRow == 1) {
      tempR++;
    } else if (deltaRow == -1) {
      tempR--;
    }

    if (deltaCol == 1) {
      tempC++;
    } else if (deltaCol == -1) {
      tempC--;
    }

    if (board[tempR][tempC] == colour && positionInBounds(n, tempR, tempC) &&
        !encounterSpace) {
      return true;
    }
  } while (positionInBounds(n, tempR, tempC));
  return false;
}

bool checkAvailableMove(char board[][26], int n, char currentColor,
                        char opponentColor, int elementRow, int elementCol) {
  if (positionInBounds(n, elementRow, elementCol) &&
      board[elementRow][elementCol] == 'U') {
    for (int row = elementRow - 1; row <= elementRow + 1; row++) {
      for (int col = elementCol - 1; col <= elementCol + 1; col++) {
        if (positionInBounds(n, row, col) && board[row][col] == opponentColor &&
            (row != elementRow || col != elementCol)) {
          // proceed search --> if row > eR, row ++, if col > eC, col ++, vise
          // versa, if row == eR || col == eC, no operation
          if (checkLegalInDirection(board, n, row, col, currentColor,
                                    row - elementRow, col - elementCol)) {
            return true;
          }
        }
      }
    }
  }
  return false;
}

void changeTheBoard(char board[][26], int n, char color, int row, int col) {
  // NW --> N --> NE --> W --> E --> SW --> S --> SE
  //  0     1     2      3     4      5     6      7
  //  Dynamically Allocate an array size of 8, using checklegalinDirection
  //  function to check direction record the result (set correct direction to 1
  //  and invalid direction to 0) in the array, and loop over array to make
  //  changes

  int diridx = 0;  // the index for dir array
  bool *dir = (bool *)malloc(sizeof(bool) * 8);

  for (int i = row - 1; i <= row + 1; i++) {
    for (int k = col - 1; k <= col + 1; k++) {
      if (i != row ||
          k != col) {  // to skip where the user want to make a change
        if (positionInBounds(n, i, k) &&
            checkLegalInDirection(board, n, i, k, color, i - row, k - col)) {
          dir[diridx] = true;
        } else {
          dir[diridx] = false;
        }
        diridx++;
      }
    }
  }
  int tempR, tempC;
  for (int i = 0; i < 8; i++) {
    tempR = row;
    tempC = col;
    if (dir[i]) {
      do {
        board[tempR][tempC] = color;
        if (i == 0) {
          tempR--;
          tempC--;
        } else if (i == 1) {
          tempR--;
        } else if (i == 2) {
          tempR--;
          tempC++;
        } else if (i == 3) {
          tempC--;
        } else if (i == 4) {
          tempC++;
        } else if (i == 5) {
          tempR++;
          tempC--;
        } else if (i == 6) {
          tempR++;
        } else if (i == 7) {
          tempR++;
          tempC++;
        }

      } while (board[tempR][tempC] != color);
    }
  }

  free(dir);
}

void usermakeAmove(char board[][26], int n, char chosenColor,
                   char opponentColor, bool *userMoveLegality) {
  int movableSpot = 0;
  for (int i = 0; i < n; i++) {
    for (int k = 0; k < n; k++) {
      if (checkAvailableMove(board, n, chosenColor, opponentColor, i, k)) {
        movableSpot++;
      }
    }
  }

  if (movableSpot == 0) {
    printf("%c player has no valid move.\n", chosenColor);
    return;
  }

  int row,  col ;
  findSmartestMove(board, n, chosenColor, &row, &col);
  printf("Testing AI move (row, col): %c%c\n", row + 'a', col + 'a');

  changeTheBoard(board, n, chosenColor, row, col);
  printBoard(board, n);
}

bool gameContinue(char board[][26], int n, char computerColor, char userColor) {
  bool condition = false;
  for (int i = 0; i < n && !condition; i++) {
    for (int k = 0; k < n && !condition; k++) {
      if (board[i][k] == 'U') {
        condition = true;
      }
      if (!checkAvailableMove(board, n, computerColor, userColor, i, k) &&
          !checkAvailableMove(board, n, userColor, computerColor, i, k)) {
        condition = false;
      }
    }
  }
  return condition;
}

void declareWinner(char board[][26], int n) {
  int scoreB = 0, scoreW = 0;
  for (int i = 0; i < n; i++) {
    for (int k = 0; k < n; k++) {
      if (board[i][k] == 'B') {
        scoreB++;
      } else if (board[i][k] == 'W') {
        scoreW++;
      }
    }
  }

  if (scoreB > scoreW) {
    printf("B player wins.");
  } else if (scoreB < scoreW) {
    printf("W player wins.");
  } else {
    printf("Draw!");
  }
}

int gridScore(char board[][26], int row, int col, char color, int n) {
  n--;  // convey n to index form

  // Condition to evaluate if the grid is around the corner
  if ((row < 2 || row > n - 2) && (col < 2 || col > n - 2) &&
      !((row == 0 || row == n) && (col == 0 || col == n))) {
    // Condition to find the grid is close to which corner
    int cornerR, cornerC;
    if (row < 2 && col < 2) {
      cornerR = cornerC = 0;
    } else if (row < 2 && col > n - 2) {
      cornerR = 0;
      cornerC = n;
    } else if (row > n - 2 && col < 2) {
      cornerR = n;
      cornerC = 0;
    } else if (row > n - 2 && col > n - 2) {
      cornerR = n;
      cornerC = n;
    }

    if (board[cornerR][cornerC] != color) {
      return -50;
    }
  }

  if ((row == 0 || row == n) && (col == 0 || col == n)) {
    return 200;  // corner
  } else if ((row == col) || (row == n - col)) {
    return 10;  // diagonal
  } else if (row == 0 || row == n || col == 0 || col == n) {
    return 5;  // side
  } else {
    return 2;
  }
}

int scoreFlipped(char board[][26], int n, char color, int row, int col) {
  int diridx = 0;  // the index for dir array
  bool *dir = (bool *)malloc(sizeof(bool) * 8);

  for (int i = row - 1; i <= row + 1; i++) {
    for (int k = col - 1; k <= col + 1; k++) {
      if (i != row ||
          k != col) {  // to skip where the user want to make a change
        if (positionInBounds(n, i, k) &&
            checkLegalInDirection(board, n, i, k, color, i - row, k - col)) {
          dir[diridx] = true;
        } else {
          dir[diridx] = false;
        }
        diridx++;
      }
    }
  }
  int tempR, tempC, scoreFlipped = 0;
  for (int i = 0; i < 8; i++) {
    tempR = row;
    tempC = col;
    if (dir[i]) {
      do {
        if (i == 0) {
          tempR--;
          tempC--;
        } else if (i == 1) {
          tempR--;
        } else if (i == 2) {
          tempR--;
          tempC++;
        } else if (i == 3) {
          tempC--;
        } else if (i == 4) {
          tempC++;
        } else if (i == 5) {
          tempR++;
          tempC--;
        } else if (i == 6) {
          tempR++;
        } else if (i == 7) {
          tempR++;
          tempC++;
        }
        scoreFlipped += gridScore(board, tempR, tempC, color, n);
      } while (board[tempR][tempC] != color);
    }
  }

  free(dir);
  return scoreFlipped;
}

int scoreFlippedOpponent(char board[][26], int n, int newpieceR, int newpieceC,
                         char mycolor, char opponentcolor) {
  // creat a temperory 2D array
  char tempBoard[26][26] = {'\0'};

  // Copy the board before the potential flipping operation
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      tempBoard[row][col] = board[row][col];
    }
  }

  // perform flipping operation to the dynamically allocated board
  changeTheBoard(tempBoard, n, mycolor, newpieceR, newpieceC);

  // calculate the highest possible score might be flipped by opponent
  int highestScoreFlipped = 0;
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (checkAvailableMove(tempBoard, n, opponentcolor, mycolor, row, col)) {
        int tempscore = gridScore(tempBoard, row, col, opponentcolor, n) +
                        scoreFlipped(tempBoard, n, opponentcolor, row, col);
        if (highestScoreFlipped < tempscore) {
          highestScoreFlipped = tempscore;
        }
      }
    }
  }

  return highestScoreFlipped;
}

int makeMove(const char board[][26], int n, char turn, int *row, int *col);
int countTiles(char board[][26], int n, char color, int row, int col);
void smartMakeMove(char board[][26], int n, char assignedColor,
                   char opponentcolor);

int main(void) {
  int n;
  char board[26][26] = {'\0'};
  printf("Enter the board dimension: ");
  scanf("%d", &n);
  bool userMoveLegality = true;

  // Initialize the Board
  createBoard(board, n);

  // Assign color for computer player and human player
  char computerColor, userColor;
  printf("Computer plays (B/W): ");
  scanf(" %c", &computerColor);
  if (computerColor == 'B') {
    userColor = 'W';
  } else {
    userColor = 'B';
  }

  // Print the Board with rows and columns labelled abcd ...
  printBoard(board, n);

  // Start the game
  if (computerColor == 'B') {
    smartMakeMove(board, n, computerColor, userColor);
  }

  while (userMoveLegality && gameContinue(board, n, computerColor, userColor)) {
    usermakeAmove(board, n, userColor, computerColor, &userMoveLegality);
    if (!(userMoveLegality &&
          gameContinue(board, n, computerColor, userColor))) {
      break;
    }
    smartMakeMove(board, n, computerColor, userColor);
  }

  if (!userMoveLegality) {
    printf("%c player wins.\n", computerColor);
  } else {
    declareWinner(board, n);
  }

  return 0;
}

// Start of new functions for lab8 part 1

int countTiles(char board[][26], int n, char color, int row, int col) {
  // modified version of change the board function
  int diridx = 0;  // the index for dir array
  bool *dir = (bool *)malloc(sizeof(bool) * 8);

  for (int i = row - 1; i <= row + 1; i++) {
    for (int k = col - 1; k <= col + 1; k++) {
      if (i != row ||
          k != col) {  // to skip where the user want to make a change
        if (positionInBounds(n, i, k) &&
            checkLegalInDirection(board, n, i, k, color, i - row, k - col)) {
          dir[diridx] = true;
        } else {
          dir[diridx] = false;
        }
        diridx++;
      }
    }
  }
  int tempR, tempC;
  int count = -1;
  for (int i = 0; i < 8; i++) {
    tempR = row;
    tempC = col;
    if (dir[i]) {
      do {
        count++;
        if (i == 0) {
          tempR--;
          tempC--;
        } else if (i == 1) {
          tempR--;
        } else if (i == 2) {
          tempR--;
          tempC++;
        } else if (i == 3) {
          tempC--;
        } else if (i == 4) {
          tempC++;
        } else if (i == 5) {
          tempR++;
          tempC--;
        } else if (i == 6) {
          tempR++;
        } else if (i == 7) {
          tempR++;
          tempC++;
        }

      } while (board[tempR][tempC] != color);
    }
  }

  free(dir);
  return count;
}

void defaultMakeMove(char board[][26], int n, char assignedColor,
                     char opponentColor) {
  int highestR, highestC;
  int highestScore = 0;

  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (checkAvailableMove(board, n, assignedColor, opponentColor, row,
                             col)) {
        int tempcount = countTiles(board, n, assignedColor, row, col);
        if (tempcount > highestScore && highestR != row) {
          highestScore = tempcount;
          highestR = row;
          highestC = col;
        }
      }
    }
  }

  if (highestScore == 0) {
    printf("%c player has no valid move.\n", assignedColor);
    return;
  }

  changeTheBoard(board, n, assignedColor, highestR, highestC);
  printf("Computer places %c at %c%c.\n", assignedColor, highestR + 'a',
         highestC + 'a');
  printBoard(board, n);
}

// Main Function for lab 8 part 2
int makeMove(const char board[][26], int n, char turn, int *row, int *col) {
  char opponentColor;
  if (turn == 'B') {
    opponentColor = 'W';
  } else {
    opponentColor = 'B';
  }

  int basicGridScore = 0;
  double TotalScore = -1000;
  for (int i = 0; i < n; i++) {
    for (int k = 0; k < n; k++) {
      if (checkAvailableMove(board, n, turn, opponentColor, i, k)) {
        basicGridScore = gridScore(board, i, k, turn, n);
        double tempTotalScore =
            basicGridScore + (1.5) * scoreFlipped(board, n, turn, i, k) -
            scoreFlippedOpponent(board, n, i, k, turn, opponentColor);
        if (tempTotalScore > TotalScore) {
          TotalScore = tempTotalScore;
          *row = i;
          *col = k;
          printf("TotalScore: %.2lf Row: %d Col %d\n", TotalScore, *row, *col);
        }
      }
    }
  }
  return (int)TotalScore;
}

void smartMakeMove(char board[][26], int n, char assignedColor,
                   char opponentcolor) {
  int highestR, highestC;
  int highestScore = -1000;

    highestScore = makeMove(board, n, assignedColor, &highestR, &highestC);

  if (highestScore == -1000) {
    printf("%c player has no valid move.\n", assignedColor);
    return;
  }

  changeTheBoard(board, n, assignedColor, highestR, highestC);
  printf("Computer places %c at %c%c.\n", assignedColor, highestR + 'a',
         highestC + 'a');
  printBoard(board, n);
}
