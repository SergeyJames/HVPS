
// HighVoltagePowerSupplyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HighVoltagePowerSupply.h"
#include "HighVoltagePowerSupplyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHighVoltagePowerSupplyDlg dialog

bool CHighVoltagePowerSupplyDlg::m_bIsComPortListEmpty = { true };
int  CHighVoltagePowerSupplyDlg::m_SpinCtrlBiasPos = 0;

CHighVoltagePowerSupplyDlg::CHighVoltagePowerSupplyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HIGHVOLTAGEPOWERSUPPLY_DIALOG, pParent),
	m_ulSliderVoltageToSetPos{ AcceleratorMin },
	m_strSliderVoltageToSetPos { std::to_wstring(m_ulSliderVoltageToSetPos) },
	m_dValue{ 0.0 },
	m_ulSliderVoltageToSetPosBias{ 0.0 }
{
	FillComPortList();

	m_bIsComPortListEmpty = m_ComPortVec.empty();

	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHighVoltagePowerSupplyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_LogToFileButton);
	DDX_Control(pDX, IDC_BUTTON_ENABLE, m_EnableButton);
	DDX_Control(pDX, IDC_EDIT_VOLTAGE_TO_SET_SPIN, m_VoltageToSetSpin);
	DDX_Control(pDX, IDC_SPIN_VOLTAGE_TO_SET, m_VoltageToSetSpinCtrl);
	DDX_Control(pDX, IDC_SLIDER_VOLTAGE_TO_SET, m_SliderVoltageToSet);
	DDX_Control(pDX, IDC_EDIT_VOLTAGE_TO_SET_KEYBORD, m_VoltageToSetKeyboard);
	DDX_Control(pDX, IDC_COMBO_COM_PORT, m_ComPortCmbBox);
	DDX_Control(pDX, IDC_EDIT_INTERNAL_DATA, m_InternalTempEdit);
	DDX_Control(pDX, IDC_EDIT_POWER_TEMP, m_PowerTempEdit);
	DDX_Control(pDX, IDC_EDIT_Humidity, m_HumidityEdit);
	DDX_Control(pDX, IDC_SLIDER_VOLTAGE_TO_SET_BIAS, m_SliderVoltageToSetBias);
	DDX_Control(pDX, IDC_SLIDER_VOLTAGE_TO_SET_BIAS2, m_SliderVoltageToSetFlament);
	DDX_Control(pDX, IDC_EDIT_VOLTAGE_TO_SET_KEYBORD_BIAS, m_VoltageToSetKeyboardBias);
	DDX_Control(pDX, IDC_EDIT_VOLTAGE_TO_SET_SPIN_BIAS, m_VoltageToSetSpinBias);
	DDX_Control(pDX, IDC_EDIT_VOLTAGE_TO_SET_SPIN_BIAS2, m_VoltageToSetSpinFlament);
	DDX_Control(pDX, IDC_EDIT_VOLTAGE_TO_SET_KEYBORD_BIAS2, m_VoltageToSetKeyboardFlament);
	DDX_Control(pDX, IDC_SPIN_VOLTAGE_TO_SET_BIAS, m_VoltageToSetSpinCtrlBias);
	DDX_Control(pDX, IDC_SPIN_VOLTAGE_TO_SET_BIAS2, m_VoltageToSetSpinCtrlFlament);
}

BEGIN_MESSAGE_MAP(CHighVoltagePowerSupplyDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CHighVoltagePowerSupplyDlg::OnBnClickedOk)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_VOLTAGE_TO_SET, &CHighVoltagePowerSupplyDlg::OnDeltaposSpinVoltageToSet)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_VOLTAGE_TO_SET, &CHighVoltagePowerSupplyDlg::OnNMCustomdrawSliderVoltageToSet)
	ON_EN_CHANGE(IDC_EDIT_VOLTAGE_TO_SET_KEYBORD, &CHighVoltagePowerSupplyDlg::OnEnChangeEditVoltageToSetKeybord)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_COM, &CHighVoltagePowerSupplyDlg::OnBnClickedButtonUpdateCom)
	ON_EN_CHANGE(IDC_EDIT_VOLTAGE_TO_SET_KEYBORD_BIAS, &CHighVoltagePowerSupplyDlg::OnEnChangeEditVoltageToSetKeybordBias)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_VOLTAGE_TO_SET_BIAS, &CHighVoltagePowerSupplyDlg::OnDeltaposSpinVoltageToSetBias)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_VOLTAGE_TO_SET_BIAS, &CHighVoltagePowerSupplyDlg::OnNMCustomdrawSliderVoltageToSetBias)
