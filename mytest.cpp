// UMBC - CMSC 341 - Spring 2022 - Proj0
//NAME: Caleb Stieber
//CLASS: CMSC 341
//DESCRIPTION: This cpp file is responsible for running all of the tests that prove that our code works after rigorous testing. In addition, it measures the time it takes for
//our code to input data into different sized arrays.

#include "section.h"

const int ITEMS = 10;
const int STUDENTS = 10;
const int NORMAL_TEST = 5;
const int ERROR = -1;
const int EDGE = 1;
const int ZERO = 0;
const double FAILED_SIG = 0.0;

class Tester {
public:
    // the following is implemented as an example of measurement
    void measureInsertionTime(int numTrials, int inputSize);
    // the following is implemented as test cases for student average
    bool testStudentAveErrorCase();
    bool testStudentAveNormalCase();
    bool testStudentAveEdgeCase();

    /******************************************
    * Test function declarations go here! *
    ******************************************/

    //the following is implemented as test cases for quizes
    bool testQuizAveErrorCase(); 
    bool testQuizAveNormalCase();
    bool testQuizAveEdgeCase();

    //the following is implemented as a test cases for class average
    bool testClassAveErrorCase();
    bool testClassAveNormalCase();
    bool testClassAveEdgeCase();

    void testCopyConstructErrorCase();
    void testCopyConstructNormalCase();
    void testCopyConstructEdgeCase();

};
int main() {
    Tester tester;// test object
    {
        // the following is a sample usage of the Section class
        // create an object with 5 grading items and 8 students
        cout << "\nSample usage of the Section class:\n\n";
        Section aSection(ITEMS,STUDENTS);
        aSection.insert();
        aSection.dump();
    }
    {
        // measuring the running time of insertion
        cout << "\nMeasuring the performance of insertion function:\n\n";
        // measuring the insertion of 100x100 grades, and 200x200 grades
        // the 100x100 grades is determined by 100 items times 100 students
        tester.measureInsertionTime(2, 100);
    }
    {
        // testing Error case for calculation of student average
        cout << "\nTesting studentAverage(...), Error case:\n\n";
        if (tester.testStudentAveErrorCase() == true)
            cout << "\tError case (student index does not exist) of studentAverage() passed!\n";
        else
            cout << "\tError case (student index does not exist) of studentAverage() failed!\n";
    }
    {
        // testing Normal case for calculation of student average
        cout << "\nTesting studentAverage(...), Normal case:\n\n";
        if (tester.testStudentAveNormalCase() == true)
            cout << "\tNormal case (student index falls in bounds) of studentAverage() passed!\n";
        else
            cout << "\tNormal case (student index falls in bounds) of studentAverage() failed!\n";
    }
    {
        // testing Edge case for calculation of student average
        cout << "\nTesting studentAverage(...), Edge case:\n\n";
        if (tester.testStudentAveEdgeCase() == true)
            cout << "\tEdge case (student index on edge of array) of studentAverage() passed!\n";
        else
            cout << "\tEdge case (student index on edge of array) of studentAverage() failed!\n";
    }
    {
        // testing Error case for calculation of quiz average
        cout << "\nTesting quizAverage(...), Error case:\n\n";
        if (tester.testQuizAveErrorCase() == true)
            cout << "\tError case (quiz index does not exist) of quizAverage() passed!\n";
        else
            cout << "\tError case (quiz index does not exist) of quizAverage() failed!\n";
    }
    {
        // testing Normal case for calculation of quiz average
        cout << "\nTesting quizAverage(...), Normal case:\n\n";
        if (tester.testQuizAveNormalCase() == true)
            cout << "\tNormal case (quiz index is in bounds) of quizAverage() passed!\n";
        else
            cout << "\tNormal case (quiz index is in bounds) of quizAverage() failed!\n";
    }
    {
        // testing Edge case for calculation of quiz average
        cout << "\nTesting quizAverage(...), Edge case:\n\n";
        if (tester.testQuizAveEdgeCase() == true)
            cout << "\tEdge case (quiz index is on edge of array) of quizAverage() passed!\n";
        else
            cout << "\tEdge case (quiz index is on edge of array) of quizAverage() failed!\n";
    }
    {
        // testing Error case for calculation of class average
        cout << "\nTesting classAverage(...), Error case:\n\n";
        if (tester.testClassAveErrorCase() == true)
            cout << "\tError case (no students in Section) of classAverage() passed!\n";
        else
            cout << "\tError case (no students in Section) of classAverage() failed!\n";
    }
    {
        // testing Normal case for calculation of class average
        cout << "\nTesting classAverage(...), Normal case:\n\n";
        if (tester.testClassAveNormalCase() == true)
            cout << "\tNormal case (students are in bounds and not an edge case) of classAverage() passed!\n";
        else
            cout << "\tNormal case (students are in bounds and not an edge case) of classAverage() failed!\n";
    }
    {
        // testing Edge case for calculation of class average
        cout << "\nTesting classAverage(...), Edge case:\n\n";
        if (tester.testClassAveEdgeCase() == true)
            cout << "\tEdge case (average of an edge case array 1x1) of classAverage() passed!\n";
        else
            cout << "\tEdge case (average of an edge case array 1x1) of classAverage() failed!\n";
    }
    tester.testCopyConstructErrorCase();
    cout << endl;
    tester.testCopyConstructNormalCase();
    cout << endl;
    tester.testCopyConstructEdgeCase();
    cout << endl;
    
    cout << endl;
    return 0;
}

