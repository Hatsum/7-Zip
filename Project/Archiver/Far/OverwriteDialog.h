// OverwriteDialog.h

#pragma once

#ifndef OVERWRITEDIALOG_H
#define OVERWRITEDIALOG_H

#include "Common/String.h"

namespace NOverwriteDialog {

struct CFileInfo
{
  bool SizeIsDefined;
  UINT64 Size;
  FILETIME Time;
  CSysString Name;
};
namespace NResult
{
  enum EEnum
  {
    kYes,
    kYesToAll,
    kNo,
    kNoToAll,
    kAutoRename,
    kCancel,
  };
}
NResult::EEnum Execute(const CFileInfo &anOldFileInfo, const CFileInfo &aNewFileInfo);

}

#endif
