#include "list.h"

template <class T>
bool List<T>::empty() const
{
    return !static_cast<bool>(this->size);
}

template <class T>
void List<T>::sort()
{
    Cell<T> *current, *preend = this->last;
    while(preend != this->first)
    {
        current = this->first;
        while(current != preend)
        {
            if(current->data > current->next->data)
            {
                T temp = current->data;
                current->data = current->next->data;
                current->next->data = temp;
            }
            current = current->next;
        }
        preend= preend->previous;
    }
}

template <class T>
void List<T>::insert(const Iterator &past, const T &past_data)
{
    bool flag = 1;
    List<T>::Iterator temp = this->begin();
    while(temp != this->end())
        {
            if(temp == past)
            {
                flag = 0;
                break;
            }
            temp++;
         }
         if(temp == past) flag = 0;
    else
    {
    Cell<T> *temp = new Cell<T>;
    List<T>::Iterator it = past;
    temp->data = past_data;
    temp->next = &it;
    temp->previous = (&it)->previous;
    (&it)->previous = temp;
    if(temp->previous != nullptr) temp->previous->next = temp;
    if(&it == this->last) this->last = temp->next;
    if(&it == this->first) this->first = (&it)->previous;
    ++this->size;
    }
}

template<class T>
void List<T>::clear()
{
    Cell<T> *current = this->first;
    while(this->first)
    {
        current = this->first;
        this->first = this->first->next;
        delete current;
    }
    this->last = this->first;
    this->size = 0;
}

template <class T>
List<T> List<T>::operator=(const List<T> &object)
{
    if(this != &object)
    {
        this->clear();
        List<T>::Iterator itb = object.begin(), ite = object.end();
        while(itb != ite)
            this->push_back(*itb++);
        if( !object.empty()) this->push_back(*itb);
    }
    return *this;
}

template <class T>
typename List<T>::Iterator List<T>::begin() const
{
    return Iterator(first);
}

template <class T>
typename List<T>::Iterator List<T>::end() const
{
    return Iterator(last);
}

template <class T>
typename List<T>::Iterator List<T>::search(const T &data)
{
    Iterator i = this->begin();
    bool flag = false;
    while(i != this->end())
        if(data == *i++)
        {
            flag = true;
            break;
        }
    if(!flag)
        if(*i == data) flag = true;

    return flag?i:Iterator();
}

template <class T>
void List<T>::push_back(T to_add)
{
    Cell<T> *temp = new Cell<T>;
    temp->data = to_add;
    temp->next = nullptr;
    temp->previous = this->last;
    if(!size)
        this->first = this->last = temp;
    else
    {
        this->last->next = temp;
        this->last = temp;
    }
    ++size;
}

template <class T>
void List<T>::show() const
{
    Cell<T> *temp = this->first;
    while(temp)
    {
        cout<< temp->data << endl;
        temp = temp->next;
    }
    cout << endl;
}

template <class T>
void List<T>::push_front(T to_add)
{
    Cell<T> *temp = new Cell<T>;
    temp->data = to_add;
    temp->previous = nullptr;
    temp->next = this->first;
    if(!size)
        this->first = this->last = temp;
    else
    {
        this->first->previous = temp;
        this->first = temp;

    }
    ++size;
}

template <class T>
T List<T>::pop_midle(Iterator &current)
{
    T data;
    if(current == this->begin()) data = this->pop_front();
    else if(current == this->end()) data = this->pop_back();
    else
    {
        (&current)->previous->next = (&current)->next;
        (&current)->next->previous = (&current)->previous;
        data = *current;
        delete &current;
    }
    return data;
}

template <class T>
T List<T>::pop_back()
{
    T data;
    if(size)
    {
    Cell<T> *temp = this->last;
    data = temp->data;
    this->last = this->last->previous;
    if(size != 1) this->last->next = nullptr;
    delete temp;
    --size;
    if(!size) this->first= this->last;
    }
    return data;
}

template <class T>
T List<T>::pop_front()
{
    T data;
    if(size)
    {
    Cell<T> *temp = this->first;
    data = temp->data;
    this->first = this->first->next;
    if (size != 1) this->first->previous = nullptr;
    delete temp;
    --size;
    if(!size) this->last = this->first;
    }
    return data;
}

template <class T>
Cell<T> *List<T>::Iterator::operator&()
{
    return this->current;
}

template <class T>
bool List<T>::Iterator::operator==(const List<T>::Iterator &object)
{
    return this->current == object.current;
}

template <class T>
bool List<T>::Iterator::operator!=(const List<T>::Iterator &object)
{
    return this->current != object.current;
}

template <class T>
typename List<T>::Iterator List<T>::Iterator::operator=(const Iterator &object)
{
    if(this != &object)
        this->current = object.current;
    return *this;
}

template <class T>
typename List<T>::Iterator List<T>::Iterator::operator++(int)
{
    List<T>::Iterator temp = *this;
    this->current = current->next;
    return temp;
}

template <class T>
typename List<T>::Iterator List<T>::Iterator::operator++()
{
    current = current->next;
    return *this;
}

template <class T>
typename List<T>::Iterator List<T>::Iterator::operator--(int)
{
    List<T>::Iterator temp = *this;
    this->current = this->current->previous;
    return temp;
}

template <class T>
typename List<T>::Iterator List<T>::Iterator::operator--()
{
   current = current->previous;
   return *this;
}

template <class T>
T &List<T>::Iterator::operator*()
{
    return current->data;
}

template <class T>
T *List<T>::Iterator::operator->()
{
    return &current->data;
}

