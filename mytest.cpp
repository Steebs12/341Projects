// UMBC - CMSC 341 - Spring 2022 - Proj2
#include "fleet.h"
#include <random>
#include <iostream>

enum RANDOM { UNIFORMINT, UNIFORMREAL, NORMAL };
class Random {
public:
    Random(int min, int max, RANDOM type = UNIFORMINT, int mean = 50, int stdev = 20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL) {
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean, stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min, max);
        }
        else { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min, (double)max);
        }
    }

    int getRandNum() {
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if (m_type == NORMAL) {
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while (result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT) {
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum() {
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result * 100.0) / 100.0;
        return result;
    }

private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};
class Tester {
public:
    /******************************************
    * Test function declarations go here! *
    ******************************************/
    bool testNormalInsert();
    bool testEdgeInsert();
    bool testErrorInsert();
    //bool recursiveIterator(Ship* thing);
    //bool testBalance();
    //bool testBSTPreserved();
    //bool testRemoveNormal();
    //bool testEdgeRemove();
    //bool testBalanceRemove();
    //bool testBSTPreservedRemove();
    //bool testRemoveLost();
    bool testBalance();


};


bool Tester::testNormalInsert() {//test insertion for a normal case
    Random idGen(MINID, MAXID);
    Random typeGen(0, 4);
    Fleet fleet;
    int fleetSize = 50;
    int ID = 0;
    for (int i = 0; i < fleetSize; i++) {
        ID = idGen.getRandNum();
        Ship ship(ID, static_cast<SHIPTYPE>(typeGen.getRandNum()));
        fleet.insert(ship);
    }
    return true;
}

bool Tester::testEdgeInsert() {//test insertion for an edge case
    Random idGen(MINID, MAXID);
    Random typeGen(0, 4);
    Fleet fleet;
    int fleetSize = 1;
    int ID = 0;
    for (int i = 0; i < fleetSize; i++) {
        ID = idGen.getRandNum();
        Ship ship(ID, static_cast<SHIPTYPE>(typeGen.getRandNum()));
        fleet.insert(ship);
    }
    return true;
}

bool Tester::testErrorInsert() {//test insertion for an error case
    Random idGen(MINID, MAXID);
    Random typeGen(0, 4);
    Fleet fleet;
    int fleetSize = 1;
    int ID = 0;
    for (int i = 0; i < fleetSize; i++) {
        ID = 12345;
        Ship ship(ID, static_cast<SHIPTYPE>(typeGen.getRandNum()));
        fleet.insert(ship);
    }
    Ship ship2(12345, static_cast<SHIPTYPE>(typeGen.getRandNum()));
    fleet.insert(ship2);
    return true;
}
bool Tester::testBalance() {
    return true;
}

int main() {
    
    Random idGen(MINID, MAXID);
    Random typeGen(0, 4);
    {
        Fleet fleet;
        int fleetSize = 10;
        int tempID = 0;
        int ID = 0;
        for (int i = 0; i < fleetSize; i++) {
            ID = idGen.getRandNum();
            if (i == fleetSize / 2) tempID = ID;//we store this ID for later use
            Ship ship(ID, static_cast<SHIPTYPE>(typeGen.getRandNum()));
            fleet.insert(ship);
            fleet.dumpTree();
            cout << endl;
        }

        cout << "\nDump after inserting " << fleetSize << " nodes:\n\n";
        fleet.dumpTree();
        cout << "\n\nList of Ships after inserting " << fleetSize << " nodes:\n";
        fleet.listShips();
        cout << endl;
        fleet.remove(tempID);
        cout << "\nDump after removing the node with ID: " << tempID << "\n\n";
        fleet.dumpTree();
        cout << "\n\nList of Ships after removing the node with ID: " << tempID << "\n";
        fleet.listShips();
        cout << endl;

    }
    /*
    Tester Test;
    if (Test.testNormalInsert()) {
        cout << "Normal Insert Test Passed" << endl;
    }
    if (Test.testEdgeInsert()) {
        cout << "Edge Insert Test Passed" << endl;
    }
    Test.testErrorInsert();
    */

    return 0;
}
