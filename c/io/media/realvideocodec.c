/***************************************************************
* @file realvideocodec.c
* rmvideo decoder using the RealNetwork Binary DLLs
*
* Implementation of RealNetwork video interface wrapper for FFmpeg
* Created by [email=dev@fastreaming.com]dev@fastreaming.com[/email],2007/04/25--2007/05/02
*
* TODO:
*      Deinterlace?
**************************************************************/

#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include "./libavcodec/avcodec.h"
#include <stdio.h>
#include <string.h>

#ifdef WIN32
typedef HRESULT (WINAPI *PRVCustomMessage)(void *, uint32_t);
typedef HRESULT (WINAPI *PRVFree)(uint32_t);
typedef HRESULT (WINAPI *PRVInit)(void *, uint32_t *dwCookie);
typedef HRESULT (WINAPI *PRVTransform)(uint8_t *, uint8_t *, void *, void *, uint32_t);
#else
typedef unsigned long (*PRVCustomMessage)(void *, uint32_t);
typedef unsigned long (*PRVFree)(uint32_t);
typedef unsigned long (*PRVInit)(void *, uint32_t *dwCookie);
typedef unsigned long (*PRVTransform)(uint8_t *, uint8_t *, void *, void *, uint32_t);
#endif


void ResizeHeight(uint8_t *pIn, uint32_t wi, uint32_t hi, uint8_t *pOut, uint32_t wo, uint32_t ho);
void ResizeRow(uint8_t *pIn, uint32_t wi, uint32_t dpi, uint8_t *pOut, uint32_t wo, uint32_t dpo);
void Resize(uint8_t *pIn, uint32_t wi, uint32_t hi, uint8_t *pOut, uint32_t wo, uint32_t ho);
void ResizeWidth(uint8_t *pIn, uint32_t wi, uint32_t hi, uint8_t *pOut, uint32_t wo, uint32_t ho);

typedef struct RealVideoDecContext {
	PRVCustomMessage RVCustomMessage;
	PRVFree RVFree;
	PRVInit RVInit;
	PRVTransform RVTransform;

#ifdef WIN32
	HMODULE m_hDrvDll;
#else
	void *m_hDrvDll;
#endif
	uint32_t m_dwCookie;
	uint8_t *m_pI420;
	uint8_t *m_pI420Temp;
	AVFrame frame;
} RealVideoDecContext;
//FIXME: redundant?
static uint32_t rm_swap(unsigned int x)
{
	x = ((x << 8) & 0xFF00FF00) | ((x >> 8) & 0x00FF00FF);
	x = (x >> 16) | (x << 16);
	return x;
}

void Resize(uint8_t *pIn, uint32_t wi, uint32_t hi, uint8_t *pOut, uint32_t wo, uint32_t ho)
{
	int si = wi * hi, so = wo * ho;

	//ASSERT(((si*so)&3) == 0);
	if(wi < wo) {
		ResizeWidth(pIn, wi, hi, pOut, wo, ho);
		ResizeWidth(pIn + si, wi / 2, hi / 2, pOut + so, wo / 2, ho / 2);
		ResizeWidth(pIn + si + si / 4, wi / 2, hi / 2, pOut + so + so / 4, wo / 2, ho / 2);

		if(hi == ho) {
			return;
		}

		ResizeHeight(pOut, wo, hi, pIn, wo, ho);
		ResizeHeight(pOut + so, wo / 2, hi / 2, pIn + so, wo / 2, ho / 2);
		ResizeHeight(pOut + so + so / 4, wo / 2, hi / 2, pIn + so + so / 4, wo / 2, ho / 2);
	} else if(hi < ho) {
		ResizeHeight(pIn, wi, hi, pOut, wo, ho);
		ResizeHeight(pIn + si, wi / 2, hi / 2, pOut + so, wo / 2, ho / 2);
		ResizeHeight(pIn + si + si / 4, wi / 2, hi / 2, pOut + so + so / 4, wo / 2, ho / 2);

		if(wi == wo) {
			return;
		}

		//ASSERT(0); // this is uncreachable code, but anyway... looks nice being so symmetric
		ResizeWidth(pOut, wi, ho, pIn, wo, ho);
		ResizeWidth(pOut + so, wi / 2, ho / 2, pIn + so, wo / 2, ho / 2);
		ResizeWidth(pOut + so + so / 4, wi / 2, ho / 2, pIn + so + so / 4, wo / 2, ho / 2);
	}
}

