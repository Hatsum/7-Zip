// PluginWrite.cpp

#include "StdAfx.h"

#include "Plugin.h"

#include "Messages.h"

#include "Common/StringConvert.h"

#include "Windows/FileDir.h"
#include "Windows/FileName.h"
#include "Windows/FileFind.h"
#include "Windows/Defs.h"
#include "Windows/PropVariant.h"

#include "../Common/ZipRegistry.h"
#include "../Common/UpdatePairBasic.h"
#include "../Common/CompressEngineCommon.h"
#include "../Common/UpdateUtils.h"

#include "../Common/OpenEngine2.h"

#include "Far/ProgressBox.h"

#include "UpdateCallback100.h"

#include "../Agent/Handler.h"

/*
#include "../../Archiver/Common/DefaultName.h"
#include "../../Archiver/Common/OpenEngine2.h"
*/

// #include "CompressEngine.h"

using namespace NWindows;
using namespace NFile;
using namespace NDirectory;
using namespace NFar;

using namespace NUpdateArchive;

static const char *kHelpTopic =  "Update";

static LPCTSTR kTempArcivePrefix = "7zi";

static const char *kArchiveHistoryKeyName = "7-ZipArcName"; 

static HRESULT SetOutProperties(IOutFolderArchive *outArchive, UINT32 method)
{
  CComPtr<ISetProperties> aSetProperties;
  if (outArchive->QueryInterface(&aSetProperties) == S_OK)
  {
    CComBSTR comBSTR;
    switch(method)
    {
      case 0:
        comBSTR = "0";
        break;
      case 1:
        comBSTR = "1";
        break;
      case 2:
        comBSTR = "X";
        break;
      default:
        return E_INVALIDARG;
    }
    CObjectVector<CComBSTR> namesReal;
    std::vector<NCOM::CPropVariant> values;
    namesReal.Add(comBSTR);
    values.push_back(NCOM::CPropVariant());
    std::vector<BSTR> names;
    for(int i = 0; i < namesReal.Size(); i++)
      names.push_back(namesReal[i]);
    RINOK(aSetProperties->SetProperties(&names.front(), 
      &values.front(), names.size()));
  }
  return S_OK;
}

