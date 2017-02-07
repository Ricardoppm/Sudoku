//
//  Sudoku.hpp
//  SudokuSolver
//
//  Created by Ricardo Martins on 04/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#ifndef Sudoku_hpp
#define Sudoku_hpp

#include <iostream>
#include <fstream>
#include <string>

#include "Cell.hpp"

#define HORIZONTAL_AXIS "     A   B   C   D   E   F   G   H   I  "
#define EXTERIOR_BOUND  "   +---+---+---+---+---+---+---+---+---+"

class Sudoku
{
    public:
        Sudoku(int boardsize);
    
        virtual void displayBoard();
        bool insertCellValue(int row, int col, int value);
    
    protected:
        Cell board[9][9];
        virtual void createBoard();
    
    private:
        int boardSize_;
        bool isValidInsertion(int row, int col, int value);
    
    
};

#endif /* Sudoku_hpp */
