// UMBC - CMSC 341 - Spring 2022 - Proj0
//NAME: Caleb Stieber
//CLASS: CMSC 341
//DESCRIPTION: This cpp file is responsible for the definitions of the Section class. In additon, we can set and change the member variable that are specific to this class.
//towards the end of the class, we have various functions that determine the averages of a specific student, of a certain quiz, and of the entire class.
#include "section.h"

Section::Section() { // default constructor, initialize students and grading items to 0 
    m_numStudents = 0;
    m_numGradingItems = 0;
    m_grades = nullptr;

}

Section::Section(int items, int students) {// overloaded constructor which initializes students and items to the values passed into the function
    if (items < 0) {//checks to makes sure items is > 0
        items = 0;
        m_numGradingItems = 0;
    }
    else if (students < 0) {//checks to make sure students is > 0
        students = 0;
        m_numStudents = 0;
    }
    else {//this sets the values to the inputted values and creates a double array [students][num_items]
        m_numStudents = students;
        m_numGradingItems = items;
        m_grades = new double* [students];
        for (int i = 0; i < students; i++) {
            m_grades[i] = new double[items];
        }
    }
}

Section::~Section() {//deconstructor deallocates all memory that we allocate throughout the program 
    for (int i = 0; i < m_numStudents; i++) {
        delete m_grades[i];
        m_grades[i] = nullptr;
    }
    delete m_grades;
    m_grades = nullptr;
}

void Section::clear() {//clear basically does the same thing that the deconstructor does, however, after deallocating memory we reinitialize students and items to 0
    for (int i = 0; i < m_numStudents; i++) {
        delete m_grades[i];
        m_grades[i] = nullptr;
    }
    delete m_grades;
    m_grades = nullptr;
    m_numStudents = 0;
    m_numGradingItems = 0;
}

void Section::insert() {//this function is responsible for populating m_grades with random values 
    Random randNum(0, 100, UNIFORMREAL);
    for (int i = 0; i < m_numStudents; i++) {//this iterates through students
        for (int j = 0; j < m_numGradingItems; j++) {//this iterates through the items at each student
            m_grades[i][j] = randNum.getRealRandNum();
        }
    }
}

double Section::studentAverage(int student) {//this function returns the average of a students grades at the index of the inputted student
    double avg = 0.0;
    if (student > m_numStudents || student <= 0) {//base case
        return avg;
    }
    else {
        for (int i = 0; i < m_numGradingItems; i++) {//iteration through m_grades at a specific student and then each of their grades
            double temp = m_grades[student - 1][i];
            avg = avg + temp;
        }
        avg = avg / m_numGradingItems;
        return avg;
    }
}

double Section::quizAverage(int quiz) {//this function is responsible for returning the average score of the inputted graded item
    double avg = 0.0; 
    if (quiz > m_numGradingItems || quiz < 0) {//base case
        return 0.0;
    }
    else {
        for (int i = 0; i < m_numStudents; i++) {//iteration through m_grades at differing students but a specific quiz #
            double temp = m_grades[i][quiz - 1];
            avg = avg + temp;
        }
        avg = avg / m_numStudents;
        return avg;
    }
}

double Section::classAverage() {//this function is responsible for returning the average of all the students averages
    double avg = 0.0;
    if (m_numStudents == 0 || m_numGradingItems == 0) {//base case
        return avg;
    }
    else {
        for (int i = 1; i <= m_numStudents; i++) {//we iterate through each students
            double temp = studentAverage(i);//we then get the average of each students grades using studentAverage(i)
            avg = avg + temp;
        }
        avg = avg / m_numStudents;
        return avg;
    }
}

//copy constructor
Section::Section(const Section& rhs) {//this copy constructor is how we take an old object and transfer it's data to a new object but with a different memory address
    m_numStudents = rhs.m_numStudents;
    m_numGradingItems = rhs.m_numGradingItems;
    m_grades = new double* [rhs.m_numStudents];
    for (int i = 0; i < m_numStudents; i++) {//this works similarly to the constructor, but instead we have a 2d array that has already been populated so we trasnfer the data over to the new m_grades
        m_grades[i] = new double[m_numStudents];
        for (int j = 0; j < m_numGradingItems; j++) {
            m_grades[i][j] = rhs.m_grades[i][j];
        }
    }
}

//assignment operator
const Section& Section::operator=(const Section& rhs) {//this works as like a backup to the copy constructor, basically doing the same thing that it does
    if (m_grades != rhs.m_grades) {//we check here to make sure that the memory addresses are different between the two objects
        clear();
        m_numStudents = rhs.m_numStudents;
        m_numGradingItems = rhs.m_numGradingItems;
        m_grades = new double* [rhs.m_numStudents];
        for (int i = 0; i < m_numStudents; i++) {
            m_grades[i] = new double[m_numStudents];
            for (int j = 0; j < m_numGradingItems; j++) {
                m_grades[i][j] = rhs.m_grades[i][j];
            }
        }
    }
    return *this;
}



void Section::dump() {//this lovely function was created by our wonderful professor to help us not have to struggle for hours about format, :)
    if (m_numGradingItems > 0 && m_numStudents > 0 && m_grades != nullptr) {
        cout << setw(8) << "        ";
        for (int a = 1; a <= m_numGradingItems; a++)
            cout << setw(8) << "Quiz" << a;
        cout << endl;
        for (int i = 1; i <= m_numStudents; i++) {
            cout << setw(8) << "Student" << i << " ";
            for (int j = 1; j <= m_numGradingItems; j++) {
                cout << setw(8) << m_grades[i - 1][j - 1] << " ";
            }
            cout << endl;
        }
    }
}