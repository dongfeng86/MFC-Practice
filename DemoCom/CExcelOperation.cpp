#include "pch.h"

#include "CExcelOperation.h"
#include <winuser.h>

void ConstCharConver(const char* pFileName, CString &pWideChar)
{
	//����char *�����С�����ֽ�Ϊ��λ��һ������ռ�����ֽ�
	int charLen = strlen(pFileName);

	//������ֽ��ַ��Ĵ�С�����ַ����㡣
	int len = MultiByteToWideChar(CP_ACP, 0, pFileName, charLen, NULL, 0);

	//Ϊ���ֽ��ַ���������ռ䣬�����СΪ���ֽڼ���Ķ��ֽ��ַ���С
	TCHAR *buf = new TCHAR[len + 1];

	//���ֽڱ���ת���ɿ��ֽڱ���
	MultiByteToWideChar(CP_ACP, 0, pFileName, charLen, buf, len);

	buf[len] = '\0';  //�����ַ�����β��ע�ⲻ��len+1

	//��TCHAR����ת��ΪCString

	pWideChar.Append(buf);
}

ExcelOperation* ExcelOperation::m_excel = nullptr;

ExcelOperation::ExcelOperation()
{
	m_lpDisp = NULL;
	m_ExcelVer = 0;
}

//��ȡ����
ExcelOperation* ExcelOperation::getInstance()
{
	if (m_excel == NULL)
	{
		m_excel = new ExcelOperation();
	}
	return m_excel;
}
//���ٶ���
void ExcelOperation::destroyInstance()
{
	if (m_excel != NULL)
	{
		delete m_excel;
		m_excel = NULL;
	}
}


