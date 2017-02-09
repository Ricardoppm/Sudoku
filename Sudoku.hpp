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
#include <bitset>
#include <vector>
#include <queue>
#include <unordered_map>
#include <array>


#include "Cell.hpp"

#define HORIZONTAL_AXIS "     A   B   C   D   E   F   G   H   I  "
#define EXTERIOR_BOUND  "   +---+---+---+---+---+---+---+---+---+"

class Sudoku
{
    public:
        Sudoku(int boardsize);
    
        virtual void displayBoard() const;
        virtual void displaySolution() const;

        bool insertCellValue(int row, int col, int value);
        virtual bool solveSudoku();
    protected:
        Cell board[9][9];
        Cell solution[9][9];

        virtual void createBoard();
    
        virtual bool computeInitialBoardState();
        void verifyRowConditionals(int row);
        void verifyColConditionals(int col);

        virtual void updateBoardState( int row, int col, int value);
    
    private:
        int boardSize_;
        std::queue< Coordinates > cellToBeSolved;
    
        bool isValidInsertion(int row, int col, int value) const;
        Coordinates* verifyIfOnlyPoss(int row, int col, int value) const;

        std::bitset<9>* checkExclusiveGroup( std::vector< std::bitset<9> > vec);
    
        void searchAlgorithm();
        bool searchRecursive(std::array<Cell, 81> curr_board, int remain, bool first , Coordinates coords);

        std::bitset<9> getHorizontalState(int row) const;
        std::bitset<9> getVerticalState(int col) const;
        std::bitset<9> getBlockState(int row, int col) const;


};

#endif /* Sudoku_hpp */
