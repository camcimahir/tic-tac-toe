# Tic-Tac-Toe (Dear ImGui)

I built a Tic-Tac-Toe game using Dear ImGui, C++ and the prebuilt boardgame building class system by Proffesor Graeme Devine.

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
* current AI randomly selects one of the empty squares and places its piece there