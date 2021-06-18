/*
 *  ali_addressof.h
 *  ali Library
 *
 *  Copyright 2012 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once


namespace ali {

template <class T>
T* addressof(T& v)
{
  return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(v)));
}

}
