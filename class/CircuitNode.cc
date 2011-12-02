/*
 * =====================================================================================
 *
 *       Filename:  CircuitNode.cc
 *
 *    Description:  This file defines the methods in the CircuitNode class.
 *                  This class stores information about every node in the circuit.
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

#include "CircuitNode.h"

// Global constants are defined in this file.

#ifndef GLOBAL_DEFINES_H_
#include "../include/global_defines.h"
#endif

namespace nameSpace_ATPG {


/*
 *--------------------------------------------------------------------------------------
 *       Class:  CircuitNode
 *      Method:  CircuitNode :: CircuitNode
 * Description:  The circuit file is read and an object of this class is created for 
 *               each line in the circuit file. Then this constructor is called which 
 *               initializes some members and remaining members are directly written to 
 *               by the values read from the file.
 *--------------------------------------------------------------------------------------
 */
CircuitNode::CircuitNode(int inNodeType) {
    this->nodeType = inNodeType;    // Set the type of current object.
    totalNodes++;                   // Total number of nodes.
    if (inNodeType == PI)
        totalInputs ++;             // Total number of inputs to circuit.
    if (inNodeType == PO)
        totalOutputs ++;            // Total number of outputs from circuit.

    this->nodeIndex = totalNodes;   // Set the node ID of current object.
}

/*
 * Static variables must be declared globally in order to work properly.
 */
int CircuitNode::totalNodes;     // Number of nodes, global.
int CircuitNode::totalInputs;    // Number of primary inputs, global.
int CircuitNode::totalOutputs;   // Number of primary outputs, global.

} /* namespace nameSpace_ATPG */