void ResizeWidth(uint8_t *pIn, uint32_t wi, uint32_t hi, uint8_t *pOut, uint32_t wo, uint32_t ho)
{
	uint32_t y;

	for( y = 0; y < hi; y++, pIn += wi, pOut += wo) {
		if(wi == wo) {
			memcpy(pOut, pIn, wo);
		} else {
			ResizeRow(pIn, wi, 1, pOut, wo, 1);
		}
	}
}

void ResizeHeight(uint8_t *pIn, uint32_t wi, uint32_t hi, uint8_t *pOut, uint32_t wo, uint32_t ho)
{
	if(hi == ho) {
		memcpy(pOut, pIn, wo * ho);
	} else {
		uint32_t x;

		for( x = 0; x < wo; x++, pIn++, pOut++) {
			ResizeRow(pIn, hi, wo, pOut, ho, wo);
		}
	}
}

void ResizeRow(uint8_t *pIn, uint32_t wi, uint32_t dpi, uint8_t *pOut, uint32_t wo, uint32_t dpo)
{
	//ASSERT(wi < wo);
	uint32_t i, j, dj;

	if(dpo == 1) {
		for( i = 0, j = 0, dj = (wi << 16) / wo; i < wo - 1; i++, pOut++, j += dj)
			//                        pOut[i] = pIn[j>>16];
		{
			uint8_t *p = &pIn[j >> 16];
			uint32_t jf = j & 0xffff;
			*pOut = (uint8_t)(((p[0] * (0xffff - jf) + p[1] * jf) + 0x7fff) >> 16);
		}

		*pOut = pIn[wi - 1];
	} else {
		for( i = 0, j = 0, dj = (wi << 16) / wo; i < wo - 1; i++, pOut += dpo, j += dj)
			//                        *pOut = pIn[dpi*(j>>16)];
		{
			uint8_t *p = &pIn[dpi * (j >> 16)];
			uint32_t jf = j & 0xffff;
			*pOut = (uint8_t)(((p[0] * (0xffff - jf) + p[dpi] * jf) + 0x7fff) >> 16);
		}

		*pOut = pIn[dpi * (wi - 1)];
	}
}

