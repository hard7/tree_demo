//
// Created by anosov on 28.10.15.
//

#ifndef TREE_TREE_SEARCH_H
#define TREE_TREE_SEARCH_H

#include <memory>

template<class Key, class Data>
struct Node {
    std::shared_ptr<Node> left, right;
    std::weak_ptr<Node> parent;
    std::shared_ptr<Data> data;
    const Key key;

    Node(Key key_, Data* data_)
            : key(key_), data(data_)
    { }
};

template<class Key, class Data>
class TreeSearch {
public:
    typedef Node<Key, Data> NodeT;
    typedef std::shared_ptr<NodeT> NodeS;
    void insert(Key key, Data* data) {
        NodeS node(new NodeT(key, data));
        NodeS prev, cur = root;

        while(cur) {
            prev = cur;
            cur = (key < cur->key ? cur->left : cur->right);
        }

        node->parent = prev;
        if(not prev) root = node;
        else if(key < prev->key) prev->left = node;
        else prev->right = node;
    }

    void insert(Key key, Data data) {
        insert(key, new Data(data));
    }
//
//    void ordered_tree_walk() const {
//        ordered_tree_walk(root);
//    }

//    static
//    void ordered_tree_walk(NodeS node) {
//        if(node) {
//            ordered_tree_walk(node->left);
//            std::cout << node->key << std::endl;
//            ordered_tree_walk(node->right);
//        }
//    }

    NodeS tree_search(Key k) const {
        NodeS cur = root;
        while(cur and k != cur->key) {
            cur = k < cur->key ? cur->left : cur->right;
        }
        return cur;
    }

    NodeS successor(NodeS node) {
        if(node->right) {
            return minimum(node->right);
        }
        NodeS parent = node->parent.lock();
        while(parent and node == parent->right) {
            node = parent;
            parent = parent->parent.lock();
        }
        return parent;
    }

    NodeS predecessor(NodeS node) {
        if(node->left) {
            return maximum(node->left);
        }
        NodeS parent = node->parent.lock();
        while(parent and node == parent->left) {
            node = parent;
            parent = parent->parent.lock();
        }
        return parent;
    }

    NodeS minimum(NodeS node) {
        while(node->left) node = node->left;
        return node;
    }

    NodeS minimum() {
        return minimum(root);
    }

    NodeS maximum(NodeS node) {
        while(node->right) node = node->right;
        return node;
    }

    NodeS maximum() {
        return maximum(root);
    }

    void transplant(NodeS u, NodeS v) {
        NodeS u_parent = u->parent.lock();
        if(not u_parent) root = v;
        else if(u_parent->left == u) u_parent->left = v;
        else u_parent->right = v;
        if(v) v->parent = u->parent;
    }

    void remove(NodeS node) {
        if(not node->left) {
            transplant(node, node->right);
        }
        else if(not node->right) {
            transplant(node, node->left);
        }
        else {
            NodeS next = minimum(node->right);
            if(next->parent.lock() != node) {
                transplant(next, next->right);
                next->right = node->right;
                next->right->parent = next;
            }
            transplant(node, next);
            next->left = node->left;
            next->left->parent = next;
        }

    }

private:
    std::shared_ptr<NodeT> root;
};

#include <iostream>
#include <random>

void tree_test() {
    TreeSearch<int, char> tree;

    for (int i = 0; i < 20; ++i) {
        int rand = (int) (random() % 20);
        tree.insert(rand, 'a');
        std::cout << rand << " ";
    }
    std::cout << std::endl;

    for (auto it = tree.minimum(); it; it = tree.successor(it)) std::cout << it->key << " ";
    std::cout << std::endl;

    tree.remove(tree.minimum());
    tree.remove(tree.maximum());

    std::cout << std::endl;
    for (auto it = tree.maximum(); it; it = tree.predecessor(it)) std::cout << it->key << " ";
    std::cout << std::endl;
}


#endif //TREE_TREE_SEARCH_H
