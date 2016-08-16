//
// Created by anosov on 28.10.15.
//

#ifndef TREE_RB_TREE_SEARCH_H
#define TREE_RB_TREE_SEARCH_H

#include <memory>
#include <iostream>
#include <queue>
#include <string>

using std::cout;
using std::endl;


template<class Key, class Data>
struct Node {
    std::shared_ptr<Node> left, right;
    std::weak_ptr<Node> parent;
    std::shared_ptr<Data> data;
    const Key key;
    bool is_black;

    Node(Key key_, Data* data_) : key(key_), data(data_), is_black(true)
    { }
};

template<class Key, class Data>
class RBTreeSearch {
public:
    typedef Node<Key, Data> NodeT;
    typedef std::shared_ptr<NodeT> NodeS;

    RBTreeSearch() : nil(new NodeT(Key(), nullptr)) {
        root = nil;
    }

    void insert(Key key, Data* data=nullptr) {
        NodeS node(new NodeT(key, data));
        node->left = nil;
        node->right = nil;
        NodeS prev = nil, cur = root;

        while(cur != nil) {
            prev = cur;
            cur = (key < cur->key ? cur->left : cur->right);
        }

        node->parent = prev;
        if(prev == nil) root = node;

        else if(key < prev->key) prev->left = node;
        else prev->right = node;

        node->is_black = false;
        insert_fixup(node);
    }

    void insert_fixup(NodeS node) {
        while(not node->parent.lock()->is_black) {
            NodeS p = node->parent.lock();
            NodeS pp = p->parent.lock();
            if(p == pp->left) {
                if(not pp->right->is_black) {
                    pp->left->is_black = true;
                    pp->right->is_black = true;
                    pp->is_black = false;
                    node = pp;
                }
                else {
                    if(node == p->right) {
                        node = p;
                        left_rotate(node);
                        p = node->parent.lock();
                        pp = p->parent.lock();
                    }
                    p->is_black = true;
                    pp->is_black = false;
                    right_rotate(pp);
                }
            }
            else {
                if(not pp->left->is_black) {
                    pp->left->is_black = true;
                    pp->right->is_black = true;
                    pp->is_black = false;
                    node = pp;
                }
                else {
                    if(node == p->left) {
                        node = p;
                        right_rotate(node);
                        p = node->parent.lock();
                        pp = p->parent.lock();
                    }

                    p->is_black = true;
                    pp->is_black = false;
                    left_rotate(pp);
                }
            }
        }
        root->is_black = true;
    }

    void remove(NodeS node) {
        bool is_black = node->is_black;
        NodeS target;
        if(node->left == nil) {
            target = node->right;
            transplant(node, target);
        }
        else if(node->right == nil) {
            target = node->left;
            transplant(node, target);
        }
        else {
            target = minimum(node->right);
            is_black = target->is_black;
            NodeS child = target->right;
            if(target->parent.lock() != node) {
                transplant(target, child);
                target->right = node->right;
                target->right->parent = target;
            }
            transplant(node, target);
            target->left = node->left;
            target->left->parent = target;
            target->is_black = node->is_black;
        }

        if(is_black) {
            remove_fixup(target);
        }
    }

    void remove_fixup(NodeS node) {
        while(node != root and node->is_black) {

        }

    }

    void transplant(NodeS u, NodeS v) {
        NodeS u_parent = u->parent.lock();
        if(u_parent == nil) root = v;
        else if(u_parent->left == u) u_parent->left = v;
        else u_parent->right = v;
        v->parent = u->parent;
    }

    int depth() const {
        std::queue<NodeS> queue;
        queue.push(root);

        NodeS first = nil, last = nil;
        while(not queue.empty()) {
            if(queue.front() != nil) {
                queue.push(queue.front()->left);
                queue.push(queue.front()->right);

                if(queue.front()->left == nil and queue.front()->right == nil) {
                    if (first == nil) first = queue.front();
                    last = queue.front();

                }
            }
            queue.pop();
        }

//        cout << "root: " << root->key << endl;
//        cout << "first: " << first->key << " depth: " << depth(first) << endl;
//        cout << "last: " << last->key << " depth: " << depth(last) << endl;

        return depth(last);
    }

    NodeS minimum() const {
        return minimum(root);
    }

    NodeS minimum(NodeS node) const {
        while(node->left != nil) node = node->left;
        return node;
    }

    NodeS successor(NodeS node) const {
        if(node->right != nil) {
            return minimum(node->right);
        }
        else {
            NodeS parent = node->parent.lock();
            while(parent and node == parent->right) {
                node = parent;
                parent = parent->parent.lock();
            }
            return parent;
        }
    }

    NodeS find(Key key) const {
        NodeS node = root;
        while(node != nil and node->key != key) {
            node = key < node->key ? node->left : node->right;
        }
        return node;
    }

    void show() const {
        std::queue<NodeS> queue;
        queue.push(root);


        while(not queue.empty()) {
            if(queue.front() != nil) {
                const NodeT& c = *queue.front();
                cout << (c.is_black ? "b" : "r") << c.key;

                cout << " " << (c.parent.lock() != nil ? std::to_string(c.parent.lock()->key) : "X");
                cout << ":" << (c.left != nil ? std::to_string(c.left->key) : "X");
                cout << ":" << (c.right != nil ? std::to_string(c.right->key) : "X");
                cout << endl;

                queue.push(c.left);
                queue.push(c.right);
            }
            queue.pop();
        }

    }

    int depth(NodeS node) const {
        int depth = 0;
        if(node == nil) return 0;

        while(node != nil) {
            node = node->parent.lock();
            depth++;
        }
        return depth;
    }

    void left_rotate(NodeS node) {
        NodeS right_child = node->right;
        node->right = right_child->left;

        if(right_child->left != nil) right_child->left->parent = node;

        NodeS node_parent = node->parent.lock();
        right_child->parent = node_parent;

        if(node_parent == nil) root = right_child;
        else if(node == node_parent->left) node_parent->left = right_child;
        else node_parent->right = right_child;

        right_child->left = node;
        node->parent = right_child;
    }

    void right_rotate(NodeS node) {
        NodeS left_child = node->left;

        node->left = left_child->right;
        if(left_child->right != nil) left_child->right->parent = node;

        NodeS node_parent = node->parent.lock();
        left_child->parent = node_parent;

        if(node_parent == nil) root = left_child;
        else if(node == node_parent->right) node_parent->right = left_child;
        else node_parent->left = left_child;

        left_child->right = node;
        node->parent = left_child;
    }

private:
    NodeS root;
    const NodeS nil;
};


#endif //TREE_RB_TREE_SEARCH_H
