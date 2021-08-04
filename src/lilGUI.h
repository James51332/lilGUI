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

/*
--------------------------------------------------
----- SECTION (LilVectors) -----------------------
--------------------------------------------------

LilVectors are convenience structs for floats.
 
-- TODO --
1) N/A
*/
                     
struct LilVec2
{
  float x, y;
  
  LilVec2(float a, float b)
    : x(a), y(b) {}
  
  LilVec2(float a)
    : x(a), y(a) {}
  
  LilVec2()
    : x(0), y(0) {}
};

struct LilVec3
{
  float x, y, z;
  
  LilVec3(float a, float b, float c)
    : x(a), y(b), z(c) {}
  
  LilVec3(float a)
    : x(a), y(a), z(a) {}
  
  LilVec3()
    : x(0), y(0), z(0) {}
};

struct LilVec4
{
  float x, y, z, w;
  
  LilVec4(float a, float b, float c, float d)
    : x(a), y(b), z(c), w(d) {}
  
  LilVec4(float a)
    : x(a), y(a), z(a), w(a) {}
  
  LilVec4()
    : x(0), y(0), z(0), w(0) {}
};

/*
--------------------------------------------------
----- SECTION (LilColors) ------------------------
--------------------------------------------------
 
Utility functions for handling colors within the library
 
-- TODO --
1) Create system for different types of texture IDs
*/

using LilU32 = uint32_t;

namespace Lil
{

constexpr LilU32 ColorFromRGBA(float r, float g, float b, float a)
{
  return (static_cast<int>(255.0f * a) << 24) +
         (static_cast<int>(255.0f * b) << 16) +
         (static_cast<int>(255.0f * g) << 8) +
         (static_cast<int>(255.0f * r));
}

constexpr LilU32 ColorFromRGB(float r, float g, float b)
{
  return (static_cast<int>(255.0f * b) << 16) +
         (static_cast<int>(255.0f * g) << 8) +
         (static_cast<int>(255.0f * r));
}

} // namespace Lil

/*
--------------------------------------------------
----- SECTION (LilFont) --------------------------
--------------------------------------------------
 
I think we'll somehow use this for fonts somehow...
 
-- TODO --
1) N/A
*/

struct LilFont
{
  
};

/*
--------------------------------------------------
----- SECTION (LilDrawList) ----------------------
--------------------------------------------------
 
LilDrawList is essentially the interface for the client-side
renderer to obtain geometry data. It's
 
-- TODO --
1) Create system for different types of texture IDs
*/

struct LilVtx
{
  LilVec3 Pos;
  LilVec2 UV;
  LilU32 Color;
  
  LilVtx(const LilVec3& pos, const LilVec2& uv, LilU32 color)
    : Pos(pos), UV(uv), Color(color) {}
  
  LilVtx()
    : Pos(), UV(), Color(0xffffffff) {}
};

using LilIdx = unsigned short;

struct LilDrawCmd
{
  LilU32 Size;
  LilU32 IdxOffset;
  LilU32 TextureID;
  
  LilDrawCmd(LilU32 size, LilU32 idxOffset, LilU32 texID)
    : Size(size), IdxOffset(idxOffset), TextureID(texID) {}
  
  LilDrawCmd()
    : Size(0), IdxOffset(0), TextureID(0) {}
};

struct LilDrawList
{
  LilArray<LilVtx> VtxArray;
  LilArray<LilIdx> IdxArray;
  LilArray<LilDrawCmd> DrawCmds; // Draw Commands should usually exist per clipping rect.
  LilU32 VtxOffset = 0;
  
  void Clear();
  void Render();
  
  void PushRect(const LilVec2& min, const LilVec2& max, LilU32 color = 0xffffffff);
};

/*
--------------------------------------------------
----- SECTION (LilContext) -----------------------
--------------------------------------------------
 
LilContext is the state of the library. All data
stored internally should be accessible from within
the context. This section also contains prototypes
for all functions that assist in manipulating the
context.

-- TODO --
1) N/A
*/

class LilContext
{
public:
  LilArray<LilDrawList> DrawLists; // I'm currently thinking each window will have a drawList
  LilFont ActiveFont;
};

namespace Lil
{

void CreateContext();
LilContext& GetContext();
void DestroyContext();

LilArray<LilDrawList>& GetDrawLists();
void BeginFrame();
void RenderFrame();

} // namespace Lil
