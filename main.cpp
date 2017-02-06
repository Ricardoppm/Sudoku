//
//  main.cpp
//  SudokuSolver
//
//  Created by Ricardo Martins on 04/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#include <iostream>

#include "Sudoku.hpp"

int main(int argc, const char * argv[]) {
    
    Sudoku sudokuGame(9);
    
    std::cout << " Welcome to Sudoku solver!\n";
    
    sudokuGame.displayBoard();
    
    return 0;
}
