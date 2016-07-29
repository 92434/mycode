/*****************************************************************************
* Tuner sample code
*
* History:
* Date Athor Version Reason
* ============ ============= ========= =================
* 1.Apr.29.2010 Version1.0
*****************************************************************************/
//#include "avl2108.h"
#include "ExtAV2011.h"
#include "IBSP.h"
#include "IBase.h"
#include "II2C.h"
#include <stdint.h>
#include <stdio.h>
AVL_DVBSx_ErrorCode AV2011_I2C_write(AVL_uchar reg_start,AVL_uchar* buff,AVL_uchar len,struct AVL_Tuner * pTuner)
{
	AVL_DVBSx_ErrorCode r=0;
	AVL_uint16 uiTimeOut = 0;
	AVL_uchar ucTemp[50];
	int i;
	//AVL_DVBSx_IBSP_Delay(5); //comment by wanghua
	ucTemp[0] = reg_start;
	for(i=1;i<len+1;i++)
	{
		ucTemp[i]=*(buff+i-1);
	}
	
	r = AVL_DVBSx_II2CRepeater_GetOPStatus( pTuner->m_pAVLChip );
	while( r != AVL_DVBSx_EC_OK)
	{
	#ifdef AV2011_ENABLE_TIMEOUT
		if( uiTimeOut++>AV2011_TIMEOUT )
			return(AVL_DVBSx_EC_TimeOut);
	#endif
		AVL_DVBSx_IBSP_Delay(1);
		r = AVL_DVBSx_II2CRepeater_GetOPStatus( pTuner->m_pAVLChip );
	}
		
	r = AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress),ucTemp, len+1, pTuner->m_pAVLChip );
	if(r != AVL_DVBSx_EC_OK){
		return(r);
	}

	//AVL_DVBSx_IBSP_Delay(5);
	return(r);
}

AVL_DVBSx_ErrorCode ExtAV2011_RegInit(struct AVL_Tuner * pTuner)
{
	AVL_uchar reg[50];
	AVL_DVBSx_ErrorCode r;
	
		//Tuner Initail registers R0~R41
		reg[0]=0x38;
		reg[1]=0x00;
		reg[2]=0x00;
		reg[3]=0x50;
		reg[4]=0x1f;
		reg[5]=0xa3;
		reg[6]=0xfd;
		reg[7]=0x58;
		reg[8]=0x0e;
		reg[9]=0x82;
		reg[10]=0x88;
		reg[11]=0xb4;
		reg[12]=0xd6; //RFLP=ON at Power on initial
		reg[13]=0x40;
#ifdef AV2011_Tuner
		reg[14]=0x94;
		reg[15]=0x4a;
#else
		reg[14]=0x5b;
		reg[15]=0x6a;
#endif
		reg[16]=0x66;
		reg[17]=0x40;
		reg[18]=0x80;
		reg[19]=0x2b;
		reg[20]=0x6a;
		reg[21]=0x50;
		reg[22]=0x91;
		reg[23]=0x27;
		reg[24]=0x8f;
		reg[25]=0xcc;
		reg[26]=0x21;
		reg[27]=0x10;
		reg[28]=0x80;
		reg[29]=0x02;
		reg[30]=0xf5;
		reg[31]=0x7f;
		reg[32]=0x4a;
		reg[33]=0x9b;
		reg[34]=0xe0;
		reg[35]=0xe0;
		reg[36]=0x36;
		reg[37]=0x00; //Disble FT-function at Power on initial
		reg[38]=0xab;
		reg[39]=0x97;
		reg[40]=0xc5;
		reg[41]=0xa8;
	
		// Sequence 1, Send Reg0 ->Reg11
		r =  AV2011_I2C_write(0,reg,12,pTuner);
		AVL_DVBSx_IBSP_Delay(1);
		// Sequence 2, Send Reg13 ->Reg24
		r = AV2011_I2C_write(13,reg+13,12,pTuner);
		// Send Reg25 ->Reg35
		r = AV2011_I2C_write(25,reg+25,11,pTuner);
		// Send Reg36 ->Reg41
		r = AV2011_I2C_write(36,reg+36,6,pTuner);
		AVL_DVBSx_IBSP_Delay(1);
		// Sequence 3, send reg12
		r = AV2011_I2C_write(12,reg+12,1,pTuner);
		//monsen 20081125, Wait 100 ms
		AVL_DVBSx_IBSP_Delay(10);
		//monsen 20081030, Reinitial again
		{
			// Sequence 1, Send Reg0 ->Reg11
			r = AV2011_I2C_write(0,reg,12,pTuner);
			AVL_DVBSx_IBSP_Delay(1);
			// Sequence 2, Send Reg13 ->Reg24		
			r = AV2011_I2C_write(13,reg+13,12,pTuner);
			// Send Reg25 ->Reg35
			r = AV2011_I2C_write(25,reg+25,11,pTuner);
			// Send Reg36 ->Reg41
			r = AV2011_I2C_write(36,reg+36,6,pTuner);
			AVL_DVBSx_IBSP_Delay(1);
			// Sequence 3, send reg12
			r = AV2011_I2C_write(12,reg+12,1,pTuner);
			AVL_DVBSx_IBSP_Delay(5);
			return(r);
	    }
}

