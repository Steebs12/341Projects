// CMSC 341 - Spring 2022 - Project 4
#include "dnadb.h"
#include <random>
#include <vector>
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
    bool normalInsert();
    bool testFind();
    bool testFindNonColliding();
    bool testFindColliding();
    bool testRemoveNonColliding();
    bool testRemoveColliding();
    bool testTriggerHash();
    bool testLoadFactorCompletion();
};

unsigned int hashCode(const string str);
string sequencer(int size, int seedNum);

int main() {
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
    dnadb.dump();
    // checking whether all data are inserted
    for (vector<DNA>::iterator it = dataList.begin(); it != dataList.end(); it++) {
        result = result && (*it == dnadb.getDNA((*it).getSequence(), (*it).getLocId()));
    }
    if (result)
        cout << "All data points exist in the DnaDb!\n";
    else
        cout << "Some data points are missing in DnaDb\n";

    cout << "\n" << "Testing Phase" << endl;
    Tester test;

    if (test.normalInsert()) {
        cout << "Test Normal Insert Passed" << endl;
    }
    else {
        cout << "Test Normal Insert failed" << endl;
    }

    if (test.testFind()) {
        cout << "Test Find Error Case Passed" << endl;
    }
    else {
        cout << "Test Find Error Case failed" << endl;
    }

    if (test.testFindNonColliding()) {
        cout << "Test Find Non Colliding Passed" << endl;
    }
    else {
        cout << "Test Find Non Colliding failed" << endl;
    }

    if (test.testFindColliding()) {
        cout << "Test Find Colliding Passed" << endl;
    }
    else {
        cout << "Test Find Colliding failed" << endl;
    }

    if (test.testRemoveNonColliding()) {
        cout << "Test Remove Non Colliding Passed" << endl;
    }
    else {
        cout << "Test Remove Non Colliding Failed" << endl;
    }

    if (test.testRemoveColliding()) {
        cout << "Test Remove Colliding Passed" << endl;
    }
    else {
        cout << "Test Remove Colliding Failed" << endl;
    }

    if (test.testTriggerHash()) {
        cout << "Test Trigger Hash Passed" << endl;
    }
    else {
        cout << "Test Trigger Hash Failed" << endl;
    }

    if (test.testLoadFactorCompletion()) {
        cout << "Test Load Factor Completion Passed" << endl;
    }
    else {
        cout << "Test Load Factor Completion Failed" << endl;
    }

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

bool Tester::normalInsert() {
    unsigned int counter = 0;
    bool testOutcome = true;
    vector<DNA> dataList;
    Random RndLocation(MINLOCID, MAXLOCID);
    DnaDb dnadb(MINPRIME, hashCode);
    bool result = true;
    for (int i = 0; i < 49; i++) {
        DNA dataObj = DNA(sequencer(5, i), i + MINLOCID);
        dataList.push_back(dataObj);
        if (!(dnadb.insert(dataObj))) {
            return false;
        }
        counter++;
    }

    for (vector<DNA>::iterator it = dataList.begin(); it != dataList.end(); it++) {
        result = result && (*it == dnadb.getDNA((*it).getSequence(), (*it).getLocId()));
    }
    if (!(result)) {
        testOutcome = false;
    }
    if (dnadb.m_currentSize != counter) {
        testOutcome = false;
    }

    return testOutcome;
}

bool Tester::testFind() {
    bool testOutcome = true;
    Random RndLocation(MINLOCID, MAXLOCID);
    DnaDb dnadb(MINPRIME, hashCode);
    DNA dataObj = DNA("ACGT", 1000);
    if (!(dnadb.insert(dataObj))) {
        return false;
    }
    if (dnadb.getDNA("AAAA", 1000) == EMPTY) {
        return testOutcome;
    }
    else {
        testOutcome = false;
        return testOutcome;
    }
}

bool Tester::testFindNonColliding() {
    int counter = 0;
    DnaDb dnadb(MINPRIME, hashCode);
    DNA dataObj1 = DNA("AAAA", MINLOCID);
    DNA dataObj2 = DNA("CCCC", MINLOCID + 1);
    DNA dataObj3 = DNA("GGGG", MINLOCID + 2);
    DNA dataObj4 = DNA("TTTT", MINLOCID + 3);
    dnadb.insert(dataObj1);
    dnadb.insert(dataObj2);
    dnadb.insert(dataObj3);
    dnadb.insert(dataObj4);
    if (!(dnadb.getDNA("AAAA", 1000) == EMPTY)) {
        counter++;
    }
    if (!(dnadb.getDNA("CCCC", 1001) == EMPTY)) {
        counter++;
    }
    if (!(dnadb.getDNA("GGGG", 1002) == EMPTY)) {
        counter++;
    }
    if (!(dnadb.getDNA("TTTT", 1003) == EMPTY)) {
        counter++;
    }

    if (counter == 4) {
        return true;
    }
    else {
        return false;
    }
}

bool Tester::testFindColliding() {
    int counter = 0;
    bool testOutcome = true;
    vector<DNA> dataList;
    Random RndLocation(MINLOCID, MAXLOCID);
    DnaDb dnadb(MINPRIME, hashCode);
    bool result = true;
    for (int i = 0; i < 49; i++) {
        DNA dataObj = DNA("AAAA", i + MINLOCID);
        dataList.push_back(dataObj);
        dnadb.insert(dataObj);
        counter++;
    }

    for (vector<DNA>::iterator it = dataList.begin(); it != dataList.end(); it++) {
        result = result && (*it == dnadb.getDNA((*it).getSequence(), (*it).getLocId()));
    }
    if (!(result)) {
        testOutcome = false;
    }

    return testOutcome;
}

bool Tester::testRemoveNonColliding() {
    int counter = 0;
    vector<DNA> dataList;
    Random RndLocation(MINLOCID, MAXLOCID);
    DnaDb dnadb(MINPRIME, hashCode);


    for (int i = 0; i < 13; i++) {
        DNA dataObj = DNA(sequencer(5, i), i + MINLOCID);
        dataList.push_back(dataObj);
        dnadb.insert(dataObj);
        counter++;
    }

    int index1 = (dataList.size() - 1);
    int index2 = (dataList.size() - 1) / 4;
    int index3 = (dataList.size() - 1) / 6;

    dnadb.remove(dataList[index1]);
    dnadb.remove(dataList[index2]);
    dnadb.remove(dataList[index3]);

    if (!(dnadb.getDNA(dnadb.returnDNASequence(dataList[index1]), dnadb.returnDNALocation(dataList[index1])) == EMPTY)) {
        return false;
    }

    if (!(dnadb.getDNA(dnadb.returnDNASequence(dataList[index2]), dnadb.returnDNALocation(dataList[index2])) == EMPTY)) {
        return false;
    }

    if (!(dnadb.getDNA(dnadb.returnDNASequence(dataList[index3]), dnadb.returnDNALocation(dataList[index3])) == EMPTY)) {
        return false;
    }

    return true;
}

bool Tester::testRemoveColliding() {
    int counter = 0;
    vector<DNA> dataList;
    Random RndLocation(MINLOCID, MAXLOCID);
    DnaDb dnadb(MINPRIME, hashCode);
    for (int i = 0; i < 13; i++) {
        DNA dataObj = DNA("AAAA", i + MINLOCID);
        dataList.push_back(dataObj);
        dnadb.insert(dataObj);
        counter++;
    }

    //dnadb.dump();

    int index1 = (dataList.size() - 1);
    int index2 = (dataList.size() - 1) / 4;
    int index3 = (dataList.size() - 1) / 6;

    dnadb.remove(dataList[index1]);
    dnadb.remove(dataList[index2]);
    dnadb.remove(dataList[index3]);

    //dnadb.dump();


    if (!(dnadb.getDNA(dnadb.returnDNASequence(dataList[index1]), dnadb.returnDNALocation(dataList[index1])) == EMPTY)) {
        return false;
    }

    if (!(dnadb.getDNA(dnadb.returnDNASequence(dataList[index2]), dnadb.returnDNALocation(dataList[index2])) == EMPTY)) {
        return false;
    }

    if (!(dnadb.getDNA(dnadb.returnDNASequence(dataList[index3]), dnadb.returnDNALocation(dataList[index3])) == EMPTY)) {
        return false;
    }

    return true;
}

bool Tester::testTriggerHash() {
    Random RndLocation(MINLOCID, MAXLOCID);
    DnaDb dnadb(MINPRIME, hashCode);
    for (int i = 0; i < 51; i++) {
        DNA dataObj = DNA(sequencer(5, i), RndLocation.getRandNum());
        if (!(dnadb.insert(dataObj))) {
            return false;
        }
    }
    return true;
}

bool Tester::testLoadFactorCompletion() {
    vector<DNA> dataList;
    Random RndLocation(MINLOCID, MAXLOCID);
    DnaDb dnadb(MINPRIME, hashCode);
    DNA dataObj1 = DNA("AAAA", MINLOCID);
    DNA dataObj2 = DNA("CCCC", MINLOCID + 1);
    DNA dataObj3 = DNA("GGGG", MINLOCID + 2);
    DNA dataObj4 = DNA("TTTT", MINLOCID + 3);
    DNA dataObj5 = DNA("ACGT", MINLOCID + 4);
    DNA dataObj6 = DNA("ACCT", MINLOCID + 5);

    for (int i = 0; i < 51; i++) {
        DNA dataObj = DNA(sequencer(5, i), RndLocation.getRandNum());
        dataList.push_back(dataObj);
        dnadb.insert(dataObj);
    }
    dnadb.insert(dataObj1);
    dnadb.insert(dataObj2);
    dnadb.insert(dataObj3);
    dnadb.insert(dataObj4);
    //dnadb.dump();
    dnadb.insert(dataObj5);
    dnadb.insert(dataObj6);

    //dnadb.dump();
    if (dnadb.m_oldTable == nullptr) {
        return true;
    }
    else {
        return false;
    }

    return true;
}