END_MESSAGE_MAP()


// CHighVoltagePowerSupplyDlg message handlers

BOOL CHighVoltagePowerSupplyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	SetAllSpinCtrlRanges();
	SetAllSliderRanges();
	FillComPortComboBox();
	m_VoltageToSetSpinBias.SetWindowTextW(L"0.0");


	
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHighVoltagePowerSupplyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHighVoltagePowerSupplyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHighVoltagePowerSupplyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHighVoltagePowerSupplyDlg::OnBnClickedOk()
{
	std::string strFilter ="Text Files (*.txt)|*.txt|";
	CString FileName;
	CString FilePath;
	CString FullPath;

	CFileDialog FileDlg(FALSE, CString(".txt"), NULL, 0, CString(strFilter.c_str()));

	if (FileDlg.DoModal() == IDOK) {
		FileName = FileDlg.GetFileName(); //filename
		FilePath = FileDlg.GetFolderPath(); //filepath (folders)
		FullPath = FilePath + "\\" + FileName;
		/*
		TODO:
		.
		.
		.
		.
		*/
	}
}

void CHighVoltagePowerSupplyDlg::OnDeltaposSpinVoltageToSet(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	std::wstring tmp;
	if (pNMUpDown->iDelta < 0) {
		if (pNMUpDown->iPos == 0) {
			tmp = std::to_wstring(0);
			m_VoltageToSetKeyboard.SetWindowTextW(tmp.c_str());
			m_SliderVoltageToSet.SetPos(m_VoltageToSetSpinCtrl.GetPos());
		}
		else {
			tmp = std::to_wstring(m_VoltageToSetSpinCtrl.GetPos() - 1);
			m_VoltageToSetKeyboard.SetWindowTextW(tmp.c_str());
			m_SliderVoltageToSet.SetPos(m_VoltageToSetSpinCtrl.GetPos() - 1);
		}
	}
	else {
		tmp = std::to_wstring(m_VoltageToSetSpinCtrl.GetPos() + 1);
		m_VoltageToSetKeyboard.SetWindowTextW(tmp.c_str());
		m_SliderVoltageToSet.SetPos(m_VoltageToSetSpinCtrl.GetPos() + 1);
	}

	
}

void CHighVoltagePowerSupplyDlg::OnNMCustomdrawSliderVoltageToSet(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	m_ulSliderVoltageToSetPos = m_SliderVoltageToSet.GetPos();
	m_strSliderVoltageToSetPos = std::to_wstring(m_ulSliderVoltageToSetPos);

	m_VoltageToSetKeyboard.SetWindowTextW(m_strSliderVoltageToSetPos.c_str());
	m_VoltageToSetSpin.SetWindowTextW(m_strSliderVoltageToSetPos.c_str());
}

