/**************************************************************************************************
Copyright (C), 1988-2008, Huawei Tech. Co., Ltd.
File name       :HfcpDefine.h
Author: [LiuYuchao(104103)]      Version: 1.00       Date:[2008-1-28 20:39:47]
Description     :
Others          :
--------------------------------------------------------------------------------
add by haolinjian 2009��11��24��
HFCP
 Huawei File Copy Protocol. Huawei's customized file transfer protocol.
 In ECC networking, IP networking and TP4 networking, you can back up or restore a database package through HFCP.

    �����׶�:
    �����ļ����ؿ�ʼ֡, ��Navigator����, ���͵�����SFTM_MLOADģ��,
    ����SFTM_MLOADģ����Ҫ���ݲ����Ĳ�ͬ���������ת��,�ⲿ����Ҫͬsftm-copy-rfile�������һ��,
    ����˵, �����ݿ�������ؽ���SFTM_MNGģ�����, �����ؼ���SWDL����, �����ļ����ؽ���SFTM����
    (���ǽ���, ǰ��ֻʵ�����ݿⲿ��)
    ͬʱ����SFTM_MLOADģ��, ������һ�������ļ����ؿ�ʼ��Ӧ֡��Navigagor,
    Navigator���ݸ���Ӧ֡, ���������״̬��, ��������Ϊ���ر����˵�Ҫ��

    ���̽��н׶�:
    �����ļ������صķ���, ��SFTM_MLOADģ������ṩAPI�ӿ�ʵ��, �м���Ҫʵ���ź�������.
    �����ļ������صĽ���֡, Ҳ��SFTM_MLOADģ������ṩAPI�ӿ�ʵ��, ��Ҫ��֪Navigator�ɹ�����ʧ��, �Լ�������Ϣ
    ע��: ����Navigator�����������ʧ�ܵ����, ����ɾ��ԭ���Ѿ����ص��ļ�

    �����׶�
    ������Navigator�����������ݵĽ���֡, ��SFTM_MLOAD�����ṩAPI�ӿ�ʵ��, ֻ��Ҫ��֪�ɹ�����ʧ��    
    Navigator���������ͽ���֡����Ӧ
**************************************************************************************************/

#ifndef __BRBASEDEFINE_H__
#define __BRBASEDEFINE_H__


//#include "../filecopydef.h"


//���ڱ�ʶʧ��
const int ERRORCODE_FAILED = -1; 

// HFCP Э�� ��Ϣ���ؼ���
#define HFCP_KEY_WORD "@SBS"

// ��Ϣ�ؼ��ֳ���
const int HFCP_KEY_WORD_LEN = 4;

// HFCP Э�� ��ʾû���ļ�
#define HFCP_NONE_FILES "none"

// �ļ�����󳤶ȣ�����·����
const  USHORT BRBASE_FILENAME_MAX = 0xff;

// ��������Ԫ�෵�ص��ļ�������󳤶�
const  USHORT BRBASE_NEFILENAME_MAX = 0x14;

// HFCP Э�� ���峤�� 800
const  USHORT HFCP_PACKET_LEN = 0x0320;

// HFCP Э��Ĭ��ʹ�õ�״̬�������������������ʱ��෢�ͼ�������

//const USHORT HFCP_UPLOAD_USEDSTATES_COUNT = 0x08;
const  USHORT HFCP_UPLOAD_USEDSTATES_COUNT = 0x0a;
const  USHORT HFCP_DOWNLOAD_USEDSTATES_COUNT = 0x01;
const  USHORT HFCP_STATE_MACHINE_MAX = 0x20;    // ����ͬʱ���͵�������

// HFCP Э�� �������ݳ��� 128
const  USHORT HFCP_ERRORNUM_OFFSET = 0x80;

// ���ݷ��ͺ� 5 ��û���յ���Ӧ�����·���
const  USHORT HFCP_INTERVAL_TIME = 5;

// ÿ������������ԵĴ���
const  USHORT HFCP_DATA_RETRY = 10;

