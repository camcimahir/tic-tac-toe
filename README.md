# Tic-Tac-Toe (Dear ImGui)

I built a Tic-Tac-Toe game with a playable AI that always plays second. The AI is built using Negamax with alpha-beta pruning. I am using Dear ImGui, C++ and the prebuilt boardgame building class system by Proffesor Graeme Devine.

## System & Tools
* **OS:** Windows 10/11
* **Language:** C++
* **Dependencies:** Dear ImGui, CMake

## Game
* **Modes:** Currently the game is played with an AI that makes random moves
* **GamePlay:** the game is played like a classic tic tac toe taking turns

## Design Process

### Board Architecture
* the game uses a class system that has (`BitHolder`) class for logic and (`Bit`) class for visuals.
* Initialized a dynamic 3x3 grid where every holder acts as a fixed "socket" for game pieces.
* Implemented a stateString for the AI implementation

### Win/Draw Detection
* I implemented a loop-up table using arrays to check each of the 8 winning triples possible in tic tac toe

### 4. AI
* I am using negamax algorithm with alpha-beta pruning for the AI
* The way my negamax algorithm works is that it recursively runs through every possible move all the way up to their end state and determine a mathematical value for that move (+1 for a win, -1 for a loss, and 0 for a tie). The best move from each call is determined and selected. the provided value from that simulated move is negated and for the next move the value is calculated in the perspective of the players turn so the function only uses the point value provided for the best move that the opponent can make.