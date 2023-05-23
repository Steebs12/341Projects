// UMBC - CMSC 341 - Spring 2022 - Proj2
#include "fleet.h"
Fleet::Fleet() {//we initialize the private member variable m_root to nullptr as there are no ships in a fleet when you first create it
    m_root = nullptr;
}

Fleet::~Fleet() {//deallocates the memory at the end of our program
    treeDestructor(m_root);
    m_root = nullptr;
    delete m_root;
}

void Fleet::insert(const Ship& ship) {//this uses the helper function checkID to determine if the ID is either out of bounds or if it already exists
    if (checkID(m_root, ship.getID()) == false) {
        cout << "This Ship cannot be inserted Test Passed" << endl;
    }
    else if (m_root == nullptr) {//If the ID doesnt already exist but it is the first Ship in the fleet we create a new ship and set the root equal to the new ship
        Ship* newShip = new Ship(ship.getID());
        m_root = newShip;
    }
    else {//otherwise we just use a helper recursion function to insert the newShip either to the right or left of a node in the AVL tree
        Ship* newShip = new Ship(ship.getID());
        insertRecurser(m_root, newShip);
        //updateHeight(m_root);
        //rebalance(m_root);
    }
}

void Fleet::clear() {//this is basically a callable destructor
    treeDestructor(m_root);
}

void Fleet::remove(int id) {//this function is responsible for removing a ship with an inputed ID
    if (checkID(m_root, id) == true) {//making sure the ID is actually real
        cout << "This ID cannot be removed" << endl;
    }
    else {
        removeRecurser(m_root, id);//helper function to recursively iterate through the BST for the Ship we want to remove and then removing it
    }

}

void Fleet::updateHeight(Ship* aShip) {//this function is responsible for updating the heights of the Ships after we insert a new one
    int heightLeft = height(aShip->getLeft());//we have two height helper functions which return the height of m_height of the ship inserted into the function
    int heightRight = height(aShip->getRight());
    aShip->setHeight(maxValue(heightLeft, heightRight) + 1);
}

int Fleet::checkImbalance(Ship* aShip) {//This function is responsible for checking if the Ships heights are imbalanced or not
    if (aShip->getLeft() == nullptr || aShip->getRight() == nullptr) {
        return -1;
    }
    return height(aShip->getLeft()) - height(aShip->getRight());
}

Ship* Fleet::rebalance(Ship* aShip) {//this function is responsible for rebalancing the nodes so they become an AVL tree
    int balance = checkImbalance(aShip);
    if (balance > 2 && aShip->getID() < aShip->getLeft()->getID()) {
        return rotateRight(aShip);
    }
    //updateHeight(aShip);

    else if (balance < -2 && aShip->getID() > aShip->getRight()->getID()) {
        return rotateLeft(aShip);
    }
    //updateHeight(aShip);
    else if (balance > 2 && aShip->getID() > aShip->getLeft()->getID()){
        aShip->setLeft(rotateLeft(aShip->getLeft()));
        return rotateRight(aShip);
    }
    //updateHeight(aShip);

    else if (balance < -2 && aShip->getID() < aShip->getRight()->getID()){
        aShip->setRight(rotateRight(aShip->getRight()));
        return rotateLeft(aShip);
    }
    else {
        //updateHeight(aShip);
        return aShip;
    }
    return aShip;
}

void Fleet::dumpTree() const {//provided function
    dump(m_root);
}

void Fleet::dump(Ship* aShip) const {//provided function
    if (aShip != nullptr) {
        cout << "(";
        dump(aShip->m_left);//first visit the left child
        cout << aShip->m_id << ":" << aShip->m_height;//second visit the node itself
        dump(aShip->m_right);//third visit the right child
        cout << ")";
    }
}

void Fleet::listShips() const {
    printShips(m_root);//helper function to recursively iterate through the ships and print out specific data
}