static int RealVideoDecoderInit(AVCodecContext *avctx)
{
	RealVideoDecContext *rvdec = avctx->priv_data;
	uint8_t *extra_data = avctx->extradata;
	unsigned int type1, type2, *pType ;
	uint8_t *pVideoUnknown;
	char buff[280] = "";
	int hr, size;
#pragma pack(push, 1)
	struct {
		unsigned short unk1, w, h, unk3;
		unsigned long unk2, subformat, unk5, format;
	} init;
#pragma pack(pop)
	//clean up
	pType = (unsigned int *)extra_data;
	type1 = rm_swap(*pType++);
	type2 = rm_swap(*pType++);
	pVideoUnknown = (uint8_t *)pType;
	init.unk1 = 11;
	init.w = avctx->width;
	init.h = avctx->height;
	init.unk3 = 0;
	init.unk2 = 0;
	init.subformat = type1;
	init.unk5 = 1;
	init.format = type2;
	rvdec->m_dwCookie = 0;
	rvdec->m_pI420 = NULL;
	rvdec->m_pI420Temp = NULL;
	//XXX: need probe more codecs

#ifdef WIN32
	strcpy(buff, "drvc.dll");
	rvdec->m_hDrvDll = LoadLibraryA(buff);

	if(!rvdec->m_hDrvDll) {
		av_log(avctx, AV_LOG_ERROR, "LoadLibraryA failed\n");
		return -1;
	}

	rvdec->RVCustomMessage = (PRVCustomMessage)GetProcAddress(rvdec->m_hDrvDll, "RV20toYUV420CustomMessage");
	rvdec->RVFree = (PRVFree)GetProcAddress(rvdec->m_hDrvDll, "RV20toYUV420Free");
	rvdec->RVInit = (PRVInit)GetProcAddress(rvdec->m_hDrvDll, "RV20toYUV420Init");
	rvdec->RVTransform = (PRVTransform)GetProcAddress(rvdec->m_hDrvDll, "RV20toYUV420Transform");

	if(!rvdec->RVCustomMessage) {
		rvdec->RVCustomMessage = (PRVCustomMessage)GetProcAddress(rvdec->m_hDrvDll, "RV40toYUV420CustomMessage");
	}

	if(!rvdec->RVFree) {
		rvdec->RVFree = (PRVFree)GetProcAddress(rvdec->m_hDrvDll, "RV40toYUV420Free");
	}

	if(!rvdec->RVInit) {
		rvdec->RVInit = (PRVInit)GetProcAddress(rvdec->m_hDrvDll, "RV40toYUV420Init");
	}

	if(!rvdec->RVTransform) {
		rvdec->RVTransform = (PRVTransform)GetProcAddress(rvdec->m_hDrvDll, "RV40toYUV420Transform");
	}

#else
	strcpy(buff, "./drvc.so");
	rvdec->m_hDrvDll = dlopen (buff, RTLD_LAZY);

	if(!rvdec->m_hDrvDll) {
		av_log(avctx, AV_LOG_ERROR, "Error in loading drvc.so: %s\n", dlerror());
		return -1;
	}

	rvdec->RVCustomMessage = dlsym(rvdec->m_hDrvDll, "RV20toYUV420CustomMessage");
	rvdec->RVFree = dlsym(rvdec->m_hDrvDll, "RV20toYUV420Free");
	rvdec->RVInit = dlsym(rvdec->m_hDrvDll, "RV20toYUV420Init");
	rvdec->RVTransform = dlsym(rvdec->m_hDrvDll, "RV20toYUV420Transform");

	if(!rvdec->RVCustomMessage) {
		rvdec->RVCustomMessage = dlsym(rvdec->m_hDrvDll, "RV40toYUV420CustomMessage");
	}

	if(!rvdec->RVFree) {
		rvdec->RVFree = dlsym(rvdec->m_hDrvDll, "RV40toYUV420Free");
	}

	if(!rvdec->RVInit) {
		rvdec->RVInit = dlsym(rvdec->m_hDrvDll, "RV40toYUV420Init");
	}

	if(!rvdec->RVTransform) {
		rvdec->RVTransform = dlsym(rvdec->m_hDrvDll, "RV40toYUV420Transform");
	}

#endif

	if(!rvdec->RVCustomMessage || !rvdec->RVFree || !rvdec->RVInit || !rvdec->RVTransform) {
		return -1;
	}

	hr = rvdec->RVInit(&init, &rvdec->m_dwCookie);

	if(hr < 0) {
		return -1;
	}

	if(avctx->codec_tag <= 0x30335652 && type2 >= 0x20200002) {
		int i;
		int nWidthHeight = (1 + ((type1 >> 16) & 7));
#pragma pack(push, 1)
		struct {
			uint32_t data1;
			uint32_t data2;
			uint32_t *dimensions;
		} cmsg_data =
		{0x24, nWidthHeight, NULL};
#pragma pack(pop)
		uint32_t *pWH = av_malloc(sizeof(uint32_t) * nWidthHeight * 2);
		pWH[0] = avctx->width;
		pWH[1] = avctx->height;

		for(i = 2; i < nWidthHeight * 2; i++) {
			pWH[i] = pVideoUnknown[i - 2] * 4;
		}

		cmsg_data.dimensions = pWH;
		hr = rvdec->RVCustomMessage(&cmsg_data, rvdec->m_dwCookie);
		av_free(pWH);
	}

	size = avctx->width * avctx->height;
	rvdec->m_pI420 = av_malloc(size * 3 / 2);
	rvdec->m_pI420Temp = av_malloc(size * 3 / 2);

	if(!rvdec->m_pI420 || !rvdec->m_pI420Temp) {
		return -1;
	}

	memset(rvdec->m_pI420, 0, size);
	memset(rvdec->m_pI420 + size, 128, size / 2);
	memset(rvdec->m_pI420Temp, 0, size);
	memset(rvdec->m_pI420Temp + size, 128, size / 2);
	rvdec->frame.data[0] = NULL;
	avctx->pix_fmt = PIX_FMT_YUV420P;
	return hr;
}