void Tester::measureInsertionTime(int numTrials, int inputSize)
{
    //Measuring the efficiency of insertion algorithm with system clock ticks
    //Clock ticks are units of time of a constant but system-specific length, as those returned by function clock().
    //Dividing a count of clock ticks by CLOCKS_PER_SEC yields the number of seconds.
    const int a = 2;//scaling factor for input size
    double T = 0.0;//to store running times
    clock_t start, stop;//stores the clock ticks while running the program
    for (int k = 0; k < numTrials; k++)
    {
        Section testObject(inputSize, inputSize);
        start = clock();
        testObject.insert();//the algorithm to be analyzed for efficiency
        stop = clock();
        T = stop - start;//number of clock ticks the algorithm took
        cout << "\tInserting " << inputSize * inputSize << " members took " << T << " clock ticks (" << T / CLOCKS_PER_SEC << " seconds)!" << endl;
        inputSize = inputSize * a;//increase the input size by the scaling factor
    }
}

bool Tester::testStudentAveErrorCase() {//test for out of bounds index case
    bool result = true;
    Section testObject(ITEMS, STUDENTS);
    testObject.insert();
    //testObject.dump();
    double average = testObject.studentAverage(ERROR);// out of range index
    if (average == FAILED_SIG) // this means something was out of range
        result = true;
    else {// if nothing out of range, the error case fails
        result = false;
    }   
    return result;
}

bool Tester::testStudentAveNormalCase() {//test for normal index case
    bool result = true;
    Section testObject(ITEMS, STUDENTS);
    testObject.insert();
    //testObject.dump();
    double average = testObject.studentAverage(NORMAL_TEST);// normal case index
    if (average == FAILED_SIG)// because it's a normal case, if the average return's 0.0 then the normal case test would fail
        result = false;
    else {
        // if student average returns an average in normal case then we know it passed
        result = true;
    }
    return result;
}

bool Tester::testStudentAveEdgeCase() {//test for edge index case
    bool result = true;
    Section testObject(ITEMS, STUDENTS);
    testObject.insert();
    //testObject.dump();
    double average = testObject.studentAverage(EDGE);// edge case index
    //if 0.0 is returned, we know the edge case failed so it returns false
    if (average == FAILED_SIG)
        result = false;
    else {
        //otherwise, if we get a value we know an average was returned from an edge case
        result = true;
    }
    return result;
}

bool Tester::testQuizAveErrorCase() {//test for out of bounds index case
    bool result = true; 
    int quizNum = ERROR;//out of bounds index
    Section testObject(ITEMS, STUDENTS);
    testObject.insert();
    //testObject.dump();
    double average = testObject.quizAverage(quizNum);
    //if quizAverage returns 0.0, that means the index was out of bounds
    if (average == FAILED_SIG) {
        result = true;
        return result;
    }
    else {
        //if it returns a value the index was in bounds
        result = false;
        return result;
    }
}; 
bool Tester::testQuizAveNormalCase() {//test for a normal index case 
    bool result = true;
    int quizNum = NORMAL_TEST;//normal index case (index is in bounds)
    Section testObject(ITEMS, STUDENTS);
    testObject.insert();
    //testObject.dump();
    double average = testObject.quizAverage(quizNum);
    if (average == FAILED_SIG) { //if it returns 0.0 then the index is out of bounds
        result = false;
        return result;
    }
    else {//if it returns a value we know the index was in bounds
        result = true;
        return result;
    }
};
bool Tester::testQuizAveEdgeCase() {//edge index case (index takes place on the adge of the array)
    bool result = true;
    int quizNum = EDGE;//edge index
    Section testObject(ITEMS, STUDENTS);
    testObject.insert();
    //testObject.dump();
    double average = testObject.quizAverage(quizNum);
    if (average == FAILED_SIG) {//if it returns 0.0 then the index is out of bounds
        result = false;
        return result;
    }
    else {//if it returns a value we know the edge index was in bounds
        result = true;
        return result;
    }
};

