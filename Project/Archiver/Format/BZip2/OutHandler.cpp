// Zip/OutHandler.cpp

#include "StdAfx.h"

#include "Handler.h"

#include "Windows/FileFind.h"
#include "Windows/Defs.h"
#include "Windows/PropVariant.h"

#include "Compression/CopyCoder.h"

#include "UpdateEngine.h"

using namespace NWindows;

static const kNumItemInArchive = 1;

namespace NArchive {
namespace NBZip2 {

STDMETHODIMP CHandler::GetFileTimeType(UINT32 *type)
{
  *type = NFileTimeType::kUnix;
  return S_OK;
}

static HRESULT CopyStreams(IInStream *inStream, IOutStream *outStream, 
    IArchiveUpdateCallback *updateCallback)
{
  CComObjectNoLock<NCompression::CCopyCoder> *copyCoderSpec = 
      new CComObjectNoLock<NCompression::CCopyCoder>;
  CComPtr<ICompressCoder> copyCoder = copyCoderSpec;
  return copyCoder->Code(inStream, outStream, NULL, NULL, NULL);
}

STDMETHODIMP CHandler::UpdateItems(IOutStream *outStream, UINT32 numItems,
    IArchiveUpdateCallback *updateCallback)
{
  if (numItems != 1)
    return E_INVALIDARG;

  INT32 newData;
  INT32 newProperties;
  UINT32 indexInArchive;
  if (!updateCallback)
    return E_FAIL;
  RINOK(updateCallback->GetUpdateItemInfo(0,
      &newData, &newProperties, &indexInArchive));
 
  if (IntToBool(newProperties))
  {
    {
      NCOM::CPropVariant propVariant;
      RINOK(updateCallback->GetProperty(0, kpidAttributes, &propVariant));
      if (propVariant.vt == VT_UI4)
      {
        if (NFile::NFind::NAttributes::IsDirectory(propVariant.ulVal))
          return E_INVALIDARG;
      }
      else if (propVariant.vt != VT_EMPTY)
        return E_INVALIDARG;
    }
    {
      NCOM::CPropVariant propVariant;
      RINOK(updateCallback->GetProperty(0, kpidIsFolder, &propVariant));
      if (propVariant.vt == VT_BOOL)
      {
        if (propVariant.boolVal != VARIANT_FALSE)
          return E_INVALIDARG;
      }
      else if (propVariant.vt != VT_EMPTY)
        return E_INVALIDARG;
    }
  }
  
  if (IntToBool(newData))
  {
    UINT64 size;
    {
      NCOM::CPropVariant propVariant;
      RINOK(updateCallback->GetProperty(0, kpidSize, &propVariant));
      if (propVariant.vt != VT_UI8)
        return E_INVALIDARG;
      size = *(UINT64 *)(&propVariant.uhVal);
    }
    return UpdateArchive(size, outStream, 0, updateCallback);
  }
  if (indexInArchive != 0)
    return E_INVALIDARG;
  RINOK(_stream->Seek(_streamStartPosition, STREAM_SEEK_SET, NULL));
  return CopyStreams(_stream, outStream, updateCallback);
}

}}