// ����дFlashʱ��ϳ�����˳�ʱʱ��ҲӦ�ñ�һ�㳤һЩ
const  USHORT HFCP_FLASH_RETRY = 24;

// ��Ԫ���سɹ���Ӧ
const  USHORT HFCP_PACK_SUCCESS = 0x0000; //�ɹ�

const  USHORT HFCP_SERIAL_NUM_ZERO = 0x0000;// �����Ϊ0
// û�к������ݰ���
const  USHORT HFCP_END_TAG      = 0xff01;

// ���к������ݰ�
const  USHORT HFCP_CONTINUE_TAG = 0xff00;

// ���ݿ������ͳɹ��󾭹������뿪ʼ����״̬
const  UINT HFCP_TEST_START    = 0x64; // 100���ʼ���///�����ò���
const  UINT HFCP_TEST_INTERVAL = 0x14;  // �������20��
const  UINT HFCP_TEST_TIMEOUT  = 0xb4;  // 180���ʱ

//const  USHORT HFCP_FILE_TIMEOUT = 1800;
/* BEGIN: Added by dengxuefeng 00119018, 2010/1/7 */
const  USHORT HFCP_FILE_TIMEOUT = 18000;
/* END:   Added by dengxuefeng 00119018, 2010/1/7 */


const  int ERROR_SIZE = 256;// ������Ϣ����

// ����/����ö�٣�����ǰ̨���ã����� ���غ����أ�
enum BRBASE_LOAD_TYPE
{
    BRBASE_UPLOAD_TYPE      = 1, // ����
    BRBASE_DOWNLOAD_TYPE     = 2  // ����
};

// ö������ʹ�� HFCP Э��Ĺ��ܣ�
// ��������䵽 HFCP ���ݰ��İ�����
enum HFCP_FUNC_TYPE
{
    HFCP_UNKNOWN_FUNC     = 0x00, // δ֪����ģ��
    DBP_RECOVER_FUNC      = 0x02, // ���ݿ������
    DBP_BACKUP_FUNC       = 0x03, // ���ݿ������
    SECU_BACKUP_FUNC      = 0x08  // ��ȫ��־����
};

//����״̬����״̬
enum HFCP_PACKET_STATE
{
    HFCP_STOPPED_STATE     = 0, // ֹ̬ͣ
    HFCP_RUNNING_STATE     = 1, // ����̬
    HFCP_HALTED_STATE      = 2  // ����̬
};

//���״̬���ǵ���״̬
enum HFCP_MS_STATE
{
    HFCP_MULTI_STATE      = 0x01, // ���״̬
    HFCP_SINGLE_STATE     = 0x02  // ����״̬
};

/* BEGIN: Added by dengxuefeng 00119018, 2010/1/7 */
//�ļ����䷽ʽ
enum HFCP_TRANS_TYPE
{
    HFCP_SINGLETRANS_STATE      = 0x01, // ���ļ�
    HFCP_BATCHTRANS_STATE     = 0x02  // �����ļ�
};
/* END:   Added by dengxuefeng 00119018, 2010/1/7 */

// HFCP Э�� ��Ϣ����ͷ��ʽ
typedef struct HFCPProtHeader
{
    char   szHeader[4];            // ��Ϣͷ (�̶�Ϊ@SBS)
    USHORT usPackType;             // ��Ϣ������
    USHORT usSerialNum;            // ��Ϣ�����
    USHORT usResult;               // �ɹ�/ʧ�ܱ��
    USHORT usEndTag;               // ����/�������
    UINT   uDataLen;               // ��Ϣ��ĳ���
    UINT   uBufAddr;               // ��д�ļ��ĵ�ǰ��ַ
    UINT   uBufLen;                // �ļ�����
    UINT   uBodyCheckSum;          // ��Ϣ��У��
    UINT   uHeaderCheckSum;        // ��ϢͷУ��
    void ToHostOrder()
    {
        usPackType      = ntohs(usPackType);
        usSerialNum     = ntohs(usSerialNum);
        usResult        = ntohs(usResult);
        usEndTag        = ntohs(usEndTag);
        uDataLen        = ntohl(uDataLen);
        uBufAddr        = ntohl(uBufAddr);
        uBufLen         = ntohl(uBufLen);
        uBodyCheckSum   = ntohl(uBodyCheckSum);
        uHeaderCheckSum = ntohl(uHeaderCheckSum);
    };
    void ToNetOrder()
    {
        usPackType  = htons(usPackType);
        usSerialNum = htons(usSerialNum);
        usResult    = htons(usResult);
        usEndTag    = htons(usEndTag);
        uDataLen    = htonl(uDataLen);
        uBufAddr    = htonl(uBufAddr);
        uBufLen     = htonl(uBufLen);

    } 
} HFCP_PROT_HEADER, *PHFCP_PROT_HEADER;


