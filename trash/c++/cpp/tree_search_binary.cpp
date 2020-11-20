#include <iostream>
#include <vector>
#include <string>

using namespace std;

template<typename K, typename V>
struct BinaryNode {
    K key;   // This is the node actual value.
    V value; // attached value

    BinaryNode<K, V>* left;
    BinaryNode<K, V>* right;

    BinaryNode()  {  }
    BinaryNode(K key, V value, BinaryNode<K, V>* left, BinaryNode<K, V>* right)
    {
        this->key = key;
        this->value = value;
        this->left = left;
        this->right = right;
    }
};
