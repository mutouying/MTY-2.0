#ifndef __EASYIPCAPKSAFEWRAP_H__
#define __EASYIPCAPKSAFEWRAP_H__

#include "EasyIpcClientWrap.h"
#include "kmobilescan/scanphone_def.h"
#include "atlstr.h"

typedef struct  
{
	CString		strAppName;				//apk����(��ʾ����
	CString		strPkgName;				//apk����
	CString		strVersion;				//apk�汾
	CStringA	bIconBuf;				//apkͼ������
}KWRAPAPKINFORESULT;

class KIpcApkSafeQuery {
public:
	/*
	* @brief �ж������Ƿ���apk�ļ�
	*
	* @param[in]   strPath    �ļ���·��
	* @param[out]  bApk		  TRUE��ʾ��apk��FALSEΪ��apk
	*
	* @return 0 �ɹ�,����Ϊʧ�ܴ�����
	*/
	int IsAndroidApk(const wchar_t* strPath, BOOL& bApk)
	{
		BOOL bRet = m_ipcClient.AddParam(L"strPath", strPath);
		if (!bRet)
			return -1;

		int iRet = m_ipcClient.Call(L"IsAndroidApk");
		if (0 != iRet)
			return -1;

		bApk = m_ipcClient.GetInt(L"bApk");
		return 0;
	}

	/*
	* @brief ��ȡapk���������֣�ͼ�����Ϣ
	*
	* @param[in]  strApkPath   apk�ļ���·��
	* @param[out] stApkInfo    apk���ƣ�ͼ�����Ϣ��@see KWRAPAPKINFORESULT
	*
	* @return 0 �ɹ�,����Ϊʧ�ܴ�����
	*/
	int __stdcall GetApkInfo(const wchar_t* strPath, KWRAPAPKINFORESULT& stApkInfo)
	{
		BOOL bRet = m_ipcClient.AddParam(L"strPath", strPath);
		if (!bRet)
			return -1;

		int iRet = m_ipcClient.Call(L"GetApkInfo");
		if (0 != iRet)
			return -1;

		stApkInfo.strAppName = m_ipcClient.GetString(L"strAppName");
		stApkInfo.strPkgName = m_ipcClient.GetString(L"strPkgName");
		stApkInfo.strVersion = m_ipcClient.GetString(L"strVersion");
		bRet = m_ipcClient.GetBinary(L"bIconBuf", stApkInfo.bIconBuf);
		if (!bRet)
			return -1;

		return 0;
	}

	/**
	* @brief  ���ƻ��ĳ��apk�İ�ȫ�����Ϣ���ú����������������������
	*
	* @param[in]	wcsApkPath	apk��ȫ·��
	* @param[out]	stSafeInfo	���ص�apk��ȫ��Ϣ
	*
	* @return 0 �ɹ�,����Ϊʧ�ܴ�����
	*/
	int	QueryApkSafeInfo(const wchar_t* wcsApkPath, KAPKSAFEINFO& stSafeInfo)
	{

		BOOL bRet = m_ipcClient.AddParam(L"strPath", wcsApkPath);
		if (!bRet)
			return -1;

		int iRet = m_ipcClient.Call(L"QueryApkSafeInfo");
		if (0 != iRet)
			return -1;

		stSafeInfo.bHaveReplace = m_ipcClient.GetInt(L"bHaveReplace");
		stSafeInfo.bMalAD = m_ipcClient.GetInt(L"bMalAD");
		stSafeInfo.bMalPri = m_ipcClient.GetInt(L"bMalPri");
		stSafeInfo.bPiracy = m_ipcClient.GetInt(L"bPiracy");
		stSafeInfo.bReplaceApk = m_ipcClient.GetInt(L"bReplaceApk");
		stSafeInfo.bRiskAD = m_ipcClient.GetInt(L"bRiskAD");
		stSafeInfo.bVirus = m_ipcClient.GetInt(L"bVirus");
		stSafeInfo.iAdCount = m_ipcClient.GetInt(L"iAdCount");
		stSafeInfo.iMalAct = m_ipcClient.GetInt(L"iMalAct");
		stSafeInfo.iPriAct = m_ipcClient.GetInt(L"iPriAct");

		return 0;
	}

	KIpcApkSafeQuery() : m_ipcClient(L"KApkSafeInfoQueryIpcServer") {};
	~KIpcApkSafeQuery() {};

private:
	KEasyIpcClientWrap m_ipcClient;

};


#endif