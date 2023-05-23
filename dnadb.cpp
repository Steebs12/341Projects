// CMSC 341 - Spring 2022 - Project 4
#include "dnadb.h"
DnaDb::DnaDb(int size, hash_fn hash) {//This is the constructor which is responsible for the creation of our DNA Database

    m_hash = hash;
    m_currNumDeleted = 0;
    m_currentSize = 0;//most of these lines are for initializing the member variable of the header file for dnadb
    m_oldCap = 0;
    m_oldNumDeleted = 0;
    m_oldSize = 0;
    m_oldTable = nullptr;

    if (isPrime(size)) {
        if (size > MAXPRIME) {
            size = MAXPRIME;
            m_currentCap = MAXPRIME;
        }
        else if (size < MINPRIME) {
            size = MINPRIME;
            m_currentCap = MINPRIME;
        }
        else {
            m_currentCap = size;
        }

        m_currentTable = new DNA[size];//here we create a new DNA object array and set the current table equal to it
        for (int i = 0; i < size; i++) {
            m_currentTable[i] = EMPTY;//then we go through and populate the current table with EMPTY DNA objects
        }
    }
    else {
        cout << "Size is not a prime number" << endl;
    }
}

DnaDb::~DnaDb() {//the destructor is responsible for deallocating all the data allocated for both current table and old table
    if (m_oldTable != nullptr) {
        delete[] m_oldTable;
        m_oldTable = nullptr;
    }
    delete[] m_currentTable;
    m_currentTable = nullptr;
}

bool DnaDb::insert(DNA dna) {//insert takes care of inserting a new DNA object into the current table
    if (dna.getLocId() > MAXLOCID || dna.getLocId() < MINLOCID) {//makes sure the dna id is within bounds
        return false;
    }
    if (inCurrList(dna.getSequence(), dna.getLocId(), m_currentCap)) {//makes sure the dna object isn't already in the current table
        return false;
    }
    else {
        int index = m_hash(dna.getSequence());
        index = quadProbing(index, m_currentCap);//using the quadratic probing equation we obtain an index for the DNA object
        
        if (m_currentTable[index] == EMPTY) {//as long as the table at that index is empty we set the dna object there and increase the currentSize
            m_currentTable[index] = dna;
            m_currentSize++;
            if (lambda() > .5 || deletedRatio() > .8) {//this checks to see if we need to rehash our table
                if (hash()) {
                    return true;
                }
                else {
                    return false;
                }
            }
            twentyFiveHelper();//helper function to bring over 25% of the data if there is anything in oldTable
        }
    }
    checkOld();//helper function to check if the old table contains any more "live" objects
    return true;
}

bool DnaDb::remove(DNA dna) {//this function is responsible for "removing" a dna object that is passed in
    bool removeBool = false;
    if (dna.getLocId() > MAXLOCID || dna.getLocId() < MINLOCID) {//bounds check for the ID
        return false;
    }
    if (m_oldTable != nullptr) {//if two tables exist we have to look at both tables
        if (inCurrList(dna.getSequence(), dna.getLocId(), m_currentCap)) {//if its in current table we get the index of the object in current
            int index = m_hash(dna.getSequence());

            index = quadProbingRemove(index, m_currentCap, dna);
            m_currentTable[index] = DELETED;//then set it to DELETED
            m_currNumDeleted++;
            removeBool = true;
            if (lambda() > .5 || deletedRatio() > .8) {//checker for if we need to rehash or not
                if (hash()) {
                    return true;
                }
                else {
                    return false;
                }

            }
            twentyFiveHelper();//brings over 25% of data if oldTable contains data
        }
        else if (inOldList(dna.getSequence(), dna.getLocId(), m_oldCap)) {//basically the exact same thing as the previous code, except this one looks in the old table
            int index = m_hash(dna.getSequence());

            index = quadProbingRemove(index, m_oldCap, dna);
            m_oldTable[index] = DELETED;
            m_oldNumDeleted++;
            removeBool = true;
            if (lambda() > .5 || deletedRatio() > .8) {
                if (hash()) {
                    return true;
                }
                else {
                    return false;
                }
            }
            twentyFiveHelper();
        }
    }
    else {
        if (inCurrList(dna.getSequence(), dna.getLocId(), m_currentCap)) {//if there is no old table, we only have to look in the current table
            int index = m_hash(dna.getSequence());

            index = quadProbingRemove(index, m_currentCap, dna);//we get it's index just like above, and set it to DELETED

            m_currentTable[index] = DELETED;
            m_currNumDeleted++;
            removeBool = true;
            if (lambda() > .5 || deletedRatio() > .8) {//then check if we need to rehash our current table
                if (hash()) {
                    return true;
                }
                else {
                    return false;
                }

            }
            twentyFiveHelper();//brings over 25% of data if oldTable contains data
        }
    }
    checkOld();//checks if the old table has any "live" objects left
    return removeBool;//return whether we successfully did the "removal" or not
}

