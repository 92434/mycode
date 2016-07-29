/*
 *           Copyright 2010 Availink, Inc.
 *
 *  This software contains Availink proprietary information and
 *  its use and disclosure are restricted solely to the terms in
 *  the corresponding written license agreement. It shall not be 
 *  disclosed to anyone other than valid licensees without
 *  written permission of Availink, Inc.
 *
 */


///
/// @file
/// @brief Implements the functions declared in IBSP.h. 
/// 
//$Revision: 563 $ 
//$Date: 2008-09-24 17:24:29 -0400 (Wed, 24 Sep 2008) $
// 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
//#include <fapi/sys_services.h>
//#include "../../src/fe_priv.h"
#include "IBSP.h"
#include <semaphore.h>
/// Please use the following table of PLL configurations if 55 Msps symbol rate is desired at the demod end. 

///	{503,  1, 6, 4, 2,  4000, 12600, 16800, 25200} < Reference clock 4 MHz,   Demod clock 126 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
///	,{447,  1, 6, 4, 2,  4500, 12600, 16800, 25200} < Reference clock 4.5 MHz, Demod clock 126 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
///	,{503,  4, 6, 4, 2, 10000, 12600, 16800, 25200} < Reference clock 10 MHz,  Demod clock 126 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
///	,{503,  7, 6, 4, 2, 16000, 12600, 16800, 25200} < Reference clock 16 MHz,  Demod clock 126 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
///	,{111,  2, 6, 4, 2, 27000, 12600, 16800, 25200} < Reference clock 27 MHz,  Demod clock 126 MHz, FEC clock 168 MHz, MPEG clock 252 MHz

/// Feel free to contact Availink Support staff for further infromation on this.

/// Users can remove unused elements from the following array to reduce the SDK footprint size.
const struct AVL_DVBSx_PllConf pll_conf[] =
{
	 {503,  1, 7, 4, 2,  4000, 11200, 16800, 25200} ///< Reference clock 4 MHz,   Demod clock 112 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
	,{447,  1, 7, 4, 2,  4500, 11200, 16800, 25200} ///< Reference clock 4.5 MHz, Demod clock 112 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
	,{503,  4, 7, 4, 2, 10000, 11200, 16800, 25200} ///< Reference clock 10 MHz,  Demod clock 112 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
	,{503,  7, 7, 4, 2, 16000, 11200, 16800, 25200} ///< Reference clock 16 MHz,  Demod clock 112 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
	,{111,  2, 7, 4, 2, 27000, 11200, 16800, 25200} ///< Reference clock 27 MHz,  Demod clock 112 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
};
const AVL_uint16 pll_array_size = sizeof(pll_conf)/sizeof(struct AVL_DVBSx_PllConf);
//FE_HANDLE A6211_i2cHandle = 0;//feHandleArray[0].A2108_i2cHandle;
uint8_t   A6211_i2cAddr   = 0;
uint8_t   A6211_feIndex   = 0;
AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_Initialize(void)
{
	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_Dispose(void)
{

	return(AVL_DVBSx_EC_OK);
}

void cs_sleepms(unsigned int msec)
{
	//does not interfere with signals like sleep and usleep do
	struct timespec req_ts;
	req_ts.tv_sec = msec/1000;
	req_ts.tv_nsec = (msec % 1000) * 1000000L;
	int32_t olderrno = errno;		// Some OS (especially MacOSX) seem to set errno to ETIMEDOUT when sleeping
	nanosleep (&req_ts, NULL);
	errno = olderrno;
}
AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_Delay( AVL_uint32 uiMS )
{
	cs_sleepms(uiMS);
	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_I2CRead(  const struct AVL_DVBSx_Chip * pAVLChip,  AVL_puchar pucBuff, AVL_puint16 puiSize )
{
	int err;
	int fd;

	fd = open("/dev/i2c-0",O_RDWR);	
	if(fd < 0){		
		printf("device open failed(%s)\n", strerror(errno));		
		return AVL_DVBSx_EC_I2CFail;		
	}
	err = ioctl(fd, I2C_SLAVE_FORCE, 0x0C);	
	if(err < 0){		
		printf("ioctl failed:%d\n",err);		
		err= AVL_DVBSx_EC_I2CFail;	
		goto i2c_read_err;
	}
	*puiSize=read(fd, pucBuff,*puiSize);
	//err = FAPI_I2C_Read(A6211_i2cHandle,(pAVLChip->m_SlaveAddr),&buffer,0,pucBuff,*puiSize);
	i2c_read_err:
	close(fd);
	if(*puiSize <= 0)
	{
		return AVL_DVBSx_EC_I2CFail;
	}
	return(AVL_DVBSx_EC_OK);
}
AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_I2CWrite(  const struct AVL_DVBSx_Chip * pAVLChip,  AVL_puchar pucBuff,  AVL_puint16  puiSize )
{
 	int err;
	int fd;

	fd = open("/dev/i2c-0",O_RDWR);	
	if(fd < 0){		
		printf("device open failed(%s)\n", strerror(errno));		
		return AVL_DVBSx_EC_I2CFail;		
	}
	err = ioctl(fd, I2C_SLAVE_FORCE, 0x0C);	
	if(err < 0){		
		printf("ioctl failed:%d\n",err);		
		err= AVL_DVBSx_EC_I2CFail;	
		goto i2c_write_err;
	}
	*puiSize=write(fd, pucBuff,*puiSize);
	i2c_write_err:
	close(fd);
    //err = FAPI_I2C_Write(A6211_i2cHandle,pAVLChip->m_SlaveAddr, pucBuff,*puiSize);
	if(*puiSize <= 0)
	{
		//printf("[AVL_DVBSx_IBSP_I2CWrite] fail...%d.\n",err);
		return AVL_DVBSx_EC_I2CFail;
	}
	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_InitSemaphore( AVL_psemaphore pSemaphore )
{
	sem_init(pSemaphore,0,1);
//*pSemaphore = (unsigned char)FAPI_SYS_CREATE_SEMAPHORE(1);
	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_WaitSemaphore( AVL_psemaphore pSemaphore )
{
//FAPI_SYS_GET_SEMAPHORE(*pSemaphore, FAPI_SYS_SUSPEND);
	sem_wait(pSemaphore);
	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_ReleaseSemaphore( AVL_psemaphore pSemaphore )
{
//FAPI_SYS_SET_SEMAPHORE(*pSemaphore, FAPI_SYS_NO_SUSPEND);
	sem_post(pSemaphore);
	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_DestroySemaphore( AVL_psemaphore pSemaphore )
{
	sem_destroy(pSemaphore);
	return(AVL_DVBSx_EC_OK);
}

