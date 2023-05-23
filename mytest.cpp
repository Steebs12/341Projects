// CMSC 341 - Spring 2022 - Project 3
#include "squeue.h"
#include <random>
// the followings are sample priority functions to be used by SQueue class
// users can define their own priority functions
// Priority functions compute an integer priority for a post.  Internal
// computations may be floating point, but must return an integer.
int priorityFn1(const Post& post);// works with a MAXHEAP
int priorityFn2(const Post& post);// works with a MINHEAP

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
    bool insertNormalCaseMINHEAP();
    bool recurseIteratorMin(Node* aNode, prifn_t priorityFn);
    bool recurseIteratorMax(Node* aNode, prifn_t priorityFn);
    bool insertNormalCaseMAXHEAP();
    bool removeTestNormalMINHEAP();
    bool removeTestNormalMAXHEAP();
    bool mergeTest();
    bool copyConstuctorTest();
};

int main() {
    Random idGen(MINPOSTID, MAXPOSTID);
    Random likesGen(MINLIKES, MAXLIKES);
    Random timeGen(MINTIME, MAXTIME);
    Random conLevelGen(MINCONLEVEL, MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL, MAXINTERESTLEVEL);
    SQueue queue1(priorityFn2, MINHEAP);
    
    for (int i = 0; i < 10; i++) {
        Post aPost(idGen.getRandNum(),
            likesGen.getRandNum(),
            conLevelGen.getRandNum(),
            timeGen.getRandNum(),
            interestGen.getRandNum());
        queue1.insertPost(aPost);
    }
    
    
    cout << "\nDump of the queue with priorityFn2 (MINHEAP):\n";
    queue1.dump();
    queue1.setPriorityFn(priorityFn1, MAXHEAP);
    cout << "\nDump of the queue with priorityFn1 (MAXHEAP):\n";
    queue1.dump();
    cout << "\nPreorder traversal of the nodes in the queue with priorityFn1 (MAXHEAP):\n";
    queue1.printPostsQueue();

    cout << endl;
    Tester test;
    test.insertNormalCaseMINHEAP();
    test.insertNormalCaseMAXHEAP();
    test.removeTestNormalMINHEAP();
    test.removeTestNormalMAXHEAP();
    test.mergeTest();
    test.copyConstuctorTest();
    return 0;

}

int priorityFn1(const Post& post) {
    //needs MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [1-510]
    //the highest priority would be 500+10 = 510
    //the lowest priority would be 0+1 = 1
    //the larger value means the higher priority
    int priority = post.getNumLikes() + post.getInterestLevel();
    return priority;
}

int priorityFn2(const Post& post) {
    //needs MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [2-55]
    //the highest priority would be 1+1 = 2
    //the lowest priority would be 5+50 = 55
    //the smaller value means the higher priority
    int priority = post.getPostTime() + post.getConnectLevel();
    return priority;
}

bool Tester::recurseIteratorMin(Node* aNode, prifn_t priorityFn) {
    if (aNode == nullptr) {
        return true;
    }
    if (aNode->m_left != nullptr) {
        if (priorityFn(aNode->getPost()) > priorityFn(aNode->m_left->getPost())) {
            return false;
        }
    }
    else if (aNode->m_right != nullptr) {
        if (priorityFn(aNode->getPost()) > priorityFn(aNode->m_right->getPost())) {
            return false;
        }
    }
    else {
        recurseIteratorMin(aNode->m_left, priorityFn);
        recurseIteratorMin(aNode->m_right, priorityFn);
    
    }
    return true;
}

bool Tester::recurseIteratorMax(Node* aNode, prifn_t priorityFn) {
    if (aNode == nullptr) {
        return true;
    }
    if (aNode->m_left != nullptr) {
        if (priorityFn(aNode->getPost()) < priorityFn(aNode->m_left->getPost())) {
            return false;
        }
    }
    else if (aNode->m_right != nullptr) {
        if (priorityFn(aNode->getPost()) < priorityFn(aNode->m_right->getPost())) {
            return false;
        }
    }
    else {
        recurseIteratorMax(aNode->m_left, priorityFn);
        recurseIteratorMax(aNode->m_right, priorityFn);

    }
    return true;
}

bool Tester::insertNormalCaseMINHEAP() {
        Random idGen(MINPOSTID, MAXPOSTID);
        Random likesGen(MINLIKES, MAXLIKES);
        Random timeGen(MINTIME, MAXTIME);
        Random conLevelGen(MINCONLEVEL, MAXCONLEVEL);
        Random interestGen(MININTERESTLEVEL, MAXINTERESTLEVEL);
        SQueue queue1(priorityFn2, MINHEAP);


        for (int i = 0; i < 100; i++) {
            Post aPost(idGen.getRandNum(),
                likesGen.getRandNum(),
                conLevelGen.getRandNum(),
                timeGen.getRandNum(),
                interestGen.getRandNum());
            queue1.insertPost(aPost);
        }
        bool test = recurseIteratorMin(queue1.m_heap, priorityFn2);
        if (test != true) {
            cout << "Normal test failed minheap" << endl;
            return false;
        }
        else {
            cout << "Normal test passed minheap" << endl;
            return true;
        }
}