DNA DnaDb::getDNA(string sequence, int location) {//this function returns the dna object at a specific sequence and location ID
    if (m_oldTable != nullptr) {//if there are two tables we look through both for the sequence and location, then return the DNA object that matches
        for (unsigned int i = 0; i < m_oldCap; i++) {
            if (m_oldTable[i].getSequence() == sequence && m_oldTable[i].getLocId() == location) {
                return m_oldTable[i];
            }
        }
        for (unsigned int i = 0; i < m_currentCap; i++) {
            if (m_currentTable[i].getSequence() == sequence && m_currentTable[i].getLocId() == location) {
                return m_currentTable[i];
            }
        }
    }
    else {//if there is only one table we only have to look through currentTable for the sequence and location
        for (unsigned int i = 0; i < m_currentCap; i++) {
            if (m_currentTable[i].getSequence() == sequence && m_currentTable[i].getLocId() == location) {
                return m_currentTable[i];
            }
        }
    }
    return EMPTY;//if we cant find anything that matches, we return the EMPTY DNA object
}

float DnaDb::lambda() const {//returns the load factor of the currentTable
    float occupiedBuckets = m_currentSize;
    float capacity = m_currentCap;

    return occupiedBuckets / capacity;
}

float DnaDb::deletedRatio() const {//returns the deletedRatio of the currentTable
    float deletedBuckets = m_currNumDeleted;
    float occupiedBuckets = m_currentSize;

    return deletedBuckets / occupiedBuckets;
}

void DnaDb::dump() const {//provided dump function
    cout << "Dump for current table: " << endl;
    if (m_currentTable != nullptr)
        for (unsigned int i = 0; i < m_currentCap; i++) {
            cout << "[" << i << "] : " << m_currentTable[i] << endl;
        }
    cout << "Dump for old table: " << endl;
    if (m_oldTable != nullptr)
        for (unsigned int i = 0; i < m_oldCap; i++) {
            cout << "[" << i << "] : " << m_oldTable[i] << endl;
        }
}

bool DnaDb::isPrime(int number) {//returns whether or not the number passed in is a prime number 
    bool result = true;
    for (int i = 2; i <= number / 2; ++i) {
        if (number % i == 0) {
            result = false;
            break;
        }
    }
    return result;
}

int DnaDb::findNextPrime(int current) {//provided function which finds the next prime number after the current number passed in
    //we always stay within the range [MINPRIME-MAXPRIME]
    //the smallest prime starts at MINPRIME
    if (current < MINPRIME) current = MINPRIME - 1;
    for (int i = current; i < MAXPRIME; i++) {
        for (int j = 2; j * j <= i; j++) {
            if (i % j == 0)
                break;
            else if (j + 1 > sqrt(i) && i != current) {
                return i;
            }
        }
    }
    //if a user tries to go over MAXPRIME
    return MAXPRIME;
}

int DnaDb::quadProbing(int hashKey, int tableCapacity) {//helper function to determine the index when inserting DNA objects
    int i = 0;
    int index = 0;

    index = (((hashKey) % tableCapacity) + (i * i)) % tableCapacity;

    while (!(m_currentTable[index] == EMPTY)) {//this while loop continues to reprob until it finds an index in the currentTable that is empty
        index = (((hashKey) % tableCapacity) + (i * i)) % tableCapacity;
        i++;
    }
    return index;
}

int DnaDb::quadProbingRemove(int hashKey, int tableCapacity, DNA dna) {//helper function for the removal of a dna object which is passed in
    int i = 0;
    int index = 0;

    index = (((hashKey) % tableCapacity) + (i * i)) % tableCapacity;
    while (!(m_currentTable[index] == dna)) {//this while continues to reprob until it gets the index of the currentTable which is the same as that which is passed in
        index = (((hashKey) % tableCapacity) + (i * i)) % tableCapacity;
        i++;
    }
    return index;
}

bool DnaDb::inOldList(string sequence, int location, int tableCapacity) {//helper function which determines whether the DNA object is in the old list
    for (int i = 0; i < tableCapacity; i++) {
        if (m_oldTable[i].getSequence() == sequence && m_oldTable[i].getLocId() == location) {
            return true;
        }
    }
    return false;
}

