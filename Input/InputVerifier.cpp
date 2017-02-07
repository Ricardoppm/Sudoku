//
//  InputVerifier.cpp
//  SudokuSolver
//
//  Created by Ricardo Martins on 06/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#include "InputVerifier.hpp"

InputVerifier::InputVerifier(){}

InputVerifier::Input InputVerifier::isValidInput( std::string input)
{
    if (input.compare("PLAY")==0){
        return INPUT_PLAY;
    }
    else if ( input.compare("SOLVE")==0){
        return INPUT_SOLVE;
    }
    else if ( input.compare("HELP")==0){
        return INPUT_HELP;
    }
    else if ( input.compare("SHOW")==0){
        return INPUT_SHOW;
    }
    else if ( input.compare("FILE")==0){
        return INPUT_FILE;
    }
    else{
        std::stringstream ss(input);
        
        bool hasFailed=false;
        char row, col, value;
        ss >> row;
        if(ss.fail() || (ss.peek()!=' ' && !ss.eof() && ss.peek()!='\t') )  hasFailed=true;
        ss >> col;
        if(ss.fail() || (ss.peek()!=' ' && !ss.eof() && ss.peek()!='\t') ) hasFailed=true;
        ss >> value;
        if(ss.fail() || (ss.peek()!=' ' && !ss.eof() && ss.peek()!='\t') ) hasFailed=true;
        
        std::string remaininput;
        ss >> remaininput;
        if( !hasFailed && remaininput.length()==0){
            
            if( row >= '1' && row <='9') row_ = row - '1';
            else return INPUT_NOK;
            
            if( col >= 'A' && col <='I') col_ = col - 'A';
            else return INPUT_NOK;
            
            if( value >= '1' && value <='9') value_ = value - '0';
            else return INPUT_NOK;

            std::cout << row_  << " " << col_ << " " << value_ << std::endl;
            return INPUT_SETUP;
        }
        else{
            return INPUT_NOK;
        }
    }
}

int InputVerifier::getRow(){ return row_; }
int InputVerifier::getCol(){ return col_; }
int InputVerifier::getValue(){ return value_; }
