/*
 * =====================================================================================
 *
 *       Filename:  radix_convert.cc
 *
 *    Description:  This function converts a decimal number to any base N number.
 *                  Possible values of N are from 2 to 35.
 *                  For our project, we only need a base 3 number 0,1 and X.
 *
 *        Version:  1.0
 *        Created:  12/02/2011
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
 *         Name:  RadixConvert
 *  Description:  This function takes in an integer value in base 10, the base you want
 *                to convert that number into and a character array and returns it.
 *
 *                We have changed the name from itoa to RadixConvert.
 * =====================================================================================
 */

/*
 * C++ version 0.4 std::string style "itoa":
 * Contributions from Stuart Lowe, Ray-Yuan Sheu,
 * Rodrigo de Salvo Braz, Luc Gallant, John Maloney
 * and Brian Hunt
 */
string RadixConvert (int value, int base) {

    string buf;

    // Check that the base if valid
    if (base < 2 || base > 16)
        return buf;

    enum { kMaxDigits = 35 };
    buf.reserve( kMaxDigits );  // Pre-allocate enough space.

    int quotient = value;

    // Translating number to string with base:
    do {
        buf += "0123456789abcdef"[ std::abs( quotient % base ) ];
        quotient /= base;
    } while ( quotient );

    // Append the negative sign
    if ( value < 0) buf += '-';

    std::reverse( buf.begin(), buf.end() );
    return buf;
}

} /* nameSpace_ATPG */

