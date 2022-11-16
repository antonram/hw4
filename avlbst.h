#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
    void removeFix(AVLNode<Key,Value>* n, int diff);
    void rotateLeft(AVLNode<Key,Value>* n);
    void rotateRight(AVLNode<Key,Value>* n);
    bool isZigZag(AVLNode<Key,Value>* n, AVLNode<Key,Value>* p, AVLNode<Key,Value>* g);
    bool isZigZig(AVLNode<Key,Value>* n, AVLNode<Key,Value>* p, AVLNode<Key,Value>* g);


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if(this->root_ == NULL) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
    }

    else {
        AVLNode<Key, Value>* tempRoot = static_cast<AVLNode<Key, Value>*>(this->root_);
        while(true) {
            //if key greater than root's key, move right
            if(new_item.first > tempRoot->getKey()) {
                //if no key to the right,
                if(tempRoot->getRight() == NULL) {
                    tempRoot->setRight(new AVLNode<Key, Value>(new_item.first, new_item.second, tempRoot));
                    tempRoot->setBalance(tempRoot->getBalance()+1);
                    //if balance was 0
                    if(tempRoot->getBalance() == 1) {
                        insertFix(tempRoot, tempRoot->getRight());
                    }
                    return;
                }
                //else, we move on to the right
                else {
                    tempRoot = tempRoot->getRight();
                }
            }

            //else if key less than root's key, move left 
            else if(new_item.first < tempRoot->getKey()) {
                //if no node to the left, add new node.
                if(tempRoot->getLeft() == NULL) {
                    tempRoot->setLeft(new AVLNode<Key, Value>(new_item.first, new_item.second, tempRoot));
                    tempRoot->setBalance(tempRoot->getBalance()-1);
                    //if balance was 0
                    if(tempRoot->getBalance() == -1) {
                        insertFix(tempRoot, tempRoot->getLeft());
                    }
                    return;
                }
                //else, move on to left
                else {
                    tempRoot = tempRoot->getLeft();
                }
            }

            //else if key equals the root's key
            else {
                tempRoot->setValue(new_item.second);
                return;
            }

        }
    }

}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n) {
    if((p == NULL) || (p->getParent() == NULL)) {
        return;
    }
    AVLNode<Key,Value>* g = p->getParent();
    // first do code for p being g's left
    if(g->getLeft() == p) {
        g->setBalance(g->getBalance()-1);
        if(g->getBalance() == 0) {
            return;
        }
        if(g->getBalance() == -1) {
            insertFix(g, p);
            return;
        }
        //if g->getBalance() == -2
        if(isZigZig(n, p, g) == true) {
            rotateRight(g);
            p->setBalance(0);
            g->setBalance(0);
        }
        else if(isZigZag(n, p, g) == true) {
            rotateLeft(p);
            rotateRight(g);
            if(n->getBalance() == -1) {
                p->setBalance(0);
                g->setBalance(1);
                n->setBalance(0);
            }
            else if(n->getBalance() == 0) {
                p->setBalance(0);
                g->setBalance(0);
                n->setBalance(0);
            }
            // n->getBalance() == +1
            else{
                p->setBalance(-1);
                g->setBalance(0);
                n->setBalance(0);
            }
        }
    }
    //if p is g's right child
    else{
        g->setBalance(g->getBalance()+1);
        if(g->getBalance() == 0) {
            return;
        }
        if(g->getBalance() == 1) {
            insertFix(g, p);
            return;
        }
        //if g->getBalance() == 2
        if(isZigZig(n, p, g) == true) {
            rotateLeft(g);
            p->setBalance(0);
            g->setBalance(0);
        }
        else if(isZigZag(n, p, g) == true) {
            rotateRight(p);
            rotateLeft(g);
            if(n->getBalance() == 1) {
                p->setBalance(0);
                g->setBalance(-1);
                n->setBalance(0);
            }
            else if(n->getBalance() == 0) {
                p->setBalance(0);
                g->setBalance(0);
                n->setBalance(0);
            }
            // n->getBalance() == -1
            else{
                p->setBalance(1);
                g->setBalance(0);
                n->setBalance(0);
            }
        }
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    // TODO
    if(this->root_ == NULL) {
        return;
    }
     AVLNode<Key, Value>* n = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key));
    if(n == NULL) {
      return;
    }

    int diff = 0;
		AVLNode<Key, Value>* p;

    if((n->getRight() != NULL) && (n->getLeft() != NULL)) {
        nodeSwap(n, static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(n)));

     p = n->getParent();

     if(p != NULL) {
         if(n == p->getLeft()) {
             diff = 1;
             p->setLeft(n->getLeft());
         }
         // n == p->getRight()
         else {
             diff = -1;
             p->setRight(n->getLeft());
         }
      }
	

			if(n->getLeft() != NULL) {
				n->getLeft()->setParent(p);
			}
		}
	// if only left child
		else if(n->getLeft() != NULL) {
			p = n->getParent();
			if(p != NULL) {
				if(n == p->getLeft()) {
             diff = 1;
             p->setLeft(n->getLeft());
         }
         // n == p->getRight()
         else {
             diff = -1;
             p->setRight(n->getLeft());
         }
			}
			else{
				this->root_ = n->getLeft();
			}
			n->getLeft()->setParent(p);

		}
		//if only right child or no children.
		else {
			p = n->getParent();
			if(p != NULL) {
				if(n == p->getLeft()) {
             diff = 1;
             p->setLeft(n->getRight());
         }
         // n == p->getRight()
         else {
             diff = -1;
             p->setRight(n->getRight());
         }
			}
			else{
				this->root_ = n->getRight();
			}
			if(n->getRight() != NULL) {
				n->getRight()->setParent(p);
			}
		}

     delete n;
     removeFix(p, diff);
}


