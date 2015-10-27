// PythonEx.cpp: implementation of the CPythonEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\SevFrame.h"
#include "PythonEx.h"
#ifndef _DEBUG
#include <Python.h>


// #ifdef _DEBUG
// #undef THIS_FILE
// static char THIS_FILE[]=__FILE__;
// #define new DEBUG_NEW
// #endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


void  PythonException(string& strErrorMsg)
{
    if (!Py_IsInitialized())
    {
        strErrorMsg = "Python 运行环境没有初始化！";
        return;
    }
	
    if (PyErr_Occurred() != NULL)
    {
        PyObject *type_obj, *value_obj, *traceback_obj;
        PyErr_Fetch(&type_obj, &value_obj, &traceback_obj);
        if (value_obj == NULL)
            return;
		
        strErrorMsg = "";

        PyErr_NormalizeException(&type_obj, &value_obj, 0);

		if (PyUnicode_Check(PyObject_Str(value_obj)))
        {
			PyObject* MbcsStr_obj = PyUnicode_AsMBCSString(PyObject_Str(value_obj));
			strErrorMsg = PyBytes_AsString(MbcsStr_obj);
			Py_XDECREF(MbcsStr_obj);
			strErrorMsg += "\r\n";
        }

        if (traceback_obj != NULL)
        {
			PyRun_SimpleString("import sys");
			PyRun_SimpleString("sys.path.append('./')");
			PyObject * pTraceModule = PyImport_ImportModule("traceback");
            if (pTraceModule != NULL)
            {
                PyObject * pModuleDict = PyModule_GetDict(pTraceModule);
                if (pModuleDict != NULL)
                {
                    PyObject * pFunc = PyDict_GetItemString(pModuleDict,"format_exception");
                    if (pFunc != NULL)
                    {
                        PyObject * errList = PyObject_CallFunctionObjArgs(pFunc,type_obj,value_obj, traceback_obj,NULL);
                        if (errList != NULL)
                        {
                            int listSize = PyList_Size(errList);
                            for (int i=0;i < listSize;++i)
                            {
								PyObject* MbcsStr_obj = PyUnicode_AsMBCSString(PyList_GetItem(errList,i));
								strErrorMsg += PyBytes_AsString(MbcsStr_obj);
								strErrorMsg += "\r\n";
								Py_XDECREF(MbcsStr_obj);
							}
						}
					}
				}
				Py_XDECREF(pTraceModule);
            }
        }
		//::AfxMessageBox(strErrorMsg.c_str());
        Py_XDECREF(type_obj);
        Py_XDECREF(value_obj);
        Py_XDECREF(traceback_obj);
    }    
}


static PyObject* PyNeCmd(PyObject *self, PyObject *args)
{
    const char *command = NULL;
    if (!PyArg_ParseTuple(args, "s", &command))
         return(PyObject*)Py_None;

	string strRsp;

	CSevFrame* pSevFrame = CSevFrame::GetSrvFrame();
	if (strlen(command) > 0)
	{
		if (command[0] == ':')
		{
			pSevFrame->m_NeOperate.SendCmd(command, strRsp);
		}
		else
		{
			strRsp = pSevFrame->LocalCmd(command);
		}
	}
	
	PyMem_Free(&command);
	const char * pp = NULL;
	PyObject* pObject = PyUnicode_DecodeMBCS(strRsp.c_str(), strRsp.length(), pp);

	return pObject;
}


