// UMBC - CMSC 341 - Spring 2022 - Proj1
#include "elevator.h"

Elevator::Elevator() {//our default constructor is responsible for initializing m_top and m_ground to being nullptr
    m_top = nullptr;
    m_ground = nullptr;
}
Elevator::~Elevator() {//Our destructor is responsible for deallocating all the memory we use throughout our program
    Floor* curr = m_top;
    while (curr != nullptr) {//it does this by iterating through the linked list and deleting each node
        m_top = curr;
        curr = curr->m_previous;
        delete m_top;

    }
    m_top = nullptr;//we then reinitialize m_top and m_ground to being nullptr
    m_ground = nullptr;

}
void Elevator::clear() {//this function performs the same operations as the destructor, the only difference is that we can call the clear function throughout our code
    Floor* curr = m_top;
    while (curr != nullptr) {
        m_top = curr;
        curr = curr->m_previous;
        delete m_top;

    }
    m_top = nullptr;
    m_ground = nullptr;

}
bool Elevator::insertAtHead(int floor, string passenger) {//this function is responsible for inserting floors from the ground up

    Floor* groundPtr = m_ground;

    if (m_ground == nullptr){//if there are no floors in the linked list we initialize the first floor as being both m_ground and m_top
        Floor* newFloor = new Floor(floor, passenger);
        m_ground = newFloor;
        m_top = newFloor;
        return true;
    }
    else {
        if (floor > groundPtr->m_floorNum) {//if the floor is not in the correct order it does not get inserted in to 
            return false;
        }
        else {
            Floor* newFloor = new Floor(floor, passenger);
            newFloor->m_next = groundPtr;
            groundPtr->m_previous = newFloor;
            m_ground = newFloor;//we change the m_ground pointer as we add another floor to the head
            return true;
        }
    }
}
bool Elevator::insertAtTail(int floor, string passenger) {//this function is responsible for adding floors from the tail 

    Floor* topPtr = m_top;
    if (m_ground == nullptr) {//if the floor is the first in the linked list, we 
        Floor* newFloor = new Floor(floor, passenger);
        m_ground = newFloor;
        m_top = newFloor;
        return true;
    }
    else {
        if (floor < topPtr->m_floorNum) {//if the floor is not in the correct order, we do not insert it and we return false
            return false;
        }
        else {
            Floor* newFloor = new Floor(floor, passenger);
            newFloor->m_previous = topPtr;
            topPtr->m_next = newFloor;
            m_top = newFloor;//we only change m_top here as we insert floors because it is the only pointer that is changing
            return true;
        }
    }

}
bool Elevator::removeFloor(int floor) {
    try {
        int counter;

        if (floor < m_ground->m_floorNum || floor > m_top->m_floorNum) {//this checks to make sure the floor we want to remove is between the top floor and bottom floor
            throw floor;
        }
        else if (!checkFloor(floor)) {//this checks to make sure that the floor we want to remove exists if it is between the top and bottom floors
            return false;
        }
        else if (floor == m_top->m_floorNum) {//if the floor we want to remove is the top floor
            Floor* curr = m_top;
            Floor* prev = curr->m_previous;
            if (prev == nullptr) {//if its the only floor we set the previous pointer to nullptr and then delete the current floor leaving an empty linked list
                delete curr;
                m_top = nullptr;
            }
            else {
                prev->m_next = nullptr;//else, we set the previous floor's next to point to null pointer and delete the current node which would be the old top node
                delete curr;
                m_top = prev;//then we reset the top pointer to the new top floor
            }

        }
        else if (floor == m_ground->m_floorNum) {//here we do the same thing that we did for removing the top floor, except we are removing the bottom floor
            Floor* curr = m_ground;
            Floor* prev = curr->m_next;
            if (prev == nullptr) {
                delete curr;
                m_ground = nullptr;
            }
            else {
                prev->m_previous = nullptr;
                delete curr;
                m_ground = prev;
            }
        }
        else {//we get to here if the floor we want to remove is in bound, and it isnt the top floor or the bottom floor
            if (m_top->m_floorNum - floor < floor - m_ground->m_floorNum) {//if there are fewer floors from the top to the removal floor 
                Floor* curr = m_top;
                Floor* prev = m_top;
                counter = m_top->m_floorNum;
                while (curr != nullptr) {//iterate to the floor we want to remove from the top
                    if (counter == floor) {//then we delete the floor when we reach it and reallign the next and previous pointers
                        prev->m_previous = curr->m_previous;
                        curr->m_previous->m_next = prev;
                        delete curr;
                        return true;
                    }
                    prev = curr;//else we just iterate through the linked list till we get to the node we want to remove
                    curr = curr->m_previous;
                    counter--;
                }
            }
            else {//if there are fewer floors from the bottom to the floor we want to remove
                Floor* curr = m_ground;//same thing happens here as before except we iterate from the bottom to the floor we want to remove
                Floor* prev = m_ground;
                counter = m_ground->m_floorNum;
                while (curr != nullptr) {
                    if (counter == floor) {
                        prev->m_next = curr->m_next;
                        curr->m_next->m_previous = prev;
                        delete curr;
                        return true;
                    }
                    prev = curr;
                    curr = curr->m_next;
                    counter++;
                }
            }
        }
        return true;
    }
    catch (int floor) {//if an out of range exeception is thrown we automatically return false
        return false;
    }
}
bool Elevator::insertFloor(int floor) {
    try {
        int counter;
        if (m_top == nullptr || m_ground == nullptr) {//if there are no floors in the linked list then we throw an exception
            throw floor;
        }

        else if (floor > m_top->m_floorNum || floor < m_ground->m_floorNum) {//if its out of bounds we throw an exception
            throw floor;
        }
        else if (checkFloor(floor)) {//if the floor already exists then we dont insert and we return fasle
            return false;
        }
        else {
            Floor* newFloorInsert = new Floor(floor);
            if (m_top->m_floorNum - floor < floor - m_ground->m_floorNum) {//from the top make it drop. iterating from the top to where the node is supposed to gp
                Floor* curr = m_top;
                Floor* prev = m_top;
                counter = m_top->m_floorNum;
                while (curr != nullptr) {
                    if (counter == floor) {//this counter helps us keep track of when we need to insert the node so it is in order
                        newFloorInsert->m_next = prev;
                        newFloorInsert->m_previous = curr;
                        curr->m_next = newFloorInsert;
                        prev->m_previous = newFloorInsert;
                        return true;
                    }
                    prev = curr;
                    curr = curr->m_previous;
                    counter--;
                }
            }
            else {//started from the bottom now we here, same thing as the above except we iterate from the bottom to where we want to insert
                Floor* curr = m_ground;
                Floor* prev = m_ground;
                counter = m_ground->m_floorNum;
                while (curr != nullptr) {
                    if (counter == floor) {
                        newFloorInsert->m_next = curr;
                        newFloorInsert->m_previous = prev;
                        prev->m_next = newFloorInsert;
                        curr->m_previous = newFloorInsert;
                        return true;
                    }
                    prev = curr;
                    curr = curr->m_next;
                    counter++;
                }
            }
        }
        return false;
    }
    catch (int floor) {//if an exception is thrown we catch it and return false
        return false;
    }
}
bool Elevator::checkFloor(int floor) {

    Floor* curr = m_top;
    if (m_top == nullptr || m_ground == nullptr) {//if there are no floors we return false
        return false;
    }
    else if (floor > m_top->m_floorNum || floor < m_ground->m_floorNum) {//if we are trying to find a floor out of bounds we return false
        throw; //exception thing to ask about
    }
    else{
        while (curr != nullptr){//we iterate through the linked list for the floor that we are looking for and return true if we find it
            if (curr->m_floorNum == floor) {
                return true;
            }
            curr = curr->m_previous;
        }
        return false;//if we cant find it we return false
    }
    

}
bool Elevator::move(int origin, int destination) {
    int counter;
    Floor* passenger = m_ground;//responsible for keeping track of the passenger
    try {
        if (origin < m_ground->m_floorNum || origin > m_top->m_floorNum) {//if the origin is out of bounds
            throw origin;
        }
        else if (destination < m_ground->m_floorNum || destination > m_top->m_floorNum) {//if the destination is out of bounds
            throw destination;
        }
        else if (!checkFloor(origin) || !checkFloor(destination)) {//if the origin or destination does not exist
            return false;
        }
        else {
            if (destination > origin) {
                //iterate from bottom up
                Floor* curr = m_ground;
                Floor* prev = m_ground;

                counter = m_ground->m_floorNum;
                while (curr != nullptr) {
                    if (counter == origin) {
                        passenger = curr;//sets the passenger pointer to the current pointer
                        prev = curr;
                    }
                    else if (counter == destination) {
                        curr->m_passenger = passenger->m_passenger;//sets the current pointer passenger to the passenger pointer passenger
                        prev->m_passenger = "";
                        return true;
                    }
                    curr = curr->m_next;//iterate to the next floor
                    counter++;
                }
                return true;
            }
            else if (destination < origin) {//same as before just from the top down
                //iterate from the top down
                Floor* curr = m_top;

                counter = m_top->m_floorNum;
                while (curr != nullptr) {
                    if (counter == origin) {
                        passenger = curr;
                    }
                    else if (counter == destination) {
                        curr->m_passenger = passenger->m_passenger;
                        return true;
                    }
                    curr = curr->m_previous;
                    counter--;
                }


                return true;
            }
            else {//if the origin and the destination are the same number the person doesnt move so the function would return false.
                return false;
            }
        }
    }
    catch (int number) {//if an exception is caught we return false
        return false;
    }
}
string Elevator::exit(int floor, string passenger) {
    Floor* temp = m_ground;
    if (checkFloor(floor)) {//if the floor exists
        while (temp != nullptr) {
            if (temp->m_floorNum == floor) {//if we are at the floor the person gets off at
                if (temp->m_passenger != "") {//set the floor passenger to an empty string
                    temp->m_passenger = "";
                    return passenger;//return the passenger
                }
            }
            temp = temp->m_next;//iterate to the next
        }
    }
    return "";//return an empty string if it doesnt work
}
bool Elevator::enter(int floor, string passenger) {//this basically does the same as exit, but instead of setting the floor passenger to an empty string we set it to the passneger that is passed in
    
    Floor* temp = m_ground;
    temp = m_ground;

    if (checkFloor(floor)) {
        while (temp != nullptr) {
            if (temp->m_floorNum == floor) {
                if (temp->m_passenger == "") {
                    temp->m_passenger = passenger;
                    return true;
                }
                else{
                    cout << "There is already someone at this floor" << endl;
                }
            }
            temp = temp->m_next;
        }
    }
    return false;

}
 

