#pragma once
#include <iostream>
#include <cassert>
typedef long long int ll;

template<typename T>
class ListNode {
public:
    T value; // Node's value
    ListNode<T>* next; // Pointer to the next node in the linked list

    ListNode(const T& val = 0, ListNode<T>* nextNode = nullptr) : value(val), next(nextNode) {}
};

template <typename T>
class ListIterator {
private:
    ListNode<T>* currentNode; // Pointer to the node of the current iteration

public:
    ListIterator(ListNode<T>* head = nullptr) : currentNode(head) {}

    T getValue() const { return currentNode->value; }

    ListNode<T>* getReference() const { return currentNode; }

    bool hasNext() const { return (currentNode && currentNode->next); }

    void moveToNext() { currentNode = currentNode->next; }
};

template<typename T>
class LinkedList {
private:
    ListNode<T>* head; // Head of the linked list
    ll length; // Length of the linked list

public:
    LinkedList(ListNode<T>* headNode = nullptr) : head(headNode), length(0) {
        if (head != nullptr) length++;
    }

    ll getLength() const { return length; }

    ListNode<T>* getHead() const { return head; }

    void print() const {
        ListNode<T>* currentNode = head;
        while (currentNode != nullptr) {
            std::cout << currentNode->value << " ";
            currentNode = currentNode->next;
        }
        std::cout << '\n';
    }

    void pushFront(const T& val) {
        ListNode<T>* newNode = new ListNode<T>(val, head);
        head = newNode;
        length++;
    }

    void pushBack(const T& val) {
        if (head == nullptr) {
            pushFront(val);
            return;
        }
        ListNode<T>* currentNode = head;
        while (currentNode->next != nullptr) {
            currentNode = currentNode->next;
        }
        ListNode<T>* newNode = new ListNode<T>(val);
        currentNode->next = newNode;
        length++;
    }

    T popFront() {
        assert(length > 0);
        T removedValue = head->value;
        ListNode<T>* tempNode = head;
        head = head->next;
        delete tempNode;
        length--;
        return removedValue;
    }

    T popBack() {
        assert(length > 0);
        if (length == 1) {
            return popFront();
        }
        ListNode<T>* currentNode = head;
        while (currentNode->next->next != nullptr) {
            currentNode = currentNode->next;
        }
        T removedValue = currentNode->next->value;
        delete currentNode->next;
        currentNode->next = nullptr;
        length--;
        return removedValue;
    }

    T getElementAt(ll index) const {
        assert(index >= 0 && index < length);
        ListNode<T>* currentNode = head;
        for (ll i = 0; i < index; i++) {
            currentNode = currentNode->next;
        }
        return currentNode->value;
    }

    ll find(const T& val) const {
        ListNode<T>* currentNode = head;
        for (ll i = 0; i < length; i++) {
            if (currentNode->value == val) return i;
            currentNode = currentNode->next;
        }
        return -1;
    }

    std::pair<ListIterator<T>, bool> locate(const T& val) {
        ListIterator<T> itr(head);
        ListIterator<T> prev;
        while (itr.getReference() != nullptr) {
            if (itr.getValue() == val) return { prev, true };
            prev = itr;
            itr.moveToNext();
        }
        return { ListIterator<T>(nullptr), false };
    }

    void insertAfter(const T& val, ListIterator<T>& itr) {
        if (!itr.getReference()) {
            pushFront(val);
            return;
        }
        ListNode<T>* newNode = new ListNode<T>(val, itr.getReference()->next);
        itr.getReference()->next = newNode;
        length++;
    }

    T removeAfter(ListIterator<T>& itr) {
        assert(length > 0);
        if (!itr.getReference()) return popFront();
        ListNode<T>* refNode = itr.getReference();
        assert(refNode->next != nullptr);
        ListNode<T>* tempNode = refNode->next;
        T removedValue = tempNode->value;
        refNode->next = tempNode->next;
        delete tempNode;
        length--;
        return removedValue;
    }

    void removeValue(const T& val) {
        while (head != nullptr && head->value == val) {
            popFront();
        }
        if (head == nullptr) return;

        ListNode<T>* currentNode = head;
        while (currentNode->next != nullptr) {
            if (currentNode->next->value == val) {
                ListNode<T>* tempNode = currentNode->next;
                currentNode->next = tempNode->next;
                delete tempNode;
                length--;
            } else {
                currentNode = currentNode->next;
            }
        }
    }
};

#endif // LINKED_LIST_H
