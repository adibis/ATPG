/*
 * =====================================================================================
 *
 *       Filename:  FaultList.h
 *
 *    Description:  This file contains the FaultList class.
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

#ifndef FAULTLIST_H_
#define FAULTLIST_H_

namespace nameSpace_ATPG {


/*
 * =====================================================================================
 *        Class:  FaultList
 *  Description:  This class stores the line number and the fault that exists at that 
 *                line number.
 *
 *                StuckAtValue = false means a stuck at 0 fault exists.
 *                StuckAtValue = true means a stuck at 1 fault exists.
 *
 *                An object of this class is created only if the line has fault.
 * =====================================================================================
 */
class FaultList {
    public:
        unsigned    int     lineNumber;
                    bool    stuckAtValue;

        /*
         *--------------------------------------------------------------------------------------
         *       Class:  FaultList
         *      Method:  FaultList :: FaultList
         * Description:  This constructor takes in the line number and the stuck at fault
         *               value and then creates an object for that line number - fault pair.
         *--------------------------------------------------------------------------------------
         */
        FaultList (int inLineNumber, bool inStuckAtValue);
};

} /* namespace nameSpace_ATPG */
#endif /* FAULTLIST_H_ */

