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
    
    std::cout << " Insert initial board state: [1-9][A-I] [value] or PLAY when done\n";
    
    bool isComplete=false;
    std::string input;
    while(isComplete){
        std::getline(std::cin, input);
        
        if ( input.compare("PLAY")==0) {
            isComplete = true;
        }
        else{
            
        }
    }
    
    
    return 0;
}
