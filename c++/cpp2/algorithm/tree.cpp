#include <iostream>
#include <vector>
#include <string>
#include <functional>

using namespace std;

class BSTree {
public:
    BSTree(): key(0xffffffff), left(nullptr), right(nullptr) {}
    ~BSTree();

    void InsertRecursion(int value);
    BSTree* SearchRecursion(int value);
    void DeleteRecursion(int value);
    void TraversePreOrder();
    void TraverseMidOrder();
    void TraverseOrder();

private:
    int key;
    BSTree* left;
    BSTree* right;
};

void BSTree::InsertRecursion(int value)
{

}

BSTree* BSTree::SearchRecursion(int value)
{

}

void BSTree::DeleteRecursion(int value)
{

}

int main()
{
    BSTree bstree;
    bstree.InsertRecursion(10);
    bstree.InsertRecursion(9);
    bstree.InsertRecursion(5);
    bstree.InsertRecursion(16);
    bstree.InsertRecursion(14);
    bstree.InsertRecursion(8);
    bstree.InsertRecursion(4);
    bstree.InsertRecursion(6);
    bstree.InsertRecursion(7);



    return 0;
}