static PyObject* PyNeFileDialog(PyObject *self, PyObject *args)
{
    const char *pFilChars  = NULL;
    const char *pDefChars  = NULL;

	int iSign = 0;
	string strDefName;
	string strFilter;
	string strAllData;
    UINT uArgsSize = PyTuple_Size(args);
	cout<<"\r\nuArgsSize="<<uArgsSize<<"\r\n";
	//参数格式：打开另存标记\n默认名称\n过滤字符串
	if(uArgsSize == 0)
	{
		strAllData = "1\n\nAll Files (*.*)|*.*||";
	}
	else if (uArgsSize == 1)
	{
		if (!PyArg_ParseTuple(args, "i", &iSign))//Py_FileSystemDefaultEncoding
		{
			//PyMem_Free(&pFilterChars);
			Py_INCREF(Py_None); 
			return (PyObject*)Py_None;
		}
		iSign == 0 ? strAllData = "0\n\nAll Files (*.*)|*.*||" : strAllData = "1\n\nAll Files (*.*)|*.*||";
	}
	else if (uArgsSize == 2)
	{
		if (!PyArg_ParseTuple(args, "ies", &iSign, "MBCS", &pDefChars))//Py_FileSystemDefaultEncoding
		{
			//PyMem_Free(&pFilterChars);
			Py_INCREF(Py_None); 
			return (PyObject*)Py_None;
		}
		strDefName = pDefChars;
		iSign == 0 ? (strAllData = "0\n"+strDefName+"\nAll Files (*.*)|*.*||") : (strAllData = "1\n"+strDefName+"\nAll Files (*.*)|*.*||");
	}
	else if (uArgsSize == 3)
	{
		if (!PyArg_ParseTuple(args, "ieses", &iSign, "MBCS", &pDefChars, "MBCS", &pFilChars))//Py_FileSystemDefaultEncoding
		{
			//PyMem_Free(&pFilterChars);
			Py_INCREF(Py_None); 
			return (PyObject*)Py_None;
		}
		strDefName = pDefChars;
		strFilter = pFilChars;
		iSign == 0 ? (strAllData = "0\n"+strDefName+"\n" + strFilter) : (strAllData = "1\n"+strDefName+"\n" + strFilter);
		
	}
	

	string strRsp;
	CSevFrame* pSevFrame = CSevFrame::GetSrvFrame();
	pSevFrame->m_obPythonEx.m_strFilePath = "";
	pSevFrame->m_obTcpServer.Send(strAllData.c_str(), CMD_FILE_DIALOG, strAllData.length() + 1);

	pSevFrame->m_obPythonEx.m_DialogEvent.WaitEvent();
	strRsp = pSevFrame->m_obPythonEx.m_strFilePath;

	if (pDefChars != NULL)
		PyMem_Free(&pDefChars);
	if (pFilChars != NULL)
		PyMem_Free(&pFilChars);
	const char * pp = NULL;
	PyObject* pObject = PyUnicode_DecodeMBCS(strRsp.c_str(), strRsp.length(), pp);
	
	return pObject;
}

static PyObject* PyNeFolderDialog(PyObject *self, PyObject *args)
{
	string strRsp;
	CSevFrame* pSevFrame = CSevFrame::GetSrvFrame();
	pSevFrame->m_obPythonEx.m_strFilePath = "";
	pSevFrame->m_obTcpServer.Send("", CMD_FOLDER_DIALOG, 0);
	
	pSevFrame->m_obPythonEx.m_DialogEvent.WaitEvent();
	strRsp = pSevFrame->m_obPythonEx.m_strFilePath;
	
	const char * pp = NULL;
	PyObject* pObject = PyUnicode_DecodeMBCS(strRsp.c_str(), strRsp.length(), pp);
	
	return pObject;
}


static PyObject* PyNeUploadFile(PyObject *self, PyObject *args)
{
	const char *pArg1 = NULL;
	const char *pArg2 = NULL;
    if (!PyArg_ParseTuple(args, "eses", "MBCS", &pArg1, "MBCS", &pArg2))
		return(PyObject*)Py_None;
	
	CSevFrame* pSevFrame = CSevFrame::GetSrvFrame();
	pSevFrame->m_NeOperate.UploadFile(pArg1 , pArg2);
	//pSevFrame->m_obNeOper.UploadFile("/ofs1/hwx/NE.INI", "c:\\text.txt");
	
// 	if (pArg1 != NULL)
// 		PyMem_Free(&pArg1);
// 	if (pArg2 != NULL)
// 		PyMem_Free(&pArg2);
	Py_INCREF(Py_None); 
	return (PyObject*)Py_None;
}


