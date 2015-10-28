//#include "tree_search.h"
#include "rb_tree_search.h"

#include <iostream>
#include <random>

using namespace std;


int main() {
    {
        RBTreeSearch<int, char> rb_tree;
        for(int i=250; i>0; i--) {
            int r = static_cast<int>(random() % 40);
//            cout << r << endl;
            rb_tree.insert(r);
        }


        cout << "depth: " << rb_tree.depth() << endl;

//        for(auto it=rb_tree.minimum(); it; it=rb_tree.successor(it)) {
//            cout << it->key << endl;
//        }
    }


    cout << endl;
    cout << "done" << endl;
    return 0;
}