/********************************************************************************
* AVL_DVBSx_ErrorCode Tuner_control(uint32_t channel_freq, uint32_t bb_sym, AVL_DVBSx_Chip * pAVLChip,struct AV2011_Setting * AV2011_Configure)
*
* Arguments:
* Parameter1: uint32_t channel_freq : Channel frequency (Unit: MHz)
* Parameter2: uint32_t bb_sym : Baseband Symbol Rate (Unit: KHz)
* Paramiter3: AVL_DVBSx_Chip * pAVLChip : AVLChip structure
* Paramiter4: struct AV2011_Setting * AV2011_Configure : Special configuration for AV2011(IQ mode, Auto_Gain, Scan mode, RF_LP)
* Return Value: INT32 : Result
*****************************************************************************/
AVL_DVBSx_ErrorCode Tuner_control(uint32_t channel_freq, uint32_t tuner_lpf,  struct AVL_Tuner * pTuner, struct AV2011_Setting * AV2011_Configure)
{
	uint8_t reg[50];
	uint32_t fracN;
	uint32_t BW;
	uint32_t BF;
	AVL_DVBSx_ErrorCode r;
	AVL_DVBSx_IBSP_Delay(50);
	fracN = (channel_freq + tuner_crystal/2)/tuner_crystal;
	if(fracN > 0xff)
	fracN = 0xff;
	reg[0]=(char) (fracN & 0xff);
	fracN = (channel_freq<<17)/tuner_crystal;
	fracN = fracN & 0x1ffff;
	reg[1]=(char) ((fracN>>9)&0xff);
	reg[2]=(char) ((fracN>>1)&0xff);
	// reg[3]_D7 is frac<0>, D6~D0 is 0x50
#ifdef AV2011_Tuner
	//AV2011 IQ Single_end/Differential mode at bit2
	reg[3]=(char) (((fracN<<7)&0x80) | 0x50 |(AV2011_Configure->Single_End)<<2);
#else
    //AV2020 no IQ mode
   	reg[3]=(char) (((fracN<<7)&0x80) | 0x50 );
#endif
	printf("[Tuner_control]channel_freq:%d,tuner_lpf:%d\n",channel_freq,tuner_lpf);
	// Channel Filter Bandwidth Setting.
	//"sym" unit is Hz;
	if(AV2011_Configure->Auto_Scan)//auto scan requested by BB
	{
		reg[5] = 0xA3; //BW=27MHz
		// Sequence 4
		// Send Reg0 ->Reg4
		r = AV2011_I2C_write(0,reg,4,pTuner);
		if(r!=AVL_DVBSx_EC_OK)
		{
			return(r);
		}
		AVL_DVBSx_IBSP_Delay(5);

		// Sequence 5
		// Send Reg5
		r = AV2011_I2C_write(5, reg+5, 1,pTuner);
		if(r!=AVL_DVBSx_EC_OK)
		{
			return(r);
		}
		AVL_DVBSx_IBSP_Delay(5);

		// Fine-tune Function Control
		// Auto-scan mode 
		// FT_block=1, FT_EN=0, FT_hold=1
		reg[37] = 0x05;
		r = AV2011_I2C_write(37, reg+37, 1,pTuner);
		if(r!=AVL_DVBSx_EC_OK)
		{
			return(r);
		}
		// Time delay 4ms
		AVL_DVBSx_IBSP_Delay(4);
	}
	else
	{
		BW = tuner_lpf;
		// Bandwidth can be tuned from 4M to 40M
		if( BW< 4000)
		{
			BW = 4000;
		}
		if( BW> 40000)
		{
			BW = 40000;
		}
		BF = (BW*127 + 21100/2) / (21100); 
		reg[5] = (uint8_t)BF;

		// Sequence 4
		// Send Reg0 ->Reg4
		AVL_DVBSx_IBSP_Delay(5);
		r = AV2011_I2C_write(0,reg,4,pTuner);
		if(r!=AVL_DVBSx_EC_OK)
		{
			return(r);
		}
		AVL_DVBSx_IBSP_Delay(5);
		r = AV2011_I2C_write(0,reg,4,pTuner);
		if(r!=AVL_DVBSx_EC_OK)
		{
			return(r);
		}
		AVL_DVBSx_IBSP_Delay(5);
		// Sequence 5
		// Send Reg5
		r = AV2011_I2C_write(5, reg+5, 1,pTuner);
		if(r!=AVL_DVBSx_EC_OK)
		{
			return(r);
		}
		AVL_DVBSx_IBSP_Delay(5);
		
		// Fine-tune Function Control
		// not auto-scan case. enable block function.
		// FT_block=1, FT_EN=1, FT_hold=0
		reg[37] = 0x04 | ((AV2011_Configure->AutoGain)<<1);
		r = AV2011_I2C_write(37, reg+37, 1,pTuner);
		if(r!=AVL_DVBSx_EC_OK)
		{
		return(r);
		}
		AVL_DVBSx_IBSP_Delay(5);
		//Disable RFLP at Lock Channel sequence after reg[37]
		//RFLP=OFF at Lock Channel sequence
		// RFLP can be Turned OFF, only at Receiving mode.
		reg[12] = 0x96 + ((AV2011_Configure->RF_Loop)<<6);
		r = AV2011_I2C_write(12, reg+12, 1,pTuner);
		if(r!=AVL_DVBSx_EC_OK)
		{
			return(r);
		}
	}
	return r;
}

