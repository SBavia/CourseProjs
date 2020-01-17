#ifndef LIST_H
#define LIST_H

#include <iostream>
using namespace std;

template <class T>
struct Cell
{
    Cell *previous;
    Cell *next;
    T data;
};



// собсвенный контейнер
// двунаправленный список
template <class T>
class List
{
    Cell<T> *first; // указатель на начало
    Cell<T> *last; // конец
    unsigned int size; // размер
public:
    class Iterator;
    List():first(nullptr), last(nullptr), size(0) {}
    List(const List &object):first(nullptr),last(nullptr),size(0)
    {
        List<T>::Iterator current = object.begin(), ite = object.end();
        while(current != ite)
            this->push_back(*current++);
        if (!object.empty()) this->push_back(*current++);
    }
    List operator=(const List &);
    void push_back(T);
    void push_front(T);
    T pop_back();
    T pop_front();
    T pop_midle(Iterator &);
    void insert(const Iterator &, const T &);
    void sort();
    Iterator search(const T &);
    void clear();
    bool empty() const;
    void show() const;
    Iterator begin() const;
    Iterator end() const;
    ~List()
    {
        this->clear();
    }
};


// class Iterator
template <class T>
class List<T>::Iterator
{
    Cell<T> *current;
public:
    Iterator(): current(nullptr) {}
    Iterator(const Iterator &object)
    {
        this->current = object.current;
    }
    Iterator(Cell<T> *temp)
    {
        this->current = temp;
    }
    Iterator operator=(const Iterator &object);
    Iterator operator++(int);
    Iterator operator++();
    Iterator operator--(int);
    Iterator operator--();
    Cell<T> *operator&();
    T &operator*();
    T *operator->();
    bool operator==(const Iterator &);
    bool operator!=(const Iterator &);

};



#endif // LIST_H
