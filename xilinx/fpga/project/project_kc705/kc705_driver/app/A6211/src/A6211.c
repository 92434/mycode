/******************************************************************************
**
** \file      A6211.c
**
** \brief     Front-end device driver for A6211 frontend.
**
**            (C) Fujitsu Microelectronics Europe 2002 - 2007
**
** \attention THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU MICROELECTRONICS
**            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
**            OMMISSIONS.
**
** \version   $Id: $
**
******************************************************************************/
/*lint -save -e656 -e713 -e715 -e732 */

//#include "../../src/fe_priv.h"
//#include <fapi/drv_gpio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
/* FE specific includes */
#include "IBSP.h"
#include "A6211.h"
#include "IBase.h"
#include "avl_dvbsx.h"
#include "II2C.h"
#include "IDiseqc.h"
#include "IBlindScan.h"
#include "ExtSharpBS2S7VZ7801.h"
#include "ExtAV2011.h"
#include "IRx.h"
#define DEBUG 3

#define AVL_DVBSx_EC_OK 0
#define FE_OK 0
#define FE_ERR_BAD_PARAMETER -1
#define FE_ERR_FEATURE_NOT_SUPPORTED -2
#define FE_ERR_FE_NOT_LOCKED -3
#define FE_ERR_FE_TUNE -4
#define FAPI_SYS_PRINT_MSG printf
#define FE_MAX_NUM 1


typedef enum{
	FE_LNB_OFF,
	FE_LNB_13V,
	FE_LNB_18V
}FE_LNB_VOLTAGE_E;

typedef enum{
	FE_22KHZ_OFF,
	FE_22KHZ_ON,
	FE_22KHZ_TONE_0,
	FE_22KHZ_TONE_1
}FE_22KHZ_E;

typedef enum{
	FE_POL_HOR,
	FE_POL_VER
}FE_POL_E;

typedef enum{
	FE_STATUS_LOCKED,
	FE_STATUS_TUNING,
	FE_STATUS_NOT_LOCKED,
	FE_STATUS_UNKNOWN,
}FE_STATUS_E;

/*---------------------------------------------------------------------------*/
/* local defines                                                             */
/*---------------------------------------------------------------------------*/
//#define  TUNER_SHARP_7801
//#define TUNER_RDA_AV2020_DUL
//#define TUNER_ALLO_AV2011
#define TUNER_ALLO_AV2012
#define tuner_slave_address    0xC0 //The Tuner slave address. It is the write address of the tuner device. i.e. it is 8 bits address and the last bit is the R/W flag which is zero. Although this data structure support 10 bits I2C address. The AVL6211 chip not supports it. 
#define tuner_I2Cbus_clock      200 //The clock speed of the tuner dedicated I2C bus, in a unit of kHz.

struct Signal_Level
{
    uint16_t SignalLevel;
    int16_t  SignalDBM;
};

typedef enum{
	BLIND_LNBFRE_CSINGLE=0,
	BLIND_LNBFRE_KUSINGLE,
	BLIND_LNBFRE_KUBAUD_H,
	BLIND_LNBFRE_KUBAUD_V,
	BLIND_LNBFRE_CBAUD_H,
	BLIND_LNBFRE_CBAUD_V,
}BLIND_SCAN_LNBTYPE;

#if 0
#define SignalLevelLUT_SIZE     47
struct Signal_Level  SignalLevelLUT [SignalLevelLUT_SIZE]=
{
    {8285,  -922},{10224, -902},{12538, -882},{14890, -862},{17343, -842},
    {19767, -822},{22178, -802},{24618, -782},{27006, -762},{29106, -742},
    {30853, -722},{32289, -702},{33577, -682},{34625, -662},{35632, -642},
    {36552, -622},{37467, -602},{38520, -582},{39643, -562},{40972, -542},
    {42351, -522},{43659, -502},{44812, -482},{45811, -462},{46703, -442},
    {47501, -422},{48331, -402},{49116, -382},{49894, -362},{50684, -342},
    {51543, -322},{52442, -302},{53407, -282},{54314, -262},{55208, -242},
    {56000, -222},{56789, -202},{57544, -182},{58253, -162},{58959, -142},
    {59657, -122},{60404, -102},{61181,  -82},{62008,  -62},{63032,  -42},
    {65483,  -22},{65535,  -12}


};
#else
#define SignalLevelLUT_SIZE     30
struct Signal_Level  SignalLevelLUT [SignalLevelLUT_SIZE]=
{
    {200,  -70},{1200, -68},{4250, -65},{6900, -63},{9000, -61},
    {10400, -59},{10800, -57},{12150, -55},{13651, -53},{16097, -51},
    {15950, -49},{18870, -47},{19520, -45},{21060, -43},{23380, -41},
    {25159, -39},{26470, -37},{27520, -35},{28350, -33},{29060, -31},
    {29720, -29},{30300, -27},{30900, -25},{31500, -23},{32060, -21},
    {32669, -19},{33129, -17},{33783, -15},{34350, -13},{34980, -11},
};

#endif
struct AVL_DVBSx_Chip   a6211Hdl[FE_MAX_NUM];
struct AVL_Tuner    tunerHdl[FE_MAX_NUM];
//static feHandleT    *pData[FE_MAX_NUM];
extern AVL_uchar AVL6211_ucPatchData [];
//extern FE_HANDLE A6211_i2cHandle;
extern uint8_t   A6211_i2cAddr;
extern uint8_t   A6211_feIndex;

#define tuner_LPF               340         //The LPF of the tuner,in a unit of 100kHz.
/*---------------------------------------------------------------------------*/
/* callback functions                                                        */
/*---------------------------------------------------------------------------*/
static int32_t A6211_Open(uint8_t index);
static int32_t A6211_Close(uint8_t index);
//static int32_t A6211_SetSystemParams(uint8_t index,FE_SYSTEM_PARAMS_S* pSystemParams);
//static int32_t A6211_GetSystemParams(uint8_t index,FE_SYSTEM_PARAMS_S* pSystemParams);
//static int32_t A6211_Reset(uint8_t index);
static int32_t A6211_Tune(uint8_t index, uint32_t uiFrequency_kHz, uint32_t SymbolRate_kHz);
static int32_t A6211_GetStatus(uint8_t index, FE_STATUS_E* pStatus);
static int32_t A6211_GetAFC(uint8_t index, int32_t* pAFC);
static int32_t A6211_GetBER(uint8_t index, uint32_t* pBER);
static int32_t A6211_GetSignalQuality(uint8_t index, uint16_t* pQuality);
static int32_t A6211_GetSignalStrength(uint8_t index, uint16_t* pStrength);
//static int32_t A6211_GetTuneParams(uint8_t index, FE_TUNE_PARAMS_S *pTuneParams);
//static int32_t A6211_SendDiseqcCommand(uint8_t index, uint8_t len,uint8_t *pCommand,
                                      //uint8_t *pRetLen, uint8_t *pRetBuf);
static int32_t A6211_Control22kHz(uint8_t index, FE_22KHZ_E mode);
static int32_t A6211_ControlLnbVoltage(uint8_t index, FE_LNB_VOLTAGE_E mode);
//static int32_t A6211_Scan(uint8_t index, FE_SCAN_PARAMS_S *pScanParams);

//static int32_t Demod_CheckChipId(feHandleT *hp, uint8_t i2cAddress);
//static void    A6211_SetFunctions(feHandleT *hp);
static int32_t A6211_Initialize(struct AVL_DVBSx_Chip  *pAVLChip, struct AVL_Tuner * pTuner);
static uint16_t A2108_TestSinalLevel(void);
static BLIND_SCAN_LNBTYPE  A6211_Blindsacn_lnbtype(uint32_t freqhi,uint32_t freqlo,FE_POL_E pol);
//#define    TS_EXTERN 
/*!
*******************************************************************************
**
** \brief Check chip ID.
** \param hp - FE handle for current FE front-end.
** \param i2cAddress  - i2c address for demod.
**
*******************************************************************************
*/
/*
static int32_t Demod_CheckChipId(feHandleT *hp, uint8_t i2cAddress)
{
	int32_t err;
	uint8_t rxBuff[4]; 
	uint8_t txBuff[4];

	ChunkAddr(rom_ver_addr,txBuff);
	err = FAPI_I2C_Read(hp->i2cHandle,i2cAddress,txBuff,3,rxBuff,4);
	if((rxBuff[0] == 1) && (rxBuff[1]== 0) &&( (rxBuff[2]+rxBuff[3])== 2) )
	{	
		FAPI_SYS_PRINT_MSG( "[read data success] !\n");
		return FE_OK;
	}

	FAPI_SYS_PRINT_MSG( "[AVL6222 Demod_CheckChipId] FAIL !\n");
	return FE_ERR_FE_UNKNOWN_DEVICE;
	
	return FE_OK;
}
*/

