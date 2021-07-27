#pragma once

#include <memory>

#if defined(LITTLE_RELEASE) || defined (LITTLE_DIST)
#define NDEBUG
#endif
#include <cassert>

// Efficient dynamic array for us to use throughout library
template <typename T>
struct LilArray
{
  using Type = T;
  using Iterator = T*;
  using ConstIterator = const Iterator;

  uint32_t Size;
  uint32_t Capacity;
  T* Data;

  inline LilArray() { Size = 0; Capacity = 0; Data = nullptr; }
  inline LilArray(uint32_t size) { Size = 0; Capacity = Size; Data = (T*)malloc(size * (uint32_t)sizeof(T)); }
  inline ~LilArray() { Clear(); Size = 0; Capacity = 0; Data = nullptr; }

  inline T& operator[](int index) { assert(index >= 0 && (uint32_t)index < Size); return Data[index]; }
  inline const T& operator[](int index) const { assert(index >= 0 && index < Size); return Data[index]; }

  inline uint32_t _NextSize() const { return Size ? (uint32_t)((Size * 3) / 2) : 8; }
  inline bool Empty() const { return m_Size == 0; }
  inline void Clear() { if (Capacity == 0) return; free(Data); Size = 0; Capacity = 0; Data = nullptr; }
  inline void Resize(uint32_t size) { if (size <= Capacity) return; T* tmp = Data; Data = (T*)malloc(size * (uint32_t)sizeof(T)); memcpy(Data, tmp, Size * (uint32_t)sizeof(T)); Capacity = size; }
  inline void PushBack(const T& val) { if (Size == Capacity) Resize(_NextSize()); Data[Size] = val; Size++; }
  inline void PopBack() { assert(Size > 0); Size--; }

  inline Iterator begin() { return Data; }
  inline Iterator end() { return Data + Size; }
  inline ConstIterator cbegin() const { return Data; }
  inline ConstIterator cend() const { return Data + Size; }
};