void CHighVoltagePowerSupplyDlg::OnEnChangeEditVoltageToSetKeybord()
{
	CString text;
	m_VoltageToSetKeyboard.GetWindowTextW(text);
	std::wstring wstext = text;

	if (!wstext.empty()) {
		m_SliderVoltageToSet.SetPos(std::stol(wstext));
	}


	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CHighVoltagePowerSupplyDlg::SetAllSpinCtrlRanges()
{
	// Accelerator
	m_VoltageToSetSpinCtrl.SetRange(AcceleratorMin, AcceleratorMax);
	m_VoltageToSetSpinCtrl.SetPos(AcceleratorMin);

	// BIAS 
	m_VoltageToSetSpinCtrlBias.SetRange(0, 100); // temp min max
	m_VoltageToSetSpinCtrlBias.SetPos(0); // temp min max

	// FLAMENT
	m_VoltageToSetSpinCtrlFlament.SetRange(0, 10); // temp min max
	m_VoltageToSetSpinCtrlFlament.SetPos(0); // temp min max
}

void CHighVoltagePowerSupplyDlg::SetAllSliderRanges()
{
	// Accelerator
	m_SliderVoltageToSet.SetRange(AcceleratorMin, AcceleratorMax);

	// BIAS 
	m_SliderVoltageToSetBias.SetRange(0, 100); // temp min max

	// FLAMENT
	m_SliderVoltageToSetFlament.SetRange(0, 10); // temp min max

}

void CHighVoltagePowerSupplyDlg::FillComPortComboBox()
{
	if (!m_ComPortVec.empty()) {
		std::vector<std::wstring>::const_iterator it = m_ComPortVec.begin();
		for (; it != m_ComPortVec.end(); ++it) {
			m_ComPortCmbBox.AddString((*it).c_str());
		}
	}
	else {
		m_ComPortCmbBox.AddString(LR"(empty)");
	}
}

void CHighVoltagePowerSupplyDlg::RemoveZeros(std::wstring& a_wstr)
{
	for (size_t i = 0; i < 5; ++i) {
		a_wstr.pop_back();
	}
}

void CHighVoltagePowerSupplyDlg::FillComPortList()
{
	HKEY hKey;
	LONG lResult;

	DWORD typeValue;
	TCHAR data[MAX_PATH] = TEXT("aa");
	CHAR dataChar[MAX_PATH] = "aa";
	DWORD MaxData = sizeof(data);

	DWORD BufferSize = 32;//TOTALBYTES;
	PPERF_DATA_BLOCK PerfData = (PPERF_DATA_BLOCK)malloc(BufferSize);

	TCHAR achValue[MAX_VALUE_NAME] = {};
	DWORD cchValue = MAX_VALUE_NAME;


	TCHAR  achClass[MAX_PATH] = TEXT(""); // buffer for class name 
	DWORD  cchClassName = MAX_PATH; // size of class string 
	DWORD  cValues;       // number of values for key 
	DWORD  cchMaxValue;     // longest value name 
	DWORD  cbMaxValueData;    // longest value data 
	DWORD  cbSecurityDescriptor; // size of security descriptor 
	FILETIME ftLastWriteTime;   // last write time 

	lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), NULL, KEY_READ, &hKey);
	
	/*if (lResult != ERROR_SUCCESS) {
		if (lResult == ERROR_FILE_NOT_FOUND) {
			MessageBoxEx(0, TEXT("Ключ реестра не найден"), TEXT("Ошибка"), MB_ICONERROR | MB_OK, NULL);

		}
		else {
			MessageBoxEx(0, TEXT("Ошибка открытия ключа реестра"), TEXT("Ошибка"), MB_ICONERROR | MB_OK, NULL);
			return "";
		}
	}*/

	// Get the class name and the value count. 
	lResult = RegQueryInfoKey(
		hKey,          // key handle 
		achClass,        // buffer for class name 
		&cchClassName,      // size of class string 
		NULL,          // reserved 
		NULL,//&cSubKeys,        // number of subkeys 
		NULL,//&cbMaxSubKey,      // longest subkey size 
		NULL,//&cchMaxClass,      // longest class string 
		&cValues,        // number of values for this key 
		&cchMaxValue,      // longest value name 
		&cbMaxValueData,     // longest value data 
		&cbSecurityDescriptor,  // security descriptor 
		&ftLastWriteTime);    // last write time 

	if (cValues) {
		for (size_t i = 0, lResult = ERROR_SUCCESS; i < cValues; ++i) {
			cchValue = MAX_VALUE_NAME;
			achValue[0] = '\0';

			lResult = RegEnumValue(hKey, i, achValue, &cchValue, NULL, &typeValue, (BYTE*)data, &MaxData);
			if (lResult == ERROR_SUCCESS) {
				//В переменной data имеем имя COM порта
				m_ComPortVec.push_back(data);
			}
		}
	}

	RegCloseKey(hKey);

	////После того, как имя COM порта получено, дальнейшая работа с ним происходит как с файлом.Но для начала нам надо его настроить

	//HANDLE Port;   //Дескриптор COM-порта
	//			   //Открываем COM-порт
	//			   // data - Имя COM-порта
	//Port = CreateFile(data, (GENERIC_READ | GENERIC_WRITE), FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	//if (Port == INVALID_HANDLE_VALUE) {
	//	MessageBox(NULL, TEXT("Невозможно открыть последовательный порт"), TEXT("Error"), MB_OK);
	//	return "";
	//}

	////Получаем состояние управляющей структуры COM-порта, 
	////если не удалось выводим сообщение об ошибке и выходим из 
	////обработчика
	//COMMCONFIG comm;
	//GetCommState(Port, &comm.dcb);

	////Настраиваем управляющую структуру COM-порта
	//comm.dcb.ByteSize = 8;        //Размер байта
	//comm.dcb.Parity = NOPARITY;    //Паритет отключен
	//comm.dcb.StopBits = ONESTOPBIT;//Один стоповый бит
	//							   //Можно также воспользоваться стандартным диалоговым
	//							   //окном настройки управляющей структуры COM-порта:

	//							   // CommConfigDialog(data, NULL, &comm); /// временно не нужен

	//							   //Применяем настроенную структуру к COM-порту, если не 
	//							   //удалось выводим сообщение об ошибке 
	//if (!SetCommState(Port, &comm.dcb)) {
	//	MessageBox(NULL, TEXT("Невозможно сконфигурировать последовательный порт"), TEXT("Error"), MB_OK);
	//	CloseHandle(Port);
	//	return "";
	//}

	////Получаем текущие настройки тайм-аутов COM-порта
	//COMMTIMEOUTS commTimeouts;
	//GetCommTimeouts(Port, &commTimeouts);

	////Перенастраиваем тайм-ауты: 
	////Максимальный интервал чтения в миллисекундах между 
	////двумя принимаемыми символами
	//commTimeouts.ReadIntervalTimeout = 100;
	////Константа в миллисекундах используемая для вычисления 
	////полного тайм-аута операции чтения
	//commTimeouts.ReadTotalTimeoutConstant = 300;
	////Множитель используемый для вычисления полного тайм-аута
	////операции чтения в миллисекундах
	//commTimeouts.ReadTotalTimeoutMultiplier = 50;
	////Полный максимальный тайм-аут операции чтения 
	////вычисляется следующим образом //ReadTotalTimeoutConstant + (ReadIntervalTimeout * количество считываемых байт)

	////Устанавливаем тайм-ауты для COM-порта
	//if (!SetCommTimeouts(Port, &commTimeouts)) {
	//	MessageBox(NULL, TEXT("Невозможно настроить тайм-ауты последовательного порта"), TEXT("Error"), MB_OK);
	//	CloseHandle(Port);
	//	return "";
	//}
	////Передача данных по COM - порту выглядит следующим образом :

	//DWORD feedback = 0;
	////LONG lResult; //Помещаем сюда количество данных, которые
	//// необходимо передать
	////if ((!WriteFile(Port, &data[0], lResult * sizeof(data[0]), &feedback, 0) || feedback != lResult * sizeof(data[/*i*/0]))) {
	////	CloseHandle(Port);
	////	Port = INVALID_HANDLE_VALUE;
	////}
	////При передаче мы проверяем количество переданных байт, и вообще удалось ли выполнить передачу.


	//TCHAR send[MAX_PATH] = {};

	//for (size_t i = 0; i < a_str.size(); ++i) {
	//	send[i] = a_str.at(i);
	//}

	//TCHAR save[MAX_PATH] = {};


	//WriteFile(Port, &send, sizeof(send), &feedback, NULL);


	////Чтение данных можно реализовать следующим образом(лучше это делать по таймеру) :
	//feedback = 0; /*DWORD*/
	//int i = 0;
	////Попытка чтения первого символа хранящегося в COM-порте
	//ReadFile(Port, &save, sizeof(save), &feedback, NULL);

	///*if (feedback > 0)	{
	////Если чтение удалось продолжаем чтение пока не встретится символ #13
	//do {
	//send[i] = save[i];
	//ReadFile(Port, &save, sizeof(save), &feedback, NULL);
	//++i;
	//} while (feedback > 0);
	//send[i] = '\0';
	//}
	//*/


	////И закрываем порт после работы :
	//if (Port != INVALID_HANDLE_VALUE)
	//{    //Если порт открыт
	//	CloseHandle(Port);
	//}

	//string tmp;
	//for (size_t i = 0; i < sizeof(save); ++i) {
	//	if (save[i] != '\0') {
	//		tmp.push_back(save[i]);
	//	}
	//}
}


