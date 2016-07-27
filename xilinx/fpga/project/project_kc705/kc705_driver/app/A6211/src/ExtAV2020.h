#ifndef AV2020_h_h
#define AV2020_h_h

#include "II2CRepeater.h"
#include "avl_dvbsx.h"
#include "ITuner.h"

#ifdef AVL_CPLUSPLUS
extern "C" {
#endif
#define  tuner_crystal 27 //unit is MHz

AVL_DVBSx_ErrorCode AVL_DVBSx_ExtAV2020_Initialize(struct AVL_Tuner * pTuner);
AVL_DVBSx_ErrorCode AVL_DVBSx_ExtAV2020_GetLockStatus(struct AVL_Tuner * pTuner);
AVL_DVBSx_ErrorCode AVL_DVBSx_ExtAV2020_Lock( struct AVL_Tuner * pTuner);

#ifdef AVL_CPLUSPLUS
}
#endif
#endif
