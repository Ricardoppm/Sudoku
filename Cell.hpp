//
//  Cell.hpp
//  SudokuSolver
//
//  Created by Ricardo Martins on 04/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#ifndef Cell_hpp
#define Cell_hpp

#include <stdio.h>
#include "Coordinates.hpp"

class Cell
{

    public:
        Cell();
        Cell(int row, int col, int value);
    
        void setCoordinates(int row, int col);
        void setValue(int value);
    
        Coordinates getCoordinates();
        int getValue();
    
    
    private:
        Coordinates coords_;
        int value_ = -1;

};


#endif /* Cell_hpp */
