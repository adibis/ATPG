/*
 * =====================================================================================
 *
 *       Filename:  CircuitLine.h
 *
 *    Description:  The CircuitLine class. This class is used to store the information
 *                  about the faults on a circuit line. It has the line number and
 *                  stuck at 0 and stuck at 1 faults as members.
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

#ifndef CIRCUITLINE_H_
#define CIRCUITLINE_H_

namespace nameSpace_ATPG {


/*
 * =====================================================================================
 *        Class:  CircuitLine
 *  Description:  Every line in the circuit can have a stuck at 0 or a stuck at 1 fault.
 *                Objects of this class store the line number, which is unique, as the
 *                object ID and then the fault values associated with them.
 * =====================================================================================
 */
class CircuitLine {
    public:
        unsigned    int     lineNumber;
                    bool    isStuckAt_0;
                    bool    isStuckAt_1;
        /*
         *--------------------------------------------------------------------------------------
         *       Class:  CircuitLine
         *      Method:  CircuitLine :: CircuitLine
         * Description:  Constructor for CircuitLine class. It takes in the line number.
         *               Before doing any fault collapsing, all the faults are possible in the
         *               circuit so both stuck at 0 and 1 faults are set to true.
         *--------------------------------------------------------------------------------------
         */
        CircuitLine (int inLineNumber);

};

} /* namespace nameSpace_ATPG */
#endif /* CIRCUITLINE_H_ */