/*!
*******************************************************************************
**
** \brief Set callback function for the current FE handle.
** \param hp - FE handle
**
*******************************************************************************
*/
/*
static void A6211_SetFunctions(feHandleT *hp)
{
    hp->systemType             = FE_SYSTEM_TYPE_SATELLITE;
    hp->feOpenFct              = A6211_Open;
    hp->feCloseFct             = A6211_Close;
    hp->feSetSystemParamsFct   = A6211_SetSystemParams;
    hp->feGetSystemParamsFct   = A6211_GetSystemParams;
    hp->feResetFct             = A6211_Reset;
    hp->feTuneFct              = A6211_Tune;
    hp->feScanFct              = A6211_Scan;
    hp->feSendDiseqcCommandFct = A6211_SendDiseqcCommand;
    hp->feGetStatusFct         = A6211_GetStatus;
    hp->feGetFECFct            = NULL;
    hp->feGetAFCFct            = A6211_GetAFC;
    hp->feGetBERFct            = A6211_GetBER;
    hp->feGetSignalQualityFct  = A6211_GetSignalQuality;
    hp->feGetSignalStrengthFct = A6211_GetSignalStrength;
    hp->feGetTuneParamsFct     = A6211_GetTuneParams;
    hp->feSetAntennaPowerFct   = NULL;
    hp->feControl22kHz         = A6211_Control22kHz;
    hp->feControlLnbVoltage    = A6211_ControlLnbVoltage;
    hp->isPresent              = 1;
    
}
*/
/*!
*******************************************************************************
**
** \brief Detect demod.
** 
** \param slot  slot number
** \param hp    FE handle
**
*******************************************************************************
*/
/*
int32_t A6211_Detect( uint8_t slot, feHandleT *hp)
{
    FAPI_SYS_PRINT_MSG( "[A6211_Detect] @@@@A6211_Detect:%d !\n",slot);
    if (slot == 0)
    {
        hp->i2cAddrDemod  =AVL_DVBSx_SA_0<<1;
		
    }
    else if (slot == 1)
    {
        hp->i2cAddrDemod  =AVL_DVBSx_SA_1<<1;
    }
    else
    {
        return (FE_ERR_BAD_PARAMETER);
    }
#if 0
    if(Demod_CheckChipId(hp, hp->i2cAddrDemod) != FE_OK)
    {
      FAPI_SYS_PRINT_MSG( "[A6211_Detect] UNKNOWN_DEVICE !\n");
       // return FE_ERR_FE_UNKNOWN_DEVICE;
    }
 #endif  
 	A6211_SetFunctions(hp);
#ifdef TUNER_RDA_AV2020_DUL
if(slot == 0)
{
    hp->i2cAddrTuner = 0xC6;
}
else if (slot == 1)
{
	hp->i2cAddrTuner = 0xC0;
}
#endif

#if defined( TUNER_SHARP_7801)||defined( TUNER_ALLO_AV2011)
	hp->i2cAddrTuner = 0xC0;
#endif

    hp->slotNum      = slot;
    hp->tunerType    = 0;
    
    return FE_OK;
}
*/
/*!
*******************************************************************************
**
** \brief Open demod.
** 
** \param index         slot number
** \param pDeviceData   An pointer point to the current FE handle.
**
*******************************************************************************
*/
static int32_t A6211_Open(uint8_t index)
{
    AVL_DVBSx_ErrorCode         error;

	FAPI_SYS_PRINT_MSG( "[A6211_Open] AVL6211 OPEN,index:%d!\n",index);

 //   (void)A6211_Reset(index);
    AVL_DVBSx_IBSP_Delay( 100 );
    //(void)FAPI_I2C_SetProtocol(A6211_i2cHandle, FE_I2C_COMMON_PROTOCOL);

 
	a6211Hdl[index].m_uiBusId=index;
    error = A6211_Initialize(&a6211Hdl[index],&tunerHdl[index]);
    if(error != AVL_DVBSx_EC_OK)
    {
        return FE_ERR_BAD_PARAMETER;
    }
 
    return FE_OK;
}

/*!
*******************************************************************************
**
** \brief Close demod.
** 
** \param index     slot number.
**
*******************************************************************************
*/
static int32_t A6211_Close(uint8_t index)
{
    if( index >= FE_MAX_NUM )
    {
        return FE_ERR_BAD_PARAMETER;
    }
    // TODO 
    return FE_OK;
}

/*!
*******************************************************************************
**
** \brief Set systemParams to the FE handle defined by index..
** 
** \param index             index of FE
** \param pSystemParams     pointer to system params.
**
*******************************************************************************
*/
/*
static int32_t A6211_SetSystemParams(uint8_t index,
                                     FE_SYSTEM_PARAMS_S* pSystemParams)
{
    if( (index >= FE_MAX_NUM) ||
        (pSystemParams == NULL) )
    {
        return FE_ERR_BAD_PARAMETER;
    }
    
    pData[index]->pSystemParams = *pSystemParams;
    	{
			#if 0
			    struct // satellite systems: 
    {
        uint8_t             classic;       //!< Enable classic H/V control with 22KHz and 14/18V.
        uint8_t             posBy22kHz;    //!< Enable Sat selection with 22KHz and H/V control 14/18V.
        uint8_t             toneburst;     //!< Enable toneburst control for 2 satellites (sat ID 0/1).
        FE_DISEQC_E         diseqc;        //!< DiSEqC type.
        uint8_t             diseqc10Rep;   //!< Command repetition for DiSEqC 1.0 (0 = command sent once).
        uint8_t             diseqc11Rep;   //!< Command repetition for DiSEqC 1.1 DTH.
        uint8_t             ifFrequency;   //!< IF Frequency DiSEqC 1.1 SMATV.
        uint32_t            lowFrequency;  //!< LNB low oscillator frequency [KHz].
        uint32_t            highFrequency; //!< LNB high oscillator frequency [KHz].
        uint8_t             autoFEC;       //!< Enable automatic FEC.
        FE_IQ_E             defaultIQ;     //!< Default inversion mode.
        uint32_t*           custom;        //!< For custom use.
        uint8_t             motorAddress;  //!< DiSEqC address for motor control.
    } sat;
#endif
	    FAPI_SYS_PRINT_MSG( "[A6211_SetSystemParams]  is call\n");	
	 FAPI_SYS_PRINT_MSG( "classic:%d.\n",pData[index]->pSystemParams.sat.classic);
	  FAPI_SYS_PRINT_MSG( "posBy22kHz:%d.\n",pData[index]->pSystemParams.sat.posBy22kHz);
	   FAPI_SYS_PRINT_MSG( "toneburst:%d.\n",pData[index]->pSystemParams.sat.toneburst);
	    FAPI_SYS_PRINT_MSG( "diseqc:%d.\n",pData[index]->pSystemParams.sat.diseqc);
		}
    return FE_OK;
}
*/

