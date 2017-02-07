//
//  InputVerifier.hpp
//  SudokuSolver
//
//  Created by Ricardo Martins on 06/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#ifndef InputVerifier_hpp
#define InputVerifier_hpp

#include <iostream>
#include <sstream>

class InputVerifier
{
    public:
        enum Input { INPUT_PLAY, INPUT_SOLVE, INPUT_SETUP, INPUT_NOK, INPUT_HELP, INPUT_SHOW, INPUT_FILE };

        InputVerifier();
    
        virtual Input isValidInput( std::string input);
        int getRow();
        int getCol();
        int getValue();
    
    private:
        Input input;
        int row_, col_, value_;
};


#endif /* InputVerifier_hpp */
