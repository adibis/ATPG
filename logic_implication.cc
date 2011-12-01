/*
 ******************************************************************************
 *
 *  Filename    :   logic_implication.cc
 *
 *  Author      :   Aditya Shevade <aditya.shevade@gmail.com>
 *
 *  Version     :   2.0.0
 *
 *  Created     :   11/10/2011
 *  Modified    :   11/28/2011
 *
 *  Changelog   :
 *
 *  11/28/2011
 *      Removed lot of code (commented) since this file is no longer used
 *      as main function. This is now a library of the implication function.
 *
 ******************************************************************************
 */

// These are already defined in the main program hence removed from here.
// TODO: Replace with #ifndef.
//#include <iostream>
//#include <set>

using namespace std;

// Defined below are the 5 possible "binary" logic values.
// Tables are pretty simple. B is invert of D and X is unknown.

#define ZERO    0
#define ONE     1
#define D       2
#define B       3   // This is Dbar.
#define X       4

// Gate types. Integer value which would be read from a file.

#define PS_PI   0   // Primary input. Not named PI to avoid conflict.
#define BRNCH   1
#define XOR     2
#define OR      3
#define NOR     4
#define INV     5
#define NAND    6
#define AND     7
#define XNOR    8   // Not implemented yet in the circuit file.

// Logic implication tables. These tables are fixed for a 2 input gate.
// Simple table lookup is the fastest way to get the value of the output.

                                    //  0   1   D   B   X
int ANDTABLE[5][5]  =   {/* 0 */    {   0,  0,  0,  0,  0   },
                         /* 1 */    {   0,  1,  D,  B,  X   },
                         /* D */    {   0,  D,  D,  0,  X   },
                         /* B */    {   0,  B,  0,  B,  X   },
                         /* X */    {   0,  X,  X,  X,  X   }
                        };

                                    //  0   1   D   B   X
int NANDTABLE[5][5]  =  {/* 0 */    {   1,  1,  1,  1,  1   },
                         /* 1 */    {   1,  0,  B,  D,  X   },
                         /* D */    {   1,  B,  B,  1,  X   },
                         /* B */    {   1,  D,  1,  D,  X   },
                         /* X */    {   1,  X,  X,  X,  X   }
                        };

                                    //  0   1   D   B   X
int ORTABLE[5][5]   =   {/* 0 */    {   0,  1,  D,  B,  X   },
                         /* 1 */    {   1,  1,  1,  1,  1   },
                         /* D */    {   D,  1,  D,  1,  X   },
                         /* B */    {   B,  1,  1,  B,  X   },
                         /* X */    {   X,  1,  X,  X,  X   }
                        };

                                    //  0   1   D   B   X
int NORTABLE[5][5]  =   {/* 0 */    {   1,  0,  B,  D,  X   },
                         /* 1 */    {   0,  0,  0,  0,  0   },
                         /* D */    {   B,  0,  B,  0,  X   },
                         /* B */    {   D,  0,  0,  D,  X   },
                         /* X */    {   X,  0,  X,  X,  X   }
                        };

                                    //  0   1   D   B   X
int XORTABLE[5][5]  =   {/* 0 */    {   0,  1,  D,  B,  X   },
                         /* 1 */    {   1,  0,  B,  D,  X   },
                         /* D */    {   D,  B,  0,  1,  X   },
                         /* B */    {   B,  D,  1,  0,  X   },
                         /* X */    {   X,  X,  X,  X,  X   }
                        };

                                    //  0   1   D   B   X
int XNORTABLE[5][5]  =  {/* 0 */    {   1,  0,  B,  D,  X   },
                         /* 1 */    {   0,  1,  D,  B,  X   },
                         /* D */    {   B,  D,  1,  0,  X   },
                         /* B */    {   D,  B,  0,  1,  X   },
                         /* X */    {   X,  X,  X,  X,  X   }
                        };

                                    //  0   1   D   B   X
int INVTABLE [5]    =   {               1,  0,  B,  D,  X   };

                                    //  0   1   D   B   X
int BRNCHTABLE [5]  =   {               0,  1,  D,  B,  X   };

                                    //  0   1   D   B   X
int ONEXORTABLE [5] =   {               0,  0,  0,  0,  X   };

// 
// Decided that overloading the function was the best way to go about this.
// The function takes in an STL set as input and returns a set as output.
// This saves a lot of work on our end since we don't have to worry about multiple inputs resulting in same output.
//
// Again, pretty simple code as you can see. It just returns a value from the table. I figured that using
// multiple if else statements (since C/C++ does not understand multi valued logic) I just hardcoded it.
//
// If there are 3 inputs, the function is called twice.
//
// ***************************************************************************************************************
// NOTE - If you see, for a 3 input NAND, NOR or XNOR gate, only the final gate is inverted. So, the initial
// table lookup is done on the non inverting logic. (AND instead of NAND, OR instead of NOR and XOR instead of 
// XNOR) but the subsequent (final) lookup is done on the inverting gate.
// ***************************************************************************************************************
//
// ***************************************************************************************************************
// If you have a fault at the input, then, only the input set needs to be changed to one single value. There
// won't be a D or B in the input set (since we have not encountered the fault yet). So, we have to reduce the
// input set from {0, 1} to either a {0} for stuck at 0 and {1} for stuck at 1.
// ***************************************************************************************************************
//

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

//
// This is just to test the functions out. Nothing intelligent here.
// The actual code will read these gates as a big badass list from a file.
//
// Have to implement a way to do something about somehow adding this function
// to either the class of circuit or individual gate (which seems logically correct)
//
/*
int main () {

    set <int> input_a, input_b, input_c;
    set <int> output1Set, output2Set, output3Set;
    set <int>::iterator itr;

    input_a.insert(0);
    input_a.insert(1);
    input_a.insert(3);
    
    input_b.insert(0);
    input_b.insert(1);

    input_c.insert(1);
    input_c.insert(3);

    output1Set = forwardImplication (INV, input_a);

    for (itr = output1Set.begin(); itr != output1Set.end(); itr++) {
        cout << "Output 1 Set = " << *itr << endl;
    }

    output2Set = forwardImplication (AND, input_a, input_b);

    for (itr = output2Set.begin(); itr != output2Set.end(); itr++) {
        cout << "Output 2 Set = " << *itr << endl;
    }

    output3Set = forwardImplication (OR, input_a, input_b, input_c);

    for (itr = output3Set.begin(); itr != output3Set.end(); itr++) {
        cout << "Output 3 Set = " << *itr << endl;
    }

    return 0;

}
*/