BOOL ExcelOperation::judgeExcelVer(int Ver)
{
	HKEY hkey;
	int ret;
	CString str;
	LONG len;
	str.Format(_T("Excel.Application.%d"), Ver);
	str += _T("\\CLSID");
	ret = RegCreateKey(HKEY_CLASSES_ROOT, str, &hkey);
	if (ret == ERROR_SUCCESS)
	{
		RegQueryValue(HKEY_CLASSES_ROOT, str, NULL, &len);
		//���ע����� HKEY_CLASSES_ROOT\Excel.Application.x\CPLSID�е�ֵΪ�գ����ȡ��'\0'������Ϊ2
		return len == 2 ? FALSE : TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL ExcelOperation::createServer(CString officeVer)
{
	//ȥ��ǰ��ո�
	officeVer.Trim();
	//��ȡ�汾���ַ�
	CString verNum = officeVer.Right(4);
	int ver = _ttoi(verNum);
	switch (ver)
	{
	case 2003:
		if (judgeExcelVer(11))
		{
			if (m_ExcelApp.CreateDispatch(_T("Excel.Application.11"), NULL))
			{
				m_ExcelVer = 2003;
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
		break;
	case 2007:
		if (judgeExcelVer(12))
		{
			if (m_ExcelApp.CreateDispatch(_T("Excel.Application.12"), NULL))
			{
				m_ExcelVer = 2007;
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
		break;
	case 2010:
		if (judgeExcelVer(14))
		{
			if (m_ExcelApp.CreateDispatch(_T("Excel.Application.14"), NULL))
			{
				m_ExcelVer = 2010;
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
		break;
	case 2013:
		if (judgeExcelVer(15))
		{
			if (m_ExcelApp.CreateDispatch(_T("Excel.Application.15"), NULL))
			{
				m_ExcelVer = 2013;
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
		break;
	case 2016:
		if (judgeExcelVer(16))
		{
			if (m_ExcelApp.CreateDispatch(_T("Excel.Application.16"), NULL))
			{
				m_ExcelVer = 2016;
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
		break;
	}
	return TRUE;
}

BOOL ExcelOperation::init()
{
	InitializeUI();

	CString strOfficeVer[5] = { _T("office 2003"), _T("office 2007"), _T("office 2010"), _T("office 2013"), _T("office 2016") };
	BOOL result = FALSE;
	for (int i = 4; i >= 0; i--)
	{
		if (!createServer(strOfficeVer[i]))
			continue;
		else
		{
			result = TRUE;
		}
	}
	if (m_ExcelVer == 0)
	{
		result = FALSE;
	}
	return result;
}

void ExcelOperation::setView(bool show)
{
	m_ExcelApp.put_Visible(TRUE);
	m_ExcelApp.put_UserControl(FALSE);
}

void ExcelOperation::saveExcelAs(const char* savePath)
{
	CString savePathCSt;
	ConstCharConver(savePath, savePathCSt);
	savePathCSt.Trim();
	m_book.SaveAs(_variant_t(savePathCSt),
		vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, 0,
		vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing);
}

BOOL ExcelOperation::openExcelFile(const char*path, const char* excleTemplate)
{
	m_lpDisp = NULL;

	/*�жϵ�ǰExcel�İ汾*/
	CString strExcelVersion = m_ExcelApp.get_Version();
	int iStart = 0;
	strExcelVersion = strExcelVersion.Tokenize(_T("."), iStart);

	setView(false);

	/*�õ�����������*/
	m_books.AttachDispatch(m_ExcelApp.get_Workbooks());

	/*��һ�����������粻���ڣ�������һ��������*/
	CString strBookPath;
	ConstCharConver(path, strBookPath);
	try
	{
		/*��һ��������*/
		m_lpDisp = m_books.Open(strBookPath,
			vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,
			vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,
			vtMissing, vtMissing, vtMissing, vtMissing);
		m_book.AttachDispatch(m_lpDisp);
	}
	catch (...)
	{
		/*����һ���µĹ�����*/
		if (excleTemplate == nullptr)
		{
			m_lpDisp = m_books.Add(vtMissing);
		}
		else
		{
			m_lpDisp = m_books.Add(_variant_t(excleTemplate));
		}
		m_book.AttachDispatch(m_lpDisp);
		saveExcelAs(path);
	}
	return true;
}

void ExcelOperation::OpenSheet(const char* sheetName)
{
	LPDISPATCH lpDisp = NULL;
	/*�õ��������е�Sheet������*/
	m_sheets.AttachDispatch(m_book.get_Sheets());
	/*��һ��Sheet���粻���ڣ�������һ��Sheet*/
	CString strSheetName(sheetName);
	try
	{
		/*��һ�����е�Sheet*/
		lpDisp = m_sheets.get_Item(_variant_t(strSheetName));
		m_sheet.AttachDispatch(lpDisp);
	}
	catch (...)
	{
		/*����һ���µ�Sheet*/
		lpDisp = m_sheets.Add(vtMissing, vtMissing, _variant_t((long)1), vtMissing);
		m_sheet.AttachDispatch(lpDisp);
		m_sheet.put_Name(strSheetName);
	}
}

void ExcelOperation::setCellFormat(const char* ccellIndexChar, const char* cellFormat)
{
	CString cellIndex, cellFormatChar;
	ConstCharConver(ccellIndexChar, cellIndex);
	ConstCharConver(cellFormat, cellFormatChar);
	m_range = m_sheet.get_Range(_variant_t(cellIndex), _variant_t(cellIndex));
	m_range.put_NumberFormat(_variant_t(cellFormatChar));
}

void ExcelOperation::setCellsFormat(const char* cellBeginChar, const char* cellBEndChar, const char* cellFormat)
{
	CString cellBegin, cellEnd, format;
	ConstCharConver(cellBeginChar, cellBegin);
	ConstCharConver(cellBEndChar, cellEnd);
	ConstCharConver(cellFormat, format);
	m_lpDisp = m_sheet.get_Range(_variant_t(cellBegin), _variant_t(cellEnd));
	m_range.AttachDispatch(m_lpDisp);
	m_range.put_NumberFormatLocal(_variant_t(format));
}

void ExcelOperation::setCellValue(const char* ccellIndexChar, const char* valueChar)
{
	CString cellIndex, value;
	ConstCharConver(ccellIndexChar, cellIndex);
	ConstCharConver(valueChar, value);
	m_range = m_sheet.get_Range(_variant_t(cellIndex), _variant_t(cellIndex));
	m_range.put_Value2(_variant_t(value));
}

void ExcelOperation::InitializeUI()
{
	if (S_OK != CoInitialize(NULL)) {
		AfxMessageBox(_T("Initialize com failed..."));
		return;
	}
}

void ExcelOperation::UnInitializeUI()
{
	CoUninitialize();
}

void ExcelOperation::saveExcel()
{
	m_ExcelApp.put_DisplayAlerts(FALSE);
	//book.Close(vtMissing, vtMissing, vtMissing);
	m_book.Save();
}

void GetAutoWrapText(LPCTSTR pszText, int iCellWidth, int iPointSize, LPCTSTR szFontName)
{
	//CDC dcDisplay/*,dcMem*/;
	//dcDisplay.CreateIC(_T("DISPLAY"), NULL, NULL, NULL);

	//CFont font;
	//font.CreatePointFont(iPointSize * 10, szFontName, NULL);
	//CFont* pOldFont2 = dcDisplay.SelectObject(&font);

	//CString sText = pszText;
	//CString sLine;
	//CSize sizeLine;
	//for (int iBeg = 0; iBeg < sText.GetLength(); iBeg++)
	//{
	//	if (_T('\n') == sText[iBeg])
	//	{
	//		sLine.Empty();
	//		continue;
	//	}
	//	else
	//	{
	//		//�жϳ���
	//		sLine += sText[iBeg];
	//		//sizeLine = dcMem.GetTextExtent(sLine);
	//		sizeLine = dcDisplay.GetTextExtent(sLine);
	//		if (sizeLine.cx > iCellWidth)
	//		{
	//			sText.Insert(iBeg, _T('\r'));	
	//			sLine.Empty();
	//		}
	//	}
	//}

	//dcDisplay.SelectObject(pOldFont2);
	//dcDisplay.DeleteDC();



		//��ȡ��ǰ��Ļ��DC
	CWindowDC dcScreen(CWnd::GetDesktopWindow());
	CFont font;
	font.CreatePointFont(iPointSize * 10, szFontName, NULL);
	CFont* pOldFont2 = dcScreen.SelectObject(&font);

	CString sText = pszText;
	CString sLine;
	CSize sizeLine;
	for (int iBeg = 0; iBeg < sText.GetLength(); iBeg++)
	{
		if (_T('\n') == sText[iBeg])
		{
			sLine.Empty();
			continue;
		}
		else
		{
			//�жϳ���
			sLine += sText[iBeg];
			sizeLine = dcScreen.GetTextExtent(sLine);
			if (sizeLine.cx > iCellWidth)
			{
				sText.Insert(iBeg, _T('\r'));
				sLine.Empty();
			}
		}
	}
	dcScreen.SelectObject(pOldFont2);
}

CString ExcelOperation::ReadCell(const char* ccellIndexChar)
{
	CString sCellIndex, sCellValue;
	ConstCharConver(ccellIndexChar, sCellIndex);
	CRange cell = m_sheet.get_Range(_variant_t(sCellIndex), _variant_t(sCellIndex));
	VARIANT vResult = cell.get_Text();
	VARIANT vResult2 = cell.get_Value2();
	cell.get_WrapText();

	CString sCellText;
	if (vResult.vt == VT_BSTR) //�ַ���  
	{
		sCellText = vResult.bstrVal;
	}

	VARIANT vWidth = cell.get_ColumnWidth();
	VARIANT vWidth2 = cell.get_ColumnWidth();
	VARIANT vWidth3 = cell.get_Width();
	double dCellWidth = vWidth3.dblVal;

	CFont0 fontCell;
	fontCell.AttachDispatch(cell.get_Font());
	VARIANT vtFontStyle = fontCell.get_FontStyle();
	VARIANT vtFontName = fontCell.get_Name();
	VARIANT vtSize = fontCell.get_Size();
	fontCell.ReleaseDispatch();

	CString sFontName = vtFontName.bstrVal;
	double iPointSize = vtSize.dblVal;
	GetAutoWrapText(sCellText, dCellWidth, iPointSize, sFontName);

	return sCellText;
}

ExcelOperation::~ExcelOperation()
{
	try
	{
		m_books.Close();
		m_sheet.ReleaseDispatch();
		m_sheets.ReleaseDispatch();
		m_book.ReleaseDispatch();
		m_books.ReleaseDispatch();
		m_ExcelApp.ReleaseDispatch();
		m_ExcelApp.Quit();
		//�˳�αװ��app
		if (!m_ExcelApp_fake.get_ActiveSheet())
		{
			m_books_fake.ReleaseDispatch();
			m_ExcelApp_fake.ReleaseDispatch();
			m_ExcelApp_fake.Quit();
		}
	}
	catch (COleDispatchException*)
	{
		//AfxMessageBox(Notice_get_by_id(IDS_POW_OFF_EXCEL_FAIL));
		AfxMessageBox(_T("�ر�Excel���������"));
	}

	UnInitializeUI();
}