/*!
*******************************************************************************
**
** \brief Get systemParams from the FE defined by index.
** 
** \param index             index of FE
** \param pSystemParams     pointer to write system params to.
**
*******************************************************************************
*/
/*
static int32_t A6211_GetSystemParams(uint8_t index,
                                     FE_SYSTEM_PARAMS_S* pSystemParams)
{
    if( (index >= FE_MAX_NUM) ||
        (pSystemParams == NULL) )
    {
        return FE_ERR_BAD_PARAMETER;
    }
    
    *pSystemParams = pData[index]->pSystemParams;
    
    return FE_OK;
}

*/
/*!
*******************************************************************************
**
** \brief Reset demod.
** 
** \param index     index of FE
**
*******************************************************************************
*/
/*
static int32_t A6211_Reset(uint8_t index)
{
    int32_t error;
  #ifdef TS_EXTERN
 return FE_OK;
#endif  
    if (index >= FE_MAX_NUM)
    {
        return FE_ERR_BAD_PARAMETER;
    }
    
    error = FE_ControlFeReset(index, FE_RESET_PULSE);
    if (error != FE_OK)
    {
        return error;
    }
    
    return FE_OK;
}
*/
/*!
*******************************************************************************
**
** \brief Tune to TP given by pTuneParams.
**        1. Initialize demod .
**        2. Set RF parameter.
**        3. Set Bandwidth.
**        4. channel setting
**        5. Demod lock and MPEG lock.
**        
** \param index         index of FE
** \param pTuneParams   tune parameter.
**
*******************************************************************************
*/
static int32_t A6211_Tune(uint8_t index, uint32_t uiFrequency_kHz, uint32_t SymbolRate_kHz)
{
   // FE_SYSTEM_PARAMS_S*     inst;
    //uint32_t                ulIFFreq;
    //uint32_t                LNBlowfreq;
    //uint32_t                LNBhighfreq;
    //uint8_t                 cnt;
    AVL_DVBSx_ErrorCode         error=AVL_DVBSx_EC_OK;
    struct AVL_DVBSx_Channel    Channel;
   // uint32_t                signal_symbolrate = 0;
	AVL_uint16 uiCounter;
	AVL_uint16 uiLockStatus;
#ifdef TS_EXTERN
 return FE_OK;
#endif    
    if( index >= FE_MAX_NUM )
    {
        return FE_ERR_BAD_PARAMETER;
    }
	#if 0
    SET_I2C;

   // inst        = &pData[index]->pSystemParams;
    LNBlowfreq  = inst->sat.lowFrequency;
    LNBhighfreq =inst->sat.highFrequency;
    
    if (LNBlowfreq  == 0)
    {
        LNBlowfreq  = LNBhighfreq;
    }
    if (LNBhighfreq == 0)
    {
        LNBhighfreq = LNBlowfreq;
    }
    //error = FE_SetSignalRoute(pData[index],pTuneParams);
    //if(error!=FE_OK)
    //{
        //return FE_ERR_FE_TUNE;
    //}
    
    if( inst->sat.diseqc != FE_DISEQC_1_1_SMATV)
    {
        if(labs(pTuneParams->sat.frequency - LNBlowfreq) < FE_BAND_SW)
        {
            /* low band ok */
            ulIFFreq = labs(pTuneParams->sat.frequency - LNBlowfreq);
        }
        else
        {
            /* using high band */
            ulIFFreq = labs(pTuneParams->sat.frequency - LNBhighfreq);
        }
    }
    else
    {
        ulIFFreq = labs(pTuneParams->sat.ifFrequency * 1000);
    }
	if(ulIFFreq<950*1000 || pTuneParams->sat.symbolRate<2*1000)
	{
		return FE_ERR_BAD_PARAMETER;
	}

 //   FAPI_SYS_PRINT_MSG( "[A6211_Tune]  diseqc: %d\n",inst->sat.diseqc);	
    if( inst->sat.diseqc == FE_DISEQC_1_1_SMATV)
    {
        FE_Delay(50);
        if(pData[index]->feControlLnbVoltage != NULL)
        {
            error = pData[index]->feControlLnbVoltage(pData[index]->slotNum,FE_LNB_13V);
            if(error != FE_OK)
            {
                return error;
            }
        }
    }
	#endif
	AVL_DVBSx_IBSP_Delay(50);
	error= AVL_DVBSx_IBase_SetFunctionalMode(&a6211Hdl[index], AVL_DVBSx_FunctMode_Demod);
	if (error != AVL_DVBSx_EC_OK)
	{
		printf("AVL_DVBSx_IBase_SetFunctionalMode r:%d\n",error);
	 	return FE_ERR_FE_TUNE;
	}
	error= AVL_DVBSx_ITuner_CalculateLPF((AVL_uint32)(SymbolRate_kHz / 10), &tunerHdl[index]);
	tunerHdl[index].m_uiFrequency_100kHz = uiFrequency_kHz /100;
	tunerHdl[index].m_uiSymbolRate_Hz = SymbolRate_kHz*1000;
	//tunerHdl[index].m_uiLPF_100kHz = tuner_LPF;
	FAPI_SYS_PRINT_MSG( "[A6211_Tune] channel:%d, Freq:%d 100Khz,sym:%d hz,m_uiLPF_100kHz:%d\n",
		index,tunerHdl[index].m_uiFrequency_100kHz,tunerHdl[index].m_uiSymbolRate_Hz,tunerHdl[index].m_uiLPF_100kHz);
	error = tunerHdl[index].m_pLockFunc(&tunerHdl[index]);
    if(error != AVL_DVBSx_EC_OK)
    {
        FAPI_SYS_PRINT_MSG( "error lock %d\n",error);
        return (error);
    }
    AVL_DVBSx_IBSP_Delay(50); //Wait a while for tuner to lock in certain frequency.
    
    error = tunerHdl[index].m_pGetLockStatusFunc(&tunerHdl[index]);
    if((error != AVL_DVBSx_EC_OK) && (error != AVL_DVBSx_EC_Running) )
    {
        FAPI_SYS_PRINT_MSG( "error check lock %d\n",error);
        return (error);
    }
    Channel.m_uiFrequency_kHz = 0; /* zero IF*/
    Channel.m_uiSymbolRate_Hz = SymbolRate_kHz * 1000; //Change the value defined by macro when we want to lock a new channel.
    Channel.m_Flags = (CI_FLAG_IQ_NO_SWAPPED) << CI_FLAG_IQ_BIT; //Normal IQ
    Channel.m_Flags |= (CI_FLAG_IQ_AUTO_BIT_AUTO) << CI_FLAG_IQ_AUTO_BIT; //Enable automatic IQ swap detection
    Channel.m_Flags |= (CI_FLAG_DVBS2_UNDEF) << CI_FLAG_DVBS2_BIT; //Enable automatic standard detection
    
    error = AVL_DVBSx_IRx_LockChannel(&Channel, &a6211Hdl[index]);
    if (error != AVL_DVBSx_EC_OK)
    {
    	printf("AVL_DVBSx_IRx_LockChannel r:%d\n",error);
        return FE_ERR_FE_TUNE;
    }

	
	if(Channel.m_uiSymbolRate_Hz < 5000000)
		uiCounter = 25;
	else if(Channel.m_uiSymbolRate_Hz < 10000000)
		uiCounter = 12;
	else
		uiCounter = 5;
	do
	{
		AVL_DVBSx_IBSP_Delay(100);    //Wait 100ms for demod to lock the channel.

		error = AVL_DVBSx_IRx_GetLockStatus(&uiLockStatus, &a6211Hdl[index]);

		if ((AVL_DVBSx_EC_OK == error)&&(1 == uiLockStatus)){
			
			break;
		}
	}while(--uiCounter);
	printf("lock status:%d\n",uiLockStatus);
    //FE_SetTuneTimeout(index, pTuneParams->sat.symbolRate);
    
    return error;
}

/*!
*******************************************************************************
**
** \brief Get Demod status.
**
** \param index     index of FE
** \param pStatus   store the status value.
**
*******************************************************************************
*/
static int32_t A6211_GetStatus(uint8_t index, FE_STATUS_E* pStatus)
{
    uint16_t        lock_state = 0;
  #ifdef TS_EXTERN
  *pStatus = FE_STATUS_LOCKED;
 return FE_OK;
#endif  
    if( (index >= FE_MAX_NUM) ||
        (pStatus == NULL) )
    {
        return FE_ERR_BAD_PARAMETER;
    }
    
    //SET_I2C;
    
    AVL_DVBSx_IRx_GetLockStatus(&lock_state, &a6211Hdl[index]);
    if(lock_state == 1)
    {
    //   FAPI_SYS_PRINT_MSG("[A6211_GetStatus] channel:%d lock.\n",index);
        *pStatus = FE_STATUS_LOCKED;
    }
    else
    {
    	//FAPI_SYS_PRINT_MSG("[A6211_GetStatus] LOCKing....\n");
        if(0/* FE_IsTuneTimeout(index) == 0*/ )
        {
            *pStatus = FE_STATUS_TUNING;
        }
        else
        {
       //  FAPI_SYS_PRINT_MSG("[A6211_GetStatus] channel:%d unlock.\n",index);
            *pStatus = FE_STATUS_NOT_LOCKED;
        }
    }
    
    return FE_OK;
}

/*!
*******************************************************************************
**
** \brief This function get current AFC value
** 
** \param index     index of FE
** \param pAFC      pointer to write AFC value.
**
*******************************************************************************
*/
static int32_t A6211_GetAFC(uint8_t index, int32_t* pAFC)
{
    return FE_OK;
}

/*!
*******************************************************************************
**
** \brief This function get current BER value
** 
** \param index     index of FE
** \param pBER      pointer to write BER value.
**
*******************************************************************************
*/
static int32_t A6211_GetBER(uint8_t index, uint32_t* pBER)
{
    AVL_DVBSx_ErrorCode error = AVL_DVBSx_EC_OK;
    FE_STATUS_E     status = FE_STATUS_UNKNOWN;
    uint32_t        uiBER = 0;
    #ifdef TS_EXTERN
     *pBER = uiBER;
 return AVL_DVBSx_EC_OK;
#endif   
    if( (index >= FE_MAX_NUM) ||
        (pBER == NULL) )
    {
        return FE_ERR_BAD_PARAMETER;
    }
    
    //SET_I2C;
    
    *pBER = 0;
    error = A6211_GetStatus(0,&status);
    if( (error  != AVL_DVBSx_EC_OK) ||
        (status != FE_STATUS_LOCKED) )
        
    {
        return FE_ERR_FE_NOT_LOCKED;
    }
    //This function can be called to read back the current BER calculation result after function A6211_IDVBSxRx_ResetErrorStat called.
    error = AVL_DVBSx_IRx_GetDVBSBER(&uiBER, &a6211Hdl[index]);
    if (error != AVL_DVBSx_EC_OK)
    {
        return (error);
    }
    *pBER = uiBER;
    
    return FE_OK;
}

/*!
*******************************************************************************
**
** \brief Get signal Quality
** 
** \param index     index of FE
** \param pQuality  pointer to write quality value.
**
*******************************************************************************
*/
static int32_t A6211_GetSignalQuality(uint8_t index, uint16_t* pQuality)
{
    struct AVL_DVBSx_SignalInfo    SignalInfo;
    uint32_t                   uiSNR;
    AVL_DVBSx_ErrorCode            error = AVL_DVBSx_EC_OK;
    uint8_t                    SNRrefer = 0;
    uint8_t DVBS_SNR[6]      ={12,32,41,52,58,62};
    uint8_t DVBS2Qpsk_SNR[8] ={10,24,32,41,47,52,63,65};
    uint8_t DVBS28psk_SNR[6] ={57,67,80,95,10,11};
     #ifdef TS_EXTERN
        *pQuality = 98;
	 *pQuality = (*pQuality)*655;
 return AVL_DVBSx_EC_OK;
#endif      
    if( (index >= FE_MAX_NUM) ||
        (pQuality == NULL) )
    {
        return FE_ERR_BAD_PARAMETER;
    }
    
    //SET_I2C;
    
    *pQuality = 0;
    error = AVL_DVBSx_IRx_GetSNR(&uiSNR, &a6211Hdl[index]);
    if(error != AVL_DVBSx_EC_OK)
    {
        return (error);
    }
    error = AVL_DVBSx_IRx_GetSignalInfo(&SignalInfo, &a6211Hdl[index]);
    if(error != AVL_DVBSx_EC_OK)
    {
        return (error);
    }
    if (SignalInfo.m_coderate < RX_DVBS2_1_4)
    {
        SNRrefer = DVBS_SNR[SignalInfo.m_coderate];
    }
    else
    {
         if (SignalInfo.m_modulation == AVL_DVBSx_MM_8PSK)
         {
             SNRrefer = DVBS28psk_SNR[SignalInfo.m_coderate - RX_DVBS2_3_5];
         }
         else
         {
             SNRrefer = DVBS2Qpsk_SNR[SignalInfo.m_coderate -RX_DVBS2_1_2];
         }
    }
	/* 以下就是大概运算百分比的判断 */
    if ((uiSNR/10) > SNRrefer)
    {
        uiSNR = uiSNR/10 - SNRrefer;
        if(uiSNR>=100)
            *pQuality = 99;
        else if(uiSNR>=50)  //  >5.0dB
            *pQuality = 80+ (uiSNR-50)*20/50;
        else if(uiSNR>=25)  //  > 2.5dB
            *pQuality = 50+ (uiSNR-25)*30/25;
        else if(uiSNR>=10)  //  > 1dB
            *pQuality = 25+ (uiSNR-10)*25/15;
        else 
            *pQuality = 5+ (uiSNR)*20/10;
    }
    else
    {
        *pQuality = 5;
    }
/*
    if ((uiSNR/10) > SNRrefer)
    {
         *pQuality = (uiSNR/10) - SNRrefer;
         if (*pQuality >= 100)
         {
            *pQuality = 98;
         }
    }
    else
    {
        *pQuality = 5;
    }
    
    *pQuality = (*pQuality)*655;
*/
    return FE_OK;
}

