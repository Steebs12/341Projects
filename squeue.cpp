// CMSC 341 - Spring 2022 - Project 3
#include "squeue.h"
SQueue::SQueue(prifn_t priFn, HEAPTYPE heapType)//This is the constructor where we initialize all the member variables of the SQueue class
{
    m_heapType = heapType;
    m_priorFunc = priFn;
    m_heap = nullptr;
    m_size = 0;
}

SQueue::~SQueue()//here we deallocate all the memory using a helper recursion function 
{
    queueDestructor(m_heap);
    m_heap = nullptr;//we then set the final "root" to nullptr and then delete it
    delete m_heap;
}

SQueue::SQueue(const SQueue& rhs)//This is the copy constructor, responsible for calling a helper recursion function which copies the data of an old queue over to a new one
{
   m_heap = copyRecurser(rhs.m_heap);
   clear();
}

SQueue& SQueue::operator=(const SQueue& rhs)//this is the overloaded operator which allows us to set queues equal to each other
{
    if (m_heap != rhs.m_heap) {
        clear();
        copyRecurser(rhs.m_heap);
    }
    clear();
    return *this;
}

void SQueue::insertPost(const Post& post) {//here we insert a new post into the queue 
    //Node* newPost = new Node(post);
    if (m_size == 0) {//if there is nothing in the queue we set the first node as the m_heap and increase the size
        m_heap = new Node(post);
        m_size++;
    }
    else {//else we use a merge helper function to merge the new node and the existing queue
        Node* newPost = new Node(post);
        m_heap = mergeHelper(m_heap, newPost);
        m_size++;
    }
}

Post SQueue::getNextPost() {//this function is responsible for removing the node with the highest priority and returning it
    try {
        Node* returnedNode = m_heap;
        if (m_size == 0) {//if there is nothing in the queue we throw an out of range exception
            throw (m_size);
        }
        else {//else we set a pointer to the node we want to get rid of
            
            m_heap = mergeHelper(m_heap->m_left, m_heap->m_right);//merge the left and right children of the m_heap node and decrease the size by 1
            m_size--;
            return returnedNode->getPost();//return the node we wanted to remove
        }
        clear();

    }
    catch (int size) {
        cout << "Squeue is empty" << endl;//catch for the exception thrown if the queue is empty
        return m_heap->getPost();
    }
}

void SQueue::mergeWithQueue(SQueue& rhs) {//this is how we merge two queues together into one
    try {
        if (rhs.getPriorityFn() == getPriorityFn()) {//this is a base case to make sure the queues are of the same priority function
            mergeHelper(m_heap, rhs.m_heap);//we then call a merge helper function to merge the two together
        }
        else
        {
            throw (getPriorityFn());//else we throw an exception if they are not the same priority function
        }
    }
    catch (prifn_t domain_exception) {
        cout << "Different priority types" << endl;//catch for the throw
    }
}

void SQueue::clear() {//basically a callable destructor which uses a helper recursion function
    clearRecurser(m_heap);
    m_heap = nullptr;//we then set the final "root" to nullptr and then delete it
    delete m_heap;
}

int SQueue::numPosts() const //returns the size of the queue
{
    return m_size;
}

void SQueue::printPostsQueue() const {//prints out the posts in the queue in a specific way using a helper recursion function
    printRecurser(m_heap);
}

prifn_t SQueue::getPriorityFn() const {//returns the priority function used to determine the priority number
    return m_priorFunc;
}

void SQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {//is supposed to set the priority to a different priority and then rebuild the heap
    m_priorFunc = priFn;
    m_heapType = heapType;
}

HEAPTYPE SQueue::getHeapType() const {//returns the heap type
    return m_heapType;
}

void SQueue::dump() const//given dump function
{
    if (m_size == 0) {
        cout << "Empty skew heap.\n";
    }
    else {
        dump(m_heap);
        cout << endl;
    }
}

void SQueue::dump(Node* pos) const {//given dump function
    if (pos != nullptr) {
        cout << "(";
        dump(pos->m_left);
        cout << m_priorFunc(pos->m_post) << ":" << pos->m_post.getPostID();
        dump(pos->m_right);
        cout << ")";
    }
}

