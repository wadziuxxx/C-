//
// Created by Maciej on 24.05.2018.
//

#include "Map.h"


template<class Key, class Value>
Element<Key, Value>::Element() {
    left = nullptr;
    right = nullptr;
    parent = nullptr;
    color = Color::red;
}

template<class Key, class Value>
Map<Key, Value>::Map() {
    root = nullptr;
}

template<class Key, class Value>
Map<Key, Value>::~Map() {
    delocate(root);
    root = nullptr;
}

template<class Key, class Value>
void Map<Key, Value>::delocate(Element<Key, Value> *toDelete) {
    if(!toDelete)
        return;

    delocate(toDelete->left);
    delocate(toDelete->right);

    delete toDelete;
}

template<class Key, class Value>
Value &Map<Key, Value>::operator[](const Key &key){
    Element<Key,Value> *i = root, *newElement, *j = nullptr;

    while(i){
        if(key == i->first)
            return i->second;
        else if(key < i->first) {
            j = i;
            i = i->left;
        }
        else{
            j = i;
            i = i->right;
        }
    }

    newElement = new Element<Key,Value>{};
    newElement->first = key;

    if(!root)
        root = newElement;
    else{
        newElement->parent = j;

        if(j->first > key)
            j->left = newElement;
        else
            j->right = newElement;

    }

    insertColorFixup(newElement);
    return newElement->second;
}

template<class Key, class Value>
typename Map<Key,Value>::Iterator Map<Key, Value>::begin() {
    Element<Key,Value> *n = root;

    while(n->left)
        n = n->left;

    return Iterator{n};
}

template<class Key, class Value>
typename Map<Key,Value>::Iterator Map<Key, Value>::end() {
    return Iterator{nullptr};
}

template<typename Key, typename Value>
void Map<Key, Value>::leftRotate(Element<Key, Value> *element) {
    Element<Key,Value> *rightChild = element->right;
    element->right = rightChild->left;

    if(rightChild->left != nullptr)
        rightChild->left->parent = element;

    rightChild->parent = element->parent;

    if(element->parent == nullptr)
        root=rightChild;
    else if(element->parent->left == element)
        element->parent->left = rightChild;
    else element->parent->right = rightChild;

    element->parent = rightChild;
    rightChild->left = element;
}

template<typename Key, typename Value>
void Map<Key, Value>::rightRotate(Element<Key, Value> *element) {
    Element<Key,Value> *leftChild = element->left;
    element->left = leftChild->right;

    if(leftChild->right != nullptr)
        leftChild->right->parent = element;

    leftChild->parent = element->parent;

    if(element->parent == nullptr)
        root = leftChild;
    else if(element->parent->right == element)
        element->parent->right = leftChild;
    else
        element->parent->left = leftChild;

    element->parent = leftChild;
    leftChild->right = element;
}

template<typename Key, typename Value>
void Map<Key, Value>::insertColorFixup(Element<Key, Value> *element) {
    Element<Key,Value> *y = nullptr;
    while(element->parent != nullptr and element->parent->color == Color::red){

        if(element->parent->parent != nullptr and element->parent == element->parent->parent->left){
            y = element->parent->right;

            if(y != nullptr and y->color == Color::red){
                element->parent->color = Color::black;
                y->color = Color::black;
                element->parent->parent->color = Color::red;
                element = element->parent->parent;
            }else{

                if(element == element->parent->right) {
                    element = element->parent;
                    leftRotate(element);
                }

                element->parent->color = Color::black;
                element->parent->parent->color = Color::red;
                rightRotate(element->parent->parent);
            }
        }else if(element->parent->parent != nullptr and element->parent == element->parent->parent->left){
            y = element->parent->left;

            if(y != nullptr and y->color == Color::red){
                element->parent->color = Color::black;
                y->color = Color::black;
                element->parent->parent->color = Color::red;
                element = element->parent->parent;
            }else{

                if(element == element->parent->right){
                    element = element->parent;
                    rightRotate(element);
                }
                element->parent->color = Color::black;
                element->parent->parent->color = Color::red;
                leftRotate(element->parent->parent);
            }
        }else
            break;
    }
    root->color = Color::black;
}


template<class Key, class Value>
typename Map<Key,Value>::Iterator &Map<Key,Value>::Iterator::operator++() {
    if(currentElement->right){
        currentElement = currentElement->right;

        while(currentElement->left)
            currentElement = currentElement->left;

        return *this;
    }

    Element<Key,Value> *n = currentElement->parent;
    while( n != nullptr and currentElement == n->right){
        currentElement = n;
        n = n->parent;
    }

    currentElement = n;
    return *this;
}

template<class Key, class Value>
bool Map<Key,Value>::Iterator::operator!=(const Iterator &o1)const {
    return currentElement != o1.currentElement;
}

template<class Key, class Value>
Map<Key,Value>::Iterator::Iterator() {
    currentElement = nullptr;
}

template<class Key, class Value>
Element<Key,Value> *Map<Key,Value>::Iterator::operator->() {
    return currentElement;
}

template<class Key, class Value>
Map<Key,Value>::Iterator::Iterator(Element<Key, Value> *n) {
        currentElement = n;
}

template<class Key, class Value>
typename Map<Key,Value>::Iterator Map<Key, Value>::Iterator::operator++(int) {
    if(currentElement->right){
        currentElement = currentElement->right;

        while(currentElement->left)
            currentElement = currentElement->left;

        return *this;
    }

    Element<Key,Value> *n = currentElement->parent;
    while( n and currentElement == n->right){
        currentElement = n;
        n = n->parent;
    }

    currentElement = n;
    return *this;
}

template<class Key, class Value>
bool Map<Key, Value>::Iterator::operator==(const Iterator &o1) const {
    return currentElement == o1.currentElement;
}