Elevator::Elevator(const Elevator& rhs) {
    //The copy constructor creates a deep copy of rhs.
    m_ground = new Floor(rhs.m_ground->m_floorNum, rhs.m_ground->m_passenger);
    Floor* currCopy = m_ground;
    Floor* oldCopy = rhs.m_ground->m_next;
    while (oldCopy != nullptr) {
        Floor* copyFloor = new Floor(oldCopy->m_floorNum, oldCopy->m_passenger);
        currCopy->m_next = copyFloor;
        copyFloor->m_previous = currCopy;
        currCopy = copyFloor;
        oldCopy = oldCopy->m_next;    
    }
}


const Elevator& Elevator::operator=(const Elevator& rhs) {
    //this creates a deep copy of our elevator making sure that we check to make sure the memory addresses are not the same
    if (rhs.m_ground != m_ground) {
        clear();
        m_ground = new Floor(rhs.m_ground->m_floorNum, rhs.m_ground->m_passenger);
        Floor* currCopy = m_ground;
        Floor* oldCopy = rhs.m_ground->m_next;
        while (oldCopy != nullptr) {
            Floor* copyFloor = new Floor(oldCopy->m_floorNum, oldCopy->m_passenger);
            currCopy->m_next = copyFloor;
            copyFloor->m_previous = currCopy;
            currCopy = copyFloor;
            oldCopy = oldCopy->m_next;
        }      
    }
    return *this;
}



void Elevator::dump() {
    if (m_top != nullptr) {
        Floor* temp = m_top;
        cout << "Top Floor ";
        while (temp->m_previous != nullptr) {
            cout << temp->m_floorNum << " (" << temp->m_passenger << ")" << endl;
            temp = temp->m_previous;
        }
        cout << "Ground Floor " << temp->m_floorNum << " (" << temp->m_passenger << ")" << endl;
    }
}