void CHighVoltagePowerSupplyDlg::OnBnClickedButtonUpdateCom()
{
	m_ComPortCmbBox.ResetContent();

	// Обязательно 
	m_ComPortVec.clear();
	m_ComPortVec.shrink_to_fit();

	FillComPortList();
	FillComPortComboBox();
}


void CHighVoltagePowerSupplyDlg::OnEnChangeEditVoltageToSetKeybordBias()
{
	CString text;
	m_VoltageToSetKeyboardBias.GetWindowTextW(text);
	std::wstring wstext = text;
	double dValue = 0.0;


	try	{
		if (!wstext.empty()) {
			dValue = boost::lexical_cast<double>(wstext);
		}
	}
	catch (...)
	{
		MessageBoxA(this->m_hWnd, "Please type Integer or Float values ( 0.0 <= value <= 10.0), charachters not allowed", "", 48);
		SendMessageA(m_VoltageToSetKeyboardBias, WM_SETTEXT, WPARAM(0), LPARAM(""));
	}

	// temp min max ( 0.0 and 10.0 )
	if (dValue <= 0.0) {
		m_SliderVoltageToSetBias.SetPos(0);
		m_VoltageToSetSpinBias.SetWindowTextW(L"0");;
		m_dValue = 0.0;
	}
	else if (dValue > 10.0) {
		m_SliderVoltageToSetBias.SetPos(100);
		m_VoltageToSetSpinBias.SetWindowTextW(L"10.0");;
		m_dValue = 10.0;
		SendMessageA(m_VoltageToSetKeyboardBias, WM_SETTEXT, WPARAM(0), LPARAM("10.0"));
	}
	else if (dValue >= 0.0 && dValue <= 10.0) {
		m_SliderVoltageToSetBias.SetPos(dValue * 10);
		m_dValue = dValue;
		std::wstring tmp = std::to_wstring(dValue);
		RemoveZeros(tmp);
		m_VoltageToSetSpinBias.SetWindowTextW(tmp.c_str());
	}
}



