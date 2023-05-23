// CMSC 341 - Spring 2022 - Project 4
#include "dnadb.h"

//The constructor takes size to specify the length of the current hash table, and hash is a function pointer to a hash function.The type of hash is defined in dnadb.h.
//The table size must be a prime number between MINPRIMEand MAXPRIME.
//If the user passes a size less than MINPRIME, the capacity must be set to MINPRIME.If the user passes a size larger than MAXPRIME, the capacity must be set to MAXPRIME.If the user passes a non - prime number the capacity must be set to the smallest prime number greater than user's value.
//Moreover, the constructor creates memory for the current table and initializes all member variables.

DnaDb::DnaDb(int size, hash_fn hash) {
    /*m_hash = hash;
    m_currNumDeleted = 0;*/
    //if size is less than MINPRIME, set as MINPRIME
    if (size < MINPRIME) {
        m_currentCap = MINPRIME;
        m_currentSize = MINPRIME;
    }
    //if greater than Max, set max
    else if (size > MAXPRIME) {
        m_currentCap = MAXPRIME;
        m_currentSize = MAXPRIME;
    }
    //if its not prime, find next prime
    else if (!isPrime(size)) {
        m_currentCap = findNextPrime(size);


    }
    else {
        m_currentCap = size;
    }
    //create table and fill it with empty nodes
    m_currentTable = new DNA[size];
    m_oldTable = nullptr;

    for (int i = 0; i < size; i++) {
        m_currentTable[i] = EMPTY;
    }

    m_hash = hash;
    m_currentSize = 0;
    m_currNumDeleted = 0;
    m_oldCap = 0;
    m_oldNumDeleted = 0;
    m_oldSize = 0;



}

DnaDb::~DnaDb() {
    //check if there is an old table, if there is, delete it, then delete all in the current table 

    if (m_oldTable != nullptr) {
        delete[] m_oldTable;
        m_oldTable = nullptr;
    }

    delete[] m_currentTable;


}



bool DnaDb::insert(DNA dna) {
    //need to check the location of dna being passed 
    if (dna.getLocId() < MINPRIME || dna.getLocId() > MAXPRIME) {
        quarterInsert(); //every return I need to rehash 
        return false;
    }
    if (lookOld(dna.getSequence(), dna.getLocId(), m_oldCap))
    {
        quarterInsert();
        return false;
    }
    if (lookCurrent(dna.getSequence(), dna.getLocId(), m_currentCap)) {
        quarterInsert();
        return false;
    }
    //checks if the index is empty and insert 
    int index = quadProbeHash(dna.m_sequence) % m_currentCap;
    m_currentTable[index] = dna;
    m_currentSize += 1;
    //requisites
    if ((lambda() >= .5) || (deletedRatio() >= .8)) {

        m_currentTable = newTable();


    }
    quarterInsert();
    return true;
}



bool DnaDb::remove(DNA dna) {
    //look through old and mark deleted 
    bool check = false;
    if (m_oldTable != nullptr) {
        if (lookOld(dna.getSequence(), dna.getLocId(), m_oldCap)) {
            int index = quadProbeRemove(dna.getSequence(), m_oldCap);
            m_oldTable[index] = DELETED;
            m_oldNumDeleted++;
            //insert quadInsert
            check = true;
        }


    }
    //look through the current and set deleted
    if (m_currentTable != nullptr) {
        if (lookCurrent(dna.getSequence(), dna.getLocId(), m_currentCap)) {
            int index = quadProbeRemove(dna.getSequence(), m_currentCap);
            m_currentTable[index] = DELETED;
            m_currNumDeleted++;
            //insert 25% 
            check = true;
        }

    }


    quarterInsert();
    return check;
}
//This function looks for the DNA object with sequenceand location in the hash table, if the object is found the function returns it,
//otherwise the function returns empty object.If there are two hash tables at the time, the function needs to look into both tables.
DNA DnaDb::getDNA(string sequence, int location) {
    // look through both tables and get all indexes passed in, else return empty 
    if (m_oldTable != nullptr) {
        for (int i = 0; i < m_oldCap; i++) {
            if ((m_oldTable[i].getSequence() == sequence) && (m_oldTable[i].getLocId() == location)) {
                return m_oldTable[i];
            }

        }
        for (int i = 0; i < m_currentCap; i++) {
            if ((m_currentTable[i].getSequence() == sequence) && (m_currentTable[i].getLocId() == location)) {
                return m_currentTable[i];
            }

        }
    }
    else {
        for (int i = 0; i < m_currentCap; i++) {
            if ((m_currentTable[i].getSequence() == sequence) && (m_currentTable[i].getLocId() == location)) {
                return m_currentTable[i];
            }

        }
    }
    return EMPTY;
}

float DnaDb::lambda() const {
    //float loadingRatio = 0.0; 
    //convert our values to floats for the purpose of ratios
    float currentCap = m_currentCap;
    float currentSize = m_currentSize;
    //The load factor is the ratio of occupied buckets to the table capacity
    //loadingRatio = currentSize / currentCap; 
    //cout << loadingRatio << endl;

    return currentSize / currentCap;
}
//same as above except for checking deletion
float DnaDb::deletedRatio() const {
    float delRatio = 0;
    float capacity = m_currNumDeleted;
    float size = m_currentSize;

    delRatio = capacity / size;
    return delRatio;
}

