/*****************************************************************************
* Tuner sample code
*
* History:
* Date Athor Version Reason
* ============ ============= ========= =================
* 1.Jun.21.2008 monsen Ver 0.1 Method1
* 2.Oct.26.2008 monsen Ver 0.2 Method3
*****************************************************************************/
#include "avl_dvbsx.h"
#include "ExtAV2020.h"
#include "IBSP.h"
#include "II2C.h"

/*
AVL_uint16 AVL_DVBSx_ITuner_CalculateLPF(AVL_uint16 uiSymbolRate_10kHz) {
	AVL_int32 lpf = uiSymbolRate_10kHz;
	//lpf *= 81;
	//lpf /= 100;
	//lpf += 500;
	return((AVL_uint16)lpf);
}
*/
AVL_DVBSx_ErrorCode ExtAV2020_I2C_write(AVL_uchar reg_start,AVL_uchar* buff,AVL_uchar len, struct AVL_Tuner * pTuner)
{
	AVL_DVBSx_ErrorCode r=0;
	//AVL_uint16 uiTimeOut = 0;
	AVL_uchar ucTemp[50];
	AVL_int32 i;
	AVL_DVBSx_IBSP_Delay(5);
	ucTemp[0] = reg_start;
	for(i=1;i<len+1;i++)
	{
		ucTemp[i]=*(buff+i-1);
	}
	r = AVL_DVBSx_II2CRepeater_SendData( (AVL_uchar)pTuner->m_uiSlaveAddress,ucTemp, len+1, pTuner->m_pAVLChip );
	if(r != AVL_DVBSx_EC_OK)
	{
		return(r);
	}
	AVL_DVBSx_IBSP_Delay(1);
	return(r);
}

/********************************************************************************
* INT32 ExtAV2020_Tuner_control(AVL_uint32 channel_freq, AVL_uint32 bb_sym)
*
* Arguments:
* Parameter1: AVL_uint32 channel_freq : Channel freqency
* Parameter2: AVL_uint32 bb_sym : Baseband Symbol Rate
*
* Return Value: INT32 : Result
*****************************************************************************/

