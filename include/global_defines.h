/*
 * =====================================================================================
 *
 *       Filename:  global_defines.h
 *
 *    Description:  Global definitions used throughout the program.
 *                  It defines the gate types and the node types used in the circuit.
 *                  It also defines information about logging and debugging.
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

#ifndef GLOBAL_DEFINES_H_
#define GLOBAL_DEFINES_H_

namespace nameSpace_ATPG {

/*
 * We are defining 2 flags here.
 * DEBUG - If this is true then all the DEBUG messages would be printed.
 * LOG - This MUST be defined with DEBUG. If this is defined then all
 * the debug messaged would be put in a log file - logFile.log.
 */
#define DEBUG
#define LOG
#define LOG_FILE_NAME "logFile.log"

/*
 * Here we define if we want the final test list to be put in a file.
 * Both the variables need to be defined in conjunction.  
 */
#define RESULT_FILE
#define RESULT_FILE_NAME "finalTest.tst"

/*
 * Provision, in case LOG is defined and DEBUG is not.
 */
#ifdef LOG
#ifndef DEBUG
#define DEBUG
#endif
#endif

/*
 * If this flag is defined then the information about the circuit is
 * printed to the console.
 */
#define DEBUG_PRINT

/*
 * If LOG is defined above, we need to first create a file object.
 * Then we need to replace all cout with that file handle and write
 * all the messages to the log file.
 */
#ifdef LOG
    #define WRITE logFile
#else
    #define WRITE cout
#endif

/*
 * These are the node type values.
 */
#define GT 0        // Gate.
#define PI 1        // Primary Input.
#define FB 2        // Forward Branch.
#define PO 3        // Primary Output.

/*
 * If the node is of gate type (read above). Then it can
 * have the following possible values.
 */
#define G_PI    0   // Primary Input.
#define G_BRNCH 1   // Forward Branch.
#define G_XOR   2   // XOR Gate.
#define G_OR    3   // OR Gate.
#define G_NOR   4   // NOR Gate.
#define G_NOT   5   // NOT Gate.
#define G_NAND  6   // NAND Gate.
#define G_AND   7   // AND Gate.
#define G_XNOR  8   // XNOR Gate. Not implemented in out circuits.

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

}

#endif /* GLOBAL_DEFINES_H_ */

