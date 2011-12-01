#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <set>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>

#include "logic_implication.cc"

using namespace std;

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
 * The log file handle is created if LOG is defined.
 */
#ifdef LOG
    ofstream logFile;
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


/*
 * Each object of this class represents one line in the circuit.
 * It can be of type Primary Input, Forward Branch, Primary Output or a Logic Gate.
 * There are a number of properties associated with each circuit line.
 * All such properties are included below.
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
         * The circuit file is read and an object of this class is created for each line in the
         * circuit file. Then this constructor is called which initializes some members and
         * remaining members are directly written to by the values read from the file.
         */
        CircuitNode (int inNodeType) {
            this->nodeType = inNodeType;    // Set the type of current object.
            totalNodes++;                   // Total number of nodes.
            if (nodeType == PI)
                totalInputs ++;             // Total number of inputs to circuit.
            if (nodeType == PO)
                totalOutputs ++;            // Total number of outputs from circuit.

            this->nodeIndex = totalNodes;   // Set the node ID of current object.
        }

};

/*
 * Every line in the circuit can have a stuck at 0 or a stuck at 1 fault.
 * Objects of this class store the line number, which is unique, as the
 * object ID and then the fault values associated with them.
 */

class CircuitLine {

    public:
        unsigned    int     lineNumber;
                    bool    isStuckAt_0;
                    bool    isStuckAt_1;

        CircuitLine (int inLineNumber) {
            this->lineNumber = inLineNumber;
            isStuckAt_0 = true;
            isStuckAt_1 = true;
        }
};

/*
 * This class stores the line number and the fault that exists at that line
 * number. 
 *
 * StuckAtValue = false means a stuck at 0 fault exists.
 * StuckAtValue = true means a stuck at 1 fault exists.
 *
 * An object of this class is created only if the line has fault.
 */
class FaultList {
    public:
        unsigned int lineNumber;
        bool stuckAtValue;

        FaultList (int inLineNumber, bool inStuckAtValue) {
            this->lineNumber = inLineNumber;
            this->stuckAtValue = inStuckAtValue;
        }
};

/*
 * Objects of this class store the line number, the fault and
 * the vector that excites that fault.
 */
class TestList {
    public:
        unsigned int lineNumber;
        bool stuckAtValue;
        bool isTestPossible;
        string testVector;

        TestList (int inLineNumber, bool inStuckAtValue, string inTestVector) {
            this->lineNumber = inLineNumber;
            this->stuckAtValue = inStuckAtValue;
            this->testVector = inTestVector;
            this->isTestPossible = true;
        }

        TestList (int inLineNumber, bool inStuckAtValue) {
            this->lineNumber = inLineNumber;
            this->stuckAtValue = inStuckAtValue;
            this->testVector = "";
            this->isTestPossible = false;
        }
};

/*
 * Static variables must be declared globally in order to work properly.
 */
int     CircuitNode::totalNodes;     // Number of nodes, global.
int     CircuitNode::totalInputs;    // Number of primary inputs, global.
int     CircuitNode::totalOutputs;   // Number of primary outputs, global.

/*
 * The number of objects of the CircuitNode class depend on the number of lines in the circuit.
 * This vector is the list of all lines in a circuit.
 */
vector <CircuitNode> masterNodeList;

/*
 * This map contains the objects of CircuitLine. Reason to use map was that
 * we need only unique line numbers for the CircuitLine objects. So, the line
 * number is used as the key of the map and object of that line as the
 * value associated with that key.
 */
map <int, CircuitLine> masterLineList;

/*
 * This vector contains the list of all possible inputs to a circuit in a 3
 * valued logic. It contains strings. Width of the string is euqal to the
 * number of inputs to the circuit. It will later be split into integers
 * while applying to the circuit.
 */
vector <string> masterInputVector;
vector <string> masterTestVector;

/*
 * This vector contains all the line numbers with faults and the possible
 * fault values at those lines.
 *
 * When running ATPG, we have to obtain a vector to test every fault in this
 * vector.
 */
vector <FaultList> masterFaultList;
vector <FaultList> providedFaultList;
/*
 * This vector contains all the possible faults in the circuit and the test
 * vector associated with them.
 */
vector <TestList> masterTestList;
vector <TestList> providedTestList;

/*
 * ============================================================================
 * Generic Algorithms Used in the Program
 * ============================================================================
 */

/*
 * This function takes in an integer value in base 10, the base you want to
 * convert that number into and a character array and returns it.
 *
 * We have changed the name from itoa to RadixConvert.
 */

/* 
 * C++ version 0.4 std::string style "itoa":
 * Contributions from Stuart Lowe, Ray-Yuan Sheu,
 * Rodrigo de Salvo Braz, Luc Gallant, John Maloney
 * and Brian Hunt
 */

string RadixConvert (int value, int base) {
    
    string buf;
    
    // Check that the base if valid
    if (base < 2 || base > 16)
        return buf;

    enum { kMaxDigits = 35 };
    buf.reserve( kMaxDigits );  // Pre-allocate enough space.
    
    int quotient = value;
    
    // Translating number to string with base:
    do {
        buf += "0123456789abcdef"[ std::abs( quotient % base ) ];
        quotient /= base;
    } while ( quotient );
    
    // Append the negative sign
    if ( value < 0) buf += '-';
    
    std::reverse( buf.begin(), buf.end() );
    return buf;
}
    
/*
 * ============================================================================
 * Functions to open file for reading or writing.
 * ============================================================================
 */

/*
 * Opens a file for reading. Program exits if file cannot be opened.
 */
