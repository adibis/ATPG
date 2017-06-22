/*
 * =====================================================================================
 *
 *       Filename:  forward_implication.cc
 *
 *    Description:  This function performs forward implication on a 1-3 input gate.
 *                  The inputs and output are integer sets. It reads the values from
 *                  logic tables.
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

// This file contains the tables which are looked up for determinig the output.

#ifndef LOGIC_TABLES_H_
#include "../include/logic_tables.h"
#endif

// Global constant definitions.

#ifndef GLOBAL_DEFINES_H_
#include "../include/global_defines.h"
#endif

namespace nameSpace_ATPG {

/*-----------------------------------------------------------------------------
 * The function forwardImplication is overloaded. The gates in the circuits
 * we analyze can have up to 3 inputs. Depending on the number of imputs passed
 * the proper function is executed.
 *
 * Sets are used since we don't want duplicate values at the output. Set will
 * remove duplicate values.
 *
 * The best way to get the result of any logical operation is to do a table
 * lookup. The tables are defined in the logic_tables.h file included above.
 * The tables can be of any valued logic. We are currently using 5 valued
 * logic in our test generator.
 *-----------------------------------------------------------------------------*/

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  forwardImlpication
 *  Description:  The function takes in an STL set as input and returns a set as output.
 *                The version defined below is a single input version.
 *                Set is used since multiple inputs may give same result.
 * =====================================================================================
 */
set <int>   forwardImplication (unsigned int gateType, set <int> & input_a) {

    set <int> returnSet;
    set <int>::iterator i;

    for (i = input_a.begin(); i != input_a.end(); i++) {
        switch (gateType) {
            case INV:
            case NAND:
            case NOR:
                returnSet.insert(INVTABLE[*i]);
                break;
            case BRNCH:
            case AND:
            case OR:
                returnSet.insert(BRNCHTABLE[*i]);
                break;
            case XOR:
                returnSet.insert(ONEXORTABLE[*i]);
                break;
            default:
                cerr << "Unknown gate type encountered for a gate with only 1 input." << endl;
                cerr << "Possible values are 0-7. Current value is " << gateType << endl;
                exit (0);
                break;
        }
    }

    return (returnSet);

}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  forwardImlpication
 *  Description:  The function takes in an STL set as input and returns a set as output.
 *                The version defined below is a two input version.
 *                Set is used since multiple inputs may give same result.
 * =====================================================================================
 */
set <int>   forwardImplication (unsigned int gateType, set <int> &input_a, set <int> &input_b) {

    set <int> returnSet;
    set <int>::iterator i, j;

    for (i = input_a.begin(); i != input_a.end(); i++) {
        for (j = input_b.begin(); j != input_b.end(); j++) {
            switch (gateType) {
                case AND:
                    returnSet.insert(ANDTABLE[*i][*j]);
                    break;
                case NAND:
                    returnSet.insert(NANDTABLE[*i][*j]);
                    break;
                case OR:
                    returnSet.insert(ORTABLE[*i][*j]);
                    break;
                case NOR:
                    returnSet.insert(NORTABLE[*i][*j]);
                    break;
                case XOR:
                    returnSet.insert(XORTABLE[*i][*j]);
                    break;
                case XNOR:
                    returnSet.insert(XNORTABLE[*i][*j]);
                    break;
                default:
                    cerr << "Unknown gate type encountered." << endl;
                    cerr << "Possible values are 2,3,4,6,7. Current value is " << gateType << endl;
                    exit (0);
                    break;
            }
        }
    }

    return returnSet;

}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  forwardImlpication
 *  Description:  The function takes in an STL set as input and returns a set as output.
 *                The version defined below is a three input version.
 *                Set is used since multiple inputs may give same result.
 * =====================================================================================
 */
set <int>   forwardImplication (unsigned int gateType, set <int> &input_a, set <int> &input_b, set <int> &input_c) {

    set <int> returnSet;
    set <int>::iterator i, j;

    for (i = input_a.begin(); i != input_a.end(); i++) {
        for (j = input_b.begin(); j != input_b.end(); j++) {
            switch (gateType) {
                case AND:
                case NAND:
                    returnSet.insert(ANDTABLE[*i][*j]);
                    break;
                case OR:
                case NOR:
                    returnSet.insert(ORTABLE[*i][*j]);
                    break;
                case XOR:
                case XNOR:
                    returnSet.insert(XORTABLE[*i][*j]);
                    break;
                default:
                    cerr << "Unknown gate type encountered." << endl;
                    cerr << "Possible values are 2,3,4,6,7. Current value is " << gateType << endl;
                    exit (0);
                    break;
            }
        }
    }

    return (forwardImplication (gateType, input_c, returnSet));

}

} /* namespace nameSpace_ATPG */