bool DnaDb::inCurrList(string sequence, int location, int tableCapacity) {//same as previous, but returns whether the object is in the current table
    for (int i = 0; i < tableCapacity; i++) {
        if (m_currentTable[i].getSequence() == sequence && m_currentTable[i].getLocId() == location) {
            return true;
        }
    }
    return false;
}

DNA::DNA(string sequence, int location) {//Provided function to create a DNA object
    if ((location >= MINLOCID && location <= MAXLOCID) ||
        (location == 0 && sequence == "DELETED")) {
        // this is a normal or a DELETED object
        m_sequence = sequence;
        m_location = location;
    }
    else {
        // this is the empty object
        m_sequence = "";
        m_location = 0;
    }
}

string DNA::getSequence() const {//returns the sequence of a dna object
    return m_sequence;
}

int DNA::getLocId() const {//returns the location id of a dna object
    return m_location;
}

string DnaDb::returnDNASequence(DNA dna) {//returns the sequence of the dna which is passed in
    return dna.getSequence();
}

int DnaDb::returnDNALocation(DNA dna) {//returns the locationID of the dna object which is passed in
    return dna.getLocId();
}

// Overloaded assignment operator
const DNA& DNA::operator=(const DNA& rhs) {//Provided Function
    if (this != &rhs) {
        m_sequence = rhs.m_sequence;
        m_location = rhs.m_location;
    }
    return *this;
}

// Overloaded insertion operator.  Prints DNA's sequence (key),
// and the location ID. This is a friend function in DNA class.
ostream& operator<<(ostream& sout, const DNA& dna) {//Provided Function
    if (!dna.m_sequence.empty())
        sout << dna.m_sequence << " (Location ID " << dna.m_location << ")";
    else
        sout << "";
    return sout;
}

// Overloaded equality operator. This is a friend function in DNA class.
// To test inequality we may negate the results of this operator.
bool operator==(const DNA& lhs, const DNA& rhs) {//Provided Function
    return ((lhs.m_sequence == rhs.m_sequence) && (lhs.m_location == rhs.m_location));
}

void DnaDb::createNewTable() {//creates a new table following the guidelines outlined in the project description
    int newTableCapacity = findNextPrime((m_currentSize - m_currNumDeleted) * 4);
    m_currentTable = new DNA[newTableCapacity];//sets the current table equal to the new DNA table like in constructor

    for (int i = 0; i < newTableCapacity; i++) {
        m_currentTable[i] = EMPTY;//fills the new table
    }
    m_currentCap = newTableCapacity;
    m_currentSize = 0;//initializes all of the values associated with the new table
    m_currNumDeleted = 0;    
}

int DnaDb::twentyFiveHelper() {//function responsible for moving over 25% of the data from the oldTable to the currentTable
    if (m_oldTable != nullptr) {//if the old table exists
        int counter = 0;
        int twentyFivePercent = floor(m_oldSize / 4);

        for (unsigned int i = 0; i < m_oldCap; i++) {
            if (!(m_oldTable[i] == DELETED) && !(m_oldTable[i] == EMPTY) && counter < twentyFivePercent) {//if the location of the old table at i is a "live" object
                int index = m_hash(m_oldTable[i].getSequence());
                index = quadProbing(index, m_currentCap);//we get the index and then add it to the new current table

                if (m_currentTable[index] == EMPTY) {
                    m_currentTable[index] = m_oldTable[i];
                    m_currentSize++;
                    m_oldTable[i] = DELETED;//and then change the spot it was in oldTable to deleted
                    counter++;

                }
                //counter++;
            }
        }
        return twentyFivePercent;
    }
    return 0;//if the old table doesn't exist we return 0;
}

bool DnaDb::hash() {
    m_oldCap = m_currentCap;
    m_oldNumDeleted = m_currNumDeleted;//setting the currentTable member variables to the oldTable member variables
    m_oldSize = m_currentSize;
    m_oldTable = m_currentTable;
    createNewTable();//create the new table
    int twentyFivePercent = floor(m_oldSize / 4);

    if (twentyFiveHelper() == twentyFivePercent) {//how we make surewe are only transfering 25% of the data
        return true;
    }
    else {
        return false;
    }
}

void DnaDb::checkOld() {//this function goes though the oldTable and if it has no "live" objects left, deletes the table and sets it equal to null pointer
    if (m_oldTable != nullptr) {
        unsigned int counter = 0;
        for (unsigned int i = 0; i < m_oldCap; i++) {
            if (m_oldTable[i] == DELETED || m_oldTable[i] == EMPTY) {
                counter++;
            }
        }
        if (counter == m_oldCap) {//measure to make sure that every object in the oldTable is either DELETED or EMPTY
            delete[] m_oldTable;
            m_oldTable = nullptr;
        }
    }
}