// HFPC Э���ѯ�ļ�������
enum HFCP_NEXT_FILE
{
    SFTM_DBP_NO_FILE       = 0,          // �ļ����ؽ���
    SFTM_DBP_FILE_READY    = 1,          // �ļ�׼������
    SFTM_DBP_FILE_NOTREADY = 2,          // �ļ���û׼����
    SFTM_DBP_ERROR         = 3           // �ļ��������쳣
};

//************�����ڴ�������Э��������*****************//
const int PROT_SFTM_MEMFILE_LOAD_STOP            = 0x00;        // �ļ�������ֹͣ
const int PROT_SFTM_MEMFILE_DOWN_LOAD_START      = 0x40;        // �����ڴ��ļ����ݿ�ʼ����֡
const int PROT_SFTM_MEMFILE_DOWN_LOAD_START_ACK  = 0x41;        // �����ڴ��ļ����ݿ�ʼ����ȷ��֡
const int PROT_SFTM_MEMFILE_UP_LOAD_START        = 0x42;        // �����ڴ��ļ����ݿ�ʼ����֡
const int PROT_SFTM_MEMFILE_UP_LOAD_START_ACK    = 0x43;        // �����ڴ��ļ����ݿ�ʼ����ȷ��֡
const int PROT_SFTM_MEMFILE_DOWN_LOAD_DATA       = 0x44;        // �����ڴ��ļ�����֡
const int PROT_SFTM_MEMFILE_DOWN_LOAD_DATA_ACK   = 0x45;        // �����ڴ��ļ�����ȷ��֡
const int PROT_SFTM_MEMFILE_UP_LOAD_DATA          = 0x46;        // �����ڴ��ļ�������������֡
const int PROT_SFTM_MEMFILE_UP_LOAD_DATA_ACK      = 0x47;        // �����ڴ��ļ���������ȷ��֡
const int PROT_SFTM_MEMFILE_DOWN_LOAD_END        = 0x48;        // �����ڴ��ļ����ݽ�������֡
const int PROT_SFTM_MEMFILE_DOWN_LOAD_END_ACK    = 0x49;        // �����ڴ��ļ����ݽ�������ȷ��֡
const int PROT_SFTM_MEMFILE_UP_LOAD_END          = 0x50;        // �����ڴ��ļ����ݽ�������֡
const int PROT_SFTM_MEMFILE_UP_LOAD_END_ACK      = 0x51;        // �����ڴ��ļ����ݽ�������ȷ��֡

const int PROT_SFTM_MEMFILE_UP_LOAD_RTRV         = 0x52;        // �����ڴ��ļ�����״̬��ѯ����֡
const int PROT_SFTM_MEMFILE_UP_LOAD_RTRV_ACK     = 0x53;        // �����ڴ��ļ�����״̬��ѯ����ȷ��֡
const int PROT_SFTM_MEMFILE_DOWN_LOAD_RTRV       = 0x54;        // �����ڴ��ļ�����״̬��ѯ����֡
const int PROT_SFTM_MEMFILE_DOWN_LOAD_RTRV_ACK   = 0x55;        // �����ڴ��ļ�����״̬��ѯ����ȷ��֡