AVL_DVBSx_ErrorCode ExtAV2020_Tuner_control(AVL_uint32 channel_freq, AVL_uint32 bb_sym,struct AVL_Tuner * pTuner)
{
	AVL_uchar reg[50];
	AVL_uchar reg_start;
	AVL_uint32 fracN;
	AVL_uint32 BW;
	AVL_uint32 BF;
	AVL_DVBSx_ErrorCode r;
	AVL_uchar auto_scan = 0;// Add flag for "’ßÌ¨"
	// Register initial flag
	static AVL_uchar tuner_initial[2] = {0,0};
	//when sym is 0 or 45000, means auto-scan channel.
	if (bb_sym == 0 || bb_sym == 45000) //auto-scan mode
	{
		auto_scan = 1;
	}
	if(pTuner->m_pAVLChip->m_uiBusId>=2)
	{
		return AVL_DVBSx_EC_GeneralFail;
	}
	// At Power ON, tuner_initial = 0
	// do the initial if not do yet.
	if (tuner_initial[pTuner->m_pAVLChip->m_uiBusId] == 0)
	{
		//Tuner Initail registers R0~R41
		reg[0]= 0x38;
		reg[1]= 0x00;
		reg[2]= 0x00;
		reg[3]= 0x50;
		reg[4]= 0x1f;
		reg[5]= 0xa3;
		reg[6]= 0xfd;
		reg[7]= 0x58;
		reg[8]= 0x0e;//org=0x32,now=0x1e
		reg[9]= 0x82;
		reg[10]= 0x88;
		reg[11]= 0xb4;
		reg[12]= 0xd6; //RFLP=ON at Power on initial
		reg[13]= 0x40;
		reg[14]= 0x5b;
		reg[15]= 0x6a;
		reg[16]= 0x66;
		reg[17]= 0x40;
		reg[18]= 0x80;
		reg[19]= 0x2b;
		reg[20]= 0x6a;
		reg[21]= 0x50;
		reg[22]= 0x91;
		reg[23]= 0x27;
		reg[24]= 0x8f;
		reg[25]= 0xcc;
		reg[26]= 0x21;
		reg[27]= 0x10;
		reg[28]= 0x80;
		reg[29]= 0x02;
		reg[30]= 0xf5;
		reg[31]= 0x7f;
		reg[32]= 0x4a;
		reg[33]= 0x9b;
		reg[34]= 0xe0;
		reg[35]= 0xe0;
		reg[36]= 0x36;
		//monsen 20080710. Disble FT-function at Power on initial
		//reg[37]= (0x02);
		reg[37]= 0x00;
		reg[38]= 0xab;
		reg[39]= 0x97;
		reg[40]= 0xc5;
		reg[41]= 0xa8;
		//monsen 20080709. power on initial at first "ExtAV2020_Tuner_control()" call
		// Sequence 1
		// Send Reg0 ->Reg11
		reg_start = 0;
		r =  ExtAV2020_I2C_write(reg_start,reg,12,pTuner);
		// Time delay 1ms
		AVL_DVBSx_IBSP_Delay(1);
		// Sequence 2
		// Send Reg13 ->Reg24
		reg_start = 13;
		r = ExtAV2020_I2C_write(reg_start,reg+13,12,pTuner);
		// Send Reg25 ->Reg35
		reg_start = 25;
		r = ExtAV2020_I2C_write(reg_start,reg+25,11,pTuner);
		// Send Reg36 ->Reg41
		reg_start = 36;
		r = ExtAV2020_I2C_write(reg_start,reg+36,6,pTuner);
		// Time delay 1ms
		AVL_DVBSx_IBSP_Delay(1);
		// Sequence 3
		// send reg12
		reg_start = 12;
		r = ExtAV2020_I2C_write(reg_start,reg+12,1,pTuner);
		//monsen 20081125, Wait 100 ms
		AVL_DVBSx_IBSP_Delay(100);
		//monsen 20081030, Reinitial again
		{
			// Sequence 1
			// Send Reg0 ->Reg11
			reg_start = 0;
			r = ExtAV2020_I2C_write(reg_start,reg,12,pTuner);
			// Time delay 1ms
			AVL_DVBSx_IBSP_Delay(1);
			// Sequence 2
			// Send Reg13 ->Reg24
			reg_start = 13;
			r = ExtAV2020_I2C_write(reg_start,reg+13,12,pTuner);
			// Send Reg25 ->Reg35
			reg_start = 25;
			r = ExtAV2020_I2C_write(reg_start,reg+25,11,pTuner);
			// Send Reg36 ->Reg41
			reg_start = 36;
			r = ExtAV2020_I2C_write(reg_start,reg+36,6,pTuner);
			// Time delay 1ms
			AVL_DVBSx_IBSP_Delay(1);
			// Sequence 3
			// send reg12
			reg_start = 12;
			r = ExtAV2020_I2C_write(reg_start,reg+12,1,pTuner);
		}
		// Wait 4 ms
		AVL_DVBSx_IBSP_Delay(50);
		// power on initial end
		tuner_initial[pTuner->m_pAVLChip->m_uiBusId] = 1;
	//***** monsen 20080709. Lock Channel sequence
		//***** After RFAGC is starting tracking
		//***** Make sure the RFAGC do not have sharp jump after sending reg[37]
		// RF Channel Fregency Setting.
		AVL_DVBSx_IBSP_Delay(50);
		fracN = (channel_freq + tuner_crystal/2)/tuner_crystal;
		if(fracN > 0xff)
		fracN = 0xff;
		reg[0]= (AVL_uchar)(fracN & 0xff);
		fracN = (channel_freq<<17)/tuner_crystal;
		fracN = fracN & 0x1ffff;
		reg[1]=(AVL_uchar) ((fracN>>9)&0xff);
		reg[2]=(AVL_uchar) ((fracN>>1)&0xff);
		// reg[3]_D7 is frac<0>, D6~D0 is 0x50
		reg[3]=(AVL_uchar) (((fracN<<7)&0x80) | 0x50); // default is 0x50
		// Channel Filter Bandwidth Setting.
		//"sym" unit is Hz;
		if(auto_scan)//’ßÌ¨ requested by BB
		{
			reg[5] = 0xA3; //BW=27MHz
		}
		else
		{
			// rolloff is 35%
			BW = bb_sym*135/200;
			// monsen 20080726, BB request low IF when sym < 6.5MHz
			// add 6M when Rs<6.5M,
			if(bb_sym<6500)
			{
				BW = BW + 6000;
			}
				// add 2M for LNB frequency shifting
			BW = BW + 2000;
				// add 8% margin since fc is not very accurate
			BW = BW*108/100;
				// Bandwidth can be tuned from 4M to 40M
			if( BW< 4000)
			{
				BW = 4000;
			}
			if( BW> 40000)
			{
				BW = 40000;
			}
			BF = (BW*127 + 21100/2) / (21100); // BW(MHz) * 1.27 / 211KHz
			reg[5] = (AVL_uchar)BF;
		}
		// Sequence 4
		// Send Reg0 ->Reg4
		r = ExtAV2020_I2C_write(0,reg,4,pTuner);
		AVL_DVBSx_IBSP_Delay(5);
		// Sequence 5
		// Send Reg5
		r = ExtAV2020_I2C_write(5, reg+5, 1,pTuner);
		AVL_DVBSx_IBSP_Delay(5);
		// Fine-tune Function Control
		//Tuner fine-tune gain function block. bit2.
		//not auto-scan case. enable block function. FT_block=1, FT_EN=1
		if (!auto_scan)
		{
			reg[37] = 0x06;
			r = ExtAV2020_I2C_write(37, reg+37, 1,pTuner);
			AVL_DVBSx_IBSP_Delay(5);
			//Disable RFLP at Lock Channel sequence after reg[37]
			//RFLP=OFF at Lock Channel sequence
			// RFLP can be Turned OFF, only at Receving mode.
			reg[12] = 0x96;
			r = ExtAV2020_I2C_write(12, reg+12, 1,pTuner);
			AVL_DVBSx_IBSP_Delay(5);
		}
	}
	else
	{
		//***** monsen 20080709. Lock Channel sequence
		//***** After RFAGC is starting tracking
		//***** Make sure the RFAGC do not have sharp jump after sending reg[37]
		// RF Channel Fregency Setting.
		fracN = (channel_freq + tuner_crystal/2)/tuner_crystal;
		if(fracN > 0xff)
		fracN = 0xff;
		reg[0]=(AVL_uchar) (fracN & 0xff);
		fracN = (channel_freq<<17)/tuner_crystal;
		fracN = fracN & 0x1ffff;
		reg[1]=(AVL_uchar) ((fracN>>9)&0xff);
		reg[2]=(AVL_uchar) ((fracN>>1)&0xff);
		// reg[3]_D7 is frac<0>, D6~D0 is 0x50
		reg[3]=(AVL_uchar) (((fracN<<7)&0x80) | 0x50); // default is 0x50
		// Channel Filter Bandwidth Setting.
		//"sym" unit is Hz;
		if(auto_scan)//’ßÌ¨ requested by BB
		{
			reg[5] = 0xA3; //BW=27MHz
		}
		else
		{
			// rolloff is 35%
			BW = bb_sym*135/200;
			// monsen 20080726, BB request low IF when sym < 6.5MHz
			// add 6M when Rs<6.5M,
			if(bb_sym<6500)
			{
				BW = BW + 6000;
			}
				// add 2M for LNB frequency shifting
			BW = BW + 2000;
				// add 8% margin since fc is not very accurate
			BW = BW*108/100;
				// Bandwidth can be tuned from 4M to 40M
			if( BW< 4000)
			{
				BW = 4000;
			}
			if( BW> 40000)
			{
				BW = 40000;
			}
			BF = (BW*127 + 21100/2) / (21100); // BW(MHz) * 1.27 / 211KHz
			reg[5] = (AVL_uchar)BF;
		}
		// Sequence 4
		// Send Reg0 ->Reg4
		r = ExtAV2020_I2C_write(0,reg,4,pTuner);
		AVL_DVBSx_IBSP_Delay(5);
		// Sequence 5
		// Send Reg5
		r = ExtAV2020_I2C_write(5, reg+5, 1,pTuner);
		AVL_DVBSx_IBSP_Delay(5);
		// Fine-tune Function Control
		//Tuner fine-tune gain function block. bit2.
		//not auto-scan case. enable block function. FT_block=1, FT_EN=1
		if (!auto_scan)
		{
			reg[37] = 0x06;
			r = ExtAV2020_I2C_write(37, reg+37, 1,pTuner);
			AVL_DVBSx_IBSP_Delay(5);
			//Disable RFLP at Lock Channel sequence after reg[37]
			//RFLP=OFF at Lock Channel sequence
			// RFLP can be Turned OFF, only at Receving mode.
			reg[12] = 0x96;
			r = ExtAV2020_I2C_write(12, reg+12, 1,pTuner);
			AVL_DVBSx_IBSP_Delay(5);
		}
	}
	return r;
}