bool Fleet::setState(int id, STATE state) {//this function is repsonsible for changing the state of the ship with the id passed into it
    Ship* ptrShip = m_root;
    while (ptrShip != nullptr) {
        if (ptrShip->getID() == id) {
            ptrShip->setState(state);
            return true;//if it works we return true
        }
        if (ptrShip->getLeft()->getID() < id) {
            ptrShip = ptrShip->getRight();
        }
        else {
            ptrShip = ptrShip->getLeft();
        }
    }
    return false;//else we return false   
}

void Fleet::removeLost() {
    iteratorLost(m_root);//helper function to iterate through BST and remove all the Ships with State "LOST"
}

bool Fleet::findShip(int id) const {//if the ship exists we return true, if it doesnt we return false
    if (iteratorID(m_root, id)) {
        return true;
    }
    else{
        return false;
    }
}

bool Fleet::checkID(Ship* aShip, int IDnum) {//helper function which checks to make sure the ID is valid and exists 
    if (aShip == nullptr) {
        return true;
    }
    if (IDnum == aShip->getID() || IDnum > MAXID || IDnum < MINID) {
        return false;
    }
    else {
        if (IDnum > aShip->getID()) {
            return checkID(aShip->getRight(), IDnum);//we recursively go through the BST until we either reach the correct place or we reach the end of the tree
        }
        else {
            return checkID(aShip->getLeft(), IDnum);
        }
    }
}

void Fleet::printShips(Ship* aShip) const {//this helper function recursively going through the BST and prints out the ID, the State, and the Type
    if (aShip != nullptr) {
        printShips(aShip->getLeft());
        cout << aShip->getID() << ":" << aShip->getStateStr() << ":" << aShip->getTypeStr() << endl;
        printShips(aShip->getRight());
    }
}

bool Fleet::iteratorID(Ship* aShip, int IDnum) const {//checks to make sure the ship ID exists using recursion
    if (aShip->getID() == IDnum) {
        return true;//returns true if it exists
    }
    else {
        if (IDnum > aShip->getID()) {
            return iteratorID(aShip->getRight(), IDnum);
        }
        else {
            return iteratorID(aShip->getLeft(), IDnum);
        }
    }
    return false;//returns false if it doesnt
}

void Fleet::iteratorLost(Ship* aShip) {//recursively goes through the tree and removes the Ships with state LOST
    if (aShip != nullptr) {
        if (aShip->getTypeStr() == "LOST") {
            remove(aShip->getID());
            //need to make sure the tree is balanced after removing the Ship
            //either call both or onecheckImbalance and rebalance
        }
        iteratorLost(aShip->getLeft());
        iteratorLost(aShip->getRight());
    }
}

void Fleet::treeDestructor(Ship* aShip) {//destructor using recursion 
    if (aShip != nullptr) {
        /*
        if (aShip->getLeft() == nullptr) {
            treeDestructor(aShip->getRight());
            delete aShip;
            aShip = nullptr;
        }
        else {
            treeDestructor(aShip->getLeft());
            delete aShip;
            aShip = nullptr;
        }
        //treeDestructor(aShip->getLeft());
        //treeDestructor(aShip->getRight());
        /*
        if (aShip->getLeft() == nullptr && aShip->getRight() == nullptr) {
            delete aShip;
            aShip = nullptr;
        } 
        */
        treeDestructor(aShip->getLeft());
        treeDestructor(aShip->getRight());
        delete aShip;
        aShip = nullptr;
    }
    delete aShip;
    aShip = nullptr;
}