NFileOperationReturnCode::EEnum CPlugin::PutFiles(
  struct PluginPanelItem *panelItems, int numItems, 
  int moveMode, int opMode)
{
  if(moveMode != 0)
  {
    g_StartupInfo.ShowMessage(NMessageID::kMoveIsNotSupported);
    return NFileOperationReturnCode::kError;
  }
  if (numItems == 0)
    return NFileOperationReturnCode::kError;

  if (!m_ArchiverInfo.UpdateEnabled)
  {
    g_StartupInfo.ShowMessage(NMessageID::kUpdateNotSupportedForThisArchive);
    return NFileOperationReturnCode::kError;
  }

  static const kYSize = 12;
  static const kXMid = 38;

  NZipSettings::NCompression::CInfo compressionInfo;

  // CZipRegistryManager aZipRegistryManager;
  NZipRegistryManager::ReadCompressionInfo(compressionInfo);
  
  if (!compressionInfo.MethodDefined)
    compressionInfo.Method = 1;

  const kMethodRadioIndex = 2;
  const kModeRadioIndex = kMethodRadioIndex + 4;

  struct CInitDialogItem initItems[]={
    { DI_DOUBLEBOX, 3, 1, 72, kYSize - 2, false, false, 0, false, NMessageID::kUpdateTitle, NULL, NULL },
    { DI_SINGLEBOX, 4, 2, kXMid - 2, 2 + 4, false, false, 0, false, NMessageID::kUpdateMethod, NULL, NULL },
    { DI_RADIOBUTTON, 6, 3, 0, 0, compressionInfo.Method == 0, 
        compressionInfo.Method == 0, 
        DIF_GROUP, false, NMessageID::kUpdateMethodStore, NULL, NULL },
    { DI_RADIOBUTTON, 6, 4, 0, 0, compressionInfo.Method == 1, 
        compressionInfo.Method == 1, 
        0, false, NMessageID::kUpdateMethodNormal, NULL, NULL },
    { DI_RADIOBUTTON, 6, 5, 0, 0, compressionInfo.Method == 2,
        compressionInfo.Method == 2, 
    false, 0, NMessageID::kUpdateMethodMaximum, NULL, NULL },
    
    { DI_SINGLEBOX, kXMid, 2, 70, 2 + 5, false, false, 0, false, NMessageID::kUpdateMode, NULL, NULL },
    { DI_RADIOBUTTON, kXMid + 2, 3, 0, 0, false, true,
        DIF_GROUP, false, NMessageID::kUpdateModeAdd, NULL, NULL },
    { DI_RADIOBUTTON, kXMid + 2, 4, 0, 0, false, false,
        0, false, NMessageID::kUpdateModeUpdate, NULL, NULL },
    { DI_RADIOBUTTON, kXMid + 2, 5, 0, 0, false, false, 
        0, false, NMessageID::kUpdateModeFreshen, NULL, NULL },
    { DI_RADIOBUTTON, kXMid + 2, 6, 0, 0, false, false,
        0, false, NMessageID::kUpdateModeSynchronize, NULL, NULL },
  
    { DI_TEXT, 3, kYSize - 4, 0, 0, false, false, DIF_BOXCOLOR|DIF_SEPARATOR, false, -1, "", NULL  },  
    
    { DI_BUTTON, 0, kYSize - 3, 0, 0, false, false, DIF_CENTERGROUP, true, NMessageID::kUpdateAdd, NULL, NULL  },
    { DI_BUTTON, 0, kYSize - 3, 0, 0, false, false, DIF_CENTERGROUP, false, NMessageID::kCancel, NULL, NULL  }
  };
  
  const kNumDialogItems = sizeof(initItems) / sizeof(initItems[0]);
  const kOkButtonIndex = kNumDialogItems - 2;
  FarDialogItem dialogItems[kNumDialogItems];
  g_StartupInfo.InitDialogItems(initItems, dialogItems, kNumDialogItems);
  int askCode = g_StartupInfo.ShowDialog(76, kYSize, 
      kHelpTopic, dialogItems, kNumDialogItems);
  if (askCode != kOkButtonIndex)
    return NFileOperationReturnCode::kInterruptedByUser;

  if (dialogItems[kMethodRadioIndex].Selected)
    compressionInfo.SetMethod(0);
  else if (dialogItems[kMethodRadioIndex + 1].Selected)
    compressionInfo.SetMethod(1);
  else if (dialogItems[kMethodRadioIndex + 2].Selected)
    compressionInfo.SetMethod(2);
  else
    throw 51751;

  const CActionSet *actionSet;

  if (dialogItems[kModeRadioIndex].Selected)
    actionSet = &kAddActionSet;
  else if (dialogItems[kModeRadioIndex + 1].Selected)
    actionSet = &kUpdateActionSet;
  else if (dialogItems[kModeRadioIndex + 2].Selected)
      actionSet = &kFreshActionSet;
  else if (dialogItems[kModeRadioIndex + 3].Selected)
      actionSet = &kSynchronizeActionSet;
  else
    throw 51751;

  NZipRegistryManager::SaveCompressionInfo(compressionInfo);

  NZipSettings::NWorkDir::CInfo workDirInfo;
  NZipRegistryManager::ReadWorkDirInfo(workDirInfo);
  CSysString workDir = GetWorkDir(workDirInfo, m_FileName);
  CreateComplexDirectory(workDir);

  CTempFile tempFile;
  CSysString tempFileName;
  if (tempFile.Create(workDir, kTempArcivePrefix, tempFileName) == 0)
    return NFileOperationReturnCode::kError;


  /*
  CSysStringVector fileNames;
  for(int i = 0; i < numItems; i++)
  {
    const PluginPanelItem &panelItem = panelItems[i];
    CSysString fullName;
    if (!MyGetFullPathName(panelItem.FindData.cFileName, fullName))
      return NFileOperationReturnCode::kError;
    fileNames.Add(fullName);
  }
  */

  CScreenRestorer screenRestorer;
  CProgressBox progressBox;
  CProgressBox *progressBoxPointer = NULL;
  if ((opMode & OPM_SILENT) == 0 && (opMode & OPM_FIND ) == 0)
  {
    screenRestorer.Save();

    progressBoxPointer = &progressBox;
    progressBox.Init(g_StartupInfo.GetMsgString(NMessageID::kWaitTitle),
        g_StartupInfo.GetMsgString(NMessageID::kUpdating), 1 << 16);
  }
 
  ////////////////////////////
  // Save FolderItem;
  UStringVector aPathVector;
  GetPathParts(aPathVector);
  
  /*
  UString anArchivePrefix;
  for(i = aPathVector.Size() - 1; i >= 0; i--)
  {
    anArchivePrefix += aPathVector[i];
    anArchivePrefix += wchar_t(NName::kDirDelimiter);
  }
  /////////////////////////////////
  */

  UStringVector fileNames;
  fileNames.Reserve(numItems);
  for(int i = 0; i < numItems; i++)
    fileNames.Add(MultiByteToUnicodeString(panelItems[i].FindData.cFileName, CP_OEMCP));
  CRecordVector<const wchar_t *> fileNamePointers;
  fileNamePointers.Reserve(numItems);
  for(i = 0; i < numItems; i++)
    fileNamePointers.Add(fileNames[i]);

  CComPtr<IOutFolderArchive> outArchive;
  HRESULT result = m_ArchiveHandler.QueryInterface(&outArchive);
  if(result != S_OK)
  {
    g_StartupInfo.ShowMessage(NMessageID::kUpdateNotSupportedForThisArchive);
    return NFileOperationReturnCode::kError;
  }
  outArchive->SetFolder(_folder);

  // CSysString aCurrentFolder;
  // MyGetCurrentDirectory(aCurrentFolder);
  // outArchive->SetFiles(MultiByteToUnicodeString(aCurrentFolder, CP_OEMCP), 
  outArchive->SetFiles(L"", 
      &fileNamePointers.Front(), fileNamePointers.Size());
  BYTE actionSetByte[NUpdateArchive::NPairState::kNumValues];
  for (i = 0; i < NUpdateArchive::NPairState::kNumValues; i++)
    actionSetByte[i] = actionSet->StateActions[i];

  CComObjectNoLock<CUpdateCallBack100Imp> *updateCallbackSpec =
    new CComObjectNoLock<CUpdateCallBack100Imp>;
  CComPtr<IFolderArchiveUpdateCallback> updateCallback(updateCallbackSpec );
  
  updateCallbackSpec->Init(m_ArchiveHandler, &progressBox);

  if (SetOutProperties(outArchive, compressionInfo.Method) != S_OK)
    return NFileOperationReturnCode::kError;

  result = outArchive->DoOperation(NULL,
      MultiByteToUnicodeString(tempFileName, CP_OEMCP), actionSetByte, 
      NULL, updateCallback);
  updateCallback.Release();
  outArchive.Release();

  /*
  HRESULT result = Compress(fileNames, anArchivePrefix, *actionSet, 
      m_ProxyHandler.get(), 
      m_ArchiverInfo.ClassID, compressionInfo.Method == 0,
      compressionInfo.Method == 2, tempFileName, progressBoxPointer);
  */

  if (result != S_OK)
  {
    ShowErrorMessage(result);
    return NFileOperationReturnCode::kError;
  }

  _folder.Release();
  m_ArchiveHandler->Close();
  
  // m_FolderItem = NULL;
  
  if (!DeleteFileAlways(m_FileName))
  {
    ShowLastErrorMessage();
    return NFileOperationReturnCode::kError;
  }

  tempFile.DisableDeleting();
  if (!MoveFile(tempFileName, m_FileName))
  {
    ShowLastErrorMessage();
    return NFileOperationReturnCode::kError;
  }
  
  result = ReOpenArchive(m_ArchiveHandler, m_DefaultName, m_FileName);
  if (result != S_OK)
  {
    ShowErrorMessage(result);
    return NFileOperationReturnCode::kError;
  }

  /*
  if(m_ProxyHandler->ReInit(NULL) != S_OK)
    return NFileOperationReturnCode::kError;
  */
  
  ////////////////////////////
  // Restore FolderItem;

  m_ArchiveHandler->BindToRootFolder(&_folder);
  for (i = 0; i < aPathVector.Size(); i++)
  {
    CComPtr<IFolderFolder> newFolder;
    _folder->BindToFolder(aPathVector[i], &newFolder);
    if(!newFolder  )
      break;
    _folder = newFolder;
  }

  /*
  if(moveMode != 0)
  {
    for(int i = 0; i < numItems; i++)
    {
      const PluginPanelItem &aPluginPanelItem = panelItems[i];
      bool result;
      if(NFile::NFind::NAttributes::IsDirectory(aPluginPanelItem.FindData.dwFileAttributes))
        result = NFile::NDirectory::RemoveDirectoryWithSubItems(
           aPluginPanelItem.FindData.cFileName);
      else
        result = NFile::NDirectory::DeleteFileAlways(
           aPluginPanelItem.FindData.cFileName);
      if(!result)
        return NFileOperationReturnCode::kError;
    }
  }
  */
  return NFileOperationReturnCode::kSuccess;
}