bool Tester::insertNormalCaseMAXHEAP() {
    Random idGen(MINPOSTID, MAXPOSTID);
    Random likesGen(MINLIKES, MAXLIKES);
    Random timeGen(MINTIME, MAXTIME);
    Random conLevelGen(MINCONLEVEL, MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL, MAXINTERESTLEVEL);
    SQueue queue1(priorityFn1, MAXHEAP);


    for (int i = 0; i < 100; i++) {
        Post aPost(idGen.getRandNum(),
            likesGen.getRandNum(),
            conLevelGen.getRandNum(),
            timeGen.getRandNum(),
            interestGen.getRandNum());
        queue1.insertPost(aPost);
    }
    bool test = recurseIteratorMax(queue1.m_heap, priorityFn1);
    if (test != true) {
        cout << "Normal test failed maxheap" << endl;
        return false;
    }
    else {
        cout << "Normal test passed maxheap" << endl;
        return true;
    }

}

bool Tester::removeTestNormalMINHEAP() {
    Random idGen(MINPOSTID, MAXPOSTID);
    Random likesGen(MINLIKES, MAXLIKES);
    Random timeGen(MINTIME, MAXTIME);
    Random conLevelGen(MINCONLEVEL, MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL, MAXINTERESTLEVEL);
    SQueue queue1(priorityFn2, MINHEAP);


    for (int i = 0; i < 100; i++) {
        Post aPost(idGen.getRandNum(),
            likesGen.getRandNum(),
            conLevelGen.getRandNum(),
            timeGen.getRandNum(),
            interestGen.getRandNum());
        queue1.insertPost(aPost);
    }
    for (int i = 0; i < 100; i++) {
        queue1.getNextPost();
    }
    if (queue1.m_size == 0) {
        cout << "test passed removed minheap" << endl;
        return true;
    }
    else {
        cout << "test failed removed minheap" << endl;
        return false;
    }
}

bool Tester::removeTestNormalMAXHEAP() {
    Random idGen(MINPOSTID, MAXPOSTID);
    Random likesGen(MINLIKES, MAXLIKES);
    Random timeGen(MINTIME, MAXTIME);
    Random conLevelGen(MINCONLEVEL, MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL, MAXINTERESTLEVEL);
    SQueue queue1(priorityFn1, MAXHEAP);


    for (int i = 0; i < 100; i++) {
        Post aPost(idGen.getRandNum(),
            likesGen.getRandNum(),
            conLevelGen.getRandNum(),
            timeGen.getRandNum(),
            interestGen.getRandNum());
        queue1.insertPost(aPost);
    }
    for (int i = 0; i < 100; i++) {
        queue1.getNextPost();
    }
    if (queue1.m_size == 0) {
        cout << "test passed removed maxheap" << endl;
        return true;
    }
    else {
        cout << "test failed removed maxheap" << endl;
        return false;
    }
}

bool Tester::mergeTest() {
    Random idGen(MINPOSTID, MAXPOSTID);
    Random likesGen(MINLIKES, MAXLIKES);
    Random timeGen(MINTIME, MAXTIME);
    Random conLevelGen(MINCONLEVEL, MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL, MAXINTERESTLEVEL);
    SQueue queue1(priorityFn2, MINHEAP);
    SQueue queue2(priorityFn2, MINHEAP);


    for (int i = 0; i < 100; i++) {
        Post aPost(idGen.getRandNum(),
            likesGen.getRandNum(),
            conLevelGen.getRandNum(),
            timeGen.getRandNum(),
            interestGen.getRandNum());
        queue1.insertPost(aPost);
    }
    queue2.mergeWithQueue(queue1);
    if (queue1.m_size = (queue1.m_size + queue2.m_size)) {
        cout << "Merge test passed" << endl;
        return true;
    }
    else {
        return false;
    }
}

bool Tester::copyConstuctorTest() {
    Random idGen(MINPOSTID, MAXPOSTID);
    Random likesGen(MINLIKES, MAXLIKES);
    Random timeGen(MINTIME, MAXTIME);
    Random conLevelGen(MINCONLEVEL, MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL, MAXINTERESTLEVEL);
    SQueue queue1(priorityFn2, MINHEAP);
    SQueue queue2(priorityFn2, MINHEAP);


    for (int i = 0; i < 100; i++) {
        Post aPost(idGen.getRandNum(),
            likesGen.getRandNum(),
            conLevelGen.getRandNum(),
            timeGen.getRandNum(),
            interestGen.getRandNum());
        queue1.insertPost(aPost);
    }
    queue2 = queue1;
    if ((queue2.m_size = queue1.m_size)) {
        if ((queue2.getHeapType() == MINHEAP && queue1.getHeapType() == MINHEAP)) {
            if ((queue2.getPriorityFn() == queue1.getPriorityFn())) {
                cout << "Copy test passed" << endl;
                return true;
            }
            return false;
        }
        return false;
    }
    else {
        cout << "Copy constructor test failed" << endl;
        return false;
    }

}
