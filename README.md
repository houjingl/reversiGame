Reversi Game README

This is a simple Reversi game written in C. The game supports two modes:

Human vs. Human

Human vs. Computer (AI).

Below is a detailed explanation of the game rules, the implementation, and particularly the algorithm behind the computer player's moves.

Game Rules

Reversi is played on an 8x8 board with two players: Black and White. The players take turns placing a piece of their color on the board. When a player places a piece, it must flank one or more of the opponent's pieces horizontally, vertically, or diagonally. All flanked pieces are then flipped to the current player's color. The game ends when no more moves are possible, and the player with the most pieces of their color wins.

How the Computer Player Algorithm Works

The algorithm for the computer player in this Reversi game uses a minimax-inspired heuristic evaluation to decide its moves. While it does not implement a full-fledged AI with deep recursive search due to simplicity, it follows a structured approach to make competitive moves. Here's a step-by-step breakdown:

Enumerating Legal Moves:

The computer first scans the entire board to identify all possible legal moves it can make. A move is legal if it satisfies the game rule of flanking at least one opponent piece in any direction.

Scoring Potential Moves:

For each legal move, the computer calculates a score based on the immediate impact of that move. The scoring function considers:

Number of pieces flipped: Moves that flip more opponent pieces are given a higher score.

Position weighting: Some positions on the board are inherently more advantageous, such as corners (which are stable and cannot be flipped once captured) and edge tiles. These positions are given additional weight in the score calculation.

Risk of counterattack: Moves that leave the computer vulnerable to losing a corner or edge tile are penalized.

Heuristic Evaluation Function:

The evaluation function assigns a score to each move using the following heuristic:

Score = (Pieces Flipped * Weight_Flip) + (Position Weight) - (Risk Penalty)

Where:

Weight_Flip is a constant that prioritizes flipping pieces.

Position Weight is a predefined value for the tile based on its strategic importance.

Risk Penalty accounts for potential counterattacks.

Choosing the Best Move:

After scoring all possible moves, the computer selects the move with the highest score. If multiple moves have the same score, it selects one at random to introduce variability.

Fallback for No Legal Moves:

If no legal moves are available, the computer passes its turn, as per the game rules.

Example of Scoring

Consider the following board state:

  A B C D E F G H
1 . . . . . . . .
2 . . . . . . . .
3 . . . W B . . .
4 . . W B W . . .
5 . . . B W . . .
6 . . . . . . . .
7 . . . . . . . .
8 . . . . . . . .

If the computer (playing as Black) considers placing a piece at D3:

It flips 1 piece (W at D4).

D3 is a non-corner, non-edge position with a neutral weight.

There is no immediate risk of losing a corner or edge in this scenario.

The move's score would be calculated as:

Score = (1 * Weight_Flip) + (Position Weight) - (Risk Penalty)

This score is then compared to other legal moves, and the highest-scoring move is chosen.

Limitations

Depth of Search: The algorithm only evaluates the immediate consequences of a move and does not simulate multiple turns ahead. This makes it less strategic compared to full minimax or alpha-beta pruning algorithms.

Static Position Weights: The weights for board positions are predefined and do not adapt dynamically during the game.

Despite these limitations, the algorithm provides a competitive challenge for casual players.

How to Play

Compile the C program using a C compiler (e.g., GCC):

gcc -o reversi reversi.c

Run the program:

./reversi

Follow the on-screen instructions to choose the game mode and make your moves. When playing against the computer, the computer's moves will be displayed automatically.

Future Improvements

Implement a deeper minimax algorithm with alpha-beta pruning for better move decisions.

Introduce difficulty levels by varying the depth of the search or adjusting the heuristic weights.

Optimize the scoring function to account for more advanced strategies.
