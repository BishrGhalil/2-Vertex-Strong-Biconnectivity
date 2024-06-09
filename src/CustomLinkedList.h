/*
File: CustomLinkedList.h
*/

#pragma once
#include <iostream>
#include <cassert>

using ll = long long int;

// Represents a node in a linked list
// Warning: always use 'nullptr' as the null value for ListNode pointers
template<typename T>
class Node { 
public:
    T value; // Node's value
    Node<T>* next; // Pointer to next node in linked list

    Node(const T& v = 0, Node<T>* n = nullptr) : value(v), next(n) {}
};

// Provides a method of iterating over a linked list
template <typename T>
class Iterator {
private:
    Node<T>* current;
public:
    Iterator(Node<T>* head = nullptr) : current(head) {}

    T getValue() { return current->value; }

    Node<T>* getReference() { return current; }

    bool hasNext() { return (current && current->next); }

    void advance() { current = current->next; }
};

template<typename T>
class LinkedList {
private:
    Node<T>* head;
    ll length;
public:
    LinkedList(Node<T>* h = nullptr) : head(h), length(0) {
        if (head != nullptr) length++;
    }

    ll getLength() { return length; }

    Node<T>* getHead() { return head; }

    void display() {
        Node<T>* temp = head;
        while (temp != nullptr) {
            std::cout << temp->value << " ";
            temp = temp->next;
        }
        std::cout << '\n';
    }

    void addFront(const T& v) {
        Node<T>* newNode = new Node<T>(v, head);
        head = newNode;
        length++;
    }

    void addBack(const T& v) {
        if (head == nullptr) {
            addFront(v);
            return;
        }
        Node<T>* temp = head;
        while (temp->next != nullptr) temp = temp->next;
        temp->next = new Node<T>(v);
        length++;
    }

    // Drops and returns head of linked list 'involves c-assertion'
    T removeFront() {
        assert(length > 0);
        T removedValue = head->value;
        head = head->next;
        length--;
        return removedValue;
    }

    T removeBack() {
        assert(length > 0);
        if (length == 1) return removeFront();
        Node<T>* temp = head;
        while (temp->next->next != nullptr) temp = temp->next;
        T removedValue = temp->next->value;
        delete temp->next;
        temp->next = nullptr;
        length--;
        return removedValue;
    }

    T getElement(const ll& index) {
        assert(index >= 0 && index < length);
        Node<T>* temp = head;
        for (ll i = 0; i < index; i++) temp = temp->next;
        return temp->value;
    }

    ll find(const T& v) {
        Node<T>* temp = head;
        for (ll i = 0; i < length; i++) {
            if (temp->value == v) return i;
            temp = temp->next;
        }
        return -1;
    }

    std::pair<Iterator<T>, bool> locate(const T& v) {
        Iterator<T> itr(head);
        Iterator<T> prev;
        while (itr.hasNext()) {
            if (itr.getValue() == v) return { prev, true };
            prev = itr;
            itr.advance();
        }
        return { nullptr, false };
    }

    void insertAfter(const T& v, Iterator<T>& itr) {
        if (!itr.getReference()) {
            addFront(v);
            return;
        }
        Node<T>* newNode = new Node<T>(v, itr.getReference()->next);
        itr.getReference()->next = newNode;
        length++;
    }

    T removeAfter(Iterator<T>& itr) {
        assert(length > 0);
        if (!itr.getReference()) return removeFront();
        assert(itr.hasNext());
        itr.advance();
        T removedValue = itr.getValue();
        itr.getReference()->next = itr.getReference()->next->next;
        length--;
        return removedValue;
    }

    void removeAll(const T& v) {
        if (length == 0) return;
        LinkedList<T> tempList;
        Iterator<T> itr(head);
        while (itr.hasNext()) {
            if (itr.getValue() != v) tempList.addFront(itr.getValue());
            itr.advance();
        }
        if (itr.getValue() != v) tempList.addFront(itr.getValue());
        head = nullptr;
        length = 0;
        while (tempList.getLength()) {
            addFront(tempList.removeFront());
        }
    }
};