Ship* Fleet::insertRecurser(Ship* oldShip, Ship* newShip) {//recursively goes through the BST until we reach the point at which the Ship needs to be inserted
    if (newShip->getID() > oldShip->getID() && oldShip->getRight() == nullptr) {
        oldShip->setRight(newShip);
        updateHeight(oldShip);
        //rebalance(oldShip);
        return oldShip;
    }
    else if (newShip->getID() < oldShip->getID() && oldShip->getLeft() == nullptr) {
        oldShip->setLeft(newShip);
        updateHeight(oldShip);
        //rebalance(oldShip);
        return oldShip;
    }
    else if (newShip->getID() < oldShip->getID()) {
        oldShip->setLeft(insertRecurser(oldShip->getLeft(), newShip));
        rebalance(oldShip);
        /*
        if (checkImbalance(oldShip) > 1 || checkImbalance(oldShip) < -1) {//problem with rebalance
            
            rebalance(oldShip);
            //updateHeight(oldShip);
        }
        //rebalance(oldShip);
        //insertRecurser(oldShip->getLeft(), newShip);
        */
    }
    else {
        oldShip->setRight(insertRecurser(oldShip->getRight(), newShip));
        rebalance(oldShip);
        /*
        if (checkImbalance(oldShip) > 1 || checkImbalance(oldShip) < -1) {//problem with rebalance
            rebalance(oldShip);
            //updateHeight(oldShip);
        }
        //rebalance(oldShip);
        //insertRecurser(oldShip->getRight(), newShip);
        */
    } 
    
    
    
    //rebalance(oldShip);
    updateHeight(oldShip);
    return oldShip;
}

Ship* Fleet::removeRecurser(Ship* oldShip, int IDnum) {//recursively goes through the tree and removes the Ship with the ID passed into the function
    if (oldShip == nullptr) {
        return oldShip;
    }
    if (IDnum == oldShip->getID()) {
        if (oldShip->getLeft() == nullptr && oldShip->getRight() == nullptr) {//the node we are removing is a leaf
            delete oldShip;
            oldShip = nullptr;
            //updateHeight(oldShip);
            return nullptr;
        }
        else if (oldShip->getLeft() == nullptr) {//the node we are removing have a right child but no left
            Ship* temp = oldShip->getRight();
            //oldShip == nullptr;
            oldShip->setID(temp->getID());
            oldShip->setRight(nullptr);
            delete temp;
            //oldShip = nullptr;
            updateHeight(oldShip);
            rebalance(oldShip);
            return oldShip;
        }
        else if (oldShip->getRight() == nullptr) {//the node we are removing have a left child but no right
            Ship* temp = oldShip->getLeft();
            oldShip->setID(temp->getID());
            oldShip->setLeft(nullptr);
            delete temp;
            //oldShip = nullptr;
            updateHeight(oldShip);
            rebalance(oldShip);
            return oldShip;
        }
        else {
            //somehow code removing a node when it has two children.
        }

    }
    else if (IDnum < oldShip->getID()) {
        removeRecurser(oldShip->getLeft(), IDnum);
    }
    else {
        removeRecurser(oldShip->getRight(), IDnum);
    }
    //rebalance(oldShip);
    updateHeight(oldShip);
    return oldShip;
}

Ship* Fleet::minNodeFinder(Ship* aShip) {//finds the lowest valued node in the list
    Ship* current = aShip;
    while (current != nullptr && current->getLeft() != nullptr) {
        current = current->getLeft();
    }
    return current;
}

int Fleet::height(Ship* aShip) {//returns the height of a ship, if its nullptr we return -1
    if (aShip == nullptr) {
        return -1;
    }
    else {
        return aShip->getHeight();
    }
}

Ship* Fleet::rotateRight(Ship* y) {//repsonsible for a right rotation of the Ships

    Ship* x = y->getLeft();
    Ship* T2 = x->getRight();

    x->setRight(y);
    y->setLeft(T2);

    updateHeight(y);
    updateHeight(x);

    return x;
}

Ship* Fleet::rotateLeft(Ship* x) {//responsible for a left rotation of the Ships

    Ship* y = x->getRight();
    Ship* T2 = y->getLeft();

    y->setLeft(x);
    x->setRight(T2);

    updateHeight(x);
    updateHeight(y);

    return y;

}

int Fleet::maxValue(int a, int b) {
    if (a > b) {
        return a;
    }
    else {
        return b;
    }
}