/*!
*******************************************************************************
**
** \brief Get signale strength value.
** 
** \param index     index of FE
** \param pStrength pointer to write strength value.
**
*******************************************************************************
*/
static int32_t A6211_GetSignalStrength(uint8_t index, uint16_t* pStrength)
{
    AVL_DVBSx_ErrorCode error = AVL_DVBSx_EC_OK;
    FE_STATUS_E     status = FE_STATUS_UNKNOWN;
    uint16_t        uiRFSignalLevel = 0;
    int16_t        uiRFSignalDB = 0;
    uint16_t        cnt;


//	A2108_TestSinalLevel();
//	 return FE_OK;
	 
      #ifdef TS_EXTERN
   *pStrength = 862;
    
 return AVL_DVBSx_EC_OK;
#endif     
    if(index >= FE_MAX_NUM)
    {
        return FE_ERR_BAD_PARAMETER;
    }
    if(pStrength == NULL)
    {
        return FE_ERR_BAD_PARAMETER;
    }
    *pStrength = 0;
    
    //SET_I2C;
    
    error = A6211_GetStatus(index,&status);
    if( ( error  != AVL_DVBSx_EC_OK) ||
        ( status != FE_STATUS_LOCKED) )
    {
        return (error);
    }
    
    error = AVL_DVBSx_IRx_GetSignalLevel(&uiRFSignalLevel, &a6211Hdl[index]);
    if (error != AVL_DVBSx_EC_OK)
    {
        return (error);
    }
    for(cnt=0; cnt<SignalLevelLUT_SIZE; cnt++) // TODO Why 47 ???
    {
        if(uiRFSignalLevel<= SignalLevelLUT[cnt].SignalLevel)
        {
            uiRFSignalDB = SignalLevelLUT[cnt].SignalDBM;
            break;
        }
    }
	if(uiRFSignalDB <= -70)
		 *pStrength = 0;
	else if(uiRFSignalDB < -60)
			 *pStrength = 30 + (70+uiRFSignalDB) * 1.5;
	else if(uiRFSignalDB < -50 )
		*pStrength = 45 + (60 + uiRFSignalDB) * 1.5;
	else if(uiRFSignalDB < -40 )
		*pStrength = 60 + (50+uiRFSignalDB )*1.5;
	else if(uiRFSignalDB < -30 )
		*pStrength = 75 + (40+uiRFSignalDB)*1.5;
	else 
		*pStrength = 100 + (uiRFSignalDB/3);
#if 0
    *pStrength = (70+uiRFSignalDB)*150/70;
	if(*pStrength > 100)
		*pStrength =98;
#endif
//     FAPI_SYS_PRINT_MSG("uiRFSignalDB:%d,pStrength:%d\n",uiRFSignalDB,*pStrength);   
    return FE_OK;
}

/*!
*******************************************************************************
**
** \brief Get tune parameters.
** 
** \param index         index of FE
** \param pTuneParams   pointer to write TuneParams value.
**
*******************************************************************************
*/
/*
static int32_t A6211_GetTuneParams(uint8_t index, FE_TUNE_PARAMS_S *pTuneParams)
{
    if(index >= FE_MAX_NUM)
    {
        return FE_ERR_BAD_PARAMETER;
    }
    pTuneParams = pTuneParams;
    return FE_OK;
}
*/
/*!
*******************************************************************************
**
** \brief Sending a DiSEqC command
** 
** \param index         index of FE
** \param len           command lenght
** \param pCommand      command buffer
** \param pRetLen       receive lenght (optional)
** \param pRetBuf       receive buffer (optional)
**
*******************************************************************************
*/
static void A6211_Motor_test(uint8_t index)
{
static uint8_t position=0;
   AVL_DVBSx_ErrorCode                error = AVL_DVBSx_EC_OK;
    struct AVL_DVBSx_Diseqc_TxStatus   sTxStatus;
  uint8_t u8Cmd[]={0xE0,0x31,0x6B,00};
  position++;
  if(position>20)
  	position=0;
  
  u8Cmd[3]=position;
  FAPI_SYS_PRINT_MSG("[A6211_Motor_test] u8Cmd[3]:%d\n",u8Cmd[3]);
     error =AVL_DVBSx_IDiseqc_SendModulationData(u8Cmd,4,&a6211Hdl[index]);
  if(error != AVL_DVBSx_EC_OK)
    {
      FAPI_SYS_PRINT_MSG("[A6211_Motor_test] err 11..\n");
        return ;
    }
    else
    {
        do
        {
            error = AVL_DVBSx_IDiseqc_GetTxStatus(&sTxStatus, &a6211Hdl[index]); //Get current status of the Diseqc transmitter data FIFO.
        }
        while(1 != sTxStatus.m_TxDone); //Wait until operation finished.
        if(error != AVL_DVBSx_EC_OK)
        {
            // TODO error Handling
              FAPI_SYS_PRINT_MSG("[A6211_Motor_test] err 22..\n");
        }
    }	 
	AVL_DVBSx_IBSP_Delay(1000);
	
}
/*
static int32_t A6211_SendDiseqcCommand(uint8_t index,uint8_t len,uint8_t *pCommand,
                                       uint8_t *pRetLen,uint8_t *pRetBuf)
{
    AVL_DVBSx_ErrorCode                error = AVL_DVBSx_EC_OK;
    struct AVL_DVBSx_Diseqc_TxStatus   sTxStatus;
	
#ifdef TS_EXTERN
 return AVL_DVBSx_EC_OK;
#endif  

//  FAPI_SYS_PRINT_MSG("[A6211_SendDiseqcCommand]  is call,pCommand:\n");


    if( (index >= FE_MAX_NUM) ||
        (pCommand == NULL) )
    {
       FAPI_SYS_PRINT_MSG("[A6211_SendDiseqcCommand]  err\n");
        return FE_ERR_BAD_PARAMETER;
    }
   	{
		int i=0;
		for(i=0;i<len;i++)
			{
//    FAPI_SYS_PRINT_MSG("0x%x,",pCommand[i]);

}
//printf("\n");

		}
	//SET_I2C;
       
    error = AVL_DVBSx_IDiseqc_SendModulationData(pCommand,len,&a6211Hdl[index]);
    if(error != AVL_DVBSx_EC_OK)
    {
        return (error);
    }
    else
    {
        do
        {
            error = AVL_DVBSx_IDiseqc_GetTxStatus(&sTxStatus, &a6211Hdl[index]); //Get current status of the Diseqc transmitter data FIFO.
        }
        while(1 != sTxStatus.m_TxDone); //Wait until operation finished.
        if(error != AVL_DVBSx_EC_OK)
        {
            // TODO error Handling
        }
    }
    //TODO handle return msg DiSEqC 2.x
//      FAPI_SYS_PRINT_MSG("[A6211_SendDiseqcCommand] finish..\n");
    return FE_OK;
}
*/
int32_t A6211_Control22kHz(uint8_t index, FE_22KHZ_E mode)
{
    int32_t error = FE_OK;
      #ifdef TS_EXTERN
    
 return AVL_DVBSx_EC_OK;
#endif        


    //SET_I2C;
	
//        FAPI_SYS_PRINT_MSG("[A6211_Control22kHz]  mode: %d\n",mode);
 
    switch(mode)
    {
        case FE_22KHZ_OFF:
            error = AVL_DVBSx_IDiseqc_StopContinuous(&a6211Hdl[index]);
            break;
        case FE_22KHZ_ON:
            error = AVL_DVBSx_IDiseqc_StartContinuous(&a6211Hdl[index]);
            break;
        case FE_22KHZ_TONE_0:
            error = AVL_DVBSx_IDiseqc_SendTone(0,1,&a6211Hdl[index]);
            break;
        case FE_22KHZ_TONE_1:
            error = AVL_DVBSx_IDiseqc_SendTone(1,1,&a6211Hdl[index]);
            break;
        default:
            error = FE_ERR_FEATURE_NOT_SUPPORTED;
    }
    return error;
}

AVL_DVBSx_ErrorCode A8293_Control0( FE_LNB_VOLTAGE_E LNB_LEVEL)  //linjian         2011-3-15
{
 AVL_DVBSx_ErrorCode r;
 struct AVL_DVBSx_Chip LNB_Chip;
 AVL_uint16 uiSize = 1;
 AVL_uchar uiData = 0x10;
 LNB_Chip.m_SlaveAddr = 0x10;  //A8293   address
 
 r = AVL_DVBSx_IBSP_I2CRead(&LNB_Chip, &uiData, &uiSize);
 if(LNB_LEVEL == FE_LNB_18V)//Horizontal
 {
  FAPI_SYS_PRINT_MSG("18V\n");
  uiData = 0x38;//0x38;//0x3d
 }
 else if(LNB_LEVEL == FE_LNB_13V)  //Vertical
 {
  FAPI_SYS_PRINT_MSG("13V\n");
  uiData =0x35;// 0x35;//0x34;//0x37
 }
 else if(LNB_LEVEL == FE_LNB_OFF)  //LNB supply off
 {
  FAPI_SYS_PRINT_MSG("LNB POWER OFF\n");
  uiData = 0x10;
 }
 r |= AVL_DVBSx_IBSP_I2CWrite(&LNB_Chip, &uiData, &uiSize);
 uiData = 0x82;
 r |= AVL_DVBSx_IBSP_I2CWrite(&LNB_Chip, &uiData, &uiSize); //Open Ruby 22K to LNB supply
 return r;
}
 
