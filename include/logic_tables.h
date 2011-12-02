/*
 * =====================================================================================
 *
 *       Filename:  logic_tables.h
 *
 *    Description:  This file contains the logic tables for various logic gates.
 *                  The tables can be looked up by providing input values.
 *                  The value stored at column X, row Y is the output of the gate
 *                  for input X and Y.
 *
 *                  This table currently uses 5 valued logic.
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


#ifndef LOGIC_TABLES_H_
#define LOGIC_TABLES_H_

// #####   HEADER FILE INCLUDES   ################################################### 

// Global constant definitions.

#ifndef GLOBAL_DEFINES_H_
#include "global_defines.h"
#endif

namespace nameSpace_ATPG {

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

} /* nameSpace_ATPG */

#endif /* LOGIC_TABLES_H_ */

