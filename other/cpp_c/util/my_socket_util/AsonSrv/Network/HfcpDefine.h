/**************************************************************************************************
Copyright (C), 1988-2008, Huawei Tech. Co., Ltd.
File name       :HfcpDefine.h
Author: [LiuYuchao(104103)]      Version: 1.00       Date:[2008-1-28 20:39:47]
Description     :
Others          :
--------------------------------------------------------------------------------
add by haolinjian 2009年11月24日
HFCP
 Huawei File Copy Protocol. Huawei's customized file transfer protocol.
 In ECC networking, IP networking and TP4 networking, you can back up or restore a database package through HFCP.

    启动阶段:
    批量文件下载开始帧, 由Navigator发起, 发送到主机SFTM_MLOAD模块,
    主机SFTM_MLOAD模块需要根据参数的不同进行命令的转发,这部分需要同sftm-copy-rfile处理基本一致,
    比如说, 对数据库的上下载交给SFTM_MNG模块出来, 包加载加载SWDL处理, 单个文件加载交给SFTM处理
    (考虑进度, 前期只实现数据库部分)
    同时主机SFTM_MLOAD模块, 将回送一个批量文件下载开始响应帧给Navigagor,
    Navigator根据该响应帧, 调整自身的状态机, 以满足作为下载被动端的要求

    过程进行阶段:
    单个文件上下载的发起, 由SFTM_MLOAD模块对外提供API接口实现, 中间需要实现信号量互斥.
    单个文件上下载的结束帧, 也由SFTM_MLOAD模块对外提供API接口实现, 需要告知Navigator成功或者失败, 以及进度信息
    注意: 对于Navigator如果遇到上载失败的情况, 不会删除原来已经上载的文件

    结束阶段
    主机向Navigator发起批量备份的结束帧, 由SFTM_MLOAD对外提供API接口实现, 只需要告知成功或者失败    
    Navigator向主机回送结束帧的响应
**************************************************************************************************/

#ifndef __BRBASEDEFINE_H__
#define __BRBASEDEFINE_H__


//#include "../filecopydef.h"


//用于标识失败
const int ERRORCODE_FAILED = -1; 

// HFCP 协议 消息包关键字
#define HFCP_KEY_WORD "@SBS"

// 消息关键字长度
const int HFCP_KEY_WORD_LEN = 4;

// HFCP 协议 表示没有文件
#define HFCP_NONE_FILES "none"

// 文件名最大长度（包含路径）
const  USHORT BRBASE_FILENAME_MAX = 0xff;

// 保留的网元侧返回的文件名的最大长度
const  USHORT BRBASE_NEFILENAME_MAX = 0x14;

// HFCP 协议 包体长度 800
const  USHORT HFCP_PACKET_LEN = 0x0320;

// HFCP 协议默认使用的状态机个数，即：多包发送时最多发送几包数据

//const USHORT HFCP_UPLOAD_USEDSTATES_COUNT = 0x08;
const  USHORT HFCP_UPLOAD_USEDSTATES_COUNT = 0x0a;
const  USHORT HFCP_DOWNLOAD_USEDSTATES_COUNT = 0x01;
const  USHORT HFCP_STATE_MACHINE_MAX = 0x20;    // 允许同时发送的最多包数

// HFCP 协议 错误内容长度 128
const  USHORT HFCP_ERRORNUM_OFFSET = 0x80;

// 数据发送后 5 秒没有收到响应则重新发送
const  USHORT HFCP_INTERVAL_TIME = 5;

// 每包数据最多重试的次数
const  USHORT HFCP_DATA_RETRY = 10;

// 由于写Flash时间较长，因此超时时间也应该比一般长一些
const  USHORT HFCP_FLASH_RETRY = 24;

// 网元返回成功响应
const  USHORT HFCP_PACK_SUCCESS = 0x0000; //成功

const  USHORT HFCP_SERIAL_NUM_ZERO = 0x0000;// 包序号为0
// 没有后续数据包了
const  USHORT HFCP_END_TAG      = 0xff01;

// 还有后续数据包
const  USHORT HFCP_CONTINUE_TAG = 0xff00;

// 数据库包命令发送成功后经过多少秒开始测试状态
const  UINT HFCP_TEST_START    = 0x64; // 100秒后开始检测///考虑用不用
const  UINT HFCP_TEST_INTERVAL = 0x14;  // 检测周期20秒
const  UINT HFCP_TEST_TIMEOUT  = 0xb4;  // 180秒后超时

//const  USHORT HFCP_FILE_TIMEOUT = 1800;
/* BEGIN: Added by dengxuefeng 00119018, 2010/1/7 */
const  USHORT HFCP_FILE_TIMEOUT = 18000;
/* END:   Added by dengxuefeng 00119018, 2010/1/7 */


