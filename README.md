**Project Description** 
This project uses two data structures, Trie and Ternary Search Tree (TST), to impliment an English utocomplete program. 
The project uses SFML to create a graphical user interface (GUI), which allows the user to input a prefix
and the program will return a list of 10 autocomplete suggested words. In the GUI users can click whether they want to
use the Trie or TST to complete there search as well as a comparison button to compare performance of the two data structures,
looking at execution time, node count, and estimated memory usage.

**Requirements to Run Program**
-C++17-compatible compiler
-CMake 3.28 or newer
-SFML 3.1 with the Graphics component
-An IDE or terminal
Note: SFML must be installed and accessible to CMake.

**How to Run**
-Clone repository
-Configure Project
    open terminal in project directory and run: cmake -S . -B build
-Compile Program
    to compile the C++ source file and generate the executible run: cmake --build build
-Run Program
    On Windows, from the project root directory, run: .\build\AutocompleteGUI.exe
    NOTE: Run the executable from the project root directory so the program can locate the font and dictionary files using their relative paths.

**Project Files**
-main.cpp – Main entry point for the program.
-GUI.cpp / GUI.h – Graphical user interface implementation.
-trie.cpp / trie.h – Trie implementation.
-TernarySearchTree.cpp / TernarySearchTree.h – Ternary Search Tree implementation.
-dictionary.cpp / dictionary.h – Dictionary loading and word processing.
-CMakeLists.txt – CMake build configuration.
-data/ – English word frequency dataset.
-resources/fonts/ – Font used by the graphical interface.