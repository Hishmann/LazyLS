# LazyLS

Terminal program for people who want a constant view into the files present in their current working directory of their terminal. It uses **ANSI VT100 escape sequences** for custom terminal rendering and **Tmux** for the split terminal linked to the main Terminal. 

The program is still currently in development and is close to completion.

#### Current Development Goals:

- [x] Figure out how to link the program with the main terminal.
- [x] Figure out how to extract cwd in code and print it as text 
- [x] Make a Basic Grid Element that takes a vector of strings as input. 
- [x] Link it with the current directory of another terminal
- [x] Figure out more rendering functions for file system view
- [x] Change to pixel-based rendering. 
- [x]  Add scroll ability and fix the other renders to work with new cell system. 
- [ ] Get collection of file names, render it, and handle exceptions properly.
- [ ] Search bar with reset button that updates current working directory.
- [ ] Bash script for installing dependency tmux and building the code.
- [ ] Code Documentation.
- [ ] Change input handling to  make it more testable.
- [ ] Generic container input for input handling.


