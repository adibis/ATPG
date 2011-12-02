/*
 * =====================================================================================
 *
 *       Filename:  file_operations.cc
 *
 *    Description:  Genetic functions to open file for reading and writing.
 *                  The functions dump information into a log file.
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
 *         Name:  openInFile
 *  Description:  Opens file for reading. Receives the filename as input parameter.
 *                The file handle to use for the file. Also receives a log file handle
 *                where all the debug information is dumped. 
 * =====================================================================================
 */
void openInFile (char *fileName, ifstream &inFile, ofstream &logFile) {
    inFile.open (fileName, ifstream::in);
    if (!inFile) { // File could not be opened.
        cerr << "ERROR: File could not be opened." << endl;
        exit (1);
    }

    else {
        #ifdef DEBUG
        WRITE << "File opened successfully" << endl;
        WRITE << "File name = " << fileName << endl;
        WRITE << "--------------------------------------------------------------------------------" << endl << endl;
        #endif
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  openOutFile
 *  Description:  Opens file for writing. Receives the filename as input parameter.
 *                The file handle to use for the file. Also receives a log file handle
 *                where all the debug information is dumped. 
 * =====================================================================================
 */
void openOutFile (char *fileName, ofstream &inFile, ofstream &logFile) {
    inFile.open (fileName, ofstream::out);
    if (!inFile) { // File could not be opened.
        cerr << "ERROR: File could not be opened." << endl;
        exit (1);
    }

    else {
        #ifdef DEBUG
        WRITE << "File opened successfully" << endl;
        WRITE << "File name = " << fileName << endl;
        WRITE << "--------------------------------------------------------------------------------" << endl << endl;
        #endif
    }
}

} /* nameSpace_ATPG */

