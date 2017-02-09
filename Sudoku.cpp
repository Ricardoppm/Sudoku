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
        
        std::cout << "Viewing (" << coords.getRow() << "," << coords.getCol() << ") . missing " << cellToBeSolved.size() << std::endl;
        
        Cell::State action = board[coords.getRow()][coords.getCol()].getMode();
        board[coords.getRow()][coords.getCol()].setRemovedQueue();
        
        switch(action){
                
            case Cell::STATE_SOLVED:{
                int value = board[coords.getRow()][coords.getCol()].getValue();
                
                updateBoardState( coords.getRow(),coords.getCol(),value);
                break;
            }
            case Cell::STATE_ROW :
                verifyRowConditionals(coords.getRow());
                break;
                
            case Cell::STATE_COL:
                verifyColConditionals(coords.getCol());
                break;
                
            case Cell::STATE_NOK:
                break;
                
            case Cell::STATE_ROWCOL:
                verifyColConditionals(coords.getCol());
                verifyRowConditionals(coords.getRow());
                
                std::bitset<9> blockState = getBlockState(coords.getRow(),coords.getCol()); // View missing values from block
                
                for(int value=0; value <9; value++){ // analyse if remaining values have only one possibility
                    if( !blockState[value]){
                        Coordinates* cell = verifyIfOnlyPoss(coords.getRow()/3, coords.getCol()/3, value+1);
                        if( cell != nullptr){
                            board[cell->getRow()][cell->getCol()].setValue(value+1);
                            if( !board[cell->getRow()][cell->getCol()].isQueued()) cellToBeSolved.push( (*cell) );
                            board[cell->getRow()][cell->getCol()].setQueued(Cell::STATE_SOLVED);
                        }
                    }
                }
                
                break;
        }
        
        
        
     
            
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
                
                // Cells with just one possibility are queued, state updated
                if( board[i][j].getNumberPossibilities()==1){
                    board[i][j].fixValue();
                    if(!board[i][j].isQueued()  )cellToBeSolved.push( Coordinates(i,j));
                    board[i][j].setQueued(Cell::STATE_SOLVED);
                }
            }
            
            if(i%3==2 && j%3==2){ // When entering a block for the last time
                for(int value=0; value <9; value++){ // analyse if remaining values have only one possibility
                    if( !blockState[value]){
                        Coordinates* cell = verifyIfOnlyPoss(i/3, j/3, value+1);
                        if( cell != nullptr){
                            board[cell->getRow()][cell->getCol()].setValue(value+1);
                            if(!board[cell->getRow()][cell->getCol()].isQueued()  )cellToBeSolved.push( (*cell));
                            board[cell->getRow()][cell->getCol()].setQueued(Cell::STATE_SOLVED);

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
    std::vector< std::bitset<9> > existantValues;

    // create map and vec with existant bitsets
    for (int i=0; i<boardSize_; i++) {
        if( board[row][i].getValue()==0){
            if(values.find(board[row][i].getPossibleValues()) != values.end()){
                values[board[row][i].getPossibleValues()]++;
            }
            else{
                values[board[row][i].getPossibleValues()]=1;
                existantValues.push_back(board[row][i].getPossibleValues());
            }
        }
    }
    // verify if there is exclusive
    for (auto it : values){
        //std::cout << " " << it.first << ":" << it.second << std::endl;
        if( (9-it.first.count())== it.second ) std::cout << it.first << " is exclusive\n";
    }
    
    // Exclusive Groups with occurance == existantbits
    for (int i=0; i<boardSize_; i++) {
        std::bitset<9> exclusiveSet = board[row][i].getPossibleValues();
        if( values[exclusiveSet] == board[row][i].getNumberPossibilities()){
            std::cout << "found exclusive pair = ("<< row << "," << i << ")\n";
            std::bitset<9> exclusiveSetFlipped = exclusiveSet;
            exclusiveSetFlipped.flip();
            // found exclusive group
            for (int j=0; j<boardSize_; j++) {
                // update all remaining
                if( board[row][j].getValue()==0 && board[row][j].getPossibleValues()!= exclusiveSet){
                    std::bitset<9> previousSet = board[row][j].getPossibleValues();
                    board[row][j].setValuePossibility( previousSet |exclusiveSetFlipped);
                    previousSet ^=board[row][j].getPossibleValues();
                    if(board[row][j].getNumberPossibilities()==1){ // add is final
                        board[row][j].fixValue();
                        if(!board[row][j].isQueued()) cellToBeSolved.push( Coordinates(row,j));
                        board[row][j].setQueued(Cell::STATE_SOLVED);
                    }else if (previousSet.any()){ // add if narrowed possibilities
                        if(!board[row][j].isQueued()  )cellToBeSolved.push( Coordinates(row,j));
                        board[row][j].setQueued(Cell::STATE_COL);
                    }
                }
            }
        }
    }
    
    // Check is group of exclusive
    std::bitset<9>* result;
    if( (result=checkExclusiveGroup(existantValues))!=nullptr){
        std::bitset<9> toRemove = *result;
        toRemove.flip();
        for(int i=0; i<9; i++){
            bool valid = ((*result) | board[row][i].getPossibleValues()) == board[row][i].getPossibleValues(); // is part of the group
            if(!valid && board[row][i].getValue()==0){ // update remaining not from group
                std::bitset<9> previousSet = board[row][i].getPossibleValues();
                board[row][i].setValuePossibility(previousSet |toRemove);
                previousSet = previousSet^board[row][i].getPossibleValues();
                if(board[row][i].getNumberPossibilities()==1){
                    board[row][i].fixValue();
                    if(!board[row][i].isQueued()) cellToBeSolved.push( Coordinates(row,i));
                    board[row][i].setQueued(Cell::STATE_SOLVED);
                }else if ( previousSet.any()){
                    if(!board[row][i].isQueued()) cellToBeSolved.push( Coordinates(row,i));
                    board[row][i].setQueued(Cell::STATE_COL);
                }
                
            }
        }
    }
    
}

void Sudoku::verifyColConditionals(int col)
{
    std::unordered_map< std::bitset<9>, int> values;
    std::vector< std::bitset<9> > existantValues;
    for (int i=0; i<boardSize_; i++) {
        if( board[i][col].getValue()==0){
            if(values.find(board[i][col].getPossibleValues()) != values.end()){
                values[board[i][col].getPossibleValues()]++;
            }
            else{
                values[board[i][col].getPossibleValues()]=1;
                existantValues.push_back(board[i][col].getPossibleValues());
            }
        }
    }
    
    
    for (auto it : values){
        //std::cout << " " << it.first << ":" << it.second << std::endl;
        if( (9-it.first.count())== it.second ) std::cout << it.first << " is exclusive\n";
    }

    
    for (int i=0; i<boardSize_; i++) {
        std::bitset<9> exclusiveSet = board[i][col].getPossibleValues();
        if( values[exclusiveSet] == board[i][col].getNumberPossibilities()){
            std::cout << "found exclusive pair = ("<< i << "," << col << ")\n";
            std::bitset<9> exclusiveSetFlipped = exclusiveSet;
            exclusiveSetFlipped.flip();
            for (int j=0; j<boardSize_; j++) {
                if( board[j][col].getValue()==0 && board[j][col].getPossibleValues()!= exclusiveSet){
                    std::bitset<9> previousSet = board[j][col].getPossibleValues();
                    board[j][col].setValuePossibility( previousSet|exclusiveSetFlipped);
                    previousSet^=board[j][col].getPossibleValues();
                    if(board[j][col].getNumberPossibilities()==1){
                        board[j][col].fixValue();
                        if(!board[j][col].isQueued()) cellToBeSolved.push( Coordinates(j,col));
                        board[j][col].setQueued(Cell::STATE_SOLVED);
                    }else if (previousSet.any()){ // add if narrowed possibilities
                        if(!board[j][col].isQueued()  )cellToBeSolved.push( Coordinates(j,col));
                        board[j][col].setQueued(Cell::STATE_ROW);
                    }
                }
            }
        }
    }
    
    std::bitset<9>* result;
    if( (result=checkExclusiveGroup(existantValues))!=nullptr){
        std::bitset<9> toRemove = *result;
        toRemove.flip();
        for(int i=0; i<9; i++){
            bool valid = ((*result) | board[i][col].getPossibleValues()) == board[i][col].getPossibleValues();
            if(!valid && board[i][col].getValue()==0){
                std::bitset<9> previousSet = board[i][col].getPossibleValues();
                board[i][col].setValuePossibility( previousSet|toRemove);
                previousSet ^= board[i][col].getPossibleValues();
                if(board[i][col].getNumberPossibilities()==1){
                    board[i][col].fixValue();
                    if(!board[i][col].isQueued()) cellToBeSolved.push( Coordinates(i,col));
                    board[i][col].setQueued(Cell::STATE_SOLVED);
                }else if( previousSet.any()){
                    if(!board[i][col].isQueued()) cellToBeSolved.push( Coordinates(i,col));
                    board[i][col].setQueued(Cell::STATE_ROW);
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
            if(board[row][i].setValuePossibility(value, true) ){
                if( !board[row][i].isQueued() )  cellToBeSolved.push( Coordinates(row,i));
                board[row][i].setQueued(Cell::STATE_ROWCOL);
            }
            if( board[row][i].getNumberPossibilities()==1 && !board[row][i].isQueued()){
                if( !board[row][i].isQueued() )cellToBeSolved.push( Coordinates(row,i));
                board[row][i].setQueued(Cell::STATE_SOLVED);
            }
        }
    }
    
    for(int i=0; i< boardSize_; i++){
        if( board[i][col].getValue()==0){
            if(board[i][col].setValuePossibility(value, true)){
                if( !board[i][col].isQueued() )  cellToBeSolved.push( Coordinates(i,col));
                board[i][col].setQueued(Cell::STATE_ROWCOL);
            }
            if( board[i][col].getNumberPossibilities()==1){
                if( !board[i][col].isQueued() )  cellToBeSolved.push( Coordinates(i,col));
                board[i][col].setQueued(Cell::STATE_SOLVED);
            }
        }
    }
    
    for(int i = (row/3)*3; i < (((row/3)+1)*3); i++){
        for( int j = (col/3)*3;  j < ((col/3)+1)*3; j++){
            if( board[i][j].getValue()==0 ){
                if(board[i][j].setValuePossibility(value, true)){
                    if( !board[i][j].isQueued()) cellToBeSolved.push( Coordinates(i,j));
                    board[i][j].setQueued(Cell::STATE_ROWCOL);
                    
                }
                if( board[i][j].getNumberPossibilities()==1){
                    if( !board[i][j].isQueued()) cellToBeSolved.push( Coordinates(i,j));
                    board[i][j].setQueued(Cell::STATE_SOLVED);
                }
            }
        }
    }
    
    
    std::bitset<9> blockState = getBlockState(row,col); // View missing values from block
    
    for(int value=0; value <9; value++){ // analyse if remaining values have only one possibility
        if( !blockState[value]){
            Coordinates* cell = verifyIfOnlyPoss(row/3, col/3, value+1);
            if( cell != nullptr){
                board[cell->getRow()][cell->getCol()].setValue(value+1);
                if( !board[cell->getRow()][cell->getCol()].isQueued()) cellToBeSolved.push( (*cell) );
                board[cell->getRow()][cell->getCol()].setQueued(Cell::STATE_SOLVED);
            }
        }
    }
}

std::bitset<9>* Sudoku::checkExclusiveGroup( std::vector< std::bitset<9> > vec){
    
    std::vector<bool> v(vec.size());
    
    
    for(int i=3 ; i < vec.size(); i++){
        std::fill(v.begin(), v.begin() + i, true);
        
        do {
            
            std::bitset<9> total( std::string("000000000"));
            for (int i = 0; i < vec.size(); ++i) {
                if (v[i]) {
                    std::bitset<9> aux = vec[i];
                    total = total| (aux.flip());
                }
            }
            if( total.count()==i){
                std::cout << "Group Exclusive -> " << total << std::endl;
                std::bitset<9> affected( std::string("000000000"));
                for (int i = 0; i < vec.size(); ++i) {
                    if( v[i]) affected.flip(i);
                }
                return new std::bitset<9>(total.flip());
            }
            
        } while (std::prev_permutation(v.begin(), v.end()));
        
    }
    return nullptr;
}




