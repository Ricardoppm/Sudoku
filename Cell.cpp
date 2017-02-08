//
//  Cell.cpp
//  SudokuSolver
//
//  Created by Ricardo Martins on 04/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#include "Cell.hpp"

// Constructors
Cell::Cell(){ }

Cell::Cell(int row, int col, int value)
{
    coords_ = Coordinates(row, col);
    value_ = value;
}


int Cell::fixValue()
{
    if( possibleValues_.count()!=8) return false;
    for(int i=0; i<9; i++){
        if(possibleValues_[i]==0){
            value_ = i+1;
            return value_;
        }
    }
    return -1;
}


//  Setters
void Cell::setCoordinates(int row, int col)
{
    coords_.setRow(row);
    coords_.setCol(col);
}

void Cell::setValue(int value){ value_=value; }

void Cell::setValuePossibility(int value, bool valid) { possibleValues_[value-1]=valid; }

void Cell::setValuePossibility(std::bitset<9> possibleValues) { possibleValues_=possibleValues; }


//  Getters
Coordinates Cell::getCoordinates() const { return coords_; }
int Cell::getValue() const { return value_; }
bool Cell::getValuePossibility(int value) const { return possibleValues_[value-1]; }
std::size_t Cell::getNumberPossibilities() const { return 9-possibleValues_.count(); }
std::bitset<9> Cell::getPossibleValues() const { return possibleValues_; }


