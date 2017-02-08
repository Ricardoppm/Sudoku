//
//  Cell.hpp
//  SudokuSolver
//
//  Created by Ricardo Martins on 04/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#ifndef Cell_hpp
#define Cell_hpp

#include <iostream>
#include <bitset>

#include "Coordinates.hpp"

class Cell
{

    public:
        Cell();
        Cell(int row, int col, int value);
    
        int fixValue();
    
        void setCoordinates(int row, int col);
        void setValue(int value);
        void setValuePossibility(int value, bool valid);
        void setValuePossibility(std::bitset<9> possibleValues);

        Coordinates getCoordinates() const;
        int getValue() const;
        bool getValuePossibility(int value) const;
        std::size_t getNumberPossibilities() const;
        std::bitset<9> getPossibleValues() const;
    
    private:
        Coordinates coords_;
        int value_ = 0;
        std::bitset<9> possibleValues_;

};


#endif /* Cell_hpp */
