/*
 * =====================================================================================
 *
 *       Filename:  TestList.cc
 *
 *    Description:  This file contains the method definitions for the TestList class.
 *                  This class stores the information about the generated tests.
 *                  
 *                  It stores the line number, stuck at fault value, test vector
 *                  and a flag of whether the test is possible for this fault.
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

#include "TestList.h"

namespace nameSpace_ATPG {


/*
 *--------------------------------------------------------------------------------------
 *       Class:  TestList
 *      Method:  TestList :: TestList
 * Description:  If the test was generated for the given line number and stuck at fault
 *               pair, then that test vector is passed to this constructor. It creates
 *               an object and sets the test possible flag to true.
 *--------------------------------------------------------------------------------------
 */
TestList::TestList (int inLineNumber, bool inStuckAtValue, string inTestVector) {
    this->lineNumber = inLineNumber;
    this->stuckAtValue = inStuckAtValue;
    this->testVector = inTestVector;
    this->isTestPossible = true;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  TestList
 *      Method:  TestList :: TestList
 * Description:  There can be cases where no test vector can detect the fault at the
 *               given line. In that case, only the line number and fault pair is sent
 *               to the constructor and the test possible flag is set to false.
 *--------------------------------------------------------------------------------------
 */
TestList::TestList (int inLineNumber, bool inStuckAtValue) {
    this->lineNumber = inLineNumber;
    this->stuckAtValue = inStuckAtValue;
    this->testVector = "";
    this->isTestPossible = false;
}

} /* namespace nameSpace_ATPG */

