/*
 * =====================================================================================
 *
 *       Filename:  FaultList.cc
 *
 *    Description:  This file contains the method definitions for the FaultList class.
 *                  This class stores the line number and the stuck at fault value
 *                  for that line.
 *
 *                  Stuck at value of true means the fault is stuck at 1 while value
 *                  false means the fault is stuck at 0.
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

#include "FaultList.h"

namespace nameSpace_ATPG {

/*
 *--------------------------------------------------------------------------------------
 *       Class:  FaultList
 *      Method:  FaultList :: FaultList
 * Description:  This constructor takes in the line number and the stuck at fault
 *               value and then creates an object for that line number - fault pair.
 *--------------------------------------------------------------------------------------
 */
FaultList::FaultList(int inLineNumber, bool inStuckAtValue) {
    this->lineNumber    = inLineNumber;
    this->stuckAtValue  = inStuckAtValue;
}

} /* namespace nameSpace_ATPG */

