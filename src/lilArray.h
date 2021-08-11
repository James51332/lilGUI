#pragma once

#include <utility>
#include <new>

/*
--------------------------------------------------
----- SECTION (LilArray) -------------------------
--------------------------------------------------
 
LilArray<T> is a fast, dynamic array intended for internal
use as a replacement for std::vector<T>. It's not strictly
necessary but some implementations of std::vector<T> are
very slow so it may benefit performance in some instances.
 
-- TODO --
1) Consider adding macro to use std::vector<T> at compile time. (would require function naming changes)
*/

template <typename T>
class LilArray {
  
  std::size_t Size;
  std::size_t MaxSize;
  T* Array;
  
public:
  
  using ValueType = T;
  using Iterator = T*;
  using ConstIterator = const T*;

public:
  LilArray() noexcept
    : Size(0), MaxSize(0), Array(nullptr)
  {
  }

  LilArray(std::size_t size)
    : Size(0), MaxSize(0), Array(nullptr)
  {
    Array = (T*) operator new(size * sizeof(T));
    MaxSize = size;
  }

  ~LilArray() noexcept
  {
    Clear();
  }

  T& operator[](std::size_t index) noexcept { return Array[index]; }
  const T& operator[](std::size_t index) const noexcept { return Array[index]; }

  bool Empty() const noexcept { return Size == 0; }
  std::size_t GetSize() const noexcept { return Size; }
  std::size_t GetMaxSize() const noexcept { return MaxSize; }
  
  void Clear() noexcept
  {
    for (int i = 0; i < Size; ++i)
      Array[i].~T();
    operator delete(Array, MaxSize * sizeof(T));
   
    Array = nullptr;
    Size = 0;
    MaxSize = 0;
  }
  
  void Reserve(std::size_t size)
  {
    if (size <= MaxSize)
      return;
      
    T* tmp = (T*) operator new(size * sizeof(T));

    for (int i = 0; i < Size; ++i)
      new (&tmp[i]) T(std::move(Array[i]));

    for (int i = 0; i < Size; ++i)
      Array[i].~T();

    operator delete(Array, MaxSize * sizeof(T));
    Array = tmp;

    MaxSize = size;
  }
  
  // Helper function that won't cause a reallocation of memory
  void Shrink(std::size_t size) noexcept
  {
    if (size >= Size)
      return;
    
    for (std::size_t i = Size; i < size; ++i)
      Array[i].~T();
    
    Size = size;
  }
  
  void PushBack(const T& val)
  {
    if (Size == MaxSize)
      Reserve(NextSize());
    
    new(&Array[Size]) T(val);
    Size++;
  }

  void PushBack(T&& val)
  {
    if (Size == MaxSize)
      Reserve(NextSize());

    new(&Array[Size]) T(std::move(val));
    Size++;
  }
  
  template<typename... Args>
  void EmplaceBack(Args&&... args)
  {
    if (Size == MaxSize)
      Reserve(NextSize());
    
    new(&Array[Size]) T(args...);
    Size++;
  }
  
  void PopBack()
  {
    Size--;
    Array[Size].~T();
  }
  
  Iterator begin() { return Array; }
  Iterator end() { return Array + Size; }
  Iterator rbegin() { return Array + Size; }
  Iterator rend() { return Array; }
  ConstIterator cbegin() const { return Array; }
  ConstIterator cend() const { return Array + Size; }
  ConstIterator crbegin() const { return Array + Size; }
  ConstIterator crend() const { return Array; }
  
private:
  std::size_t NextSize() noexcept
  {
    return MaxSize ? static_cast<std::size_t>(MaxSize + MaxSize / 2) : 8; // Use geometric growth of 1.5 (close to golden ratio; couldn't be a bad number)
  }
};