template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key,Value>* n, int diff) {
    if(n == NULL) {
        return;
    }
    if(diff == 0) {
        return;
    }

    int ndiff = 0;

    AVLNode<Key, Value>* p = n->getParent();
    if(p != NULL) {
        if(p->getLeft() == n) {
            ndiff = 1;
        }
        else{
            ndiff = -1;
        }
    }

    if(diff == -1) {
        if(n->getBalance() + diff == -2) {
            AVLNode<Key, Value>* c = n->getLeft();
            if(c->getBalance() == -1) {
                rotateRight(n);
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p, ndiff);
            }
            else if(c->getBalance() == 0) {
                rotateRight(n);
                n->setBalance(-1);
                c->setBalance(1);
            }
            else{
                AVLNode<Key, Value>* g = c->getRight();
                rotateLeft(c);
                rotateRight(n);
                if(g->getBalance() == 1) {
                    n->setBalance(0);
                    c->setBalance(-1);
                    g->setBalance(0);
                }
                else if(g->getBalance() == 0) {
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else{
                    n->setBalance(1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                removeFix(p, ndiff);
            }
        }
        else if(n->getBalance() + diff == -1) {
            n->setBalance(-1);
        }
        else{
            n->setBalance(0);
            removeFix(p, ndiff);
        }
    }
    else{
        if(n->getBalance() + diff == 2) {
            AVLNode<Key, Value>* c = n->getRight();
            if(c->getBalance() == 1) {
                rotateLeft(n);
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p, ndiff);
            }
            else if(c->getBalance() == 0) {
                rotateLeft(n);
                n->setBalance(1);
                c->setBalance(-1);
            }
            else{
                AVLNode<Key, Value>* g = c->getLeft();
                rotateRight(c);
                rotateLeft(n);
                if(g->getBalance() == -1) {
                    n->setBalance(0);
                    c->setBalance(1);
                    g->setBalance(0);
                }
                else if(g->getBalance() == 0) {
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else{
                    n->setBalance(-1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                removeFix(p, ndiff);
            }
        }
        else if(n->getBalance() + diff == 1) {
            n->setBalance(1);
        }
        else{
            n->setBalance(0);
            removeFix(p, ndiff);
        }
    }

}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* n) {
    AVLNode<Key,Value>* p = n->getParent();
    AVLNode<Key,Value>* c = n->getRight();
    AVLNode<Key,Value>* g = c->getLeft();

    n->setRight(g);
    if(g != NULL) {
        g->setParent(n);
    }

    c->setLeft(n);
    n->setParent(c);
    c->setParent(p);

    if(p != NULL){
        if(p->getLeft() == n) {
            p->setLeft(c); 
        }
        else{
            p->setRight(c);
        }
    }
    else{
        this->root_ = c;
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* n) {
    AVLNode<Key,Value>* p = n->getParent();
    AVLNode<Key,Value>* c = n->getLeft();
    AVLNode<Key,Value>* g = c->getRight();

    n->setLeft(g);
    if(g != NULL) {
        g->setParent(n);
    }

    c->setRight(n);
    n->setParent(c);
    c->setParent(p);

    if(p != NULL) {
        if(p->getRight() == n) {
            p->setRight(c);
        }
        else{
            p->setLeft(c);
        }
    }
    else{
        this->root_ = c;
    }
}


template<class Key, class Value>
bool AVLTree<Key, Value>::isZigZag(AVLNode<Key,Value>* n, AVLNode<Key,Value>* p, AVLNode<Key,Value>* g) {
    if((g->getRight() == p) && (p->getLeft() == n)) {
        return true;
    }
    if((g->getLeft() == p) && (p->getRight() == n)) {
        return true;
    }
    return false;
}


template<class Key, class Value>
bool AVLTree<Key, Value>::isZigZig(AVLNode<Key,Value>* n, AVLNode<Key,Value>* p, AVLNode<Key,Value>* g) {
    if((g->getRight() == p) && (p->getRight() == n)) {
        return true;
    }
    if((g->getLeft() == p) && (p->getLeft() == n)) {
        return true;
    }
    return false;
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