#if 1
static AVL_DVBSx_ErrorCode Frequency_LPF_Adjustment(struct AVL_Tuner * pTuner,AVL_uint16 *uiAdjustFreq)
{
	AVL_DVBSx_ErrorCode r;
	AVL_uint32 uitemp1;
	AVL_uint16 uitemp2;
	AVL_uint16 minimum_LPF_100kHz;
	AVL_uint16 carrierFrequency_100kHz;

	r = AVL_DVBSx_II2C_Read32(pTuner->m_pAVLChip, 0x263E, &uitemp1);
	r |= AVL_DVBSx_II2C_Read16(pTuner->m_pAVLChip, 0x2642, &uitemp2);
	if(r != AVL_DVBSx_EC_OK)
	{
		*uiAdjustFreq = pTuner->m_uiFrequency_100kHz;
		return r;
	}
	printf("[Frequency_LPF_Adjustment]uitemp1:%d,uitemp2:%d\n",uitemp1,uitemp2);
	if(pTuner->m_uiSymbolRate_Hz <= uitemp1)
	{
		carrierFrequency_100kHz =(AVL_uint16 )((uitemp2/10)+ pTuner->m_uiFrequency_100kHz);

		minimum_LPF_100kHz = (pTuner->m_uiSymbolRate_Hz/100000 )*135/200 +  (uitemp2/10) + 50;
		if(pTuner->m_uiLPF_100kHz < minimum_LPF_100kHz)
		{
			pTuner->m_uiLPF_100kHz = (AVL_uint16 )(minimum_LPF_100kHz);
		}
	}
	else
	{
		carrierFrequency_100kHz = pTuner->m_uiFrequency_100kHz;
	}

	*uiAdjustFreq = carrierFrequency_100kHz;

	return AVL_DVBSx_EC_OK;

}
#else
static void AVL_Set_LPF(struct AVL_Tuner * pTuner)
{	

	pTuner->m_uiLPF_100kHz = pTuner->m_uiSymbolRate_Hz*75/10000000+40;

	if(pTuner->m_uiLPF_100kHz > 440)
	{
		pTuner->m_uiLPF_100kHz = 440;
	}

}
#endif
AVL_DVBSx_ErrorCode AVL_DVBSx_ExtAV2011_Lock(  struct AVL_Tuner * pTuner)
{
	AVL_DVBSx_ErrorCode r;
	struct AV2011_Setting AV2011_Set;
	AVL_uint16 carrierFrequency_100kHz;

	Frequency_LPF_Adjustment(pTuner, &carrierFrequency_100kHz);
	//if (0) AVL_Set_LPF(pTuner);
	printf("[AVL_DVBSx_ExtAV2011_Lock]m_uiFrequency_100kHz:%d,m_uiLPF_100kHz:%d,m_pParameters:%p\n",
		pTuner->m_uiFrequency_100kHz,pTuner->m_uiLPF_100kHz,pTuner->m_pParameters);
	//pTuner->m_pParameters is used store four special setting for AV2011:IQ mode, auto Gain setting, Work mode, RF loop through
	if(pTuner->m_pParameters == 0)  //if none
	{
		AV2011_Set.Auto_Scan = 0;  //Default Setting: Normal lock mode
		AV2011_Set.AutoGain = 1;   //Defualt Setting: Auto Gain control on
		AV2011_Set.Single_End = 1; //defualt setting: IQ Differential mode
		AV2011_Set.RF_Loop = 1;    //defualt setting: open RF loop through
		//(struct AV2011_Setting)(pTuner->m_pParameters) = &AV2011_Set;  //use default setting if not set it in Struct pTuner->m_pParameters.
		r = Tuner_control((AVL_uint32) ((pTuner->m_uiFrequency_100kHz)/10), (AVL_uint32) ((pTuner->m_uiLPF_100kHz)*100), pTuner, &AV2011_Set);
	}
	else  //set AV2011 tuner spectial setting accordding to pTuner->m_pParameter
	{
		r = Tuner_control((AVL_uint32) ((pTuner->m_uiFrequency_100kHz)/10), (AVL_uint32 )((pTuner->m_uiLPF_100kHz)*100), pTuner, (struct AV2011_Setting *)(pTuner->m_pParameters));
	}
	return(r);
}
/*
AVL_DVBSx_ErrorCode AVL_DVBSx_ExtAV2011_GetLockStatus(struct AVL_Tuner * pTuner)
{
	AVL_DVBSx_ErrorCode r;
	AVL_uchar uilock;
	//Send register address
	uilock = 0x0b;
	r = AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), &uilock, 1, pTuner->m_pAVLChip );
	if( AVL_DVBSx_EC_OK != r ) 
	{
	printf("DEMOD:AV6211: AVL_DVBSx_ExtAV2011_GetLockStatus \n");

		return(r);
	}
	r = AVL_DVBSx_II2CRepeater_ReadData((AVL_uchar)(pTuner->m_uiSlaveAddress), &uilock, 1, pTuner->m_pAVLChip );
	//r = AVL_DVBSx_II2CRepeater_ReadData_Multi(  pTuner->m_uiSlaveAddress, &uilock, 0x0B,(AVL_uint16)(0x1), pTuner->m_pAVLChip );
	if( AVL_DVBSx_EC_OK == r ) 
	{
		if( 0 == (uilock & AV2011_R0_PLL_LOCK) ) 
		{
			r = AVL_DVBSx_EC_GeneralFail;
				printf("DEMOD:AV6211: AVL_DVBSx_ExtAV2011_GetLockStatus2 \n");
		}
	}
	printf("DEMOD:AV6211: AVL_DVBSx_ExtAV2011_GetLockStatus3 \n");
	return(r);
}
*/

