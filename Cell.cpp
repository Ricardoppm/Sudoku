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





//  Setters
void Cell::setCoordinates(int row, int col)
{
    coords_.setRow(row);
    coords_.setCol(col);
}

void Cell::setValue(int value){ value_=value; }

//  Getters
Coordinates Cell::getCoordinates(){ return coords_; }
int Cell::getValue(){ return value_; }