void openInFile (char *fileName, ifstream &inFile) {
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
 * Opens a file for writing. Program exits if file cannot be opened.
 */
void openOutFile (char *fileName, ofstream &inFile) {
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

/*
 * ============================================================================
 * Functions to populate objects of CircuitNode class by reading from the
 * circuit file and the fault list file.
 * ============================================================================
 */

/*
 * This function, once it can successfully open the circuit file, will then
 * populate the structure/class with the proper values from the file.
 */
void ReadCircuit (ifstream &inFile) {

    #ifdef DEBUG
        WRITE << "================================================================================" << endl;
        WRITE << "===============                In --> ReadCircuit                ===============" << endl;
        WRITE << "================================================================================" << endl << endl;
    #endif

    unsigned int nodeType;
    unsigned int fanInListMember;

    while (true) {
        inFile >> nodeType;                         // This is the type of current node.
        if (inFile.eof())
            break;
        CircuitNode *thisNode;
        thisNode = new CircuitNode (nodeType);      // Constructor checks for inputs and outputs.

        switch (nodeType) {
            case PI:                                // Check if the current node is a primary input.
                inFile >> thisNode->lineNumber;     // Unique ID for the line.
                inFile >> thisNode->gateType;       // This will be always 0 for primary inputs.
                inFile >> thisNode->numberFanOut;   // Number of lines connected to this input.
                inFile >> thisNode->numberFanIn;    // This will always be 0 for primary inputs.
                break;
            
            case FB:
                inFile >> thisNode->lineNumber;     // Unique ID for the line.
                inFile >> thisNode->gateType;       // This will be always 1 for branches.
                thisNode->numberFanIn = 1;          // Branch always has only one input.
                thisNode->numberFanOut = 100;       // Branch can have multiple outputs but only one per line.
                //TODO: Figure this out - 100 is incorrect. Needs to be proper value.
                inFile >> fanInListMember;          // This is the fan out for branch.
                thisNode->listFanIn.insert(fanInListMember);
                break;

            case GT:
                inFile >> thisNode->lineNumber;     // Unique ID for the line.
                inFile >> thisNode->gateType;       // Gate type.
                inFile >> thisNode->numberFanOut;   // Number of lines connected to this node.
                inFile >> thisNode->numberFanIn;    // Number of lines connected at this node.
                for (int i = 0; i < thisNode->numberFanIn; i++) {
                    inFile >> fanInListMember;
                    thisNode->listFanIn.insert(fanInListMember);
                }
                break;

            case PO:
                inFile >> thisNode->lineNumber;     // Unique ID for the line.
                inFile >> thisNode->gateType;       // Type of the gate output is connected to.
                inFile >> thisNode->numberFanOut;   // Always zero for outputs.
                inFile >> thisNode->numberFanIn;    // Number of lines connected at this node.
                for (int i = 0; i < thisNode->numberFanIn; i++) {
                    inFile >> fanInListMember;
                    thisNode->listFanIn.insert(fanInListMember);
                }
                break;

            default:                                // Undefined node type.
                cerr << "ERROR: Undefined Node Type." << endl 
                     << "Valid values are 0, 1, 2 and 3." << endl
                     << "Current value is " << nodeType << endl;
                exit (1);
                break;                              // No necessary since exit (1) will end program.
        }

        masterNodeList.push_back(*thisNode);

        
        delete thisNode;        
 
    }

    set <int>::iterator it, ij;
    for (int i = 0; i < masterNodeList.size(); i++) {
        for(it = masterNodeList[i].listFanIn.begin(); it != masterNodeList[i].listFanIn.end(); it++) {
            for (int j = 0; j < masterNodeList.size(); j++) {
                if (masterNodeList[j].lineNumber == *it)
                    masterNodeList[j].listFanOut.insert(masterNodeList[i].lineNumber);
            }
        }
    }

    #ifdef DEBUG
        set <int>::iterator itr;
        for (int i = 0; i < masterNodeList.size(); i++) {
            WRITE << "------------------------------------------------------------" << endl;
            WRITE << "\tProperties Associated With Each Node" << endl;
            WRITE << "------------------------------------------------------------" << endl;
            WRITE << "nodeIndex = " << masterNodeList[i].nodeIndex << endl;
            WRITE << "nodeType = " << masterNodeList[i].nodeType << endl;
            WRITE << "lineNumber = " << masterNodeList[i].lineNumber << endl;
            WRITE << "gateType = " << masterNodeList[i].gateType << endl;
            WRITE << "numberFanOut = " << masterNodeList[i].numberFanOut << endl;
            WRITE << "numberFanIn = " << masterNodeList[i].numberFanIn << endl;
            WRITE << "listFanIn = ";
            for(itr = masterNodeList[i].listFanIn.begin(); itr != masterNodeList[i].listFanIn.end(); itr++) {
                WRITE << " " << *itr << ", ";
            }
            WRITE << endl;
            WRITE << "listFanOut = ";
            for(itr = masterNodeList[i].listFanOut.begin(); itr != masterNodeList[i].listFanOut.end(); itr++) {
                WRITE << " " << *itr << ", ";
            }
            WRITE << endl << "------------------------------------------------------------" << endl;
            WRITE << endl << endl << endl;
        }
    #endif       
}

/*
 * Fault list file has the format of <lineNumber> <faultType> on each line.
 * This function just parses that file and sets the proper variable in the object if fault exists.
 */
void ReadFaultList (ifstream &inFile) {

    #ifdef DEBUG
        WRITE << "================================================================================" << endl;
        WRITE << "===============                In --> ReadFaultList              ===============" << endl;
        WRITE << "================================================================================" << endl << endl;
    #endif

    int outLineNumber;
    int outFaultValue;
    bool invalidLine;

    while (true) {
        inFile >> outLineNumber;
        if (inFile.eof())
            break;
        inFile >> outFaultValue;
        invalidLine = true;
        for (int i = 0; i < masterNodeList.size(); i++) {
            if (masterNodeList[i].lineNumber == outLineNumber) {
                FaultList *thisFault;
                thisFault = new FaultList (outLineNumber, outFaultValue);
                providedFaultList.push_back (*thisFault);
                delete thisFault;
                invalidLine = false;
            }
            // If the line number cannot be found in the circuit then print
            // a message that it is invalid.
            //
            // Continue to the next fault in the file.
            // This is not an error. The ATPG will simply ignore that invalid line.
            else {
                if (i == (masterNodeList.size() - 1) && invalidLine) {
                    cout << "------------------------------------------------------------" << endl;
                    cout << "INFO: Invalid line number in the provided fault list." << endl;
                    cout << "The line number " << outLineNumber << " does not exist in this circuit." << endl;
                    cout << "------------------------------------------------------------" << endl << endl;
                }
            }

                    
        }
    }

    for (int i = 0; i < providedFaultList.size(); i++) {
        #ifdef DEBUG
            WRITE << "------------------------------------------------------------" << endl;
            WRITE << "\tSetting Fault Values" << endl;
            WRITE << "------------------------------------------------------------" << endl;
            WRITE << "For circuit line " << providedFaultList[i].lineNumber << endl;
            WRITE << "Fault is Stuck At " << providedFaultList[i].stuckAtValue << endl; 
            WRITE << "------------------------------------------------------------" << endl;
            WRITE << endl << endl;
        #endif
    }
}

/*
 * ============================================================================
 * Functions to Perform Logical Operations on the Circuit.
 * ============================================================================
 */

/*
 * This function divides the circuit in logical levels.
 * There is no feedback in a combinational circuit so
 * we can go from level 0 to level MAX and find the proper
 * input to output paths.
 */

void SetLineLevel (vector <CircuitNode> &masterNodeList) {

    #ifdef DEBUG
        WRITE << "================================================================================" << endl;
        WRITE << "===============               In --> SetLineLevel                ===============" << endl;
        WRITE << "================================================================================" << endl << endl;
    #endif

    for (int i = 0; i < masterNodeList.size(); i++) {
        if (masterNodeList[i].numberFanIn == 0)
            masterNodeList[i].lineLevel = 0;                    // Level of primary inputs is 0.
        else
            masterNodeList[i].lineLevel = -1;                   // Level of all other nodes is -1, placeholder.
    }

    set <int>::iterator it;

    /*
     * For all nodes.
     *      -- For all nodes after the current node.
     *          -- If the input list of next node contains current node.
     *              -- Check the level of that next node.
     *                  -- If it is less than the level of current node, update it.
     */ 

    for (int i = 0; i < masterNodeList.size(); i++) {
        for (int j = (i + 1); j < masterNodeList.size(); j++) {
            for (it = masterNodeList[j].listFanIn.begin(); it != masterNodeList[j].listFanIn.end(); it++) {
                for (int k = 0; k < masterNodeList.size(); k++) {
                    if (masterNodeList[k].lineNumber == *it) {
                        if (masterNodeList[k].lineNumber == masterNodeList[i].lineNumber) {
                            if (masterNodeList[j].lineLevel < (masterNodeList[i].lineLevel + 1)) {
                                masterNodeList[j].lineLevel = (masterNodeList[i].lineLevel + 1);
                            }
                        }
                    }
                }
            }
        }
    
    #ifdef DEBUG
        WRITE << "Line Number = " << masterNodeList[i].lineNumber << ". Level = " << masterNodeList[i].lineLevel << "." << endl;
    #endif
    }
    #ifdef DEBUG
        WRITE << "--------------------------------------------------------------------------------" << endl << endl;
    #endif

}

/*
 * This function generates one object of the CircuitLine class for every unique line in the circuit.
 * So, after going through this function, we have a list of objects for each line in the circuit.
 * Every line may have a stuck at 0 or a stuck at 1 fault so values are set to true for both.
 *
 * A map is used here with the line number as the key. Duplicate elements would be removed
 * automatically by the map.
 */

void CreateFaultObjects (map <int, CircuitLine> &masterLineList, vector <CircuitNode> &masterNodeList) {

    #ifdef DEBUG
        WRITE << "================================================================================" << endl;
        WRITE << "===============            In --> CreateFaultObjects             ===============" << endl;
        WRITE << "================================================================================" << endl << endl;
    #endif

    set <int>::iterator itr;
    
    for (int i = 0; i < masterNodeList.size(); i++) {
        
        // Create an object for every node.
        CircuitLine *thisLine;
        thisLine = new CircuitLine(masterNodeList[i].lineNumber);
        masterLineList.insert(pair<int, CircuitLine>(masterNodeList[i].lineNumber, *thisLine));
        delete thisLine;
        
        // Create an object for every fan in of the node.
        for (itr = masterNodeList[i].listFanIn.begin(); itr != masterNodeList[i].listFanIn.end(); itr++) {
            CircuitLine *thisLine;
            thisLine = new CircuitLine(*itr);
            masterLineList.insert(pair<int, CircuitLine>(*itr, *thisLine));
            delete thisLine;
        }

        // Create an object for every fan out of the node.
        for (itr = masterNodeList[i].listFanOut.begin(); itr != masterNodeList[i].listFanOut.end(); itr++) {
            CircuitLine *thisLine;
            thisLine = new CircuitLine(*itr);
            masterLineList.insert(pair<int, CircuitLine>(*itr, *thisLine));
            delete thisLine;
        }

    }

    #ifdef DEBUG
        for (map<int, CircuitLine>::iterator itr = masterLineList.begin(); itr != masterLineList.end(); itr++) {
            WRITE << "------------------------------------------------------------" << endl;
            WRITE << "\tThe map contains following values" << endl;
            WRITE << "------------------------------------------------------------" << endl;
            WRITE << "The key is " << (*itr).first << endl;
            WRITE << "The Element is " << (*itr).second.lineNumber << endl;
            WRITE << "The StuckAt_0 fault is " << (*itr).second.isStuckAt_0 << endl;
            WRITE << "The StuckAt_1 fault is " << (*itr).second.isStuckAt_1 << endl;
            WRITE << "------------------------------------------------------------" << endl;
            WRITE << endl << endl;
        }
    #endif


}

/*
 * This function takes the list of lines in the circuit and then collapses the faults going from output to input.
 *
 * Fault collapse for XOR, XNOR and Branches is not yet implemented.
 */

void CollapseFaults (map <int, CircuitLine> &masterLineList, vector <CircuitNode> &masterNodeList) {

    #ifdef DEBUG
        WRITE << "================================================================================" << endl;
        WRITE << "===============               In --> CollapseFaults              ===============" << endl;
        WRITE << "================================================================================" << endl << endl;
    #endif

    map <int, CircuitLine>::iterator itrMap;
    set <int>::iterator itrSet;
    itrMap = masterLineList.end();
    itrMap--;
    
    int lastOutputLineNumber = (*itrMap).second.lineNumber;         // Line number of the last output.
    int lastOutputLineLevel = masterNodeList.back().lineLevel;      // Level of the last output.

    #ifdef DEBUG
        WRITE << "Line number of last output = " << lastOutputLineNumber << endl;
        WRITE << "Level of last output = " << lastOutputLineLevel << endl;
    #endif

    // From the highest level (last output) to zero.
    //      -- For all the nodes.
    //          -- If the current node has the highest level.
    //              -- Collapse faults.
    //              -- Reduce highest level by 1.
    //      -- Next iteration of the loop.
    //
    // In the collapse faults procedure, the fault can collapse
    // to any one of the 2/3 inputs.
    // In this code, the fault always collapses to the input with
    // the smallest line number.
    //
    // Generic structure of the fault collapse is -
    //      -- Remove both faults from output.
    //      -- Reduce one of them to single fault at one of the inputs. 
    //      -- Second fault stays on all inputs.
    //
    // XOR, XNOR and Branches are not considered here.

    for (int i = lastOutputLineLevel; i > 0; i--) {
        for (int j = 0; j < masterNodeList.size(); j++) {
            if (masterNodeList[j].lineLevel == i) {
                switch (masterNodeList[j].gateType) {
                    case G_PI:
                        break;
                    case G_BRNCH: 
                        break;
                    case G_XOR:
                        break;
                    case G_OR:
                    case G_NOR:
                        for (itrSet = masterNodeList[j].listFanOut.begin(); itrSet != masterNodeList[j].listFanOut.end(); itrSet++) {
                            masterLineList.at(*itrSet).isStuckAt_0 = false;
                            masterLineList.at(*itrSet).isStuckAt_1 = false;
                        }
                        for (itrSet = masterNodeList[j].listFanIn.begin(); itrSet != masterNodeList[j].listFanIn.end(); itrSet++) {
                            if (itrSet != masterNodeList[j].listFanIn.begin())
                                masterLineList.at(*itrSet).isStuckAt_1 = false;
                            
                        }
                        break;
                    case G_NOT:
                        for (itrSet = masterNodeList[j].listFanOut.begin(); itrSet != masterNodeList[j].listFanOut.end(); itrSet++) {
                            masterLineList.at(*itrSet).isStuckAt_0 = false;
                            masterLineList.at(*itrSet).isStuckAt_1 = false;
                        }
                        break;
                    case G_NAND:
                    case G_AND:
                        for (itrSet = masterNodeList[j].listFanOut.begin(); itrSet != masterNodeList[j].listFanOut.end(); itrSet++) {
                            masterLineList.at(*itrSet).isStuckAt_0 = false;
                            masterLineList.at(*itrSet).isStuckAt_1 = false;
                        }
                        for (itrSet = masterNodeList[j].listFanIn.begin(); itrSet != masterNodeList[j].listFanIn.end(); itrSet++) {
                            if (itrSet != masterNodeList[j].listFanIn.begin())
                                masterLineList.at(*itrSet).isStuckAt_0 = false;
                            
                        }
                        break;
                    default:
                        cerr << "Unknown gate type encountered. The possible gate type values are from 0-7." << endl;
                        cerr << "Current gate type is " << masterNodeList[j].gateType << endl;
                        exit (0);
                        break;
                }
            }
        }
    }
    
    #ifdef DEBUG
        for (map<int, CircuitLine>::iterator itr = masterLineList.begin(); itr != masterLineList.end(); itr++) {
            WRITE << "------------------------------------------------------------" << endl;
            WRITE << "\tAfter Fault Collapse, the Map contains" << endl;
            WRITE << "------------------------------------------------------------" << endl;
            WRITE << "The key is " << (*itr).first << endl;
            WRITE << "The Element is " << (*itr).second.lineNumber << endl;
            WRITE << "The StuckAt_0 fault is " << (*itr).second.isStuckAt_0 << endl;
            WRITE << "The StuckAt_1 fault is " << (*itr).second.isStuckAt_1 << endl;
            WRITE << "------------------------------------------------------------" << endl;
            WRITE << endl << endl;
        }
    #endif

}

/*
 * Once the faults have been collapsed, this function generates a list of all possible
 * faults and the line numbers associated with them. When running ATPG, we have to 
 * obtain a test for every fault in this list.
 */
void MakeFaultList (void) {
    
    #ifdef DEBUG
        WRITE << "================================================================================" << endl;
        WRITE << "===============               In --> MakeFaultList               ===============" << endl;
        WRITE << "================================================================================" << endl << endl;
    #endif

    for (map<int, CircuitLine>::iterator itr = masterLineList.begin(); itr != masterLineList.end(); itr++) {
        if ((*itr).second.isStuckAt_0) {
            FaultList *thisFault;
            thisFault = new FaultList ((*itr).first, false);
            masterFaultList.push_back(*thisFault);
            delete thisFault;
        }
        if ((*itr).second.isStuckAt_1) {
            FaultList *thisFault;
            thisFault = new FaultList ((*itr).first, true);
            masterFaultList.push_back(*thisFault);
            delete thisFault;
        }
    }

    #ifdef DEBUG
        WRITE << endl;
        WRITE << "Master Fault List Contains" << endl;
        WRITE << "------------------------------------------------------------" << endl;
        for (int i = 0; i < masterFaultList.size(); i++) {
            WRITE << "Line Number = " << masterFaultList[i].lineNumber << endl;
            WRITE << "Fault Value = " << masterFaultList[i].stuckAtValue << endl;
            WRITE << endl;
        }
        WRITE << endl;
    #endif

}

/*
 * Simple logic simulation. The number of inputs are provided to it.
 * It generates a random input vector, simulates the circuit and
 * then generates the output value.
 */

void SimpleLogicSimulation (int totalInputs, string inVector) {
    
    #ifdef DEBUG
        WRITE << "================================================================================" << endl;
        WRITE << "===============           In --> SimpleLogicSimulation           ===============" << endl;
        WRITE << "================================================================================" << endl << endl;
    #endif

    int *inputVector;
    inputVector = new int [totalInputs];
    set <int> *gateVector;
    gateVector = new set <int> [3];
    set <int>::iterator itrSet;

    // Convert the input vector string to integer
    // values to be applied to the circuit for simulation.
    // (- 48) is done to convert ascii to integer.
    for (int i = 0; i < totalInputs; i++) {
        inputVector[i] = inVector[i] - 48;
    }

    // Clear all the sets. This is important since we don't want
    // the previous values affecting current simulation.
    for (int i = 0, j = 0; i < masterNodeList.size(); i++) {
        masterNodeList[i].lineValue.clear();
    }

    // Assign the vector to inputs and keep all other values empty.
    for (int i = 0, j = 0; i < masterNodeList.size(); i++) {
        if (masterNodeList[i].numberFanIn == 0)
            masterNodeList[i].lineValue.insert(inputVector[j++]);
    }
    

    // Calls implication on all the nodes going from input to output.
    // Implication would result in a set of single value at the output.
    // If there is a fault (will be considered in FaultSimulation) then
    // the output set will have multiple values and/or D and/or B (Dbar).
    for (int i = 0; i < masterNodeList.size(); i++) {                       // For all nodes.
        if (masterNodeList[i].numberFanIn != 0) {                           // Not a primary input.
            int inputNumber = 0;

            // If not a primary input then generate the input set to be given to implication function.
            for (itrSet = masterNodeList[i].listFanIn.begin(); itrSet != masterNodeList[i].listFanIn.end(); itrSet++) {
                for (int j = 0; j < masterNodeList.size(); j++) {
                    if (masterNodeList[j].lineNumber == *itrSet) {
                        gateVector[inputNumber++] = masterNodeList[j].lineValue;
                    }
                }
            }

            // Call proper implication function depending on the number of the inputs to the gate.
            // Overloaded function, please refer to logic_implication.cc for details.
            switch (masterNodeList[i].numberFanIn) {
                case 1:
                    masterNodeList[i].lineValue = forwardImplication (masterNodeList[i].gateType, gateVector[0]);
                    break;
                case 2:
                    masterNodeList[i].lineValue = forwardImplication (masterNodeList[i].gateType, gateVector[0], gateVector[1]);
                    break;
                case 3:
                    masterNodeList[i].lineValue = forwardImplication (masterNodeList[i].gateType, gateVector[0], gateVector[1], gateVector[2]);
                    break;
                default:
                    cerr << "This ATPG generator only works if the number of inputs to a gate is less than 4." << endl;
                    cerr << "Currently there are " << masterNodeList[i].numberFanIn << " inputs to the gate." << endl;
                    exit (0);
                    break;
            }
        }
    }
    
    #ifdef DEBUG
        WRITE << "There are " << CircuitNode::totalInputs << " inputs to this circuit." << endl << endl;

        for (int i = 0; i < totalInputs; i++) {
            WRITE << "Input Vector [" << i << "] is = " << inputVector[i] << endl;
        }
        WRITE << endl;
        
        for (int i = 0; i < masterNodeList.size(); i++) {
            if (masterNodeList[i].numberFanIn == 0) {
                WRITE << "The value of the input at line number " << masterNodeList[i].lineNumber << " is = ";
                for (itrSet = masterNodeList[i].lineValue.begin(); itrSet != masterNodeList[i].lineValue.end(); itrSet++) {
                    WRITE << *itrSet << " ";
                }
                WRITE << endl;
            }
        }
        WRITE << endl;

        for (int i = 0; i < masterNodeList.size(); i++) {
            if (masterNodeList[i].numberFanIn != 0) {
                WRITE << "The value of the line number " << masterNodeList[i].lineNumber << " is = ";
                for (itrSet = masterNodeList[i].lineValue.begin(); itrSet != masterNodeList[i].lineValue.end(); itrSet++) {
                    WRITE << *itrSet << " ";
                }
                WRITE << endl;
            }
        }
        WRITE << endl;

        for (int i = 0; i < masterNodeList.size(); i++) {
            if (masterNodeList[i].numberFanOut == 0) {
                WRITE << "The value of the output at line number " << masterNodeList[i].lineNumber << " is = ";
                for (itrSet = masterNodeList[i].lineValue.begin(); itrSet != masterNodeList[i].lineValue.end(); itrSet++) {
                    WRITE << *itrSet << " ";
                }
                WRITE << endl;
            }
        }

        WRITE << endl;
    #endif
                
}

/* 
 * This function generates a vector of inputs to be tested for this circuit.
 */

void GenerateMasterInputVectors (int totalInputs) {
    
    #ifdef DEBUG
        WRITE << "================================================================================" << endl;
        WRITE << "===============        In --> GenerateMasterInputVectors         ===============" << endl;
        WRITE << "================================================================================" << endl << endl;
    #endif

    string resultVector;                                    // Input vector.
    vector <string> *tempInputVector;                       // We have to sort vectors by number of X in them.
    tempInputVector = new vector <string> [totalInputs];
    vector <string>::iterator itrVector;

    for (int i = 0; i < (pow(3, totalInputs) - 1); i++) {   // Go on converting numbers in base 3 system.
        resultVector = RadixConvert (i, 3);
        int numberOfX = 0;                                  // Total X in the vector.
        int positionOfX = resultVector.find_first_of('2');  // Iterate over the vector.
        while (positionOfX!=string::npos)
        {
            numberOfX++;                                    // Count number of 'X's
            resultVector[positionOfX] = '4';                // Replace 2 by 4 since we are using #define X 4.
            positionOfX = resultVector.find_first_of('2',(positionOfX + 1));
        }
        
        tempInputVector[numberOfX].push_back(resultVector);
    }

    // Now we have to put all the vectors in a large table.
    // There are no leading zeros in the string. We need them
    // in our simulation so padding is also done.
    for (int i = 0; i < totalInputs; i++) {
        for (itrVector = tempInputVector[i].begin(); itrVector != tempInputVector[i].end(); itrVector++) {
            resultVector = *itrVector;
            int vectorLength = resultVector.length();
	        for (int j = 0; j < (totalInputs - vectorLength); j++)
		        resultVector = "0" + resultVector;
            masterInputVector.push_back (resultVector);
        }
    }

    // We start from the case with most 'X's and go to the case with minimum 'X's.
    // For this we have to reverse the vector.
    reverse (masterInputVector.begin(), masterInputVector.end());

    #ifdef DEBUG
    WRITE << "Total Inputs = " << totalInputs << endl;
    WRITE << "------------------------------------------------------------" << endl << endl;
    WRITE << "Vector Table for this circuit." << endl;
    WRITE << "------------------------------------------------------------" << endl << endl;
    for (itrVector = masterInputVector.begin(); itrVector != masterInputVector.end(); itrVector++) {
        WRITE << *itrVector << endl;
    }
    WRITE << "------------------------------------------------------------" << endl << endl;
    #endif
}

/*
 * This function takes in the line number and the fault present at that line.
 * It also takes in an input vector to be checked.
 *
 * It then checks first if the fault can be excited. If the fault can be excited
 * then it checks whether the fault can be propagated to any one of the outputs.
 * If both of them result in true then the fault can be tested with the vector.
 */

bool CheckVectorForATPG (int totalInputs, int inLineNumber, bool inStuckAtValue, string inVector) {
    
    int *inputVector;
    inputVector = new int [totalInputs];
    set <int> *gateVector;
    gateVector = new set <int> [3];
    set <int>::iterator itrSet;

    // Convert the input vector string to integer
    // values to be applied to the circuit for simulation.
    // (- 48) is done to convert ascii to integer.
    for (int i = 0; i < totalInputs; i++) {
        inputVector[i] = inVector[i] - 48;
    }

    // Clear all the sets. This is important since we don't want
    // the previous values affecting current simulation.
    for (int i = 0, j = 0; i < masterNodeList.size(); i++) {
        masterNodeList[i].lineValue.clear();
    }

    // Assign the vector to inputs and keep all other values empty.
    for (int i = 0, j = 0; i < masterNodeList.size(); i++) {
        if (masterNodeList[i].numberFanIn == 0)
            masterNodeList[i].lineValue.insert(inputVector[j++]);
    }

    bool isFaultExcited = false;        // Check if the fault can be excited.
    bool isTestGenerated = false;       // Check if the test is generated.

    // For all the nodes in the circuit.
    //      -- For all the primary inputs.
    //          -- If the current line == the line where fault exists.
    //              -- If the fault can be excited.
    //                  -- Set the value to D or Dbar.
    for (int i = 0; i < masterNodeList.size(); i++) {
        if (masterNodeList[i].numberFanIn == 0) {
            if (inLineNumber == masterNodeList[i].lineNumber) {
                for (itrSet = masterNodeList[i].lineValue.begin(); itrSet != masterNodeList[i].lineValue.end(); itrSet++) {
                    if (*itrSet == 0 && inStuckAtValue == true) {
                        isFaultExcited = true;
                        masterNodeList[i].lineValue.clear();
                        masterNodeList[i].lineValue.insert(3);
                        break;
                    }
                    if (*itrSet == 1 && inStuckAtValue == false) {
                        isFaultExcited = true;
                        masterNodeList[i].lineValue.clear();
                        masterNodeList[i].lineValue.insert(2);
                        break;
                    }
                }
            }
        }
    }

    set <int> tempSet;  // To store the result of implication.

    // Calls implication on all the nodes going from input to output.
    // Implication would result in a set of single value at the output.
    // If there is a fault (will be considered in FaultSimulation) then
    // the output set will have multiple values and/or D and/or B (Dbar).
    for (int i = 0; i < masterNodeList.size(); i++) {                       // For all nodes.
        if (masterNodeList[i].numberFanIn != 0) {                           // Not a primary input.
            int inputNumber = 0;

            // If not a primary input then generate the input set to be given to implication function.
            for (itrSet = masterNodeList[i].listFanIn.begin(); itrSet != masterNodeList[i].listFanIn.end(); itrSet++) {
                for (int j = 0; j < masterNodeList.size(); j++) {
                    if (masterNodeList[j].lineNumber == *itrSet) {
                        gateVector[inputNumber++] = masterNodeList[j].lineValue;
                    }
                }
            }

            // Call proper implication function depending on the number of the inputs to the gate.
            // Overloaded function, please refer to logic_implication.cc for details.
            switch (masterNodeList[i].numberFanIn) {
                case 1:
                    tempSet = forwardImplication (masterNodeList[i].gateType, gateVector[0]);
                    break;
                case 2:
                    tempSet = forwardImplication (masterNodeList[i].gateType, gateVector[0], gateVector[1]);
                    break;
                case 3:
                    tempSet = forwardImplication (masterNodeList[i].gateType, gateVector[0], gateVector[1], gateVector[2]);
                    break;
                default:
                    cerr << "This ATPG generator only works if the number of inputs to a gate is less than 4." << endl;
                    cerr << "Currently there are " << masterNodeList[i].numberFanIn << " inputs to the gate." << endl;
                    exit (0);
                    break;
            }
            // If the current line number is the same as the line number with the fault.
            //      -- If the fault can be excited.
            //          -- Set proper value D or Dbar.
            if (inLineNumber == masterNodeList[i].lineNumber) {
                for (itrSet = tempSet.begin(); itrSet != tempSet.end(); itrSet++) {
                    if (*itrSet == 0 && inStuckAtValue == true) {
                        isFaultExcited = true;
                        tempSet.clear();
                        tempSet.insert(3);
                        break;
                    }
                    if (*itrSet == 1 && inStuckAtValue == false) {
                        isFaultExcited = true;
                        tempSet.clear();
                        tempSet.insert(2);
                        break;
                    }
                }
                // If fault cannot be excited, exit.
                if (isFaultExcited == false) {
                    return false;
                }
            }
            // If fault can be excited, assign the value D or Dbar to the line.
            masterNodeList[i].lineValue = tempSet;
        }
    }

    // Once the simulation is done, check all the outputs.
    //      -- If any output has either a D or a Dbar then the fault is detected.
    //      -- The vector can test the fault.
    for (int i = 0; i < masterNodeList.size(); i++) {
        if (masterNodeList[i].numberFanOut == 0) {
            for (itrSet = masterNodeList[i].lineValue.begin(); itrSet != masterNodeList[i].lineValue.end(); itrSet++) {
                if (*itrSet == 2 || *itrSet == 3) {
                    isTestGenerated = true;
                }
            }
        }
    }


    #ifdef DEBUG
        if (isTestGenerated) {
            WRITE << "================================================================================" << endl;
            WRITE << "===============            In --> CheckVectorForATPG             ===============" << endl;
            WRITE << "================================================================================" << endl << endl;
            WRITE << "There are " << CircuitNode::totalInputs << " inputs to this circuit." << endl << endl;

            for (int i = 0; i < totalInputs; i++) {
                WRITE << "Input Vector [" << i << "] is = " << inputVector[i] << endl;
            }
            WRITE << endl;
            
            for (int i = 0; i < masterNodeList.size(); i++) {
                if (masterNodeList[i].numberFanIn == 0) {
                    WRITE << "The value of the input at line number " << masterNodeList[i].lineNumber << " is = ";
                    for (itrSet = masterNodeList[i].lineValue.begin(); itrSet != masterNodeList[i].lineValue.end(); itrSet++) {
                        WRITE << *itrSet << " ";
                    }
                    WRITE << endl;
                }
            }
            WRITE << endl;

            for (int i = 0; i < masterNodeList.size(); i++) {
                if (masterNodeList[i].numberFanIn != 0) {
                    WRITE << "The value of the line number " << masterNodeList[i].lineNumber << " is = ";
                    for (itrSet = masterNodeList[i].lineValue.begin(); itrSet != masterNodeList[i].lineValue.end(); itrSet++) {
                        WRITE << *itrSet << " ";
                    }
                    WRITE << endl;
                }
            }
            WRITE << endl;

            for (int i = 0; i < masterNodeList.size(); i++) {
                if (masterNodeList[i].numberFanOut == 0) {
                    WRITE << "The value of the output at line number " << masterNodeList[i].lineNumber << " is = ";
                    for (itrSet = masterNodeList[i].lineValue.begin(); itrSet != masterNodeList[i].lineValue.end(); itrSet++) {
                        WRITE << *itrSet << " ";
                    }
                    WRITE << endl;
                }
            }

            WRITE << endl;
        }
    #endif

    if (isTestGenerated)
        return true;
    else
        return false;
}

/*
 * This function reads from the vector table.
 * It then applies each vector to CheckVectorForATPG.
 * If a test is generated then it returns true.
 * If no test is generated even after looking at all vectors, it returns false.
 */

bool TestAllVectorsATPG (int totalInputs, int inLineNumber, bool inStuckAtValue, vector <string> &inVectorList, vector <TestList> &inTestList) {
    
    #ifdef DEBUG
        WRITE << "================================================================================" << endl;
        WRITE << "===============          In --> TestAllVectorsForATPG            ===============" << endl;
        WRITE << "================================================================================" << endl << endl;
    #endif

    string outVector;
    
    for (int i = 0; i < inVectorList.size(); i++) {
        outVector = inVectorList[i];
        if (CheckVectorForATPG(totalInputs, inLineNumber, inStuckAtValue, outVector)) {
            TestList *thisTest;
            thisTest = new TestList(inLineNumber, inStuckAtValue, outVector);
            inTestList.push_back(*thisTest);
            delete thisTest;

            #ifdef DEBUG
                WRITE << endl;
                WRITE << "------------------------------------------------------------" << endl;
                WRITE << "The fault is at line number = " << inLineNumber << endl;
                WRITE << "The fault is stuck at = " << inStuckAtValue << endl;
                WRITE << "Test was generated by vector = " << outVector << endl;
                WRITE << "------------------------------------------------------------" << endl;
                WRITE << endl;
            #endif
            return true;
        }
    }

    return false;

}

/*
 * This function checks if a test can be obtained for every possible fault in the circuit.
 */
void CheckAllFaultsATPG (int totalInputs, vector <string> &inVectorList, vector <TestList> &inTestList, vector <FaultList> &inFaultList) {
    
    #ifdef DEBUG
        WRITE << "================================================================================" << endl;
        WRITE << "===============            In --> CheckAllFaultsATPG             ===============" << endl;
        WRITE << "================================================================================" << endl << endl;
    #endif

    for (int i = 0; i < inFaultList.size(); i++) {
        if(!TestAllVectorsATPG (totalInputs, inFaultList[i].lineNumber, inFaultList[i].stuckAtValue, inVectorList, inTestList)) {
            TestList *thisTest;
            thisTest = new TestList(inFaultList[i].lineNumber, inFaultList[i].stuckAtValue);
            masterTestList.push_back(*thisTest);
            delete thisTest;

            #ifdef DEBUG
                WRITE << endl;
                WRITE << "------------------------------------------------------------" << endl;
                WRITE << "The fault is at line number = " << inFaultList[i].lineNumber << endl;
                WRITE << "The fault is stuck at = " << inFaultList[i].stuckAtValue << endl;
                WRITE << "Test cannot be generated for this fault." << endl;
                WRITE << "------------------------------------------------------------" << endl;
                WRITE << endl;
            #endif
        }
    }

}


/*
 * ============================================================================
 * NOTE - this function is just written to check the working of all the 
 * methods defined above. This has no connection with how the final program
 * would work.
 * ============================================================================
 */

int main (int argc, char *argv[]) {

    #ifdef LOG
        openOutFile((char *)LOG_FILE_NAME, logFile);
        WRITE << "================================================================================" << endl;
        WRITE << "===============                                                  ===============" << endl;
        WRITE << "===============          ATPG Generator - Podem Log File         ===============" << endl;
        WRITE << "===============                                                  ===============" << endl;
        WRITE << "================================================================================" << endl;
        WRITE << endl;
        WRITE << "Generated On --> " << endl;
        WRITE << endl;
        WRITE << "================================================================================" << endl;
        WRITE << endl << endl << endl;
    #endif

    ifstream inFile;
    ofstream outFile;
    
    if (argc <= 2 || argc > 3) {
        cerr << "ERROR: Usage: " << argv[0] << " <Circuit Filename> <Faultlist Filename>" << endl;
        exit(1);
    }
    int i, j;
    openInFile (argv[1], inFile);
    ReadCircuit (inFile);
    inFile.close();

    openInFile (argv[2], inFile);
    ReadFaultList(inFile);
    inFile.close();

    SetLineLevel(masterNodeList);   
    CreateFaultObjects(masterLineList, masterNodeList);
    CollapseFaults(masterLineList, masterNodeList);
    MakeFaultList();
    GenerateMasterInputVectors(CircuitNode::totalInputs);

    vector <string>::iterator itrVector;
    for (itrVector = masterInputVector.begin(); itrVector != masterInputVector.end(); itrVector++) {
        SimpleLogicSimulation(CircuitNode::totalInputs, *itrVector);
    }
    cout << "Checking for ATPG" << endl;
    CheckAllFaultsATPG(CircuitNode::totalInputs, masterInputVector, masterTestList, masterFaultList);

    for (int i = 0; i < masterTestList.size(); i++) {
        cout << "Test Information For" << endl;
        cout << "------------------------------------------------------------" << endl;
        cout << "Line Number Is = " << masterTestList[i].lineNumber << endl;
        cout << "Fault Is Stuck At = " << masterTestList[i].stuckAtValue << endl;
        cout << "Test Vector Is = " << masterTestList[i].testVector << endl;
        cout << endl;
    }

    for (int i = 0; i < masterTestList.size(); i++) {
        masterTestVector.push_back(masterTestList[i].testVector);
    }
    cout << "=========================================================" << endl;
    cout << "=========================================================" << endl;
    cout << "=========================================================" << endl;
    cout << "=========================================================" << endl;
    cout << "Checking for given faults." << endl;
    cout << "=========================================================" << endl;
    cout << "=========================================================" << endl;
    cout << "=========================================================" << endl;
    CheckAllFaultsATPG(CircuitNode::totalInputs, masterTestVector, providedTestList, providedFaultList);

    for (int i = 0; i < providedTestList.size(); i++) {
        cout << "Test Information For" << endl;
        cout << "------------------------------------------------------------" << endl;
        cout << "Line Number Is = " << providedTestList[i].lineNumber << endl;
        cout << "Fault Is Stuck At = " << providedTestList[i].stuckAtValue << endl;
        cout << "Test Vector Is = " << providedTestList[i].testVector << endl;
        cout << endl;
    }

    /*
     * ============================================================================
     *  NOTE TO AWATI - 
     *  Don't pay attention to this - just copied this from that crappy C code to 
     *  check the output and make sure all the objects are properly populated.
     * ============================================================================
     */ 

    #ifdef DEBUG_PRINT
        set <int>::iterator it;
        for (i = 0; i < masterNodeList.size(); i++) {
            cout << "-----------------------------------------------------" << endl;
            cout << endl << endl; 
            cout << " Node   Type \tIn\t\tOut\t\tFaults" << endl;
            for(i = 0; i<CircuitNode::totalNodes; i++) {
                cout << "\t\t\t\t";
                for(it = masterNodeList[i].listFanOut.begin(); it != masterNodeList[i].listFanOut.end(); it++) {
                    cout << *it << ", ";
                }
                cout << "\b\b ";

                cout << "\r\t\t\t\t\t\t";

                printf("\r%5d      %d\t", masterNodeList[i].lineNumber, masterNodeList[i].gateType);
               
                for(it = masterNodeList[i].listFanIn.begin(); it != masterNodeList[i].listFanIn.end(); it++) {
                    cout << *it << ", ";
                }
                cout << "\b\b ";
               
                cout << endl;
            }

            cout << endl << endl;
            cout << "Number of nodes = " << CircuitNode::totalNodes << endl;
            cout << "Number of primary inputs = " << CircuitNode::totalInputs << endl;
            cout << "List of inputs = ";
            for(i = 0; i<CircuitNode::totalNodes; i++) {
                if (masterNodeList[i].numberFanIn == 0)
                    cout << masterNodeList[i].lineNumber << ", ";
            }
            cout << "\b\b " << endl;
            cout << "Number of primary outputs = " << CircuitNode::totalOutputs << endl;
            cout << "List of outputs = ";
            for(i = 0; i<CircuitNode::totalNodes; i++) {
                if (masterNodeList[i].numberFanOut == 0)
                    cout << masterNodeList[i].lineNumber << ", ";
            }
            cout << "\b\b " << endl;

        }
    #endif

    #ifdef LOG
        logFile.close();
    #endif
}