AVL_DVBSx_ErrorCode A8293_Control1( FE_LNB_VOLTAGE_E LNB_LEVEL)  //linjian         2011-3-15
{
 AVL_DVBSx_ErrorCode r;
 struct AVL_DVBSx_Chip LNB_Chip;
 AVL_uint16 uiSize = 1;
 AVL_uchar uiData = 0x10;
 LNB_Chip.m_SlaveAddr = 0x16;  //A8293   address
 
 r = AVL_DVBSx_IBSP_I2CRead(&LNB_Chip, &uiData, &uiSize);
 if(LNB_LEVEL == FE_LNB_18V)//Horizontal
 {
  FAPI_SYS_PRINT_MSG("18V\n");
  uiData = 0x38;//0x38;//0x3d
 }
 else if(LNB_LEVEL == FE_LNB_13V)  //Vertical
 {
  FAPI_SYS_PRINT_MSG("13V\n");
  uiData =0x35;// 0x35;//0x34;//0x37
 }
 else if(LNB_LEVEL == FE_LNB_OFF)  //LNB supply off
 {
  FAPI_SYS_PRINT_MSG("LNB POWER OFF\n");
  uiData = 0x10;
 }
 r |= AVL_DVBSx_IBSP_I2CWrite(&LNB_Chip, &uiData, &uiSize);
 uiData = 0x82;
 r |= AVL_DVBSx_IBSP_I2CWrite(&LNB_Chip, &uiData, &uiSize); //Open Ruby 22K to LNB supply
 return r;
}
AVL_DVBSx_ErrorCode DemodPin_LNB_V_H_Control(  struct AVL_DVBSx_Chip * pAVLChip,FE_LNB_VOLTAGE_E LNB_LEVEL)  //linjian         2011-3-15
{
	AVL_DVBSx_ErrorCode r=AVL_DVBSx_EC_OK;

	if(LNB_LEVEL == FE_LNB_18V)//Horizontal
	{
		FAPI_SYS_PRINT_MSG("18V\n");
		r=AVL_DVBSx_IDiseqc_SetLNBOut(1, pAVLChip);
	}
	else if(LNB_LEVEL == FE_LNB_13V)  //Vertical
	{
		FAPI_SYS_PRINT_MSG("13V\n");
		r=AVL_DVBSx_IDiseqc_SetLNBOut(0, pAVLChip);
	}
	else if(LNB_LEVEL == FE_LNB_OFF)  //LNB supply off
	{
		FAPI_SYS_PRINT_MSG("LNB POWER OFF not Control by Demod....Pls Check you SCH.\n");
	}

	return r;
}

int32_t A6211_ControlLnbVoltage(uint8_t index, FE_LNB_VOLTAGE_E mode)
{

  
    int32_t error = FE_OK;
       #ifdef TS_EXTERN
 return AVL_DVBSx_EC_OK;
#endif       
    //SET_I2C;

//         FAPI_SYS_PRINT_MSG("[A6211_ControlLnbVoltage] index:%d, mode: %d\n",index,mode);

    
	    switch(mode)
	    {
	        case FE_LNB_13V:
				#if 0
	        //   error =  AVL_DVBSx_IDiseqc_SetLNBOut(1, &a6211Hdl[index] );
	        if (hp->i2cAddrDemod  ==0x18)
	             error=A8293_Control0(  mode);
		  if (hp->i2cAddrDemod  ==0x1a)
		  	error=A8293_Control1(  mode);
		  #else
		  DemodPin_LNB_V_H_Control(&a6211Hdl[index],FE_LNB_13V);
		  #endif
	            break;
	        case FE_LNB_18V:
				#if 0
	         //   error =  AVL_DVBSx_IDiseqc_SetLNBOut(0, &a6211Hdl[index] );
               if (hp->i2cAddrDemod  ==0x18)
	             error=A8293_Control0(  mode);
		if(hp->i2cAddrDemod  ==0x1a)
		    error=A8293_Control1(  mode);
		    	#else
				DemodPin_LNB_V_H_Control(&a6211Hdl[index],FE_LNB_18V);
				#endif
	            break;
	        default:
	            error = FE_ERR_FEATURE_NOT_SUPPORTED;
	    }
	    	
	
		
    return error;
}

static int32_t A6211_Initialize(struct AVL_DVBSx_Chip  *pAVLChip, struct AVL_Tuner * pTuner)
{
    struct AVL_DVBSx_Diseqc_Para sDiseqcPara;
    struct AVL_DVBSx_MpegInfo sMpegMode;
    AVL_DVBSx_ErrorCode  r=AVL_DVBSx_EC_OK;


#ifdef TS_EXTERN
 return AVL_DVBSx_EC_OK;
#endif


    if( ( pAVLChip == NULL) ||
        ( pTuner     == NULL ) )
    {
        return FE_ERR_BAD_PARAMETER;
    }
    // This function should be called after bsp initialized to initialize the chip object.
	if(pAVLChip->m_uiBusId == 0)
		r = Init_AVL_DVBSx_ChipObject(pAVLChip, AVL_DVBSx_SA_0<<1);	
	else
		r = Init_AVL_DVBSx_ChipObject(pAVLChip, AVL_DVBSx_SA_1<<1);	
	if( AVL_DVBSx_EC_OK !=r ) 
	{
		return (r);
	}
	
    //This function should be called after chip object initialized to initialize the IBase,using reference clock as 10M. Make sure you pickup the right pll_conf since it may be modified in BSP.
#ifdef TUNER_RDA_AV2020_DUL
	r = AVL_DVBSx_IBase_Initialize((pll_conf+4), AVL6211_ucPatchData, pAVLChip); 
#elif defined(TUNER_ALLO_AV2011)
	r = AVL_DVBSx_IBase_Initialize((pll_conf+4), AVL6211_ucPatchData, pAVLChip); 
#elif defined(TUNER_ALLO_AV2012)
	r = AVL_DVBSx_IBase_Initialize((pll_conf+4), AVL6211_ucPatchData, pAVLChip); 
#else
	r = AVL_DVBSx_IBase_Initialize((pll_conf+2), AVL6211_ucPatchData, pAVLChip); 
#endif
	if( AVL_DVBSx_EC_OK !=r ) 
	{
		FAPI_SYS_PRINT_MSG("IBase Initialization failed:%d !\n",r);
		return (r);
	}
	else
		FAPI_SYS_PRINT_MSG("IBase Initialization ok !\n");

	AVL_DVBSx_IBSP_Delay(100);	  //Wait 100 ms to assure that the AVL_DVBSx chip boots up.This function should be implemented by customer.
  
	//This function should be called to verify the AVL_DVBSx chip has completed its initialization procedure.
    r = AVL_DVBSx_IBase_GetStatus(pAVLChip);
	if( AVL_DVBSx_EC_OK != r )       
	{
		FAPI_SYS_PRINT_MSG("Booted failed:%d !\n",r);
	//	return (r);
	}

	FAPI_SYS_PRINT_MSG("Booted !\n");
    //This function should be called after IBase initialized to initialize the demod.
	r = AVL_DVBSx_IRx_Initialize(pAVLChip);
	if(AVL_DVBSx_EC_OK != r)
	{
		FAPI_SYS_PRINT_MSG("Demod Initialization failed !\n");
		return (r);
	}
	
	//modify rx_config.aagc_ref
	r |= AVL_DVBSx_II2C_Write16(pAVLChip, rc_aagc_ref_addr, 48);
	r |= AVL_DVBSx_IRx_DriveRFAGC(pAVLChip);


	//This function should be called after demod initialized to set RF AGC polar.
	//User does not need to setup this for Sharp tuner since it is the default value. But for other tuners, user may need to do it here.
	r = AVL_DVBSx_IRx_SetRFAGCPola(AVL_DVBSx_RA_Invert, pAVLChip);
	if(AVL_DVBSx_EC_OK != r)
	{
		FAPI_SYS_PRINT_MSG("Set RF AGC Polar failed !\n");
		return (r);
	}

	//Setup MPEG mode parameters.
	sMpegMode.m_MpegFormat = AVL_DVBSx_MPF_TS;
	sMpegMode.m_MpegMode = AVL_DVBSx_MPM_Parallel;
	sMpegMode.m_MpegClockPolarity =AVL_DVBSx_MPCP_Falling;
	
	//This function should be called after demod initialized to set MPEG mode.
	r = AVL_DVBSx_IRx_SetMpegMode(&sMpegMode,pAVLChip );
	if(AVL_DVBSx_EC_OK != r)
	{
		FAPI_SYS_PRINT_MSG("Set MPEG output mode failed !\n");
		return (r);
	}

	
    	r = AVL_DVBSx_IRx_SetMpegValidPolarity(pAVLChip,AVL_DVBSx_MPVP_Normal);
	if(AVL_DVBSx_EC_OK != r)
	{
		FAPI_SYS_PRINT_MSG("Set MPEG Valid Polarity failed !\n");
		return (r);
	}
	r = AVL_DVBSx_IRx_SetMpegErrorPolarity(pAVLChip, AVL_DVBSx_MPEP_Normal, AVL_DVBSx_MPEP_Normal);
	if(AVL_DVBSx_EC_OK != r)
	{
		FAPI_SYS_PRINT_MSG("Set MPEG Error Polarity failed !\n");
		return (r);
	}
	r = AVL_DVBSx_IRx_SetMpegBitOrder(pAVLChip, AVL_DVBSx_MPM_Parallel, AVL_DVBSx_MPBO_Normal);
	if(AVL_DVBSx_EC_OK != r)
	{
		FAPI_SYS_PRINT_MSG("Set MPEG BitOrder failed !\n");
		return (r);
	}
	// Enable the MPEG output (this function call has no effect for the AVL_DVBSxLG and AVL_DVBSxLGa devices)
	r = AVL_DVBSx_IRx_DriveMpegOutput(pAVLChip);
	if(AVL_DVBSx_EC_OK != r)
	{
		FAPI_SYS_PRINT_MSG("AVL_DVBSx_IRx_DriveMpegOutput failed (r:%d)!\n",r);
		return (r);
	}
	//Setup tuner parameters for tuner initialization.
#ifdef TUNER_RDA_AV2020_DUL
	if(pAVLChip->m_uiBusId == 0)
		pTuner->m_uiSlaveAddress = 0xc6;    
	else
		pTuner->m_uiSlaveAddress = 0xc0;    
	
	pTuner->m_pInitializeFunc = &AVL_DVBSx_ExtAV2020_Initialize;     
	pTuner->m_pLockFunc = &AVL_DVBSx_ExtAV2020_Lock; 
	pTuner->m_pGetLockStatusFunc = &AVL_DVBSx_ExtAV2020_GetLockStatus;	

#endif
#ifdef TUNER_ALLO_AV2011	
	pTuner->m_uiSlaveAddress = 0xc0;    	
	pTuner->m_pInitializeFunc = &AVL_DVBSx_ExtAV2011_Initialize;     
	pTuner->m_pLockFunc = &AVL_DVBSx_ExtAV2011_Lock; 
	pTuner->m_pGetLockStatusFunc = &AVL_DVBSx_ExtAV2011_GetLockStatus;	

#endif
#ifdef TUNER_ALLO_AV2012	
	pTuner->m_uiSlaveAddress = 0xc6;    	
	pTuner->m_pInitializeFunc = &AVL_DVBSx_ExtAV2011_Initialize;     
	pTuner->m_pLockFunc = &AVL_DVBSx_ExtAV2011_Lock; 
	pTuner->m_pGetLockStatusFunc = &AVL_DVBSx_ExtAV2011_GetLockStatus;	

#endif
#ifdef TUNER_SHARP_7801
	pTuner->m_uiSlaveAddress = 0xc0;  
	pTuner->m_pInitializeFunc = &ExtSharpBS2S7VZ7801_Initialize;     
	pTuner->m_pLockFunc = &ExtSharpBS2S7VZ7801_Lock; 
	pTuner->m_pGetLockStatusFunc = &ExtSharpBS2S7VZ7801_GetLockStatus;
#endif
	pTuner->m_uiI2CBusClock_kHz = tuner_I2Cbus_clock;      	
    pTuner->m_pParameters = 0;          
    pTuner->m_pAVLChip = pAVLChip;    
	


	//This function should be called after IBase initialized to initialize the tuner.
	r = pTuner->m_pInitializeFunc(pTuner);
	if(AVL_DVBSx_EC_OK != r)
	{
		FAPI_SYS_PRINT_MSG("Tuner Initialization failed !\n");
		return (r);
	}

	//Setup Diseqc parameters for Diseqc initialization.
	sDiseqcPara.m_RxTimeout = AVL_DVBSx_DRT_210ms;
	sDiseqcPara.m_RxWaveForm = AVL_DVBSx_DWM_Normal;
	sDiseqcPara.m_ToneFrequency_kHz = 22;		
	sDiseqcPara.m_TXGap = AVL_DVBSx_DTXG_15ms;
	sDiseqcPara.m_TxWaveForm = AVL_DVBSx_DWM_Normal;

	//The Diseqc should be initialized if AVL_DVBSx need to supply power to LNB. This function should be called after IBase initialized to initialize the Diseqc.
	r = AVL_DVBSx_IDiseqc_Initialize(&sDiseqcPara, pAVLChip);
	if(AVL_DVBSx_EC_OK != r)
	{
		FAPI_SYS_PRINT_MSG("Diseqc Initialization failed !\n");
	}
    return (r);
}

