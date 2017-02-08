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

void Sudoku::displayBoard() const
{
    std::cout << HORIZONTAL_AXIS << std::endl;
    std::cout << EXTERIOR_BOUND << std::endl;
    
    int cellvalue;
    for(int i = 0; i < boardSize_; i++ ){
        std::cout << " " << (i+1) << " |";
        for(int j = 0; j < boardSize_; j++){
            cellvalue = board[i][j].getValue();
            if( cellvalue == 0){
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

bool Sudoku::isValidInsertion(int row, int col, int value) const
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

bool Sudoku::solveSudoku()
{
    computeInitialBoardState();
    
    while (!cellToBeSolved.empty())
    {
        Coordinates coords = cellToBeSolved.front();
        cellToBeSolved.pop();
        
        std::cout << "Added (" << coords.getRow() << "," << coords.getCol() << ") . missing " << cellToBeSolved.size() << std::endl;
        
        int value = board[coords.getRow()][coords.getCol()].getValue();
        
        updateBoardState( coords.getRow(),coords.getCol(),value);
            
        displayBoard();
    }
    

    return true;
}


bool Sudoku::computeInitialBoardState(){

    // Compute the bitset possibilities for each row
    std::vector<std::bitset<9>> horizontalStates;
    for(int i=0; i<boardSize_; i++) horizontalStates.push_back( getHorizontalState(i));
    
    //Compute the bitset possibilities for each column and
    std::vector<std::bitset<9>> verticalStates;
    for(int i=0; i<boardSize_; i++){ verticalStates.push_back(getVerticalState(i)); }
    
    
    //Compute the bitset possibilities for each Block
    for(int i=0; i< boardSize_; i++){
        std::bitset<9> blockState;
        for(int j=0; j < boardSize_; j++){
            if(j%3==0) blockState = getBlockState(i,j); // When entering a block(3x3) compute bitset
            
            if( board[i][j].getValue()==0){ //Only compute bitset for unsolved cells
                
                std::bitset<9> finalState = ( horizontalStates[i] | verticalStates[j] | blockState);
                board[i][j].setValuePossibility(finalState);
                
                // Cells with just one possibility are queued to be solved
                if( board[i][j].getNumberPossibilities()==1){
                    board[i][j].fixValue();
                    cellToBeSolved.push( Coordinates(i,j));
                }
            }
            
            if(i%3==2 && j%3==2){ // When entering a block for the last time
                for(int value=0; value <9; value++){ // analyse if remaining values have only one possibility
                    if( !blockState[value]){
                        Coordinates* cell = verifyIfOnlyPoss(i/3, j/3, value+1);
                        if( cell != nullptr){
                            board[cell->getRow()][cell->getCol()].setValue(value+1);
                            cellToBeSolved.push( (*cell) );
                        }
                    }
                }
            }
            
        }
    }
    
    for(int i=0; i<boardSize_; i++){
        verifyRowConditionals(i);
        verifyColConditionals(i);
    }
    
    return true;
}

void Sudoku::verifyRowConditionals(int row)
{
    std::unordered_map< std::bitset<9>, int> values;
    
    for (int i=0; i<boardSize_; i++) {
        if( board[row][i].getValue()==0){
            if(values.find(board[row][i].getPossibleValues()) != values.end()){
                values[board[row][i].getPossibleValues()]++;
            }
            else{
                values[board[row][i].getPossibleValues()]=1;
            }
        }
    }
    
    for (int i=0; i<boardSize_; i++) {
        std::bitset<9> exclusiveSet = board[row][i].getPossibleValues();
        if( values[exclusiveSet] == board[row][i].getNumberPossibilities()){
            std::cout << "found exclusive pair = "<< exclusiveSet << std::endl;
            exclusiveSet.flip();
            for (int j=0; j<boardSize_; j++) {
                if( board[row][j].getValue()==0 && board[row][j].getPossibleValues()!= exclusiveSet){
                    board[row][j].setValuePossibility( board[row][j].getPossibleValues()|exclusiveSet);
                    if(board[row][j].getNumberPossibilities()==1){
                        cellToBeSolved.push( Coordinates(row,j));
                    }
                }
            }
        }
    }
}

void Sudoku::verifyColConditionals(int col)
{
    std::unordered_map< std::bitset<9>, int> values;
    
    for (int i=0; i<boardSize_; i++) {
        if( board[i][col].getValue()==0){
            if(values.find(board[i][col].getPossibleValues()) != values.end()){
                values[board[i][col].getPossibleValues()]++;
            }
            else{
                values[board[i][col].getPossibleValues()]=1;
            }
        }
    }
    
    for (int i=0; i<boardSize_; i++) {
        std::bitset<9> exclusiveSet = board[i][col].getPossibleValues();
        if( values[exclusiveSet] == board[i][col].getNumberPossibilities()){
            std::cout << "found exclusive pair = "<< exclusiveSet << std::endl;
            exclusiveSet.flip();
            for (int j=0; j<boardSize_; j++) {
                if( board[j][col].getValue()==0 && board[j][col].getPossibleValues()!= exclusiveSet){
                    board[j][col].setValuePossibility( board[j][col].getPossibleValues()|exclusiveSet);
                    if(board[j][col].getNumberPossibilities()==1){
                        cellToBeSolved.push( Coordinates(j,col));
                    }
                }
            }
        }
    }
}



std::bitset<9> Sudoku::getHorizontalState(int row) const
{
    std::bitset<9> lineState;
    int cellValue;
    for(int i=0; i< boardSize_; i++){
        cellValue = board[row][i].getValue();
        if( cellValue !=0) lineState[cellValue-1]=true;
    }
    return lineState;
}

std::bitset<9> Sudoku::getVerticalState(int col) const
{
    std::bitset<9> lineState;
    int cellValue;
    for(int i=0; i< boardSize_; i++){
        cellValue = board[i][col].getValue();
        if( cellValue !=0) lineState[cellValue-1]=true;
    }
    return lineState;
}

std::bitset<9> Sudoku::getBlockState(int row, int col) const
{
    std::bitset<9> lineState;
    int cellValue;
    for(int i = (row/3)*3; i < (((row/3)+1)*3); i++){
        for( int j = (col/3)*3;  j < ((col/3)+1)*3; j++){
            cellValue = board[i][j].getValue();
            if( cellValue!=0) lineState[cellValue-1]=true;
        }
    }
    return lineState;
}


Coordinates* Sudoku::verifyIfOnlyPoss(int blockRow, int blockCol, int value) const
{
    int row=-1,col=-1;
    int val_counter=0;
    for(int i= blockRow*3; i< (blockRow+1)*3; i++){
        for(int j= blockCol*3; j< (blockCol+1)*3; j++){
            if(!board[i][j].getValuePossibility(value) && board[i][j].getValue()==0){
                val_counter++;
                row = i;
                col = j;
            }
        }
    }
    if( val_counter==1) return new Coordinates(row,col);
    else return nullptr;
}



void Sudoku::updateBoardState(int row, int col, int value)
{
    for(int i=0; i< boardSize_; i++){
        if( board[row][i].getValue()==0){
            board[row][i].setValuePossibility(value, true);
            if( board[row][i].getNumberPossibilities()==1) cellToBeSolved.push( Coordinates(row,i));
        }
    }
    
    for(int i=0; i< boardSize_; i++){
        if( board[i][col].getValue()==0){
            board[i][col].setValuePossibility(value, true);
            if( board[i][col].getNumberPossibilities()==1) cellToBeSolved.push( Coordinates(i,col));
        }
    }
    
    for(int i = (row/3)*3; i < (((row/3)+1)*3); i++){
        for( int j = (col/3)*3;  j < ((col/3)+1)*3; j++){
            if( board[i][j].getValue()==0 ){
                board[i][j].setValuePossibility(value, true);
                if( board[i][j].getNumberPossibilities()==1) cellToBeSolved.push( Coordinates(i,j));
            }
        }
    }
    
    
    std::bitset<9> blockState = getBlockState(row,col); // View missing values from block
    
    for(int value=0; value <9; value++){ // analyse if remaining values have only one possibility
        if( !blockState[value]){
            Coordinates* cell = verifyIfOnlyPoss(row/3, col/3, value+1);
            if( cell != nullptr){
                board[cell->getRow()][cell->getCol()].setValue(value+1);
                cellToBeSolved.push( (*cell) );
            }
        }
    }
}






