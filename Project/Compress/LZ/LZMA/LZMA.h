// LZMA.h

#pragma once 

#include "LenCoder.h"

#ifndef __LZMA_H
#define __LZMA_H

namespace NCompress {
namespace NLZMA {

const UINT32 kNumRepDistances = 4;

const BYTE kNumStates = 12;

const BYTE kLiteralNextStates[kNumStates] = {0, 0, 0, 0, 1, 2, 3, 4,  5,  6,   4, 5};
const BYTE kMatchNextStates[kNumStates]   = {7, 7, 7, 7, 7, 7, 7, 10, 10, 10, 10, 10};
const BYTE kRepNextStates[kNumStates]     = {8, 8, 8, 8, 8, 8, 8, 11, 11, 11, 11, 11};
const BYTE kShortRepNextStates[kNumStates]= {9, 9, 9, 9, 9, 9, 9, 11, 11, 11, 11, 11};

class CState
{
public:
  BYTE m_Index;
  void Init()
    { m_Index = 0; }
  void UpdateChar()
    { m_Index = kLiteralNextStates[m_Index]; }
  void UpdateMatch()
    { m_Index = kMatchNextStates[m_Index]; }
  void UpdateRep()
    { m_Index = kRepNextStates[m_Index]; }
  void UpdateShortRep()
    { m_Index = kShortRepNextStates[m_Index]; }
};

class CBaseCoder
{
protected:
  CState m_State;
  BYTE m_PreviousByte;
  bool m_PeviousIsMatch;
  UINT32 m_RepDistances[kNumRepDistances];
  void Init()
  {
    m_State.Init();
    m_PreviousByte = 0;
    m_PeviousIsMatch = false;
    for(int i = 0 ; i < kNumRepDistances; i++)
      m_RepDistances[i] = 0;
  }
};

const kNumPosSlotBits = 6; 
const kDicLogSizeMax = 28; 
const kDistTableSizeMax = kDicLogSizeMax * 2; 

extern UINT32 kDistStart[kDistTableSizeMax];
const BYTE kDistDirectBits[kDistTableSizeMax] = 
{
  0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9,
  10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 
  20, 20, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25, 26, 26 
};

const UINT32 kNumLenToPosStates = 4;
inline UINT32 GetLenToPosState(UINT32 aLen)
{
  aLen -= 2;
  if (aLen < kNumLenToPosStates)
    return aLen;
  return kNumLenToPosStates - 1;
}

const kMatchMinLen = 2;

const kMatchMaxLen = kMatchMinLen + NLength::kNumSymbolsTotal - 1;

const kNumAlignBits = 4;
const kAlignTableSize = 1 << kNumAlignBits;
const UINT32 kAlignMask = (kAlignTableSize - 1);

const kStartPosModelIndex = 4;
const kEndPosModelIndex = 14;
const kNumPosModels = kEndPosModelIndex - kStartPosModelIndex;

const kNumFullDistances = 1 << (kEndPosModelIndex / 2);


const kMainChoiceLiteralIndex = 0;
const kMainChoiceMatchIndex = 1;

const kMatchChoiceDistanceIndex= 0;
const kMatchChoiceRepetitionIndex = 1;

const kNumMoveBitsForMainChoice = 5;
const kNumMoveBitsForPosCoders = 5;

const kNumMoveBitsForAlignCoders = 5;

const kNumMoveBitsForPosSlotCoder = 5;

const kNumLitPosStatesBitsEncodingMax = 4;
const kNumLitContextBitsMax = 8;

}}

#endif