/*
// {23170F69-40C1-278A-1000-000100030000}
DEFINE_GUID(CLSID_CAgentArchiveHandler, 
  0x23170F69, 0x40C1, 0x278A, 0x10, 0x00, 0x00, 0x01, 0x00, 0x03, 0x00, 0x00);
*/

HRESULT CompressFiles(const CObjectVector<PluginPanelItem> &pluginPanelItems)
{
  if (pluginPanelItems.Size() == 0)
    return E_FAIL;

  UStringVector fileNames;
  for(int i = 0; i < pluginPanelItems.Size(); i++)
  {
    const PluginPanelItem &panelItem = pluginPanelItems[i];
    CSysString fullName;
    if (strcmp(panelItem.FindData.cFileName, "..") == 0 && 
        NFind::NAttributes::IsDirectory(panelItem.FindData.dwFileAttributes))
      return E_FAIL;
    if (strcmp(panelItem.FindData.cFileName, ".") == 0 && 
        NFind::NAttributes::IsDirectory(panelItem.FindData.dwFileAttributes))
      return E_FAIL;
    if (!MyGetFullPathName(panelItem.FindData.cFileName, fullName))
      return E_FAIL;
    fileNames.Add(MultiByteToUnicodeString(fullName, CP_OEMCP));
  }

  NZipSettings::NCompression::CInfo compressionInfo;
  // CZipRegistryManager aZipRegistryManager;
  NZipRegistryManager::ReadCompressionInfo(compressionInfo);
  if (!compressionInfo.MethodDefined)
    compressionInfo.Method = 1;
 
  int archiverIndex = 0;

  CObjectVector<NZipRootRegistry::CArchiverInfo> archiverInfoList;
  {
    CObjectVector<NZipRootRegistry::CArchiverInfo> aFullArchiverInfoList;
    NZipRootRegistry::ReadArchiverInfoList(aFullArchiverInfoList);
    for (int i = 0; i < aFullArchiverInfoList.Size(); i++)
    {
      const NZipRootRegistry::CArchiverInfo &archiverInfo = aFullArchiverInfoList[i];
      if (archiverInfo.UpdateEnabled)
      {
        if (archiverInfo.ClassID == compressionInfo.LastClassID && 
            compressionInfo.LastClassIDDefined)
          archiverIndex = archiverInfoList.Size();
        archiverInfoList.Add(archiverInfo);
      }
    }
  }
  if (archiverInfoList.IsEmpty())
    throw "There is no update achivers";


  UString resultPath;
  {
    NName::CParsedPath parsedPath;
    parsedPath.ParsePath(fileNames.Front());
    if(parsedPath.PathParts.Size() == 0)
      return E_FAIL;
    if (fileNames.Size() == 1 || parsedPath.PathParts.Size() == 1)
    {
      // CSysString pureName, dot, extension;
      resultPath = parsedPath.PathParts.Back();
    }
    else
    {
      parsedPath.PathParts.DeleteBack();
      resultPath = parsedPath.PathParts.Back();
    }
  }
  CSysString archiveNameSrc = UnicodeStringToMultiByte(resultPath, CP_OEMCP);
  CSysString archiveName = archiveNameSrc;

  const NZipRootRegistry::CArchiverInfo &archiverInfo = archiverInfoList[archiverIndex];
  int prevFormat = archiverIndex;
 
  if (!archiverInfo.KeepName)
  {
    int dotPos = archiveName.ReverseFind('.');
    int slashPos = MyMax(archiveName.ReverseFind('\\'), archiveName.ReverseFind('/'));
    if (dotPos > slashPos)
      archiveName = archiveName.Left(dotPos);
  }
  archiveName += '.';
  archiveName += archiverInfo.Extension;
  
  const CActionSet *actionSet = &kAddActionSet;

  while(true)
  {
    static const kYSize = 14;
    static const kXMid = 38;
  
    const kArchiveNameIndex = 2;
    const kMethodRadioIndex = kArchiveNameIndex + 2;
    const kModeRadioIndex = kMethodRadioIndex + 4;

    const NZipRootRegistry::CArchiverInfo &archiverInfo = archiverInfoList[archiverIndex];

    char updateAddToArchiveString[512];
    sprintf(updateAddToArchiveString, 
        g_StartupInfo.GetMsgString(NMessageID::kUpdateAddToArchive), archiverInfo.Name);

    struct CInitDialogItem initItems[]=
    {
      { DI_DOUBLEBOX, 3, 1, 72, kYSize - 2, false, false, 0, false, NMessageID::kUpdateTitle, NULL, NULL },

      { DI_TEXT, 5, 2, 0, 0, false, false, 0, false, -1, updateAddToArchiveString, NULL },
      
      { DI_EDIT, 5, 3, 70, 3, true, false, DIF_HISTORY, false, -1, archiveName, kArchiveHistoryKeyName},
      // { DI_EDIT, 5, 3, 70, 3, true, false, 0, false, -1, archiveName, NULL},
      
      { DI_SINGLEBOX, 4, 4, kXMid - 2, 4 + 4, false, false, 0, false, NMessageID::kUpdateMethod, NULL, NULL },
      { DI_RADIOBUTTON, 6, 5, 0, 0, false, 
          compressionInfo.Method == 0, 
          DIF_GROUP, false, NMessageID::kUpdateMethodStore, NULL, NULL },
      { DI_RADIOBUTTON, 6, 6, 0, 0, false, 
          compressionInfo.Method == 1, 
          0, false, NMessageID::kUpdateMethodNormal, NULL, NULL },
      { DI_RADIOBUTTON, 6, 7, 0, 0, false,
          compressionInfo.Method == 2, 
          false, 0, NMessageID::kUpdateMethodMaximum, NULL, NULL },
      
      { DI_SINGLEBOX, kXMid, 4, 70, 4 + 5, false, false, 0, false, NMessageID::kUpdateMode, NULL, NULL },
      { DI_RADIOBUTTON, kXMid + 2, 5, 0, 0, false, 
          actionSet == &kAddActionSet,
          DIF_GROUP, false, NMessageID::kUpdateModeAdd, NULL, NULL },
      { DI_RADIOBUTTON, kXMid + 2, 6, 0, 0, false, 
          actionSet == &kUpdateActionSet,
          0, false, NMessageID::kUpdateModeUpdate, NULL, NULL },
      { DI_RADIOBUTTON, kXMid + 2, 7, 0, 0, false, 
          actionSet == &kFreshActionSet,
          0, false, NMessageID::kUpdateModeFreshen, NULL, NULL },
      { DI_RADIOBUTTON, kXMid + 2, 8, 0, 0, false, 
          actionSet == &kSynchronizeActionSet,
          0, false, NMessageID::kUpdateModeSynchronize, NULL, NULL },
      
      { DI_TEXT, 3, kYSize - 4, 0, 0, false, false, DIF_BOXCOLOR|DIF_SEPARATOR, false, -1, "", NULL  },  
      
      { DI_BUTTON, 0, kYSize - 3, 0, 0, false, false, DIF_CENTERGROUP, true, NMessageID::kUpdateAdd, NULL, NULL  },
      { DI_BUTTON, 0, kYSize - 3, 0, 0, false, false, DIF_CENTERGROUP, false, NMessageID::kUpdateSelectArchiver, NULL, NULL  },
      { DI_BUTTON, 0, kYSize - 3, 0, 0, false, false, DIF_CENTERGROUP, false, NMessageID::kCancel, NULL, NULL  }
    };

    const kNumDialogItems = sizeof(initItems) / sizeof(initItems[0]);
    
    const kOkButtonIndex = kNumDialogItems - 3;
    const kSelectarchiverButtonIndex = kNumDialogItems - 2;

    FarDialogItem dialogItems[kNumDialogItems];
    g_StartupInfo.InitDialogItems(initItems, dialogItems, kNumDialogItems);
    int askCode = g_StartupInfo.ShowDialog(76, kYSize, 
        kHelpTopic, dialogItems, kNumDialogItems);

    archiveName = dialogItems[kArchiveNameIndex].Data;
    archiveName.Trim();

    if (dialogItems[kMethodRadioIndex].Selected)
      compressionInfo.SetMethod(0);
    else if (dialogItems[kMethodRadioIndex + 1].Selected)
      compressionInfo.SetMethod(1);
    else if (dialogItems[kMethodRadioIndex + 2].Selected)
      compressionInfo.SetMethod(2);
    else
      throw 51751;

    if (dialogItems[kModeRadioIndex].Selected)
      actionSet = &kAddActionSet;
    else if (dialogItems[kModeRadioIndex + 1].Selected)
      actionSet = &kUpdateActionSet;
    else if (dialogItems[kModeRadioIndex + 2].Selected)
      actionSet = &kFreshActionSet;
    else if (dialogItems[kModeRadioIndex + 3].Selected)
      actionSet = &kSynchronizeActionSet;
    else
      throw 51751;

    if (askCode == kSelectarchiverButtonIndex)
    {
      CSysStringVector archiverNames;
      for(int i = 0; i < archiverInfoList.Size(); i++)
        archiverNames.Add(archiverInfoList[i].Name);
    
      int index = g_StartupInfo.Menu(FMENU_AUTOHIGHLIGHT, 
          g_StartupInfo.GetMsgString(NMessageID::kUpdateSelectArchiverMenuTitle),
          NULL, archiverNames, archiverIndex);
      if(index >= 0)
      {
        const NZipRootRegistry::CArchiverInfo &prevArchiverInfo = archiverInfoList[prevFormat];
        if (prevArchiverInfo.KeepName)
        {
          const CSysString &prevExtension = prevArchiverInfo.Extension;
          const int prevExtensionLen = prevExtension.Length();
          if (archiveName.Right(prevExtensionLen).CompareNoCase(prevExtension) == 0)
          {
            int pos = archiveName.Length() - prevExtensionLen;
            CSysString temp = archiveName.Left(pos);
            if (pos > 1)
            {
              int dotPos = archiveName.ReverseFind('.');
              if (dotPos == pos - 1)
                archiveName = archiveName.Left(dotPos);
            }
          }
        }

        archiverIndex = index;
        const NZipRootRegistry::CArchiverInfo &archiverInfo = 
            archiverInfoList[archiverIndex];
        prevFormat = archiverIndex;
        
        if (archiverInfo.KeepName)
          archiveName = archiveNameSrc;
        else
        {
          int dotPos = archiveName.ReverseFind('.');
          int slashPos = MyMax(archiveName.ReverseFind('\\'), archiveName.ReverseFind('/'));
          if (dotPos > slashPos)
            archiveName = archiveName.Left(dotPos);
        }
        archiveName += '.';
        archiveName += archiverInfo.Extension;
      }
      continue;
    }

    if (askCode != kOkButtonIndex)
      return E_ABORT;
    
    break;
  }

  const CLSID &classID = archiverInfoList[archiverIndex].ClassID;
  compressionInfo.SetLastClassID(classID);
  NZipRegistryManager::SaveCompressionInfo(compressionInfo);

  NZipSettings::NWorkDir::CInfo workDirInfo;
  NZipRegistryManager::ReadWorkDirInfo(workDirInfo);

  CSysString fullArchiveName;
  if (!MyGetFullPathName(archiveName, fullArchiveName))
    return E_FAIL;
   
  CSysString workDir = GetWorkDir(workDirInfo, fullArchiveName);
  CreateComplexDirectory(workDir);

  CTempFile tempFile;
  CSysString tempFileName;
  if (tempFile.Create(workDir, kTempArcivePrefix, tempFileName) == 0)
    return E_FAIL;


  CScreenRestorer screenRestorer;
  CProgressBox progressBox;
  CProgressBox *progressBoxPointer = NULL;

  screenRestorer.Save();

  progressBoxPointer = &progressBox;
  progressBox.Init(g_StartupInfo.GetMsgString(NMessageID::kWaitTitle),
     g_StartupInfo.GetMsgString(NMessageID::kUpdating), 1 << 16);


  // std::auto_ptr<CProxyHandler> proxyHandler;
  NFind::CFileInfo fileInfo;

  CComPtr<IOutFolderArchive> outArchive;

  CComPtr<IInFolderArchive> archiveHandler;
  if(NFind::FindFile(fullArchiveName, fileInfo))
  {
    if (fileInfo.IsDirectory())
      throw "There is Directory with such name";

    NZipRootRegistry::CArchiverInfo archiverInfoResult;
    UString defaultName;
    RINOK(OpenArchive(fullArchiveName, &archiveHandler, 
        archiverInfoResult, defaultName, NULL));

    if (archiverInfoResult.ClassID != classID)
      throw "Type of existing archive differs from specified type";
    HRESULT result = archiveHandler.QueryInterface(&outArchive);
    if(result != S_OK)
    {
      g_StartupInfo.ShowMessage(NMessageID::kUpdateNotSupportedForThisArchive);
      return E_FAIL;
    }
  }
  else
  {
    // HRESULT result = outArchive.CoCreateInstance(classID);
    CComObjectNoLock<CAgent> *agentSpec = new CComObjectNoLock<CAgent>;
    outArchive = agentSpec;

    /*
    HRESULT result = outArchive.CoCreateInstance(CLSID_CAgentArchiveHandler);
    if (result != S_OK)
    {
      g_StartupInfo.ShowMessage(NMessageID::kUpdateNotSupportedForThisArchive);
      return E_FAIL;
    }
    */
  }

  CRecordVector<const wchar_t *> fileNamePointers;
  fileNamePointers.Reserve(fileNames.Size());
  for(i = 0; i < fileNames.Size(); i++)
    fileNamePointers.Add(fileNames[i]);

  outArchive->SetFolder(NULL);
  // CSysString aCurrentFolder;
  // MyGetCurrentDirectory(aCurrentFolder);
  // outArchive->SetFiles(MultiByteToUnicodeString(aCurrentFolder, CP_OEMCP), 
  outArchive->SetFiles(L"", 
    &fileNamePointers.Front(), fileNamePointers.Size());
  BYTE actionSetByte[NUpdateArchive::NPairState::kNumValues];
  for (i = 0; i < NUpdateArchive::NPairState::kNumValues; i++)
    actionSetByte[i] = actionSet->StateActions[i];

  CComObjectNoLock<CUpdateCallBack100Imp> *updateCallbackSpec =
    new CComObjectNoLock<CUpdateCallBack100Imp>;
  CComPtr<IFolderArchiveUpdateCallback> updateCallback(updateCallbackSpec );
  
  updateCallbackSpec->Init(archiveHandler, &progressBox);


  RINOK(SetOutProperties(outArchive, compressionInfo.Method));

  HRESULT result = outArchive->DoOperation(&classID,
      MultiByteToUnicodeString(tempFileName, CP_OEMCP), actionSetByte, 
      NULL, updateCallback);
  updateCallback.Release();
  outArchive.Release();

  if (result != S_OK)
  {
    ShowErrorMessage(result);
    return result;
  }
 
  if(archiveHandler)
  {
    archiveHandler->Close();
    if (!DeleteFileAlways(fullArchiveName))
    {
      ShowLastErrorMessage();
      return NFileOperationReturnCode::kError;
    }
  }
  tempFile.DisableDeleting();
  if (!MoveFile(tempFileName, fullArchiveName))
  {
    ShowLastErrorMessage();
    return E_FAIL;
  }
  
  return S_OK;
}