static PyObject* PyNeArgs(PyObject *self, PyObject *args)
{
// 	const char *command = NULL;
//     if (!PyArg_ParseTuple(args, "es", "MBCS", &command))
//          return(PyObject*)Py_None;

	CSevFrame* pSevFrame = CSevFrame::GetSrvFrame();
	//cout<<"m_strArgs11="<<pSevFrame->m_obPythonEx.m_strArgs.c_str()<<"\r\n";
	const char * pp = NULL;
	PyObject* pObject = PyUnicode_DecodeMBCS(pSevFrame->m_obPythonEx.m_strArgs.c_str(), pSevFrame->m_obPythonEx.m_strArgs.length(), pp);
	return pObject;
}


static PyObject* PyNeDebug(PyObject *self, PyObject *args)
{
    char* strMsg = NULL;
	//int iLength;
	//if (!PyArg_ParseTuple(args, "es#", "MBCS", &strMsg, &iLength))//Py_FileSystemDefaultEncoding
	if (!PyArg_ParseTuple(args, "es", "MBCS", &strMsg))//Py_FileSystemDefaultEncoding
	{
		PyMem_Free(&strMsg);
		return (PyObject*)Py_None;
	}

	CSevFrame* pSevFrame = CSevFrame::GetSrvFrame();
	string strRet = strMsg;
	g_obFileLog.WriteLog("\r\n*************DEBUG BEGIN********************\r\n");
	g_obFileLog.WriteLog(strRet.c_str());
	g_obFileLog.WriteLog("\r\n**************DEBUG END*********************\r\n");
	pSevFrame->m_obTcpServer.Send(strRet.c_str(), CMD_PY_DEBUG, strRet.length() + 1);
	//cout<<"debug="<<strMsg<<"\r\n";
	PyMem_Free(&strMsg);
	Py_INCREF(Py_None); 
	return (PyObject*)Py_None;
}


static PyObject* PyNePrint(PyObject *self, PyObject *args)
{
	//cout<<"Print=1\r\n";
    char* strMsg = NULL;
	//int iLength;
    if (!PyArg_ParseTuple(args, "es", "MBCS", &strMsg))
	{
		//cout<<"Print=2\r\n";
		//PyMem_Free(&strMsg);
		return (PyObject*)Py_None;
	}
	//cout<<"Print=3\r\n";
	CSevFrame* pSevFrame = CSevFrame::GetSrvFrame();
	string strRet = strMsg;
	//cout<<"Print=4\r\n";
	g_obFileLog.WriteLog("\r\n*************PRINT BEGIN********************\r\n");
	g_obFileLog.WriteLog(strRet.c_str());
	g_obFileLog.WriteLog("\r\n**************PRINT END*********************\r\n");
	//cout<<"Print=5\r\n";
	pSevFrame->m_obTcpServer.Send(strRet.c_str(), CMD_PY_PRINT, strRet.length());
	//cout<<"Print=6\r\n";
	PyMem_Free(&strMsg);
	//cout<<"Print=7\r\n";
	Py_INCREF(Py_None); 
	return (PyObject*)Py_None;
}


static PyObject* PyNeResult(PyObject *self, PyObject *args)
{
    char* strMsg = NULL;
	int iLength;
    if (!PyArg_ParseTuple(args, "es#", "MBCS", &strMsg, &iLength))
	{
		PyMem_Free(&strMsg);
		return (PyObject*)Py_None;
	}

	CSevFrame* pSevFrame = CSevFrame::GetSrvFrame();
	pSevFrame->m_obPythonEx.m_strNeRelust = strMsg;
	
	PyMem_Free(&strMsg);
	Py_INCREF(Py_None); 
	return (PyObject*)Py_None;
}


static PyObject* PyNeAllGatewayIp(PyObject *self, PyObject *args)
{
	const char *strLocalIp = NULL;
	if (!PyArg_ParseTuple(args, "es", "MBCS", &strLocalIp))
		return(PyObject*)Py_None;
	
	CSevFrame* pSevFrame = CSevFrame::GetSrvFrame();
	string strAllNeIp;
	pSevFrame->m_obUdpClient.GetAllGipGroup(strLocalIp, strAllNeIp);
	const char * pp = NULL;
	PyObject* pObject = PyUnicode_DecodeMBCS(strAllNeIp.c_str(), strAllNeIp.length(), pp);
	return pObject;
}