AVL_DVBSx_ErrorCode AVL_DVBSx_ExtAV2020_Lock(  struct AVL_Tuner * pTuner)
{
	AVL_DVBSx_ErrorCode r;
	
	r = ExtAV2020_Tuner_control((AVL_uint32)((pTuner->m_uiFrequency_100kHz)/10), (AVL_uint32)((pTuner->m_uiLPF_100kHz)*100), pTuner);
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_ExtAV2020_GetLockStatus(struct AVL_Tuner * pTuner)
{
	AVL_DVBSx_ErrorCode r;
	//Send register address
		r = AVL_DVBSx_II2CRepeater_GetOPStatus(pTuner->m_pAVLChip);
		if( AVL_DVBSx_EC_OK != r ) 
		{
			return(r);
		}
		AVL_DVBSx_IBSP_Delay(1);
 		//r = AVL_DVBSx_II2CRepeater_ReadData(AVL_uchar(0xc6),regTemp, 0x14, pTuner->m_pAVLChip);
		r = AVL_DVBSx_II2CRepeater_GetOPStatus(pTuner->m_pAVLChip);
		if( AVL_DVBSx_EC_OK != r ) 
		{
			return(r);
		}
		AVL_DVBSx_IBSP_Delay(1);
 		//r = AVL_DVBSx_II2CRepeater_ReadData(AVL_uchar(0xc6),(regTemp+0x14), 0x14, pTuner->m_pAVLChip );
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_ExtAV2020_Initialize(struct AVL_Tuner * pTuner) {
	AVL_DVBSx_ErrorCode r;

	r = AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_tuner_slave_addr_addr, pTuner->m_uiSlaveAddress);
	r |= AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_tuner_use_internal_control_addr, 0);
	r |= AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_tuner_LPF_margin_100kHz_addr, 0);	//clean up the LPF margin for blind scan. for external driver, this must be zero.
	r |= AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_tuner_max_LPF_100kHz_addr, 340);	//set up the right LPF for blind scan to regulate the freq_step. This field should corresponding the flat response part of the LPF.
	r |= AVL_DVBSx_II2CRepeater_Initialize(pTuner->m_uiI2CBusClock_kHz, pTuner->m_pAVLChip);

	return(r);	
}

