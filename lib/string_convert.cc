/*
 * =====================================================================================
 *
 *       Filename:  string_convert.cc
 *
 *    Description:  This function takes in a string and then replaces all the 4 by X.
 *
 *        Version:  1.0
 *        Created:  12/07/2011
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aditya Shevade <aditya.shevade@gmail.com>,
 *                  Amey Marathe <marathe.amey@gmail.com>,
 *                  Samik Biswas <samiksb@gmail.com>,
 *                  Viraj Bhogle <viraj.bhogle@gmail.com>
 *
 * =====================================================================================
 */

using namespace std;


// #####   HEADER FILE INCLUDES   ###################################################  

// Global constant definitions.

#ifndef GLOBAL_DEFINES_H_
#include "../include/global_defines.h"
#endif

namespace nameSpace_ATPG {

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  StringConvert
 *  Description:  This function takes in a string and then replaces all the 4 by X.
 * =====================================================================================
 */

string StringConvert (string inString) {

    int positionOfX = inString.find_first_of('4');  // Find the first occurance of 4.
    while (positionOfX != string::npos)
    {
        inString[positionOfX] = 'X';                // Replace 4 by X since we are using #define X 4.
        positionOfX = inString.find_first_of('4',(positionOfX + 1));
    }
    
    return inString;

}

} /* nameSpace_ATPG */

