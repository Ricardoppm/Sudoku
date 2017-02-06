//
//  Sudoku.cpp
//  SudokuSolver
//
//  Created by Ricardo Martins on 04/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#include "Sudoku.hpp"

Sudoku::Sudoku(int boardsize){
    boardSize_ = boardsize;
    createBoard();
};

void Sudoku::createBoard()
{
    for(int i = 0; i < boardSize_; i++ ){
        for(int j = 0; j < boardSize_; j++){
            board[i][j].setCoordinates(i, j);
        }
    }
}

void Sudoku::displayBoard()
{
    std::cout << EXTERIOR_BOUND << std::endl;
    
    int cellvalue;
    for(int i = 0; i < boardSize_; i++ ){
        std::cout << "|";
        for(int j = 0; j < boardSize_; j++){
            cellvalue = board[i][j].getValue();
            if( cellvalue == -1){
                std::cout << "   |";
            }
            else{
                std::cout << " " << cellvalue << " |";
            }
        }
        std::cout << std::endl;
        std::cout << EXTERIOR_BOUND << std::endl;

    }
    
}