//****************�����������ص����Э����*************//
//========= ����Э���� =========
const int PROT_SFTM_MEMFILE_BATCH_DOWNLOAD_START        = 0x60;   // ���������ļ���ʼ����֡(���ܲ෢��)
const int PROT_SFTM_MEMFILE_BATCH_DOWNLOAD_START_ACK    = 0x61;   // ���������ļ���ʼ������Ӧ֡

const int PROT_SFTM_MEMFILE_SINGLE_DOWNLOAD_START       = 0x62;   // �������󵥸��ļ����ؿ�ʼ֡(��������)
const int PROT_SFTM_MEMFILE_SINGLE_DOWNLOAD_END         = 0x63;   // �������󵥸��ļ����ؽ���֡(��������)

const int PROT_SFTM_MEMFILE_BATCH_DOWNLOAD_END          = 0x64;   // ���������ļ���������֡(��������)
const int PROT_SFTM_MEMFILE_BATCH_DOWNLOAD_END_ACK      = 0x65;   // ���������ļ�����������Ӧ֡

//========= ����Э���� =========
const int PROT_SFTM_MEMFILE_BATCH_UPLOAD_START          = 0x66;   // ���������ļ���ʼ����֡(���ܲ෢��)
const int PROT_SFTM_MEMFILE_BATCH_UPLOAD_START_ACK      = 0x67;   // ���������ļ���ʼ������Ӧ֡

const int PROT_SFTM_MEMFILE_SINGLE_UPLOAD_START         = 0x68;   // �������󵥸��ļ����ؿ�ʼ֡(��������)
const int PROT_SFTM_MEMFILE_SINGLE_UPLOAD_END           = 0x69;   // �������󵥸��ļ����ؽ���֡(��������)

const int PROT_SFTM_MEMFILE_BATCH_UPLOAD_END            = 0x6A;   // ���������ļ���������֡(��������)
const int PROT_SFTM_MEMFILE_BATCH_UPLOAD_END_ACK        = 0x6B;   // ���������ļ�����������Ӧ֡///��������ò���

//======== ���������ع����¼�Э���� ========
//const int PROC_SFTM_MEMFILE_BATCH_EVENT                 = 0x6C;   //�������ع��̽���Э����

//======== ����������ȡ��Э���� ==========
const int PROC_SFTM_MEMFILE_BATCH_ABORTCMD              = 0x6D;   //����������ȡ��Э����

//======== ����������û�п�ʼЭ���� ==========
//const int PROC_SFTM_MEMFILE_STOPPED               = 0x00;   //����������û�п�ʼЭ����
const int PROC_SFTM_MEMFILE_BATCH_HALTED                = 0x01;   //�����������Ѿ�����Э����

//======== HFCP�������ع�������Ԫ������ ==========
const int HFCP_ERROR_BASE               = 0x8000; //HFCPЭ��Ĵ�������ʼֵ
const int HFCP_ERROR_HEAD_CHECKERR      = 0x8001; //ͷ�ṹУ�����
const int HFCP_ERROR_BODY_CHECKERR      = 0x8002; //��������У�����
const int HFCP_ERROR_FILE_DOWNLOADING   = 0x8003;  // �ļ���������
const int HFCP_ERROR_FILE_UPLOADING     = 0x8004;  // �ļ���������
const int HFCP_ERROR_FILE_OPEN          = 0x8005;  // �ļ���ʧ��
const int HFCP_ERROR_FILE_WRITE         = 0x8006; // �ļ�дʧ��
const int HFCP_ERROR_FILE_READ          = 0x8007; // �ļ���ʧ��
const int HFCP_ERROR_FILE_CLOSE         = 0x8008; // �ļ��ر�ʧ��
const int HFCP_ERROR_DOWNLOAD_STATUS    = 0x8009; // ����״̬����
const int HFCP_ERROR_UPLOAD_STATUS      = 0x800a; // ����״̬����
const int HFCP_ERROR_PACKNO             = 0x800b; //����Ŵ���



#endif __BRBASEDEFINE_H__