//{"displayhook",    sys_displayhook, METH_O, displayhook_doc},
//{"exc_info",    sys_exc_info, METH_NOARGS, exc_info_doc},
//{"exc_clear",    sys_exc_clear, METH_NOARGS, exc_clear_doc},


static PyMethodDef PyExMethods[] = {
    {"NeCmd", PyNeCmd, METH_VARARGS, "Execute a Ne command."},
    {"NeArgs", PyNeArgs, METH_VARARGS, "Args to py."},
    {"NeDebug",  PyNeDebug, METH_VARARGS, "Shell debug."},
	{"NePrint",  PyNePrint, METH_VARARGS, "Print begin."},
	{"NeResult",  PyNeResult, METH_VARARGS, "Relust."},
	{"NeFileDialog",  PyNeFileDialog, METH_VARARGS, "File dialog."},
	{"NeFolderDialog",  PyNeFolderDialog, METH_VARARGS, "Folder dialog."},
	{"NeAllGatewayIp",  PyNeAllGatewayIp, METH_VARARGS, "AllGatewayIp."},
	{"NeUploadFile",  PyNeUploadFile, METH_VARARGS, "Upload file."},
    {NULL, NULL, 0, NULL}        /* Sentinel */

	//METH_NOARGS
	//METH_O
};


static struct PyModuleDef spammodule = {
   PyModuleDef_HEAD_INIT,
   "NePyEx",   /* name of module */
   NULL, /* module documentation, may be NULL */
   -1,       /* size of per-interpreter state of the module,
                or -1 if the module keeps state in global variables. */
   PyExMethods
};


PyMODINIT_FUNC PyInit_NePythonEx(void)
{
    return PyModule_Create(&spammodule);
}





CPythonEx::CPythonEx()
{
	m_strArgs = "";
	m_bThreadOpen = FALSE;
	//CreateProcessThread();
}

CPythonEx::~CPythonEx()
{
	//CloseProcess();
}

//PyThreadState* g_pMainThreadState;

UINT CPythonEx::CreatePython()
{
// 	struct	_inittab  dd[2];
// 	memset(&dd, '\0', sizeof(dd));
// 	dd[0].initfunc = PyInit_NePythonEx;
// 	dd[0].name = "NePyEx";
// 	PyImport_ExtendInittab(dd);

	

	Py_Initialize();
	int err = Py_IsInitialized();
	if (!err)  
	{
		return -1;
	}

	PyImport_AppendInittab("NePyEx", PyInit_NePythonEx);

	// 初始化线程支持
//	PyEval_InitThreads();

//	g_pMainThreadState = PyThreadState_Get();
//	PyGILState_STATE state;
//	state = PyGILState_Ensure( );


	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");
//	PyRun_SimpleString("from NeAssistant import *");
//	PyRun_SimpleString("sys.path.append('\"PYMODULE_PATH\"')");
	

//	PyEval_ReleaseThread(PyThreadState_Get());
//	PyEval_ReleaseLock();
//	CreateProcessThread();
//	PyGILState_Release( state );



	return err;
}



void CPythonEx::ReleasePython()
{
//	PyGILState_Ensure();
//	PyEval_ThreadsInitialized();
	Py_Finalize();
}

void CPythonEx::SetArgs(string strArgs)
{
	m_strArgs = strArgs;
}


