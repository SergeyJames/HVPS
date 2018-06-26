#ifndef COM_PORT_H_
#define COM_PORT_H_

#include "stdafx.h"

/// this defination for GetComPortList() function
#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383


class CComPort
{
private:
	static const std::chrono::seconds cWaitingTime;
	HANDLE m_HandleCom;
	OVERLAPPED m_sOverlapped;
	std::wstring m_strNameComPort{};
private:
	bool SetEventCom();
protected:
	enum {
		INTERVAL_YIMEOUT = 0xFFFFFFFF,
		TIMEOUT = 1000,
		LEN_QUEUE = 1500,
		BITE_SIZE = 8
	};
public:
	CComPort();
	virtual ~CComPort();
	bool isOk() const;
	bool Connect(LPCWSTR a_pchrPort);
	bool Disconnect();
	std::vector<std::wstring> GetComPortList();
		
	/*������ ������� ������
	[out] a_pvdDate ���������� ������
	[in] a_nBytes ������� ����
	[out] ����� ����������� ������*/
	DWORD ReadData(void* a_pvdDate, int a_nBytes);
	
	//������ ������� ������
	DWORD WriteDate(char* a_pvdDate, int a_nBytes);
	
	std::wstring GetNamePort();
};

#endif //!COM_PORT_H_
