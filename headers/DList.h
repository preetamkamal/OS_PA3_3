#ifndef LAB2_DLIST_H
#define LAB2_DLIST_H

#include "PCB.h"
#include <clocale>
#include <iostream>

template <typename T>
class DList
{
    struct node
    {
        T data;
        node *next, *prev;
        explicit node(T data_) : data(data_)
        {
            next = prev = NULL;
        }
    };
    node *head, *tail;
    int _size;

public:
    DList()
    {
        head = tail = NULL;
        _size = 0;
    }
    ~DList() { clear(); }
    void add_start(T p);
    void add_end(T p);
    T *gethead();
    T *gettail();
    T *getindex(int index);
    T remove_head();
    T remove_tail();
    T removeindex(int index);
    int size();
    void print();
    void clear();
};

template <typename T>
void DList<T>::add_start(T p)
{
    auto temp = new DList::node(p);
    if (_size > 0)
        head->prev = temp;
    else
        tail = temp;
    temp->next = head;
    temp->prev = NULL;
    head = temp;
    _size++;
}

template <typename T>
void DList<T>::add_end(T data)
{
    auto temp = new node(data);
    if (_size > 0)
        tail->next = temp;
    else
        head = temp;
    temp->next = NULL;
    temp->prev = tail;
    tail = temp;
    _size++;
}

template <typename T>
T *DList<T>::gethead()
{
    if (_size)
        return &(head->data);
}

template <typename T>
T *DList<T>::gettail()
{
    if (_size)
        return &(tail->data);
}

template <typename T>
T *DList<T>::getindex(int index)
{
    if (index < 0 || index >= _size)
        return NULL;
    node *curr = head;
    int curr_index = 0;
    while (curr != NULL && curr_index < index)
    {
        curr = curr->next;
        curr_index++;
    }
    if (curr != NULL)
        return &(curr->data);
    return NULL; // Return NULL if index is out of bounds
}

template <typename T>
T DList<T>::remove_head()
{
    if (_size == 0)
    {
        return T(); // Return default value
    }
    else if (_size == 1)
    {
        T temp = head->data;
        delete head;
        head = tail = NULL;
        _size = 0;
        return temp;
    }
    else
    {
        T temp = head->data;
        node *old_head = head;
        head = head->next;
        head->prev = NULL;
        delete old_head;
        _size--;
        return temp;
    }
}

template <typename T>
T DList<T>::remove_tail()
{
    if (_size == 0)
    {
        return T(); // Return default value
    }
    else if (_size == 1)
    {
        T temp = tail->data;
        delete tail;
        head = tail = NULL;
        _size = 0;
        return temp;
    }
    else
    {
        T temp = tail->data;
        node *old_tail = tail;
        tail = tail->prev;
        tail->next = NULL;
        delete old_tail;
        _size--;
        return temp;
    }
}

template <typename T>
T DList<T>::removeindex(int index)
{
    if (index == 0)
        return remove_head();
    else if (index == _size - 1)
        return remove_tail();
    else if (index > 0 && index < _size - 1)
    {
        node *curr = head;
        int curr_index = 0;
        while (curr != NULL && curr_index < index)
        {
            curr = curr->next;
            curr_index++;
        }
        if (curr != NULL)
        {
            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;

            T temp = curr->data;
            delete curr;
            _size--;
            return temp;
        }
    }
    // Return default T if index is invalid
    return T();
}

template <typename T>
int DList<T>::size()
{
    return _size;
}

template <typename T>
void DList<T>::clear()
{
    node *next = head;
    while (head != NULL)
    {
        next = head->next;
        delete head;
        head = next;
    }
    tail = NULL;
    _size = 0;
}

template <typename T>
void DList<T>::print()
{
    std::cout << "----Back----" << std::endl;
    node *curr = head;
    while (curr != NULL)
    {
        curr->data.print();
        curr = curr->next;
    }
    std::cout << "----Front----" << std::endl;
}
#endif // LAB2_DLIST_H
