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
    std::cout << HORIZONTAL_AXIS << std::endl;
    std::cout << EXTERIOR_BOUND << std::endl;
    
    int cellvalue;
    for(int i = 0; i < boardSize_; i++ ){
        std::cout << " " << (i+1) << " |";
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

bool Sudoku::insertCellValue(int row, int col, int value)
{
    if(!isValidInsertion(row, col, value)) return false;
    board[row][col].setValue(value);
    return true;
}

bool Sudoku::isValidInsertion(int row, int col, int value)
{
    for(int i=0; i< boardSize_; i++){
        if( board[row][i].getValue()==value) return false;
    }
    
    for(int i=0; i< boardSize_; i++){
        if( board[i][col].getValue()==value) return false;
    }
    
    for(int i = (row/3)*3; i < (((row/3)+1)*3); i++){
        for( int j = (col/3)*3;  j < ((col/3)+1)*3; j++){
            if( board[i][j].getValue()==value) return false;
        }
    }
    return true;
    
}



