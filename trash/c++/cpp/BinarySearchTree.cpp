#include <iostream>

using namespace std;

template<class T>
class BinarySearchTree {
public:
    BinarySearchTree(): root(NULL) {  }
    ~BinarySearchTree() {  }

    const T &findMin() const;
    const T &findMax() const;
    bool contains(const T &x) const;
    bool isEmpty() const
    {
        return root ? false : true;
    }
    void printTree() const;
    void insert(const T &x);
    void remove(const T &x);
    void makeEmpty() const;

private:
    BinaryNide<T> *root;
};