AVL_DVBSx_ErrorCode AVL_DVBSx_ExtAV2011_GetLockStatus(struct AVL_Tuner * pTuner)
{
	AVL_DVBSx_ErrorCode r;
	AVL_uchar uilock;
	//Send register address
	r = AVL_DVBSx_II2CRepeater_GetOPStatus(pTuner->m_pAVLChip);
	if( AVL_DVBSx_EC_OK != r ) 
	{
		printf("DEMOD:AV6211: AVL_DVBSx_ExtAV2011_GetLockStatus1 \n");
		return(r);
	}
	AVL_DVBSx_IBSP_Delay(1);
	r = AVL_DVBSx_II2CRepeater_ReadData_Multi((AVL_uchar)(pTuner->m_uiSlaveAddress), &uilock, 0x0B,(AVL_uint16)(0x1), pTuner->m_pAVLChip );
	if( AVL_DVBSx_EC_OK == r ) 
	{
		if( 0 == (uilock & AV2011_R0_PLL_LOCK) ) 
		{
			printf("DEMOD:AV6211: AVL_DVBSx_ExtAV2011_GetLockStatus2 ,r:%d\n",r);
			r = AVL_DVBSx_EC_Running;
			
		}
	}
	printf("DEMOD:AV6211: AVL_DVBSx_ExtAV2011_GetLockStatus3,uilock:%d,r:%d \n",uilock,r);
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_ExtAV2011_Initialize(struct AVL_Tuner * pTuner) {
	AVL_DVBSx_ErrorCode r;
	r = AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_tuner_slave_addr_addr, pTuner->m_uiSlaveAddress);
	r |= AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_tuner_use_internal_control_addr, 0);
	r |= AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_tuner_LPF_margin_100kHz_addr, 0);	//clean up the LPF margin for blind scan. for external driver, this must be zero.
	r |= AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_tuner_max_LPF_100kHz_addr, 400);	//set up the right LPF for blind scan to regulate the freq_step. This field should corresponding the flat response part of the LPF.
	r |= AVL_DVBSx_II2CRepeater_Initialize(pTuner->m_uiI2CBusClock_kHz, pTuner->m_pAVLChip);
	r |= ExtAV2011_RegInit(pTuner); //init all tuner register
	return(r);	
}

