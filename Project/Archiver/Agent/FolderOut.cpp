// FolderOut.cpp

#include "StdAfx.h"
#include "Handler.h"

#include "Common/StringConvert.h"
#include "Windows/FileDir.h"

#include "../Common/CompressEngineCommon.h"
#include "../Common/ZipRegistry.h"
#include "../Common/UpdateUtils.h"

using namespace NWindows;
using namespace NFile;
using namespace NDirectory;

static LPCTSTR kTempArcivePrefix = TEXT("7zi");

static inline UINT GetCurrentFileCodePage()
  {  return AreFileApisANSI() ? CP_ACP : CP_OEMCP; }

void CAgentFolder::GetPathParts(UStringVector &pathParts)
{
  pathParts.Clear();
  CComPtr<IFolderFolder> folder = this;
  while (true)
  {
    CComPtr<IFolderFolder> newFolder;
    folder->BindToParentFolder(&newFolder);  
    if (newFolder == NULL)
      break;
    CComBSTR name;
    folder->GetName(&name);
    pathParts.Insert(0, (const wchar_t *)name);
    folder = newFolder;
  }
}

HRESULT CAgentFolder::CommonUpdateOperation(
    bool deleteOperation,
    const NUpdateArchive::CActionSet *actionSet,
    const UINT32 *indices, UINT32 numItems,
    IUpdateCallback100 *updateCallback100)
{
  UINT codePage = GetCurrentFileCodePage();
  // CZipRegistryManager aZipRegistryManager;
  NZipSettings::NWorkDir::CInfo workDirInfo;
  NZipRegistryManager::ReadWorkDirInfo(workDirInfo);
  CSysString archiveFilePath  = _agentSpec->_archiveFilePath;
  CSysString workDir = GetWorkDir(workDirInfo, archiveFilePath );
  CreateComplexDirectory(workDir);

  CTempFile tempFile;
  CSysString tempFileName;
  if (tempFile.Create(workDir, kTempArcivePrefix, tempFileName) == 0)
    return E_FAIL;


  BYTE actionSetByte[NUpdateArchive::NPairState::kNumValues];
  for (int i = 0; i < NUpdateArchive::NPairState::kNumValues; i++)
    actionSetByte[i] = actionSet->StateActions[i];

  /*
  if (SetOutProperties(anOutArchive, aCompressionInfo.Method) != S_OK)
    return NFileOperationReturnCode::kError;
  */
  
  ////////////////////////////
  // Save FolderItem;

  UStringVector pathParts;
  GetPathParts(pathParts);

  HRESULT result;
  if (deleteOperation)
    result = _agentSpec->DeleteItems(GetUnicodeString(tempFileName, codePage),
        indices, numItems, updateCallback100);
  else
    result = _agentSpec->DoOperation(NULL,GetUnicodeString(tempFileName, codePage),
        actionSetByte, NULL, updateCallback100);
  
  if (result != S_OK)
    return result;

  _agentSpec->Close();
  
  // m_FolderItem = NULL;
  
  if (!DeleteFileAlways(archiveFilePath ))
    return GetLastError();

  tempFile.DisableDeleting();
  if (!MoveFile(tempFileName, archiveFilePath ))
    return GetLastError();
  
  RETURN_IF_NOT_S_OK(_agentSpec->FolderReOpen(NULL));

 
  ////////////////////////////
  // Restore FolderItem;

  CComPtr<IFolderFolder> archiveFolder;
  RETURN_IF_NOT_S_OK(_agentSpec->BindToRootFolder(&archiveFolder));
  for (i = 0; i < pathParts.Size(); i++)
  {
    CComPtr<IFolderFolder> newFolder;
    archiveFolder->BindToFolder(pathParts[i], &newFolder);
    if(!newFolder)
      break;
    archiveFolder = newFolder;
  }

  CComPtr<IArchiveFolderInternal> archiveFolderInternal;
  RETURN_IF_NOT_S_OK(archiveFolder.QueryInterface(&archiveFolderInternal));
  CAgentFolder *agentFolder;
  RETURN_IF_NOT_S_OK(archiveFolderInternal->GetAgentFolder(&agentFolder));
  _proxyFolderItem = agentFolder->_proxyFolderItem;
  _proxyHandler = agentFolder->_proxyHandler;
  _parentFolder = agentFolder->_parentFolder;

  return S_OK;
}

STDMETHODIMP CAgentFolder::CopyFrom(
    const wchar_t *fromFolderPath, // test it
    const wchar_t **itemsPaths,
    UINT32 numItems,
    IProgress *progress)
{
  RETURN_IF_NOT_S_OK(_agentSpec->SetFiles(fromFolderPath, itemsPaths, numItems));
  RETURN_IF_NOT_S_OK(_agentSpec->SetFolder(this));
  CComPtr<IUpdateCallback100> updateCallback100;
  if (progress != 0)
  {
    CComPtr<IProgress> progressWrapper = progress;
    RETURN_IF_NOT_S_OK(progressWrapper.QueryInterface(&updateCallback100));
  }
  return CommonUpdateOperation(false, &kAddActionSet, 0, 0, 
      updateCallback100);
}

STDMETHODIMP CAgentFolder::Delete(const UINT32 *indices, UINT32 numItems, IProgress *progress)
{
  RETURN_IF_NOT_S_OK(_agentSpec->SetFolder(this));
  CComPtr<IUpdateCallback100> updateCallback100;
  if (progress != 0)
  {
    CComPtr<IProgress> progressWrapper = progress;
    RETURN_IF_NOT_S_OK(progressWrapper.QueryInterface(&updateCallback100));
  }
  return CommonUpdateOperation(true, &kDeleteActionSet, indices, 
      numItems, updateCallback100);
}