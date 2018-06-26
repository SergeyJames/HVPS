#include "stdafx.h"
#include "ComPort.h"

const std::chrono::seconds CComPort::cWaitingTime = std::chrono::seconds{ 5 };

bool CComPort::SetEventCom()
{
	if (!SetCommMask(m_HandleCom, EV_DSR | EV_CTS)) {
		CloseHandle(m_HandleCom);
		return false;
	}
	m_sOverlapped.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!m_sOverlapped.hEvent) {
		CloseHandle(m_HandleCom);
		return false;
	}
	return true;
}

CComPort::CComPort() :
	m_HandleCom{ INVALID_HANDLE_VALUE }
{
	ZeroMemory(&m_sOverlapped, sizeof(OVERLAPPED));
}

CComPort::~CComPort()
{
	Disconnect();
}

bool CComPort::isOk() const
{
	return m_HandleCom != INVALID_HANDLE_VALUE;
}

bool CComPort::Connect(LPCWSTR a_pchrPort)
{
	Disconnect();
	DCB comDCB;
	m_HandleCom = CreateFile(a_pchrPort, (GENERIC_READ | GENERIC_WRITE), FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_HandleCom == INVALID_HANDLE_VALUE) {
		return false;
	}
	if (!GetCommState(m_HandleCom, &comDCB)) {
		return false;
	}
	comDCB.BaudRate = CBR_19200;
	comDCB.ByteSize = BITE_SIZE;
	comDCB.StopBits = ONESTOPBIT;
	comDCB.Parity = NOPARITY;
	if (!SetCommState(m_HandleCom, &comDCB)) {
		CloseHandle(m_HandleCom);
		return false;
	}
	if (!SetEventCom()) {
		return false;
	}
	m_strNameComPort = a_pchrPort;
	return true;
}

bool CComPort::Disconnect()
{
	if (m_HandleCom != INVALID_HANDLE_VALUE) {
		if (CloseHandle(m_HandleCom)) {
			m_HandleCom = INVALID_HANDLE_VALUE;
		}
		else {
			return false;
		}
	}
	if (m_sOverlapped.hEvent != INVALID_HANDLE_VALUE) {
		if (CloseHandle(m_sOverlapped.hEvent)) {
			m_sOverlapped.hEvent = INVALID_HANDLE_VALUE;
			return true;
		}
	}
	return false;
}

std::vector<std::wstring> CComPort::GetComPortList()
{
	std::vector<std::wstring> ComPortVec;
	HKEY hKey;
	LONG lResult;

	DWORD typeValue;
	TCHAR data[MAX_PATH] = {};
	DWORD MaxData = sizeof(data);

	DWORD BufferSize = 32;//TOTALBYTES;

	TCHAR achValue[MAX_VALUE_NAME] = {};
	DWORD cchValue = MAX_VALUE_NAME;


	TCHAR  achClass[MAX_PATH] = TEXT("");	// buffer for class name 
	DWORD  cchClassName = MAX_PATH;			// size of class string 
	DWORD  cValues;							// number of values for key 
	DWORD  cchMaxValue;						// longest value name 
	DWORD  cbMaxValueData;					// longest value data 
	DWORD  cbSecurityDescriptor;			// size of security descriptor 
	FILETIME ftLastWriteTime;				// last write time 

	RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), NULL, KEY_READ, &hKey);

	// Get the class name and the value count. 
	lResult = RegQueryInfoKey(
		hKey,					// key handle 
		achClass,				// buffer for class name 
		&cchClassName,			// size of class string 
		NULL,					// reserved 
		NULL,					//&cSubKeys,        // number of subkeys 
		NULL,					//&cbMaxSubKey,      // longest subkey size 
		NULL,					//&cchMaxClass,      // longest class string 
		&cValues,				// number of values for this key 
		&cchMaxValue,			// longest value name 
		&cbMaxValueData,		// longest value data 
		&cbSecurityDescriptor,  // security descriptor 
		&ftLastWriteTime);		// last write time 

	if (cValues) {
		for (size_t i = 0, lResult = ERROR_SUCCESS; i < cValues; ++i) {
			cchValue = MAX_VALUE_NAME;
			achValue[0] = '\0';

			lResult = RegEnumValue(hKey, i, achValue, &cchValue, NULL, &typeValue, (BYTE*)data, &MaxData);
			if (lResult == ERROR_SUCCESS) {
				//В переменной data имеем имя COM порта
				ComPortVec.push_back(data);
			}
		}
	}

	RegCloseKey(hKey);

	return ComPortVec;
}

DWORD CComPort::ReadData(void * a_pvdDate, int a_nBytes)
{
	ULONG ulState{}, ulWait{};
	DWORD dbByteReadValue{};
	WaitCommEvent(m_HandleCom, &ulState, &m_sOverlapped);
	ulWait = WaitForSingleObject(m_sOverlapped.hEvent, cWaitingTime.count());
	ReadFile(m_HandleCom, a_pvdDate, a_nBytes, &dbByteReadValue, NULL);
	if (ulWait == WAIT_OBJECT_0) {
		ReadFile(m_HandleCom, a_pvdDate, a_nBytes, &dbByteReadValue, NULL);
		ulWait = WaitForSingleObject(m_sOverlapped.hEvent, cWaitingTime.count());
		if (ulWait == WAIT_OBJECT_0) {
			if (GetOverlappedResult(m_HandleCom, &m_sOverlapped, &dbByteReadValue, FALSE)) {
				return dbByteReadValue;
			}
		}
	}
	return NULL;
}

DWORD CComPort::WriteDate(char * a_pvdDate, int a_nBytes)
{
	DWORD dwBytesWrite = 0;
	WriteFile(m_HandleCom, a_pvdDate, a_nBytes, &dwBytesWrite, NULL);
	return dwBytesWrite;
}

std::wstring CComPort::GetNamePort()
{
	return m_strNameComPort;
}
