#pragma once

#include "lilArray.h"

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
  
  void PushRect(const LilVec2& min, const LilVec2& max, LilU32 color);
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
  LilArray<LilDrawList> DrawLists; // I'm currently thinking each window will have a drawList (the first draw list is reserved for user shapes atm)
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

/*
--------------------------------------------------
----- SECTION (LilDraw) --------------------------
--------------------------------------------------
 
Prototypes for public API for interfacing with the GUI.

-- TODO --
1) N/A
*/

namespace Lil
{

void Rect(float x, float y, float w, float h, LilU32 color = 0xffffffff);

} // namespace Lil

