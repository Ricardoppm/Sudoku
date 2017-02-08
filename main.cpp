//
//  main.cpp
//  SudokuSolver
//
//  Created by Ricardo Martins on 04/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#include <iostream>

#include "Sudoku.hpp"
#include "InputVerifier.hpp"


#define INPUT_INFORMATION "Insert command. HELP for command information:"

#define HELP "Existant Commands:\nInsert Cell: [1-9] [A-I] [value: 1-9] \nPlay the game: PLAY\nView solution: SOLVE\n Show board: SHOW\n"

// FILE is = /Users/ricardomartins/Desktop/Projects/C++/SudokuSolver/Puzzles/Easy/sudoku1.txt


// FILE is = /Users/ricardomartins/Desktop/Projects/C++/SudokuSolver/Puzzles/Medium/medium1.txt

bool buildFromFile( InputVerifier inputVerifier, Sudoku &sudoku)
{
    std::cout << "Please insert filename\n";
    std::string filename;
    std::cin >> filename;
    std::ifstream f_input;
    f_input.open(filename);
    if (!f_input.is_open()) return false;
    
    std::string line;
    while ( std::getline(f_input,line) ) {
        if( inputVerifier.isValidInput(line) == InputVerifier::INPUT_SETUP ){
            if(!sudoku.insertCellValue(inputVerifier.getRow(), inputVerifier.getCol(), inputVerifier.getValue())){
                return false;
            }
        }
    }
    f_input.close();
    return true;
}


int main(int argc, const char * argv[]) {
    
    Sudoku sudokuGame(9);
    InputVerifier inputVerifier;
    
    std::cout << " Welcome to Sudoku solver!\n";
    
    sudokuGame.displayBoard();
    
    
    bool isComplete=false;
    std::string input;
    while(!isComplete){
        std::cout << std::endl << INPUT_INFORMATION << std::endl;
        
        std::getline(std::cin, input);
        
        switch(inputVerifier.isValidInput(input)){
            case InputVerifier::INPUT_SETUP :
                std::cout << "Setup input\n";
                if(!sudokuGame.insertCellValue(inputVerifier.getRow(), inputVerifier.getCol(), inputVerifier.getValue())){
                    std::cout << "Impossible input\n";
                }
                break;
                
            case InputVerifier::INPUT_PLAY :
                std::cout << "PLAY input\n";
                break;
                
            case InputVerifier::INPUT_SOLVE :
                std::cout << "SOLVE input\n";
                sudokuGame.solveSudoku();
                break;
                
            case InputVerifier::INPUT_NOK :
                std::cout << "Invalid command\n";
                //std::cout << HELP << std::endl;
                break;
                
            case InputVerifier::INPUT_SHOW :
                sudokuGame.displayBoard();
                break;
                
            case InputVerifier::INPUT_HELP :
                std::cout << HELP << std::endl;
                break;
                
            case InputVerifier::INPUT_FILE:
                if(buildFromFile(inputVerifier, sudokuGame)) std::cout << "Input successful\n";
                else std::cout << "Input from file failed\n";
                break;
                
        }
    }
    
    
    return 0;
}
