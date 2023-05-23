// UMBC - CMSC 341 - Spring 2022 - Proj1
#include "elevator.h"
#include "iostream"
class Tester {
public:
    /******************************************
    * Test function declarations go here! *
    ******************************************/
    bool normalCaseInsertAtHead();
    bool testFloorInOrderFromHead();
    bool errorCaseInsertAtHead();
    bool normalCaseInsertAtTail();
    bool errorCaseInsertAtTail();
    bool testFloorInOrderFromTail();
    bool testRemoveNormalCase();
    bool testRemoveReturnTrueAtFloor();
    bool errorCaseRemoveGreaterThen();
    bool errorCaseRemoveLessThen();
    bool errorCaseRemoveAlreadyRemoved();
    bool testInsertFloorFirstFloor();
    bool testInsertFloorGreaterThen();
    bool testInsertFloorLessThen();
    bool testInsertCorrectLocation();
    bool testAssignOperatorGround();
    bool testAssignOperatorTop();
    bool testAssignOperatorValues();



};

bool Tester::normalCaseInsertAtHead() {
    Elevator anElev;
    for (int i = 300; i >= 1; i--) {
        anElev.insertAtHead(i);
    }
    for (int j = 1; j <= 300; j++) {
        if (anElev.checkFloor(j) != true){
            cout << "Test failed" << endl;
            return false;
        }
    }
    cout << "Test normal case insert at head(300 floors) passed" << endl;

    return true;
}
bool Tester::testFloorInOrderFromHead() {
    Elevator anElev;
    int counter = 300;
    for (int i = 300; i >= 1; i--) {
        anElev.insertAtHead(i);
        if (counter != i){
            cout << "Test Failed" << endl;
            return false;
        }
        else{
            counter--;
        }
    }
    cout << "Test floor orderfrom head (300 floors) passed" << endl;
    return true;

}
bool Tester::errorCaseInsertAtHead() {
    Elevator anElev;
    for (int i = 300; i >= 1; i--) {
        anElev.insertAtHead(i);
    }
    if (anElev.insertAtHead(5) == false) {
        cout << "Test error case insert at head passed" << endl;
        return true;
    }
    else {
        cout << "Test Failed" << endl;
        return false;
    }
}
bool Tester::normalCaseInsertAtTail() {
    Elevator anElev;
    for (int i = 1; i <= 300; i++) {
        anElev.insertAtTail(i);
    }
    for (int j = 1; j <= 300; j++) {
        if (anElev.checkFloor(j) != true) {
            cout << "Test failed" << endl;
            return false;
        }
    }
    cout << "Test normal case insert at tail(300 floors) passed" << endl;
    return true;
}
bool Tester::errorCaseInsertAtTail() {
    Elevator anElev;
    for (int i = 1; i <= 300; i++) {
        anElev.insertAtTail(i);
    }
    if (anElev.insertAtTail(5) == false) {
        cout << "Test error case insert at tail passed" << endl;
        return true;
    }
    else {
        cout << "Test Failed" << endl;
        return false;
    }
}
bool Tester::testFloorInOrderFromTail() {
    Elevator anElev;
    int counter = 1;
    for (int i = 1; i <= 300; i++) {
        anElev.insertAtHead(i);
        if (counter != i) {
            cout << "Test Failed" << endl;
            return false;
        }
        else {
            counter++;
        }
    }
    cout << "Test floor order from tail (300 floors) passed" << endl;
    return true;
}
bool Tester::testRemoveNormalCase() {
    Elevator anElev;

    for (int i = 1; i <= 300; i++) {
        anElev.insertAtTail(i);
    }
    for (int j = 1; j <= 300; j++) {
        anElev.removeFloor(j);
    }
    for (int q = 1; q <= 300; q++) {
        if (anElev.checkFloor(q) != false) {
            cout << "Test failed" << endl;
            return false;
        }
    }
    cout << "Test remove normal case (300 floors) passed" << endl;
    return true;
}
bool Tester::testRemoveReturnTrueAtFloor() {
    Elevator anElev;

    for (int i = 1; i <= 300; i++) {
        anElev.insertAtTail(i);
    }
    for (int j = 1; j <= 300; j++) {
        if (anElev.removeFloor(j) != true) {
            cout << "Test failed" << endl;
            return false;
        }
    }
    cout << "Test remove returning true at each floor (300 floors) passed" << endl;
    return true;
}
bool Tester::errorCaseRemoveGreaterThen() {
    Elevator anElev;

    for (int i = 1; i <= 10; i++) {
        anElev.insertAtTail(i);
    }
    if (anElev.removeFloor(11) != true) {
        cout << "Removing error case (floor is > m_top) test passed" << endl;
        return true;
    }
    return false;
}
bool Tester::errorCaseRemoveLessThen() {
    Elevator anElev;

    for (int i = 1; i <= 10; i++) {
        anElev.insertAtTail(i);
    }
    if (anElev.removeFloor(0) != true) {
        cout << "Removing error case (floor is < m_ground) test passed" << endl;
        return true;
    }
    return false;
}
bool Tester::errorCaseRemoveAlreadyRemoved() {
    Elevator anElev;

    for (int i = 1; i <= 10; i++) {
        anElev.insertAtTail(i);
    }
    anElev.removeFloor(5);
    if (anElev.removeFloor(5) != true) {
        cout << "Removing error case (floor is already removed) test passed" << endl;
        return true;
    }
    return false;
}
bool Tester::testInsertFloorFirstFloor() {
    Elevator anElev;
    if (anElev.insertFloor(1) != true) {
        cout << "Insert floor with no floors in list test passed" << endl;
        return true;
    }
    else{
        return false;
    }
}
bool Tester::testInsertFloorGreaterThen() {
    Elevator anElev;

    for (int i = 10; i >= 1; i--) {
        anElev.insertAtHead(i);
    }
    if (anElev.insertFloor(11) != true) {
        cout << "Insert floor which is greater then m_top test passed" << endl;
        return true;
    }
    else {
        return false;
    }
}
bool Tester::testInsertFloorLessThen() {
    Elevator anElev;

    for (int i = 10; i >= 1; i--) {
        anElev.insertAtHead(i);
    }
    if (anElev.insertFloor(0) != true) {
        cout << "Insert floor which is less than m_ground test passed" << endl;
        return true;
    }
    else {
        return false;
    }
}
bool Tester::testInsertCorrectLocation() {
    Elevator anElev;

    for (int i = 10; i >= 1; i--) {
        anElev.insertAtHead(i);
    }
    anElev.removeFloor(3);
    if (anElev.checkFloor(3) == false) {
        anElev.insertFloor(3);
    }
    if (anElev.checkFloor(3) != false) {
        cout << "Insert at correct location test passed" << endl;
        return true;
    }
    else {
        cout << "Test Failed" << endl;
        return false;    
    }
}
/*
bool Tester::testAssignOperatorGround() {
    Elevator anElev;
    Elevator newAnElev;
    for (int i = 1; i <= 10; i++) {
        anElev.insertAtHead(i);
    } 
    newAnElev = anElev;
    if (newAnElev.m_top != anElev.m_top) {
        cout << "Assignment operator (&top pointers matching) test passed" << endl;
        return true;
    }
    else {
        cout << "Test failed" << endl;
        return false;
    }
}
bool Tester::testAssignOperatorTop() {
    Elevator anElev;
    Elevator newAnElev;
    for (int i = 1; i <= 10; i++) {
        anElev.insertAtHead(i);
    }
    newAnElev = anElev;
    if (newAnElev.m_ground != anElev.m_ground) {
        cout << "Assignment operator (&ground pointers matching) test passed" << endl;
        return true;
    }
    else {
        cout << "Test failed" << endl;
        return false;
    }
}
bool Tester::testAssignOperatorValues() {
    Elevator anElev;
    Elevator newAnElev;
    Floor* temp1 = newAnElev.m_ground;
    Floor* temp2 = anElev.m_ground;

    for (int i = 1; i <= 10; i++) {
        anElev.insertAtHead(i);
    }
    newAnElev = anElev;
    while (temp1 != nullptr){
        if (temp1->m_floorNum != temp2->m_floorNum || temp1->m_passenger != temp2->m_passenger) {
            cout << "Test failed" << endl;
            return false;
        }
    }
    cout << "Assignment operator (values match) test passed" << endl;
    return true;
    
}
*/