const  int ERROR_SIZE = 256;// 错误信息长度

// 上载/下载枚举（用于前台设置，包括 上载和下载）
enum BRBASE_LOAD_TYPE
{
    BRBASE_UPLOAD_TYPE      = 1, // 上载
    BRBASE_DOWNLOAD_TYPE     = 2  // 下载
};

// 枚举所有使用 HFCP 协议的功能，
// 该数据填充到 HFCP 数据包的包体中
enum HFCP_FUNC_TYPE
{
    HFCP_UNKNOWN_FUNC     = 0x00, // 未知功能模块
    DBP_RECOVER_FUNC      = 0x02, // 数据库包下载
    DBP_BACKUP_FUNC       = 0x03, // 数据库包上载
    SECU_BACKUP_FUNC      = 0x08  // 安全日志上载
};

//单包状态机的状态
enum HFCP_PACKET_STATE
{
    HFCP_STOPPED_STATE     = 0, // 停止态
    HFCP_RUNNING_STATE     = 1, // 运行态
    HFCP_HALTED_STATE      = 2  // 挂起态
};

//多包状态还是单包状态
enum HFCP_MS_STATE
{
    HFCP_MULTI_STATE      = 0x01, // 多包状态
    HFCP_SINGLE_STATE     = 0x02  // 单包状态
};

/* BEGIN: Added by dengxuefeng 00119018, 2010/1/7 */
//文件传输方式
enum HFCP_TRANS_TYPE
{
    HFCP_SINGLETRANS_STATE      = 0x01, // 单文件
    HFCP_BATCHTRANS_STATE     = 0x02  // 批量文件
};
/* END:   Added by dengxuefeng 00119018, 2010/1/7 */