static int RealVideoDecoderTransform(AVCodecContext *avctx, void *outdata, int *outdata_size, uint8_t *buf, int buf_size)
{
	RealVideoDecContext *rvdec = avctx->priv_data;
	int hr;
	unsigned char *pDataIn = buf;
	int len = buf_size;
	int offset = 1 + ((*pDataIn) + 1) * 8;
	//FIXME?
	int64_t rtStart = avctx->timecode_frame_start;
#pragma pack(push, 1)
	struct {
		uint32_t len, unk1, chunks;
		uint32_t *extra;
		uint32_t unk2, timestamp;
	} transform_in =
	{len - offset, 0, *pDataIn, (uint32_t *)(pDataIn + 1), 0, (uint32_t)(rtStart / 10000)};
	struct {
		uint32_t unk1, unk2, timestamp, w, h;
	} transform_out =
	{0, 0, 0, 0, 0};
#pragma pack(pop)
	pDataIn += offset;

	hr = rvdec->RVTransform(pDataIn, (uint8_t *)rvdec->m_pI420, &transform_in, &transform_out, rvdec->m_dwCookie);

	if(hr >= 0) {
		uint8_t *pI420[3] = {rvdec->m_pI420, rvdec->m_pI420Temp, NULL};

		if(transform_out.w != avctx->width || transform_out.h != avctx->height) {
			//TODO: use libswscale
			Resize(pI420[0], transform_out.w, transform_out.h, pI420[1], avctx->width, avctx->height);

			// only one of these can be true, and when it happens the result image must be in the tmp buffer
			if(transform_out.w == avctx->width || transform_out.h == avctx->height) {
				pI420[2] = pI420[1], pI420[1] = pI420[0], pI420[0] = pI420[2];
			}
		}

		avpicture_fill((AVPicture *)&rvdec->frame, pI420[0], avctx->pix_fmt, avctx->width, avctx->height);
		*(AVFrame *)outdata = rvdec->frame;
		*outdata_size = sizeof(AVFrame);
		return buf_size;
	}

	return -1;
}
static int RealVideoDecoderClose(AVCodecContext *avctx)
{
	RealVideoDecContext *rvdec = avctx->priv_data;

	if(rvdec->m_dwCookie) {
		rvdec->RVFree(rvdec->m_dwCookie);
		rvdec->m_dwCookie = 0;
	}

	if(rvdec->m_pI420) {
		av_free(rvdec->m_pI420);
		rvdec->m_pI420 = NULL;
	}

	if(rvdec->m_pI420Temp) {
		av_free(rvdec->m_pI420Temp);
		rvdec->m_pI420Temp = NULL;
	}

	if(rvdec->m_hDrvDll) {
#ifdef WIN32
		FreeLibrary(rvdec->m_hDrvDll);
#else
		dlclose(rvdec->m_hDrvDll);
#endif
		rvdec->m_hDrvDll = 0;
	}

	return 0;
}
AVCodec binary_rv30_decoder = {
	"RealVideo30",
	CODEC_TYPE_VIDEO,
	CODEC_ID_RV30,
	sizeof(RealVideoDecContext),
	RealVideoDecoderInit,
	NULL,
	RealVideoDecoderClose,
	RealVideoDecoderTransform,
	CODEC_CAP_DR1,
};
AVCodec binary_rv40_decoder = {
	"RealVideo40",
	CODEC_TYPE_VIDEO,
	CODEC_ID_RV40,
	sizeof(RealVideoDecContext),
	RealVideoDecoderInit,
	NULL,
	RealVideoDecoderClose,
	RealVideoDecoderTransform,
	CODEC_CAP_DR1,
};

void register_rm_binary_codec(void)
{
	static int realvideo_register = 0;

	if(realvideo_register) {
		return;
	}

	register_avcodec(&binary_rv30_decoder);
	register_avcodec(&binary_rv40_decoder);
	realvideo_register = 1;
}
