/*
 * =====================================================================================
 *
 *       Filename:  CircuitNode.h
 *
 *    Description:  This class contains all the parameters associated with every node
 *                  in the circuit such as the inputs, outputs, type, line number.
 *
 *                  There are static members which keep track of total inputs, outputs
 *                  and lines in the entire circuit.
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

#ifndef CIRCUITNODE_H_
#define CIRCUITNODE_H_

namespace nameSpace_ATPG {


/*
 * =====================================================================================
 *        Class:  CircuitNode
 *  Description:  Each object of this class represents one line in the circuit.
 *                It can be of type Primary Input, Forward Branch, Primary Output or
 *                a Logic Gate.
 *
 *                There are a number of properties associated with each circuit line.
 *                All such properties are included below.
 * =====================================================================================
 */
class CircuitNode {
    public:
        unsigned    int     nodeIndex;      // Node ID, unique.
        unsigned    int     nodeType;       // Type of the node.
        unsigned    int     lineNumber;     // Unique line number.
        unsigned    int     gateType;       // Type of the gate if the node is gate.
        unsigned    int     numberFanIn;    // Total inputs to this node.
        unsigned    int     numberFanOut;   // Total outputs from this node.
        set         <int>   listFanIn;      // List of inputs to this node.
        set         <int>   listFanOut;     // List of outputs from this node.
                    int     lineLevel;      // Level of the line (after levelization).
        set         <int>   lineValue;      // Value of the line (for simulation).

        static      int     totalNodes;     // Number of nodes, global.
        static      int     totalInputs;    // Number of primary inputs, global.
        static      int     totalOutputs;   // Number of primary outputs, global.

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
        CircuitNode (int inNodeType);
};

} /* namespace nameSpace_ATPG */
#endif /* CIRCUITNODE_H_ */