//执行python模块
UINT CPythonEx::ExePyModule(const char* pModName)
{
	CSevFrame* pSevFrame = CSevFrame::GetSrvFrame();
 	pSevFrame->m_obTcpServer.Send("", CMD_PY_DEBUG_CLEAR, 0);//清除Debug信息


	PyObject* pModDict = PyImport_GetModuleDict();
	PyObject * pDictModule = PyDict_GetItemString(pModDict, pModName);
//	Py_XDECREF(pModDict);
		
	PyObject * pModule = NULL; 
	if (pDictModule == NULL)
	{
// 		string strModule = "import ";
// 		strModule += pModName;
// 		PyRun_SimpleString(strModule.c_str());

	//	state = PyGILState_Ensure( );
		pModule = PyImport_ImportModule(pModName);
	//	pModule = PyImport_ImportModuleEx("yintest");//pModName
	//	pModule = PyImport_AddModule("yintest");//pModName
	//	PyGILState_Release( state );
	}
	else
	{
		pModule = PyImport_ReloadModule(pDictModule);
	}

	
	if( pModule == NULL)
	{
		string strErrorMsg;
		PythonException(strErrorMsg);
//		pOutputDebug->ClearTextDebug();//清除Debug信息
//		pOutputDebug->AddTextDebug((char*)strErrorMsg.c_str());
//		pOutputDebug->SetCulFocus(1);
		g_obFileLog.WriteLog(strErrorMsg.c_str());
		pSevFrame->m_obTcpServer.Send(strErrorMsg.c_str(), CMD_PY_DEBUG, strErrorMsg.length() + 1);
		return 1;
	}
	
	Py_DECREF(pModule);
//	Py_XDECREF(pDictModule);
//	PyGILState_Release( state );
	return 0;
}


UINT CPythonEx::GetPyResult(char* pModName, string& strRelust)
{
	if ( ExePyModule(pModName) == 0 )
	{
		strRelust = m_strNeRelust;
		m_strNeRelust = "";
		return 0;
	}
	return 1;
}


// void CPythonEx::ExePyModuleAsy(const char* pModName)
// {
// 	string strModName = pModName;
// 	m_obCSection.Lock();
// 	m_qStrModule.push(strModName);
// 	m_obCSection.Unlock();
// }


// void CPythonEx::CreateProcessThread()
// {
// 	unsigned threadID;
// 	m_bThreadOpen = TRUE;
// 	m_hThread = (HANDLE)_beginthreadex( NULL, 0, &FunProcessThread, this, 0, &threadID );
// 	//等待线程运行
// 	Sleep(200);
// }
// 
// unsigned __stdcall CPythonEx::FunProcessThread( void* _this )
// {
// //	PyEval_AcquireLock();
// //	Py_NewInterpreter();
// //		PyEval_ReleaseLock();
//  	CPythonEx* pPythonEx = (CPythonEx*)_this;
// //	pPythonEx->CreatePython();
// 	while(pPythonEx->m_bThreadOpen)
// 	{
// 		if ( true == pPythonEx->m_qStrModule.empty() )
// 		{
// 			continue;
// 		}
// 
// 		string strModule;
// 		pPythonEx->m_obCSection.Lock();
// 		strModule = pPythonEx->m_qStrModule.front();
// 		pPythonEx->m_qStrModule.pop();
// 		pPythonEx->m_obCSection.Unlock();
// 
// 	//	PyGILState_STATE state;
// 	//	state = PyGILState_Ensure( );
// 		//CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
// 		PyEval_AcquireLock();
// //		PyInterpreterState* maininterp = g_pMainThreadState->interp;
// //		PyThreadState* threadInfo = PyThreadState_New(maininterp);
// 		pPythonEx->ExePyModule(strModule.c_str());
// 		PyEval_ReleaseLock();
// 	//	pPythonEx->ExePyModule(strModule.c_str());
// 	//	PyGILState_Release( state );
// 
// 		Sleep(200);
// 	}
// 
// //	pPythonEx->ReleasePython();
//  
//  	_endthreadex( 0 );
//  	return 0;
// }

// void CPythonEx::SendMSG()
// {
// 	PostThreadMessage(m_obUiThread.m_nThreadID,WM_MSG_EXE_PY_MOD, 0, 0);
// //	m_obUiThread.GetMainWnd()->postt
// 	//m_obUiThread.GetMainWnd()->SendMessage(WM_MSG_EXE_PY_MOD, 0, 0);
// }


// void CPythonEx::CloseProcess()
// {
// 	if( TRUE == m_bThreadOpen )
// 	{
// 		m_bThreadOpen = FALSE;
// 		WaitForSingleObject(m_hThread, INFINITE);
// 		CloseHandle( m_hThread );
// 	}
// }

// 
// CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
// 	PostMessage(pWnd->GetSafeHwnd(), WM_MY_MESSAGE, 123, 0);

#endif