// HFCP 协议 消息包包头格式
typedef struct HFCPProtHeader
{
    char   szHeader[4];            // 消息头 (固定为@SBS)
    USHORT usPackType;             // 消息包类型
    USHORT usSerialNum;            // 消息包序号
    USHORT usResult;               // 成功/失败标记
    USHORT usEndTag;               // 继续/结束标记
    UINT   uDataLen;               // 消息体的长度
    UINT   uBufAddr;               // 读写文件的当前地址
    UINT   uBufLen;                // 文件长度
    UINT   uBodyCheckSum;          // 消息体校验
    UINT   uHeaderCheckSum;        // 消息头校验
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


// HFPC 协议查询文件的类型
enum HFCP_NEXT_FILE
{
    SFTM_DBP_NO_FILE       = 0,          // 文件上载结束
    SFTM_DBP_FILE_READY    = 1,          // 文件准备就绪
    SFTM_DBP_FILE_NOTREADY = 2,          // 文件还没准备好
    SFTM_DBP_ERROR         = 3           // 文件上载有异常
};

//************新增内存上下载协议命令字*****************//
const int PROT_SFTM_MEMFILE_LOAD_STOP            = 0x00;        // 文件上下载停止
const int PROT_SFTM_MEMFILE_DOWN_LOAD_START      = 0x40;        // 下载内存文件数据开始命令帧
const int PROT_SFTM_MEMFILE_DOWN_LOAD_START_ACK  = 0x41;        // 下载内存文件数据开始命令确认帧
const int PROT_SFTM_MEMFILE_UP_LOAD_START        = 0x42;        // 上载内存文件数据开始命令帧
const int PROT_SFTM_MEMFILE_UP_LOAD_START_ACK    = 0x43;        // 上载内存文件数据开始命令确认帧
const int PROT_SFTM_MEMFILE_DOWN_LOAD_DATA       = 0x44;        // 下载内存文件数据帧
const int PROT_SFTM_MEMFILE_DOWN_LOAD_DATA_ACK   = 0x45;        // 下载内存文件数据确认帧
const int PROT_SFTM_MEMFILE_UP_LOAD_DATA          = 0x46;        // 上载内存文件数据请求命令帧
const int PROT_SFTM_MEMFILE_UP_LOAD_DATA_ACK      = 0x47;        // 上载内存文件数据请求确认帧
const int PROT_SFTM_MEMFILE_DOWN_LOAD_END        = 0x48;        // 下载内存文件数据结束命令帧
const int PROT_SFTM_MEMFILE_DOWN_LOAD_END_ACK    = 0x49;        // 下载内存文件数据结束命令确认帧
const int PROT_SFTM_MEMFILE_UP_LOAD_END          = 0x50;        // 上载内存文件数据结束命令帧
const int PROT_SFTM_MEMFILE_UP_LOAD_END_ACK      = 0x51;        // 上载内存文件数据结束命令确认帧

const int PROT_SFTM_MEMFILE_UP_LOAD_RTRV         = 0x52;        // 上载内存文件数据状态查询命令帧
const int PROT_SFTM_MEMFILE_UP_LOAD_RTRV_ACK     = 0x53;        // 上载内存文件数据状态查询命令确认帧
const int PROT_SFTM_MEMFILE_DOWN_LOAD_RTRV       = 0x54;        // 下载内存文件数据状态查询命令帧
const int PROT_SFTM_MEMFILE_DOWN_LOAD_RTRV_ACK   = 0x55;        // 下载内存文件数据状态查询命令确认帧

//****************关于批量下载的相关协议字*************//
//========= 下载协议字 =========
const int PROT_SFTM_MEMFILE_BATCH_DOWNLOAD_START        = 0x60;   // 批量下载文件开始命令帧(网管侧发起)
const int PROT_SFTM_MEMFILE_BATCH_DOWNLOAD_START_ACK    = 0x61;   // 批量下载文件开始命令响应帧

const int PROT_SFTM_MEMFILE_SINGLE_DOWNLOAD_START       = 0x62;   // 主机请求单个文件下载开始帧(主机发起)
const int PROT_SFTM_MEMFILE_SINGLE_DOWNLOAD_END         = 0x63;   // 主机请求单个文件下载结束帧(主机发起)

const int PROT_SFTM_MEMFILE_BATCH_DOWNLOAD_END          = 0x64;   // 批量下载文件结束命令帧(主机发起)
const int PROT_SFTM_MEMFILE_BATCH_DOWNLOAD_END_ACK      = 0x65;   // 批量下载文件结束命令响应帧

//========= 上载协议字 =========
const int PROT_SFTM_MEMFILE_BATCH_UPLOAD_START          = 0x66;   // 批量上载文件开始命令帧(网管侧发起)
const int PROT_SFTM_MEMFILE_BATCH_UPLOAD_START_ACK      = 0x67;   // 批量上载文件开始命令响应帧

const int PROT_SFTM_MEMFILE_SINGLE_UPLOAD_START         = 0x68;   // 主机请求单个文件上载开始帧(主机发起)
const int PROT_SFTM_MEMFILE_SINGLE_UPLOAD_END           = 0x69;   // 主机请求单个文件上载结束帧(主机发起)

const int PROT_SFTM_MEMFILE_BATCH_UPLOAD_END            = 0x6A;   // 批量上载文件结束命令帧(主机发起)
const int PROT_SFTM_MEMFILE_BATCH_UPLOAD_END_ACK        = 0x6B;   // 批量上载文件结束命令响应帧///这个可能用不到

//======== 批量上下载过程事件协议字 ========
//const int PROC_SFTM_MEMFILE_BATCH_EVENT                 = 0x6C;   //批量下载过程进度协议字

//======== 批量上下载取消协议字 ==========
const int PROC_SFTM_MEMFILE_BATCH_ABORTCMD              = 0x6D;   //批量上下载取消协议字

//======== 批量上下载没有开始协议字 ==========
//const int PROC_SFTM_MEMFILE_STOPPED               = 0x00;   //批量上下载没有开始协议字
const int PROC_SFTM_MEMFILE_BATCH_HALTED                = 0x01;   //批量上下载已经挂起协议字

//======== HFCP上载下载过程中网元错误码 ==========
const int HFCP_ERROR_BASE               = 0x8000; //HFCP协议的错误码起始值
const int HFCP_ERROR_HEAD_CHECKERR      = 0x8001; //头结构校验出错
const int HFCP_ERROR_BODY_CHECKERR      = 0x8002; //数据内容校验出错
const int HFCP_ERROR_FILE_DOWNLOADING   = 0x8003;  // 文件正在下载
const int HFCP_ERROR_FILE_UPLOADING     = 0x8004;  // 文件正在上载
const int HFCP_ERROR_FILE_OPEN          = 0x8005;  // 文件打开失败
const int HFCP_ERROR_FILE_WRITE         = 0x8006; // 文件写失败
const int HFCP_ERROR_FILE_READ          = 0x8007; // 文件读失败
const int HFCP_ERROR_FILE_CLOSE         = 0x8008; // 文件关闭失败
const int HFCP_ERROR_DOWNLOAD_STATUS    = 0x8009; // 下载状态错误
const int HFCP_ERROR_UPLOAD_STATUS      = 0x800a; // 上载状态错误
const int HFCP_ERROR_PACKNO             = 0x800b; //包序号错误



#endif __BRBASEDEFINE_H__
