# **Checkers-Algorithm**

Checkers, or draughts, is a strategy board game played by two players. There are many variants of checkers. Your task is to implement a program that reads, prints, and plays our variant of the game.

![Screen Shot 2024-06-22 at 01 11 53](https://github.com/clarissaso/Checkers-Algorithm/assets/80309625/02197e57-55e9-453a-af0a-fbe80c4547a5)

Figure 1: Example board configurations, moves, and captures.

**Setup.** An 8x8 chessboard with 12 black and 12 white pieces initially positioned as shown in Figure 1a.

**Gameplay.** Each player plays all pieces of the same color. Black open the game by making a move, then white make a move, and then players alternate their turns. In a single turn, the player either makes a move or capture. For example, the arrow in Figure 1a denotes an opening move of the black piece from cell G6 to cell F5.

**Moves.** A piece may move to an empty cell diagonally forward (toward the opponent; north for black and south for white) one square. The arrows in Figure 1b show all the legal moves of black and white pieces.
Towers. When a piece reaches the furthest row (the top row for black and the bottom row for white), it becomes a tower (a pile of pieces). The only move of the white piece at cell D7 in Figure 1b promotes it to the tower. A tower may move to an empty cell diagonally, both forward and backward, one square. The arrows in Figure 1c show all the legal moves of black and white towers.

**Captures.** To capture a piece or a tower of the opponent, a piece or a tower of the player jumps over it and lands in a straight diagonal line on the other side. This landing cell must be empty. When a piece or tower is captured, it is removed from the board. Only one piece or tower may be captured in a single jump, and, in our variant of the game, only one jump is allowed in a single turn. Hence, if another capture is available after the first jump, it cannot be taken in this turn. Also, in our variant of the game, if a player can make a move or a capture, they may decide which of the two to complete. A piece always jumps forward (toward the opponent), while a tower can jump forward and backward. The arrows in Figure 1d show all the legal captures for both players.

**Game end.** A player wins the game if it is the opponent’s turn and they cannot take action, move or capture, either because no their pieces and towers are left on the board or because no legal move or capture is possible.
