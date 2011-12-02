/*
 * =====================================================================================
 *
 *       Filename:  CircuitLine.cc
 *
 *    Description:  This file defines the methods in the CircuitLine class.
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

#include "CircuitLine.h"

namespace nameSpace_ATPG {

/*
 *--------------------------------------------------------------------------------------
 *       Class:  CircuitLine
 *      Method:  CircuitLine :: CircuitLine
 * Description:  Constructor for CircuitLine class. It takes in the line number.
 *               Before doing any fault collapsing, all the faults are possible in the
 *               circuit so both stuck at 0 and 1 faults are set to true.
 *--------------------------------------------------------------------------------------
 */
CircuitLine::CircuitLine(int inLineNumber) {
    this->lineNumber = inLineNumber;
    this->isStuckAt_0 = true;
    this->isStuckAt_1 = true;

}

} /* namespace nameSpace_ATPG */