/*!
*******************************************************************************
**
** \brief   BlindScan A6211.
**
** \param   index           Unused here.
** \param   pScanParams     Scan params.
**
*******************************************************************************
*/
#if 0
static int32_t A6211_Scan(uint8_t index, FE_SCAN_PARAMS_S *pScanParams)
{
    struct  AVL_DVBSx_BlindScanPara  bsPara;
    struct AVL_DVBSx_BlindScanInfo  bsInfo;
    struct AVL_DVBSx_Channel        channel[120];
    uint16_t                    currChannel = 0;
    uint16_t                    channelCnt = 0;
    AVL_uint16                  uiNextStartFreq_100kHz;
    AVL_uint16                  uiMaxLPF;
    AVL_char                    ScanFlag;
    AVL_DVBSx_ErrorCode             error = AVL_DVBSx_EC_OK;
    FE_TUNE_PARAMS_S            foundTuneParams;
    FE_TUNE_PARAMS_S            tuneParams;
    FE_SYSTEM_PARAMS_S*         inst;
    uint8_t                     hiLowBand = 0;
    uint32_t                    startFrequency;
    uint32_t                    stopFrequency;
    uint32_t                    bandsw;
    uint8_t                     cnt;
	uint8_t 					blindScanCnt = 0;
	BLIND_SCAN_LNBTYPE			scanlnbtype;
      #ifdef TS_EXTERN
    
 return AVL_DVBSx_EC_OK;
#endif        
    SET_I2C;
    
    if( (&pData[index] == NULL) ||
        (pScanParams   == NULL) ||
        (pScanParams->startSymbolRate <  BLINDSCAN_MIN_SYMB) ||
        (pScanParams->stopSymbolRate  >  BLINDSCAN_MAX_SYMB)
      )
    {
        return FE_ERR_BAD_PARAMETER;
    } 
    inst = &pData[index]->pSystemParams;
    startFrequency  = pScanParams->startFrequency - inst->sat.lowFrequency;
    startFrequency /= 100;
    stopFrequency   = pScanParams->stopFrequency  - inst->sat.highFrequency;
    stopFrequency  /= 100;
	FAPI_SYS_PRINT_MSG("startFrequency :%d stopFrequency:%d	\n",startFrequency,stopFrequency);
    if( (startFrequency  <  (BLINDSCAN_MIN_FREQ/100)) ||
        (stopFrequency   >  (BLINDSCAN_MAX_FREQ/100)) )
    {
        return FE_ERR_BAD_PARAMETER;
    }
    memset(&tuneParams, 0 ,sizeof(tuneParams));
    tuneParams.sat.pol       = pScanParams->pol;
    tuneParams.sat.satID     = pScanParams->satID;
    
 
    //Setup blind scan parameters and start to perform blind scan.  
    bsPara.m_uiMinSymRate_kHz = pScanParams->startSymbolRate;
    bsPara.m_uiMaxSymRate_kHz = pScanParams->stopSymbolRate;
    error = AVL_DVBSx_II2C_Read16(&a6211Hdl[index], rc_tuner_max_LPF_100kHz_addr, &uiMaxLPF);     //Get the right LPF for blind scan to regulate the freq_step.
    
    uiNextStartFreq_100kHz = startFrequency;
    ScanFlag = 1;
    FAPI_SYS_PRINT_MSG("pScanParams->startFrequency :%d pScanParams->stopFrequency:%d   \n",pScanParams->startFrequency,pScanParams->stopFrequency);
    FAPI_SYS_PRINT_MSG("inst->sat.lowFrequency :%d inst->sat.highFrequency:%d   \n",inst->sat.lowFrequency,inst->sat.highFrequency);
    error = AVL_DVBSx_II2C_Write32(&a6211Hdl[index], rc_specinv_addr, 0);   //For sharp tuner do not invert the spectrum.But for other tuners, user may need to invert the spectrum.
    if (error != AVL_DVBSx_EC_OK)
    {
        FAPI_SYS_PRINT_MSG("Set IQ sawp status failed !\n");
        return error;
    }
    AVL_DVBSx_IBase_SetFunctionalMode( &a6211Hdl[index],AVL_DVBSx_FunctMode_BlindScan);
	AVL_DVBSx_IBlindScan_Reset(&a6211Hdl[index]);
    scanlnbtype = A6211_Blindsacn_lnbtype(inst->sat.highFrequency,inst->sat.lowFrequency,pScanParams->pol);

	switch(scanlnbtype){
		case BLIND_LNBFRE_CSINGLE:
			bandsw = 2150000;
			break;
		case BLIND_LNBFRE_CBAUD_H:
			bandsw = 1550000;
			break;
		case BLIND_LNBFRE_CBAUD_V:
			bandsw = 2150000;
			break;			
		case BLIND_LNBFRE_KUSINGLE:
			bandsw = 2150000;
			break;
		case BLIND_LNBFRE_KUBAUD_H:
			bandsw = 1950000;
			break;
		case BLIND_LNBFRE_KUBAUD_V:
			bandsw = 1950000;
			break;
	}
//	scanlnbtype = A6211_Blindsacn_lnbtype(inst->sat.highFrequency,inst->sat.lowFrequency,pScanParams->pol);
    while(ScanFlag)
    {
    	

//		bandsw = stopFrequency*100;
        if(uiNextStartFreq_100kHz >= (bandsw/100))
        {
            if(hiLowBand == 0)
            {
                if( (inst->sat.lowFrequency  == inst->sat.highFrequency) ||
                    (inst->sat.highFrequency == 0) )
                {
                    /* stop scanning if only one band is available */
                    break;
                }
                /* HI Band */
                hiLowBand = 1;	

				bandsw = 2150000;
                uiNextStartFreq_100kHz  = ( ( inst->sat.lowFrequency +
                                              bandsw) -
                                            inst->sat.highFrequency) /
                                          100;
                
                FAPI_SYS_PRINT_MSG("Hi BAND \n");
                (void)AVL_DVBSx_IBlindScan_Reset(&a6211Hdl[index]);
                currChannel = 0;
                memset(channel,0,sizeof(struct AVL_DVBSx_Channel)*120);
            }
            else
            {
                break;
            }
        }
        
        tuneParams.sat.frequency = uiNextStartFreq_100kHz*100;
		FAPI_SYS_PRINT_MSG("uiNextStartFreq_100kHz:%d\n",uiNextStartFreq_100kHz);
		#if 0
        if(hiLowBand == 0)
        {
            tuneParams.sat.frequency += inst->sat.lowFrequency;
        }
        else
        {
            tuneParams.sat.frequency += inst->sat.highFrequency;
        }
		#endif
		if(hiLowBand == 0){
				switch(scanlnbtype){
					case BLIND_LNBFRE_CSINGLE:
					case BLIND_LNBFRE_CBAUD_H:
					case BLIND_LNBFRE_CBAUD_V:
						tuneParams.sat.frequency = inst->sat.lowFrequency- tuneParams.sat.frequency;
						break;			
					case BLIND_LNBFRE_KUSINGLE: 				
					case BLIND_LNBFRE_KUBAUD_H:
					case BLIND_LNBFRE_KUBAUD_V:
						tuneParams.sat.frequency += inst->sat.lowFrequency;
						break;
				}
		}else {
				switch(scanlnbtype){
					case BLIND_LNBFRE_CSINGLE:
					case BLIND_LNBFRE_CBAUD_H:
					case BLIND_LNBFRE_CBAUD_V:
						tuneParams.sat.frequency = inst->sat.highFrequency- tuneParams.sat.frequency;
						break;			
					case BLIND_LNBFRE_KUSINGLE: 				
					case BLIND_LNBFRE_KUBAUD_H:
					case BLIND_LNBFRE_KUBAUD_V:
						tuneParams.sat.frequency += inst->sat.highFrequency;
						break;
				}
		}
        if(tuneParams.sat.frequency >= pScanParams->stopFrequency)
        {
            break;          //Do not need to do scan
        }
//		if(hiLowBand == 0 && tuneParams.sat.frequency > bandsw)
//		{
//			continue;
//		}
        //FAPI_SYS_PRINT_MSG("SCAN Freq: %8d  (next:%4d) \n",tuneParams.sat.frequency,uiNextStartFreq_100kHz/10);
        //error = FE_SetSignalRoute(pData[index],&tuneParams);
       // if(error != FE_OK)
        //{
            
        //}
        
        bsPara.m_uiStartFreq_100kHz = uiNextStartFreq_100kHz;    // Give the start scan frequency.
        //Sharp tuner has a maximum 34 MHz LPF, but only 32MHz is flat.
        //The scan frequency range is 640(32MHz*2*10),unit is 100KHz.
        bsPara.m_uiStopFreq_100kHz = bsPara.m_uiStartFreq_100kHz + 320*2; 
        
        if( (bsPara.m_uiStopFreq_100kHz) > bandsw/100)
        {
            bsPara.m_uiStopFreq_100kHz = bandsw/100;
        }
        
        //Setup the tuner before scan
        tunerHdl[index].m_uiFrequency_100kHz = (bsPara.m_uiStartFreq_100kHz + bsPara.m_uiStopFreq_100kHz)/2;  //Calculate the center frequency.
        tunerHdl[index].m_uiLPF_100kHz = 400;//tuner_LPF; 
        error = tunerHdl[index].m_pLockFunc(&tunerHdl[index]);   //Lock the tuner.
        if (error != AVL_DVBSx_EC_OK)
        {
            FAPI_SYS_PRINT_MSG("Tuner lock failed !\n");
            return error;
        }
        
        AVL_DVBSx_IBSP_Delay(150);      //wait a while for tuner to lock in certain frequency.
        error = tunerHdl[index].m_pGetLockStatusFunc(&tunerHdl[index]);    //Check the lock status of the tuner.
        
        if (error != AVL_DVBSx_EC_OK)
        {
                FAPI_SYS_PRINT_MSG("Tuner can not lock to %f MHz !!!\n", tunerHdl[index].m_uiFrequency_100kHz/10.0);
                break;
        }
        
        //This function should be called after tuner lock to perform a blind scan.
        error = AVL_DVBSx_IBlindScan_Scan(&bsPara, tunerHdl[index].m_uiLPF_100kHz,&a6211Hdl[index]);
        if (error != AVL_DVBSx_EC_OK)
        {
                FAPI_SYS_PRINT_MSG("Blind Scan failed !\n");
                    return error;
        }
        
        //Monitor the blind scan progress.
        do 
        {
            AVL_DVBSx_IBSP_Delay(100);     //wait 1 second
            error = AVL_DVBSx_IBlindScan_GetScanStatus(&bsInfo, &a6211Hdl[index]);  //Query the internal blind scan procedure information.
            if (error != AVL_DVBSx_EC_OK)
            {
                FAPI_SYS_PRINT_MSG("Get scan status failed !\n");
                return error;
            }
            if(pScanParams->feScanProgressFct != NULL)
            {
                pScanParams->feScanProgressFct(bsInfo.m_uiProgress);
            }
            if(pScanParams->feScanAbortFct != NULL)
            {
                if(pScanParams->feScanAbortFct() == 1)
                {
                    (void)AVL_DVBSx_IBlindScan_Cancel( &a6211Hdl[index]);
                    ScanFlag = 0;
                    break;
                }
            }
        }
        while(100 != bsInfo.m_uiProgress);  //100 indicates the blind scan is finished. 
        
        uiNextStartFreq_100kHz = bsInfo.m_uiNextStartFreq_100kHz;
        //The blind scan is finished now, read out the channel information
        
        if(pScanParams->feScanNotifyFct != NULL)
        {
            //The blind scan is finished now, read out the channel information
            if(currChannel < bsInfo.m_uiChannelCount)
            {
                /* read channels */
                channelCnt = bsInfo.m_uiChannelCount - currChannel;
                error = AVL_DVBSx_IBlindScan_ReadChannelInfo(currChannel, &channelCnt, channel, &a6211Hdl[index]);
                /* Notify the Application about new TP's */
                for(cnt=0;currChannel<bsInfo.m_uiChannelCount;cnt++,currChannel++)
                {
                    if((channel[cnt].m_uiSymbolRate_Hz/1000) < pScanParams->startSymbolRate)
                    {
                        continue;
                    }
                    FAPI_SYS_PRINT_MSG("Ch%2d: Freq=%4d MHz Sym=%5d k         ",
                                       currChannel,
                                       ((channel[cnt].m_uiFrequency_kHz)/1000),
                                       ((channel[cnt].m_uiSymbolRate_Hz)/1000));
                    memset(&foundTuneParams, 0,sizeof(FE_TUNE_PARAMS_S));
                    foundTuneParams.sat.frequency   = channel[cnt].m_uiFrequency_kHz;
					#if 0
                    if(hiLowBand == 0)
                    {
                        foundTuneParams.sat.frequency += inst->sat.lowFrequency;
                    }
                    else
                    {
                        foundTuneParams.sat.frequency += inst->sat.highFrequency;
                    }
					#endif
					if(hiLowBand == 0){
							switch(scanlnbtype){
								case BLIND_LNBFRE_CSINGLE:
								case BLIND_LNBFRE_CBAUD_H:
								case BLIND_LNBFRE_CBAUD_V:
									foundTuneParams.sat.frequency =inst->sat.lowFrequency - foundTuneParams.sat.frequency;
									break;			
								case BLIND_LNBFRE_KUSINGLE: 				
								case BLIND_LNBFRE_KUBAUD_H:
								case BLIND_LNBFRE_KUBAUD_V:
									foundTuneParams.sat.frequency += inst->sat.lowFrequency;
									break;
							}
					}else {
							switch(scanlnbtype){
								case BLIND_LNBFRE_CSINGLE:
								case BLIND_LNBFRE_CBAUD_H:
								case BLIND_LNBFRE_CBAUD_V:
									foundTuneParams.sat.frequency = inst->sat.highFrequency - foundTuneParams.sat.frequency;
									break;			
								case BLIND_LNBFRE_KUSINGLE: 				
								case BLIND_LNBFRE_KUBAUD_H:
								case BLIND_LNBFRE_KUBAUD_V:
									foundTuneParams.sat.frequency += inst->sat.highFrequency;
									break;
							}
					}

					
                    foundTuneParams.sat.frequency +=  500;
                    foundTuneParams.sat.frequency /= 1000;
                    foundTuneParams.sat.frequency *= 1000;
                    foundTuneParams.sat.symbolRate  = channel[cnt].m_uiSymbolRate_Hz/1000;
                    if(foundTuneParams.sat.symbolRate > 10000)
                    {
                        foundTuneParams.sat.symbolRate += 50;
                        foundTuneParams.sat.symbolRate /= 100;
                        foundTuneParams.sat.symbolRate *= 100;
                    }
                    foundTuneParams.sat.pol         = pScanParams->pol;
                    foundTuneParams.sat.satID       = pScanParams->satID;
                    switch(((channel[cnt].m_Flags&CI_FLAG_DVBS2_BIT_MASK)>>CI_FLAG_DVBS2_BIT))
                    {
                        case 0:
                            foundTuneParams.sat.modulation = FE_MOD_QPSK;
                            break;
                        case 1:
                            foundTuneParams.sat.modulation = FE_MOD_QPSK_S2;
                            break;
                        default:
                            foundTuneParams.sat.modulation = FE_MOD_NONE;
                            break;
                    }
                    if((channel[cnt].m_Flags&CI_FLAG_IQ_AUTO_BIT_MASK) != 0)
                    {
                        foundTuneParams.sat.iq = FE_IQ_AUTO;
                    }
                    else
                    {
                        if((channel[cnt].m_Flags&CI_FLAG_IQ_BIT_MASK) != 0)
                        {
                            foundTuneParams.sat.iq = FE_IQ_NORMAL;
                        }
                        else
                        {
                            foundTuneParams.sat.iq = FE_IQ_INVERTED;
                        }
                    }
                    FAPI_SYS_PRINT_MSG("to appl: Freq=%5d MHz Sym=%5d k\n",
                                                    foundTuneParams.sat.frequency/1000,
                                                    foundTuneParams.sat.symbolRate);
                                        
                    pScanParams->feScanNotifyFct(&foundTuneParams);
                }
            }
        }
    }
    
    // Switch to demodulator functional mode
    error = AVL_DVBSx_IBase_SetFunctionalMode( &a6211Hdl[index],AVL_DVBSx_FunctMode_Demod);
    if (error != AVL_DVBSx_EC_OK) 
    {
        FAPI_SYS_PRINT_MSG("IBase SetFunctionalMode failed !\n");
        return error;
    }
    AVL_DVBSx_IBSP_Delay(100);    //Wait 100 ms to assure that the AVL6211 chip boots up.This function should be implemented by customer.
    
    //This function should be called to verify the AVL6211 chip has completed its initialization procedure.
    error = AVL_DVBSx_IBase_GetStatus(&a6211Hdl[index]);
    if (error != AVL_DVBSx_EC_OK)
    {
        FAPI_SYS_PRINT_MSG("SetFunctionalMode failed !\n");
        return error;
    }

    return error;
}
#endif
static AVL_DVBSx_ErrorCode CPU_Halt(struct AVL_DVBSx_Chip * pAVLChip)
{
     AVL_DVBSx_ErrorCode r;
     AVL_uint16 i= 0;
	 
     r = AVL_DVBSx_IBase_SendRxOP(OP_RX_HALT, pAVLChip );

     if(AVL_DVBSx_EC_OK == r)
     {
          while(i++<20)
          {
              r = AVL_DVBSx_IBase_GetRxOPStatus(pAVLChip);
              if(AVL_DVBSx_EC_OK == r)
              {
                   break;
              }
              else
              {
                   AVL_DVBSx_IBSP_Delay(10);
              }
          }
     }
	 return (r);
}
static uint16_t	A2108_TestSinalLevel(void)
{
		uint16_t	r = 0;
		uint16_t uiRFSignalLevel=0;
		uint32_t uiRF_LevelTotal=0;
		uint16_t i; //This function can be called to get the RF signal level after the channel locked.
		for(i=0;i<50;i++)
		{
		uiRFSignalLevel=0;
		// r=A2108_IDVBSxRx_GetSignalLevel(&uiRFSignalLevel, pAVLChip);
		r= AVL_DVBSx_IRx_GetSignalLevel(&uiRFSignalLevel, &a6211Hdl[0]);

		uiRF_LevelTotal=uiRF_LevelTotal+uiRFSignalLevel;
//		A2108_IBSP_Delay(100); 
		AVL_DVBSx_IBSP_Delay(50);
		}
		uiRFSignalLevel=uiRF_LevelTotal/50;
		FAPI_SYS_PRINT_MSG("Currnet Signal Level:%d\n",uiRFSignalLevel);
		return (r);
}
static BLIND_SCAN_LNBTYPE  A6211_Blindsacn_lnbtype(uint32_t freqhi,uint32_t freqlo,FE_POL_E pol)
{
	if(pol == FE_POL_HOR){
			if(freqhi== freqhi){
				if(freqlo <=5950000)
				 return BLIND_LNBFRE_CSINGLE;
				else 
				 return BLIND_LNBFRE_KUSINGLE;
			}else if(freqlo <=5950000){
				return BLIND_LNBFRE_CBAUD_H;
			}else if(freqhi >5950000){
				return BLIND_LNBFRE_KUBAUD_H;
			}
	}else if(pol == FE_POL_VER){
			if(freqhi== freqhi){
				if(freqlo <=5950000)
				 return BLIND_LNBFRE_CSINGLE;
				else 
				 return BLIND_LNBFRE_KUSINGLE;
			}else if(freqlo <=5950000){
				return BLIND_LNBFRE_CBAUD_V;
			}else if(freqhi >5950000){
				return BLIND_LNBFRE_KUBAUD_V;
			}

	}
	return 0;
}
/*
void A6211_reset(void){
	char gpio_no[4]={"177"};
	int handle_gpio_rst;
	char one='1';
	char zero='0';
	
	int handle_gpio_export=open("/proc/gpio_test/export", O_RDWR);
	if(handle_gpio_export < 0){
		printf("open handle_gpio_export fail:%s\n",strerror(errno));
		return;
	}
	write(handle_gpio_export,gpio_no,3);
	
	handle_gpio_rst= open("/proc/gpio_test/177", O_RDWR);
	if(handle_gpio_rst < 0){
		printf("open handle_gpio_rst fail:%s\n",strerror(errno));
		return;
	}
	write(handle_gpio_rst,&one,1);
	
	sleep(1);
	write(handle_gpio_rst,&zero,1);
	close(handle_gpio_rst);
	close(handle_gpio_export);
}
*/
int main(int argc,char *  argv[]){
	int tuner_idx=0,i;
	int32_t AFC;
	uint32_t BER;
	uint16_t Quality,Strength;
	if(argc < 3){
		printf("arg error,format: ./bin freq(MHZ) symbol(Ksym)\n");
		return -1;	
	}
	//A6211_reset();
	int r=A6211_Open(tuner_idx);
	printf("A6211_Open ret:%d\n",r);
	if(r==0){
		struct AVL_DVBSx_VerInfo  VerInfo;
		AVL_uint32 uiDeviceID=0;
		memset(&VerInfo,0,sizeof(struct AVL_DVBSx_VerInfo));
		r=AVL_DVBSx_IBase_GetVersion(&VerInfo,&a6211Hdl[0]);
		//printf("get version ret:%d,%x:%x:%x\n",r,VerInfo.m_Chip.m_Major,VerInfo.m_Chip.m_Minor,VerInfo.m_Chip.m_Build);
		
		printf("Chip Ver:{%d}.{%d}.{%d}  API Ver:{%d}.{%d}.{%d}  Patch Ver:{%d}.{%d}.{%d} \n", 
		VerInfo.m_Chip.m_Major, VerInfo.m_Chip.m_Minor, VerInfo.m_Chip.m_Build, 
		VerInfo.m_API.m_Major, VerInfo.m_API.m_Minor, VerInfo.m_API.m_Build, 
		VerInfo.m_Patch.m_Major, VerInfo.m_Patch.m_Minor, VerInfo.m_Patch.m_Build);	
		r=AVL_DVBSx_IRx_GetDeviceID(&a6211Hdl[tuner_idx],&uiDeviceID);
		printf("AVL_DVBSx_IRx_GetDeviceID ret:%d,uiDeviceID:%x\n",r,uiDeviceID);
		
	}
	r=A6211_ControlLnbVoltage(tuner_idx,FE_LNB_18V);
	printf("A6211_ControlLnbVoltage ret:%d\n",r);
	
	r=A6211_Control22kHz(tuner_idx,FE_22KHZ_TONE_0);
	printf("A6211_Control22kHz ret:%d\n",r);
	
	r=A6211_Tune(tuner_idx, atoi(argv[1])*1000, atoi(argv[2]));
	printf("A6211_Tune ret:%d,freq:%d KHZ,symbol:%d Ksym\n",r,atoi(argv[1])*1000, atoi(argv[2]));
	A6211_GetAFC(tuner_idx, &AFC);
	for(i=0;i<3;i++){
		AVL_DVBSx_IBSP_Delay(1000);
		r=A6211_GetBER(tuner_idx, &BER);
		if(r==0){
			printf("A6211_GetBER ret:%d,BER:%d\n",r,BER);
			break;
		}
	}
	//
	for(i=0;i<3000;i++){
		AVL_DVBSx_IBSP_Delay(1000);
		r=A6211_GetSignalQuality(tuner_idx, &Quality);
		r|=A6211_GetSignalStrength(tuner_idx, &Strength);
		if(r==0){
			printf("A6211_GetSignalQuality ret:%d,Quality:%d,Strength:%d\n",r,Quality,Strength);
			//break;
		}
	}

	if(0){
		r=A2108_TestSinalLevel();
		A6211_Control22kHz(tuner_idx,FE_22KHZ_ON);
		//A2108_TestSinalLevel();
	
		A6211_ControlLnbVoltage(tuner_idx,FE_LNB_13V);
		A6211_Blindsacn_lnbtype(2000,975,FE_POL_HOR);
		A6211_Motor_test(tuner_idx);
		CPU_Halt(&a6211Hdl[0]);
	}
	A6211_Close(tuner_idx);
	
	return 0;
}
