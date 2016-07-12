# Modified-Dynamic-Connect-4
Implementation of a game-playing agent to play a modified version of connect-4 against a human.

This modified version of connect-4 is played on a 7x7 grid as follows: Starting from an initial position,
players take turns moving one piece of their colour by a number of squares as explained below, either horizontally or vertically.
(Clarification: No jumping is allowed.) White plays first.

Pieces can only move to unoccupied squares. The winner is the first player to form a 2x2 square of four
pieces. In addition, pieces create an "impedance" field in the immediate (8-squares) neighbourhood
around them, affecting the number of squares an opponent piece can move. The following table
describes the number of squares a piece can move given the number of opponent pieces in its
neighbourhood:

Number of opponent pieces in the        Number of squares the piece can move
surrounding 8 squares
0                                        [1-3]
1                                        [1-2]
2                                         1
3 or more                                 0 (pinned)

The game-playing agent plays the game of dynamic connect-4 against a human.
It is capable of playing either white or black.
The time limit for the computer to display its move is 20 seconds.
The interface is a simple text-only system, which displays the current board state as a matrix of comma-separated characters, with O denoting a white piece, X denoting a black piece, and suitably formatted whitespace denoting an empty square.

Each square in the grid can be labeled by its position, (e.g., <2,1> corresponds to column 2, row 1)
Moves are entered by specifying the position of the piece to move in the
form followed by one of the compass directions, N, S, E, W; and the number of squares to move. 

For example, to move the black piece at the top left of the board one square to the left, the command
would be 21W1. Each time it performs a move, the agent displays the associated command in the terminal.

Game states are evaluated as being either a win (+1), a loss (-1), or a neutral state (0).
It also has an option for specifying the initial game state.

The implementatino uses the minimax and alpha-beta algorithms.
It also implements a useful evaluation function for non-terminal nodes.
This evaluation function plays well when confronted with non-terminal nodes, as well as:
-when victory is certain, it tries to win as soon as possible
-when defeat is certain, it plays to delay its defeat as much as possible
