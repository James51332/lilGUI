#include "lilGUI.h"

/*
--------------------------------------------------
----- IMPLEMENTATION (LilDrawList) ---------------
--------------------------------------------------
*/

void LilDrawList::Clear()
{
  VtxArray.Clear();
  IdxArray.Clear();
  DrawCmds.Clear();
  VtxOffset = 0;
}

void LilDrawList::Render()
{
  // This system should grow more complicated when we support textures and clipping rects
  DrawCmds.EmplaceBack(static_cast<LilU32>(IdxArray.GetSize()), 0, 0);
}

void LilDrawList::PushRect(const LilVec2& min, const LilVec2& max, LilU32 color)
{
  VtxArray.EmplaceBack(LilVec3(min.x, min.y, 0.0f), LilVec2(0.0f, 0.0f), color);
  VtxArray.EmplaceBack(LilVec3(max.x, min.y, 0.0f), LilVec2(1.0f, 0.0f), color);
  VtxArray.EmplaceBack(LilVec3(max.x, max.y, 0.0f), LilVec2(1.0f, 1.0f), color);
  VtxArray.EmplaceBack(LilVec3(min.x, max.y, 0.0f), LilVec2(0.0f, 1.0f), color);
  
  IdxArray.PushBack(VtxOffset);
  IdxArray.PushBack(VtxOffset + 1);
  IdxArray.PushBack(VtxOffset + 2);
  IdxArray.PushBack(VtxOffset);
  IdxArray.PushBack(VtxOffset + 2);
  IdxArray.PushBack(VtxOffset + 3);
  
  VtxOffset += 4;
}

/*
--------------------------------------------------
----- IMPLEMENTATION (LilContext) ----------------
--------------------------------------------------
*/

namespace Lil
{

static LilContext s_Context;

void CreateContext()
{
  s_Context.DrawLists.EmplaceBack(); // Create a DrawList
}

LilContext& GetContext()
{
  return s_Context;
}

void DestroyContext()
{
  // Global initialization will go here when needed
}

LilArray<LilDrawList>& GetDrawLists()
{
  return s_Context.DrawLists;
}

void BeginFrame()
{
  for (auto& drawList : GetDrawLists())
    drawList.Clear();
}

void RenderFrame()
{
  for (auto& drawList : GetDrawLists())
    drawList.Render();
}

} // namespace Lil
