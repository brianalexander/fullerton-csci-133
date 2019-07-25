#pragma once
#include <iostream>

/*
  dlist.h
  Doubly-linked lists of ints
 */
class dlist {
  public:
    dlist() { }

    struct node {
        int value;
        node* next;
        node* prev;
    };

    node* head() const { return _head; }
    node* tail() const { return _tail; }

    // **** Implement ALL the following methods ****

    // Returns the node at a particular index (0 is the head). If n >= size()
    // return nullptr; if n < 0, return the head of the list.
    // Must run in O(n) time.
    node* at(int n) const {
      if (n >= sz) {
        return nullptr;
      }
      if (n < 0) {
        return _head;
      }
      
      node* temp = _head;
      for (int i = 0; i <= n; i++) {
        temp = temp->next;
      }

      return temp;
    };

    // Insert a new value, after an existing one. If previous == nullptr, then
    // the list is assumed to be empty.
    // Must run in O(1) time.
    void insert(node *previous, int value) {
      if (empty()) {
        _head = _tail = new node{value, nullptr, nullptr};
      } else if (previous == nullptr) {
        node* n = new node{value, _head, nullptr};
        _head->prev = n;
        _head = n;
      } else if (previous == _tail) {
        node* n = new node{value, nullptr, previous};
        previous->next = n;
        _tail = n;
      } else {
        previous = new node{value, previous->next, previous};
        previous->prev->next = previous;
        previous->next->prev = previous;
      }

      sz++;
    };

    // Delete the given node. Should do nothing if which == nullptr.
    // Must run in O(1) time.
    void remove(node* which) {
      if (empty()) {
        return;
      }
      if (sz == 1) {
        delete _head;
      } else if (which == _head) {
        node* n = _head;
        _head = _head->next;
        _head->prev = nullptr;
        delete n;
      } else if (which == _tail) {
        node* n = _tail;
        _tail = _tail->prev;
        _tail->next = nullptr;
        delete n;
      } else {
        which->next->prev = which->prev;
        which->prev->next = which->next;
        delete which; 
      }

      sz--;
    };

    // Add a new element to the *end* of the list
    // Must run in O(1) time.
    void push_back(int value) {
      insert(_tail, value);
    };

    // Add a new element to the *beginning* of the list
    // Must run in O(1) time.
    void push_front(int value) {
      insert(nullptr, value);
    };

    // Remove the first element. 
    // If the list is empty, do nothing.
    // Must run in O(1) time
    void pop_front() {
      remove(_head);
    };

    // Remove the last element. 
    // If the list is empty, do nothing.
    // Must run in O(1) time
    void pop_back() {
      remove(_tail);
    };

    // Get the size of the list
    // Should run in O(n) time at the worst
    int size() const {
      return sz;
    };

    // Returns true if the list is empty
    // Must run in O(1) time
    bool empty() const {
      if (sz == 0) {
        return true;
      } else {
        return false;
      }
    }; 

  private:
    node* _head = nullptr;
    node* _tail = nullptr;

    // Add any other private members you need
    int sz = 0;
};

// **** Implement ALL the following functions ****

/* a == b
   Compares two lists for equality, returning true if they have the same
   elements in the same positions. (Hint: it is *not* enough to just compare
   pointers! You have to compare the values stored in the nodes.)

   Must run in O(m) time, where m is the length of the shorter of the two lists.
*/ 
bool operator== (const dlist& a, const dlist& b) {
  if(a.size() != b.size()) {
    return false;
  }  
  dlist::node* m = a.head();
  dlist::node* n = b.head(); 
  while(m!=nullptr) {
    if(m->value != n->value) {
      return false;
    }
    m = m->next;
    n = n->next;
  }

  return true;
};

/* a + b
   Returns a new list consisting of all the elements of a, followed by all the
   elements of b (i.e., the list concatenation).

   Must run in O(n) time in the length of the result (i.e., the length of a
   plus the length of b).
*/
dlist operator+ (const dlist& a, const dlist& b) {
  dlist* temp = new dlist;
  dlist::node* currentNode = a.head();
  while(currentNode != nullptr) {
    temp->push_back(currentNode->value);
    currentNode = currentNode->next;
  }
  currentNode = b.head();
  while(currentNode != nullptr) {
    temp->push_back(currentNode->value);
    currentNode = currentNode->next;
  }
  
  return *(temp);
};

/* reverse(l)
   Returns a new list that is the *reversal* of l; that is, a new list 
   containing the same elements as l but in the reverse order.

   Must run in O(n) time. 
*/
dlist reverse(const dlist& l) {
  dlist* temp = new dlist;
  dlist::node* currentNode = l.head();
  while(currentNode != nullptr) {
    temp->push_front(currentNode->value);
    currentNode = currentNode->next;
  }
  return *(temp);
};
