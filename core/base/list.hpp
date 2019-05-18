
#pragma once
#ifndef SMART_GARDEN_LIST_H
#define SMART_GARDEN_LIST_H

#include <string.h>

// Kích thước mặc định của List
#define LIST_SIZE 20

template<class T>
class List {
  private:
  int _begin;
  int _end;

  public:
  int length;
  T arr[LIST_SIZE];

  List() {
    this->_begin = 0;
    this->_end = 0;
    this->length = 0;
  }

  bool push(T &newElement) {
    if (this->length == LIST_SIZE) return false;
    ++this->length;
    this->_end = (this->_end + 1) % LIST_SIZE;
    this->arr[this->_end] = newElement;
    return true;
  }

  T* shift() {
    if (this->length > 0) --this->length;
    else return NULL;
    this->_begin = (this->_begin + 1) % LIST_SIZE;
    return &this->arr[this->_begin];
  }
  

  T* operator [](int index) {
    return &arr[index];
  }
};



#endif