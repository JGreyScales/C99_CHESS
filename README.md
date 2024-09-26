A simple terminal based chess game made in C99
Only uses String.h and Stdio.h


Although probably not the best decision if I was to redo in just plain C and 100% not if I redid in C++, I choose to represent the board as a char array and that led to some interesting design choices



- [x] The game 'works'
- [x] The game does not crash
- [x] Full player movement & attacking & path detection
- [ ] Checking (forcing enemies to react properly to checks
- [ ] Checkmates & game over (most likely wont be implimented)
- [ ] Backing out of a piece selection (Currently, if you select a piece you are stuck with that piece)