int main() {
    Elevator anElev;
    //Creating an elevator with 10 floors
    for (int i = 1; i <= 10; i++)
        anElev.insertAtTail(i);
    cout << "\nCarrying Alice from 6th floor to 10th floor:\n";
    cout << "\tAlice enters the elevator:\n";
    anElev.enter(6, "Alice");
    anElev.dump();
    cout << "\n\tMoving to 10th floor:\n";
    anElev.move(6, 10);
    anElev.dump();
    cout << "\n\tAlice exits the elevator\n";
    anElev.exit(10, "Alice");
    cout << "\n\tRemoving the 2nd floor:\n";
    anElev.removeFloor(2);
    anElev.dump();
    cout << endl;

    Tester testCase;
    testCase.normalCaseInsertAtHead();
    testCase.testFloorInOrderFromHead();
    testCase.errorCaseInsertAtHead();
    testCase.normalCaseInsertAtTail();
    testCase.errorCaseInsertAtTail();
    testCase.testFloorInOrderFromTail();
    testCase.testRemoveNormalCase();
    testCase.testRemoveReturnTrueAtFloor();
    testCase.errorCaseRemoveGreaterThen();
    testCase.errorCaseRemoveLessThen();
    testCase.errorCaseRemoveAlreadyRemoved();
    testCase.testInsertFloorFirstFloor();
    testCase.testInsertFloorGreaterThen();
    testCase.testInsertFloorLessThen();
    testCase.testInsertCorrectLocation();
    testCase.testAssignOperatorGround();
    testCase.testAssignOperatorTop();
    testCase.testAssignOperatorValues();



    return 0;
}