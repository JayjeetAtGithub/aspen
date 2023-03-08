#include "../graph/api.h"
#include "../trees/utils.h"
#include "../lib_extensions/sparse_table_hash.h"
#include "../graph/tree_plus/immutable_graph_tree_plus.h"
#include "../pbbslib/random_shuffle.h"

#include <cstring>

#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
#include <cmath>


#include <iostream>
#include <fstream>

#include "rmat_util.h"
using namespace std;
 
class BTreeNode
{
    int *keys;  // An array of keys
    int t;      // Minimum degree (defines the range for number of keys)
    BTreeNode **C; // An array of child pointers
    int n;     // Current number of keys
    bool leaf; // Is true when node is leaf. Otherwise false
public:
    BTreeNode(int _t, bool _leaf);   // Constructor
 
    void insertNonFull(int k);
 
    void splitChild(int i, BTreeNode *y);
 
    void traverse();
 
    BTreeNode *search(int k);   // returns NULL if k is not present.

friend class BTree;
};
 
class BTree
{
    BTreeNode *root; // Pointer to root node
    int t;  // Minimum degree
public:
    BTree(int _t)
    {  root = NULL;  t = _t; }
 
    void traverse()
    {  if (root != NULL) root->traverse(); }
 
    BTreeNode* search(int k)
    {  return (root == NULL)? NULL : root->search(k); }
 
    void insert(int k);
};
 
BTreeNode::BTreeNode(int t1, bool leaf1)
{
    t = t1;
    leaf = leaf1;
 
    keys = new int[2*t-1];
    C = new BTreeNode *[2*t];
 
    n = 0;
}
 
void BTreeNode::traverse()
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (leaf == false)
            C[i]->traverse();
        cout << " " << keys[i];
    }
 
    if (leaf == false)
        C[i]->traverse();
}
 
BTreeNode *BTreeNode::search(int k)
{
    int i = 0;
    while (i < n && k > keys[i])
        i++;
 
    if (keys[i] == k)
        return this;
 
    if (leaf == true)
        return NULL;
 
    return C[i]->search(k);
}
 
void BTree::insert(int k)
{
    if (root == NULL)
    {
        root = new BTreeNode(t, true);
        root->keys[0] = k;
        root->n = 1;
    }
    else
    {
        if (root->n == 2*t-1)
        {
            BTreeNode *s = new BTreeNode(t, false);
 
            s->C[0] = root;
 
            s->splitChild(0, root);
 
            int i = 0;
            if (s->keys[0] < k)
                i++;
            s->C[i]->insertNonFull(k);
 
            root = s;
        }
        else 
            root->insertNonFull(k);
    }
}

void BTreeNode::insertNonFull(int k)
{
    int i = n-1;
 
    if (leaf == true)
    {
        while (i >= 0 && keys[i] > k)
        {
            keys[i+1] = keys[i];
            i--;
        }
 
        keys[i+1] = k;
        n = n+1;
    }
    else
    {
        while (i >= 0 && keys[i] > k)
            i--;
 
        if (C[i+1]->n == 2*t-1)
        {
            splitChild(i+1, C[i+1]);
 

            if (keys[i+1] < k)
                i++;
        }
        C[i+1]->insertNonFull(k);
    }
}
 

void BTreeNode::splitChild(int i, BTreeNode *y)
{

    BTreeNode *z = new BTreeNode(y->t, y->leaf);
    z->n = t - 1;
 
    for (int j = 0; j < t-1; j++)
        z->keys[j] = y->keys[j+t];
 
    if (y->leaf == false)
    {
        for (int j = 0; j < t; j++)
            z->C[j] = y->C[j+t];
    }
 
    y->n = t - 1;

    for (int j = n; j >= i+1; j--)
        C[j+1] = C[j];
 
    C[i+1] = z;
 
    for (int j = n-1; j >= i; j--)
        keys[j+1] = keys[j];
 
    keys[i] = y->keys[t-1];
 
    n = n + 1;
}
 
int main()
{
    BTree t(500000);
    
    auto r = pbbs::random(500000);
    
    timer st;
    st.start();
    for (int i = 0; i < 500000; i++) {
        t.insert(r.ith_rand(i));
    }
    double runtime = st.stop();
    std::cout << "runtime (one-at-a-time) = " << runtime << std::endl;


    return 0;
}