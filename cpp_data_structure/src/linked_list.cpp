/******************************************************************************
*
* Copyright (c) 2016-2017 Shuyang Sun
*
* License: MIT
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
******************************************************************************/

#include "../include/linked_list.hpp"

template<typename T>
cppds::LinkedListNode<T>::LinkedListNode() : value_{}, next_{ nullptr }
{
}

template<typename T>
cppds::LinkedListNode<T>::LinkedListNode(T const value) : value_{ value }, next_{ nullptr }
{
}

template<typename T>
cppds::LinkedListNode<T>::LinkedListNode(T & const value) : value_{ value }, next_{ nullptr }
{
}

template<typename T>
cppds::LinkedListNode<T>::LinkedListNode(T && value) : value_{ value }, next_{ nullptr }
{
}

template<typename T>
cppds::LinkedListNode<T>::~LinkedListNode()
{
  next_ = nullptr;
}

template<typename T>
T cppds::LinkedListNode<T>::Value() const
{
  return value_;
}

template<typename T>
void cppds::LinkedListNode<T>::SetValue(T const value)
{
  value_ = std::copy(value);
}

template<typename T>
void cppds::LinkedListNode<T>::SetValue(T & const value)
{
  value_ = std::copy(value);
}

template<typename T>
void cppds::LinkedListNode<T>::SetValue(T && value)
{
  value_ = value;
}

template<typename T>
cppds::LinkedListNode<T> * cppds::LinkedListNode<T>::Next() const
{
  return next_;
}

template<typename T>
void cppds::LinkedListNode<T>::SetNext(LinkedListNode const * const nextNode) const
{
  next_ = nextNode;
}

template<typename T>
inline cppds::LinkedListNode<T>::operator std::string() const
{
  return "[" + value_ + "]";
}

