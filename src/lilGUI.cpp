#include "lilGUI.h"


/*
--------------------------------------------------
----- IMPLEMENTATION (LilDrawList) ---------------
--------------------------------------------------
*/

void LilDrawList::PushRect(float x, float y, float w, float h, LilU32 color)
{
  VtxArray.EmplaceBack(LilVec3(x + 0, y + 0, 0.0f), LilVec2(1.0f), color);
  VtxArray.EmplaceBack(LilVec3(x + w, y + 0, 0.0f), LilVec2(1.0f), color);
  VtxArray.EmplaceBack(LilVec3(x + w, y + h, 0.0f), LilVec2(1.0f), color);
  VtxArray.EmplaceBack(LilVec3(x + 0, y + h, 0.0f), LilVec2(1.0f), color);
  
  IdxArray.PushBack(0);
  IdxArray.PushBack(1);
  IdxArray.PushBack(2);
  IdxArray.PushBack(0);
  IdxArray.PushBack(2);
  IdxArray.PushBack(3);
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
  // Global initialization will go here when needed
}

LilContext& GetContext()
{
  return s_Context;
}

} // namespace Lil
