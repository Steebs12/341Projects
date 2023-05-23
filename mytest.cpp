// CMSC 341 - Spring 2022 - Project 4
#include "dnadb.h"
#include <random>
#include <vector>
#include <math.h>
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
    void setSeed(int seedNum) {
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
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
    //nc = non-colliding
    //c = colliding

    bool normalInsert();
    bool errorCaseInsert();
    bool getError();
    bool normalRemove();
    /* bool getNonCollide();*/
    bool getCollision();
};

unsigned int hashCode(const string str);
string sequencer(int size, int seedNum);

int main() {
    Tester test;

    vector<DNA> dataList;
    Random RndLocation(MINLOCID, MAXLOCID);
    DnaDb dnadb(MINPRIME, hashCode);
    bool result = true;
    for (int i = 0; i < 49; i++) {
        // generating random data
        DNA dataObj = DNA(sequencer(5, i), RndLocation.getRandNum());
        // saving data for later use
        dataList.push_back(dataObj);
        // inserting data in to the DnaDb object
        dnadb.insert(dataObj);
    }
    // dumping the data points to the standard output
    /*dnadb.dump();*/
    // checking whether all data are inserted

   /* for (vector<DNA>::iterator it = dataList.begin(); it != dataList.end(); it++){
        result = result && (*it == dnadb.getDNA((*it).getSequence(), (*it).getLocId()));
    }
    if (result)
        cout << "All data points exist in the DnaDb!\n";
    else
        cout << "Some data points are missing in DnaDb\n";
    */


    if (test.normalInsert()) {
        cout << "Normal insert test passed!" << endl;

    }
    else {
        cout << "Normal Insert Test Failed!" << endl;
    }
    if (test.errorCaseInsert()) {
        cout << "Error Insert test passed!" << endl;
    }
    else {
        cout << "Error insert test Failed! " << endl;
    }
    if (test.getError()) {
        cout << "getError test Passed!" << endl;
    }
    else {
        cout << "getError test failed!" << endl;
    }
    cout << "Testing Normal Remove..." << endl;
    cout << (test.normalRemove() ? "Passed!" : "Failed") << endl;

    /*cout << "Testing getCollision..." << endl;
    cout << (test.getCollision() ? "Passed" : "Fail") <<endl;*/

    return 0;

}

unsigned int hashCode(const string str) {
    unsigned int val = 0;
    const unsigned int thirtyThree = 33;  // magic number from textbook
    for (unsigned int i = 0; i < str.length(); i++)
        val = val * thirtyThree + str[i];
    return val;
}

string sequencer(int size, int seedNum) {
    //this function returns a random DNA sequence
    string sequence = "";
    Random rndObject(0, 3);
    rndObject.setSeed(seedNum);
    for (int i = 0; i < size; i++) {
        sequence = sequence + ALPHA[rndObject.getRandNum()];
    }
    return sequence;
}
//check if its inserted
//check index
//check sequence 
//check id 
//check capacity 
//check numDeleted 
//check size
bool Tester::normalInsert() {
    const int NUMINSERT = MINPRIME / 7;
    bool testCase = true;
    DnaDb table(MINPRIME, hashCode);
    Random randID(MINLOCID, MAXLOCID);
    vector <DNA> DNAList;
    int index = 0;


    vector<DNA> dataList;
    Random RndLocation(MINLOCID, MAXLOCID);
    DnaDb dnadb(MINPRIME, hashCode);
    bool result = true;
    for (int i = 0; i < 51; i++) {

        DNA dataObj = DNA(sequencer(5, i), RndLocation.getRandNum());

        dataList.push_back(dataObj);


        bool test = dnadb.insert(dataObj);
        if (test == false) {
            return false;
            cout << "3" << endl;
        }

    }
    for (vector<DNA>::iterator it = dataList.begin(); it != dataList.end(); it++) {
        result = result && (*it == dnadb.getDNA((*it).getSequence(), (*it).getLocId()));
    }
    if (result == false) {
        cout << "1" << endl;
        testCase = false;
    }
    dnadb.dump();
    return testCase;
}

bool Tester::errorCaseInsert() {
    Random RndLocation(MINLOCID, MAXLOCID);
    DnaDb dnadb(MINPRIME, hashCode);
    DNA dataObj1 = DNA("AAAA", MINLOCID);
    dnadb.insert(dataObj1);
    if (dnadb.insert(dataObj1) == false) {
        return true;
    }
    else {
        return false;
    }

}

bool Tester::getError() {
    Random RndLocation(MINLOCID, MAXLOCID);
    DnaDb dnadb(MINPRIME, hashCode);
    DNA dataObj1 = DNA("ACCT", MINLOCID);
    dnadb.insert(dataObj1);

    if (dnadb.getDNA("ACCT", MINLOCID + 1) == EMPTY) {
        return true;
    }
    else {
        return false;
    }
}

//bool Tester::getNonCollide() {
//    const int NUMINSERT = MINPRIME / 7;
//    bool testCase = true;
//    DnaDb table(MINPRIME, hashCode);
//    Random randID(MINLOCID, MAXLOCID);
//    vector <DNA> DNAList;
//    int index = 0;
//
//
//    vector<DNA> dataList;
//    Random RndLocation(MINLOCID, MAXLOCID);
//    DnaDb dnadb(MINPRIME, hashCode);
//    bool result = true;
//    for (int i = 0; i < 7; i++) {
//        // generating random data
//        DNA dataObj = DNA(sequencer(5, i), RndLocation.getRandNum());
//        // saving data for later use
//        dataList.push_back(dataObj);
//        // inserting data in to the DnaDb object
//
//        bool test = dnadb.insert(dataObj);
//        if (test == false) {
//            return false;
//        }
//
//    }
//    for (int i = 0; i < dataList.size(); i++) {
//        if ((table.getDNA(dataList[i].getSequence(), dataList[i].getLocId()) == EMPTY)) {
//            testCase = false;
//        }
//        if (table.m_currentCap != MINPRIME) {
//            testCase = false;
//        }
//        if (table.m_currentSize != 7) {
//            testCase = false;
//        }
//        if (table.m_currNumDeleted != 0) {
//            testCase = false;
//        }
//        
//    }
//}

bool Tester::normalRemove() {

    bool testCase = true;
    DnaDb table(MINPRIME, hashCode);
    DNA dataObj1 = DNA("ACGT", MINLOCID + 1);
    DNA dataObj2 = DNA("ACCT", MINLOCID + 2);
    DNA dataObj3 = DNA("AAAT", MINLOCID + 3);
    DNA dataObj4 = DNA("GGGG", MINLOCID + 4);
    int numberRemove = 2;
    table.insert(dataObj1);
    table.insert(dataObj2);
    table.insert(dataObj3);
    table.insert(dataObj4);


    /*table.dump();*/

    try {
        if (table.remove(dataObj2) == false) {

            throw testCase;

        }

        if (table.remove(dataObj4) == false) {

            throw testCase;

        }


        if (testCase == true) {
            testCase = true;
        }
        if (table.m_currentSize != 4) {

            throw(testCase);
        }
        if (table.m_currentCap != MINPRIME) {
            throw(testCase);
        }
        if (table.m_currNumDeleted != 2) {
            throw(testCase);
        }
    }
    catch (...) {
        testCase = false;
        cout << "remove Test failed!" << endl;

    }


    return testCase;
}
//bool Tester::getCollision() {
//
//}