ostream& operator<<(ostream& sout, const Post& post) {//given overloaded operator
    sout << "Post ID: " << post.getPostID()
        << ", number of likes: " << post.getNumLikes()
        << ", interest level: " << post.getInterestLevel()
        << ", connection level: " << post.getConnectLevel()
        << ", post time: " << post.getPostTime();
    return sout;
}

ostream& operator<<(ostream& sout, const Node& node) {//given overloaded operator
    sout << node.getPost();
    return sout;
}

void SQueue::printRecurser(Node* tempNode) const {//helper recursion function for printing out each post and it's data
    if (tempNode == nullptr) {
        return;
    }
    cout << "[" << m_priorFunc(tempNode->getPost()) << "]" << "Post ID: " << tempNode->getPost().getPostID() << " ," << "number of likes: " << tempNode->getPost().getNumLikes() << " ," << "interest level: " <<
        tempNode->getPost().getInterestLevel() << " ," << "connection level: " << tempNode->getPost().getConnectLevel() << " ," << "post time: " << tempNode->getPost().getPostTime() << endl;
    printRecurser(tempNode->m_left);
    printRecurser(tempNode->m_right);
}

void SQueue::clearRecurser(Node* aNode) {//helper recursion function to deallocate all the memory and reinitialize the values of each node
    if (aNode != nullptr) {
        clearRecurser(aNode->m_left);
        clearRecurser(aNode->m_right);
        delete aNode;
        aNode = nullptr;
    }
    delete aNode;
    aNode = nullptr;
}

int SQueue::priorityFn1(const Post& post) {//first priority function
    //needs MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [1-510]
    //the highest priority would be 500+10 = 510
    //the lowest priority would be 0+1 = 1
    //the larger value means the higher priority
    int priority = post.getNumLikes() + post.getInterestLevel();
    return priority;
}

int SQueue::priorityFn2(const Post& post) {//second priority function
    //needs MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [2-55]
    //the highest priority would be 1+1 = 2
    //the lowest priority would be 5+50 = 55
    //the smaller value means the higher priority
    int priority = post.getPostTime() + post.getConnectLevel();
    return priority;
}

Node* SQueue::mergeHelper(Node* p1, Node* p2) {//helper recursion function which aids in the merging of two nodes and follows the descriptions outlined in the projec document
    if (p1 == nullptr) {
        return p2;
    }
    else if (p2 == nullptr) {
        return p1;
    }
    else if (getHeapType() == MINHEAP) {//if the heap is of a MINHEAP
        int p1Priority = m_priorFunc(p1->getPost());
        int p2Priority = m_priorFunc(p2->getPost());
        if (p2Priority < p1Priority) {//we should always have p1 as the higher priority node
            Node* temp = p1;
            p1 = p2;
            p2 = temp;
        }
        Node* temp = p1->m_left;
        p1->m_left = p1->m_right;
        p1->m_right = temp;
        p1->m_left = mergeHelper(p1->m_left, p2);//recursively calls the function so that we can merge the two nodes
        return p1;
    }
    else if (getHeapType() == MAXHEAP) {//if the heap is of a MAXHEAP
        int p1Priority = m_priorFunc(p1->getPost());
        int p2Priority = m_priorFunc(p2->getPost());
        if (p2Priority > p1Priority) {
            Node* temp = p1;
            p1 = p2;
            p2 = temp;
        }
        Node* temp = p1->m_left;
        p1->m_left = p1->m_right;
        p1->m_right = temp;
        p1->m_left = mergeHelper(p1->m_left, p2);
        return p1;
    }
    return p1;
}

void SQueue::queueDestructor(Node* aNode) {//helper recursion function which aids in the deallocation of memory and reinitialization of member variables to avoid memory leaks
    if (aNode != nullptr) {
        queueDestructor(aNode->m_left);
        queueDestructor(aNode->m_right);
        delete aNode;
        aNode = nullptr;
    }
    delete aNode;
    aNode = nullptr;
}

Node* SQueue::copyRecurser(Node* oldNode) {//helper recursion function which aids in the copying of one queue to a new cue
    if (oldNode == nullptr) {
        return nullptr;
    }
    Node* newNode = new Node(oldNode->getPost());
    m_size++;
    newNode->m_left = copyRecurser(oldNode->m_left);
    newNode->m_right = copyRecurser(oldNode->m_right);

    clear();
    return newNode;
}
