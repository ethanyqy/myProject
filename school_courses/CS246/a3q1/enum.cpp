//
//  enum.cpp
//  a3q1
//
//  Created by Ethan on 2019-06-18.
//  Copyright © 2019 Ethan. All rights reserved.
//

// Simple example of an enumerated struct.
// Discussion of what it is and why you might want to use one can be found at:
//   https://en.cppreference.com/w/cpp/language/enum
//   https://isocpp.org/wiki/faq/cpp11-language-types#enum-class

#include <iostream>
#include <string>

enum struct Colour {Red, Green, Blue};

/*int main() {
    std::string ColourString[] = { "Red", "Green", "Blue"};
    Colour myColour = Colour::Green;
    
    // Convert the enumerated class instance into an unsigned integer so can
    // index into the string array.
    std::cout << ColourString[(unsigned int) myColour] << std::endl;
    
    return 0;
}
*/