bool Tester::testClassAveErrorCase() {//Out of bounds case for class average
    bool result = true;
    Section testObject(ZERO, ZERO);//out of bounds case
    testObject.insert();
    //testObject.dump();
    double classAve = testObject.classAverage();
    if (classAve == FAILED_SIG) {//classAve will return 0.0 if m_grades is a 0x0 array, thus being out of bounds
        result = true;
        return result;
    }
    else {//if it's not out of bounds, it will return the average of the class
        result = false;
        return result;
    }
};
bool Tester::testClassAveNormalCase() {//normal case for class average
    bool result = true;
    Section testObject(ITEMS, STUDENTS);//normal case
    testObject.insert();
    //testObject.dump();
    double classAve = testObject.classAverage();
    if (classAve == FAILED_SIG) {//classAve will return 0.0 if m_grades is a 0x0 array, thus being out of bounds
        result = false;
        return result;
    }
    else {//if it's not out of bounds, it will return the average of the class
        result = true;
        return result;
    }
};
bool Tester::testClassAveEdgeCase() {//edge case for class average
    bool result = true;
    Section testObject(EDGE, EDGE);//edge case
    testObject.insert();
    //testObject.dump();
    double classAve = testObject.classAverage();
    if (classAve == FAILED_SIG) {//classAve will return 0.0 if m_grades is a 0x0 array, thus being out of bounds
        result = false;
        return result;
    }
    else {//if it's not out of bounds, it will return the average of the class, thus the edge case works
        result = true;
        return result;
    }
};

void Tester::testCopyConstructErrorCase() {//test for error case in object
    Section testObject(ERROR, ERROR);
    testObject.insert();
    Section newtestObject(testObject);
    
    cout << "Copy Constructor Error Case:" << endl;

    // compare m_numGradingItems
    if (testObject.m_numGradingItems == newtestObject.m_numGradingItems) {
        cout << "The number of graded items is the same for both Section objects" << endl;
    }
    // compare m_numStudents
    if (testObject.m_numStudents == newtestObject.m_numStudents) {
        cout << "The number of students is the same for both Section objects" << endl;
    }
    // compare memory of m_grades
    if (testObject.m_grades != newtestObject.m_grades) {
        cout << "The memory address between the two Section objects is different." << endl;
    }

}
void Tester::testCopyConstructNormalCase() {//test for normal case in object
    Section testObject(ITEMS, STUDENTS);
    testObject.insert();
    Section newtestObject(testObject);

    cout << "Copy Constructor Normal Case:" << endl;

    // compare m_numGradingItems
    if (testObject.m_numGradingItems == newtestObject.m_numGradingItems) {
        cout << "The number of graded items is the same for both Section objects" << endl;
    }
    // compare m_numStudents
    if (testObject.m_numStudents == newtestObject.m_numStudents) {
        cout << "The number of students is the same for both Section objects" << endl;
    }
    // compare memory of m_grades
    if (testObject.m_grades != newtestObject.m_grades) {
        cout << "The memory address between the two Section objects is different." << endl;
    }

}
void Tester::testCopyConstructEdgeCase() {//test for edge case in object
    Section testObject(EDGE, EDGE);
    testObject.insert();
    Section newtestObject(testObject);

    cout << "Copy Constructor Edge Case:" << endl;

    // compare m_numGradingItems
    if (testObject.m_numGradingItems == newtestObject.m_numGradingItems) {
        cout << "The number of graded items is the same for both Section objects" << endl;
    }
    // compare m_numStudents
    if (testObject.m_numStudents == newtestObject.m_numStudents) {
        cout << "The number of students is the same for both Section objects" << endl;
    }
    // compare memory of m_grades
    if (testObject.m_grades != newtestObject.m_grades) {
        cout << "The memory address between the two Section objects is different." << endl;
    }
}
