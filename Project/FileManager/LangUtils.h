// LangUtils.h

#pragma once

#ifndef __LANGUTILS_H
#define __LANGUTILS_H

#include "Common/Lang.h"

// extern CLang g_Lang;
extern CSysString g_LangPath;

struct CIDLangPair
{
  int ControlID;
  UINT32 LangID;
};

void ReloadLang();

void LangSetDlgItemsText(HWND aDialogWindow, CIDLangPair *anIDLangPairs, int aNumItems);
void LangSetWindowText(HWND aWindow, UINT32 aLangID);

CSysString LangLoadString(UINT32 aLangID);
CSysString LangLoadString(UINT aResourceID, UINT32 aLangID);
UString LangLoadStringW(UINT aResourceID, UINT32 aLangID);


#endif