void DnaDb::dump() const {
    cout << "Dump for current table: " << endl;
    if (m_currentTable != nullptr)
        for (int i = 0; i < m_currentCap; i++) {
            cout << "[" << i << "] : " << m_currentTable[i] << endl;
        }
    cout << "Dump for old table: " << endl;
    if (m_oldTable != nullptr)
        for (int i = 0; i < m_oldCap; i++) {
            cout << "[" << i << "] : " << m_oldTable[i] << endl;
        }
}

bool DnaDb::isPrime(int number) {
    bool result = true;
    for (int i = 2; i <= number / 2; ++i) {
        if (number % i == 0) {
            result = false;
            break;
        }
    }
    return result;
}

int DnaDb::findNextPrime(int current) {
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

DNA::DNA(string sequence, int location) {
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

string DNA::getSequence() const {
    return m_sequence;
}

int DNA::getLocId() const {
    return m_location;
}

// Overloaded assignment operator
const DNA& DNA::operator=(const DNA& rhs) {
    if (this != &rhs) {
        m_sequence = rhs.m_sequence;
        m_location = rhs.m_location;
    }
    return *this;
}

// Overloaded insertion operator.  Prints DNA's sequence (key),
// and the location ID. This is a friend function in DNA class.
ostream& operator<<(ostream& sout, const DNA& dna) {
    if (!dna.m_sequence.empty())
        sout << dna.m_sequence << " (Location ID " << dna.m_location << ")";
    else
        sout << "";
    return sout;
}

// Overloaded equality operator. This is a friend function in DNA class.
// To test inequality we may negate the results of this operator.
bool operator==(const DNA& lhs, const DNA& rhs) {
    return ((lhs.m_sequence == rhs.m_sequence) && (lhs.m_location == rhs.m_location));
}



//helper functions to iterate through current and old tables 

bool DnaDb::lookCurrent(string sequence, int location, int t_size) {
    for (int i = 0; i < t_size; i++) {
        if ((m_currentTable[i].getSequence() == sequence) && (m_currentTable[i].getLocId() == location)) {
            return true;
        }

    }
    return false;

}
bool DnaDb::lookOld(string sequence, int location, int t_size) {
    for (int i = 0; i < t_size; i++) {
        if ((m_oldTable[i].getSequence() == sequence) && (m_oldTable[i].getLocId() == location)) {
            return true;
        }
    }
    return false;
}

//helper function for hashing with quadratic probing
int DnaDb::quadProbeHash(string key)
{
    int i = 0;
    int hash = ((m_hash(key) % m_currentCap) + (i * i)) % m_currentCap;
    while (!(m_currentTable[hash] == EMPTY)) {
        i++;
        hash = ((m_hash(key) % m_currentCap) + (i * i)) % m_currentCap;
    }
    return hash;
}
//quadratic probing helper function given formula for old
int DnaDb::quadProbeHashOld(string key)
{
    int i = 0;
    int hash = ((m_hash(key) % m_oldCap) + (i * i)) % m_oldCap;
    while (!(m_oldTable[hash] == EMPTY)) {
        i++;
        hash = ((m_hash(key) % m_oldCap) + (i * i)) % m_oldCap;
    }
    return hash;
}
//helper for a new table 
int DnaDb::quadProbeRemove(string key, int capacity) {
    int i = 0;
    int hash = ((m_hash(key) % capacity) + (i * i)) % capacity;
    return hash;
}
DNA* DnaDb::newTable() {
    //have to copy the current to an old table before creating a new one 

    m_oldTable = m_currentTable;
    m_oldCap = m_currentCap;
    m_oldSize = m_currentSize;
    m_oldNumDeleted = 0;

    int newCapacity = 0;
    newCapacity = findNextPrime(((m_currentSize - m_currNumDeleted) * 4));
    m_currentTable = new DNA[newCapacity];

    //have to fill the new table with EMPTY
    for (int i = 0; i < newCapacity; i++) {
        m_currentTable[i] = EMPTY;
    }

    m_currentSize = 0;
    m_currNumDeleted = 0;
    m_currentCap = newCapacity;

    return m_currentTable;
}

void DnaDb::quarterInsert() {
    //set indexes and counters to keep track of begin and end of 25% chunks 
    int index = 0;
    bool endList = true;
    int transfer = 0;
    int startPosition = m_oldNumDeleted;
    int endPosition = (m_oldSize / 4);


    if (m_oldTable != nullptr) {
        for (int i = startPosition; i < startPosition + endPosition; i++) {
            if (i < m_oldCap) {
                if (!(m_oldTable[i] == EMPTY)) {
                    if (m_oldTable[i] == DELETED) {
                        transfer++;
                    }
                    else {
                        transfer++;
                        index = quadProbeHash(m_oldTable[i].getSequence());
                        m_currentTable[index] = m_oldTable[i];
                        m_oldTable[i] = DELETED;
                        m_currentSize++;
                    }
                }
                startPosition++;
            }
        }

        m_oldNumDeleted = startPosition;


        for (int i = 0; i < m_oldCap; i++) {
            if (!(m_oldTable[i] == EMPTY) && !(m_oldTable[i] == DELETED)) {
                endList = false;
            }
        }

        if (endList == true) {
            delete[] m_oldTable;
            m_oldTable = nullptr;
           /* m_oldCap = 0;
            m_oldNumDeleted = 0;
            m_oldSize = 0;*/
        }
    }
}