void CHighVoltagePowerSupplyDlg::OnDeltaposSpinVoltageToSetBias(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	pNMUpDown->iPos = m_dValue * 10;

	float d = 0.0;
	std::wstring tmp;

	if (pNMUpDown->iDelta < 0) {
		if (pNMUpDown->iPos == 0) {
			tmp = std::to_wstring(0.0);
			m_VoltageToSetSpinBias.SetWindowTextW(L"0.0");
			m_SliderVoltageToSetBias.SetPos(0);
		}
		else {
			m_SpinCtrlBiasPos = pNMUpDown->iPos - 1;
			d = m_SpinCtrlBiasPos / 10.0;
			tmp = std::to_wstring(d);
			RemoveZeros(tmp);
			m_VoltageToSetSpinBias.SetWindowTextW(tmp.c_str());
			m_SliderVoltageToSetBias.SetPos(m_SpinCtrlBiasPos - 1);
			m_VoltageToSetKeyboardBias.SetWindowTextW(tmp.c_str());
		}
	}
	else {
		if (pNMUpDown->iPos == 100) {
			m_SpinCtrlBiasPos = pNMUpDown->iPos;
		}
		else if (pNMUpDown->iPos < 100) {
			m_SpinCtrlBiasPos = pNMUpDown->iPos + 1;
		}
	
		d = m_SpinCtrlBiasPos / 10.0;
		tmp = std::to_wstring(d);
		RemoveZeros(tmp);
		m_VoltageToSetSpinBias.SetWindowTextW(tmp.c_str());
		m_SliderVoltageToSetBias.SetPos(m_SpinCtrlBiasPos + 1);
		m_VoltageToSetKeyboardBias.SetWindowTextW(tmp.c_str());
	}
}


void CHighVoltagePowerSupplyDlg::OnNMCustomdrawSliderVoltageToSetBias(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	m_ulSliderVoltageToSetPosBias = m_SliderVoltageToSetBias.GetPos() / 10.0;
	std::wstring tmp = std::to_wstring(m_ulSliderVoltageToSetPosBias);
	RemoveZeros(tmp);
	m_strSliderVoltageToSetPosBias = tmp;

	m_VoltageToSetKeyboardBias.SetWindowTextW(m_strSliderVoltageToSetPosBias.c_str());
	m_VoltageToSetSpinBias.SetWindowTextW(m_strSliderVoltageToSetPosBias.c_str());
}
