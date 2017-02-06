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

class InputVerifier
{
    public:
        InputVerifier();
    
        virtual bool isValidInput( std::string input);
};


#endif /* InputVerifier_hpp */
