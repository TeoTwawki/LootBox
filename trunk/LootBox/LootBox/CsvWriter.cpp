#include "stdafx.h"
#include "CsvWriter.h"

/*! \brief CsvWriter constructor */
template <typename T>CsvWriter<T>::CsvWriter()
	     : m_bFileOpened(false), m_LineCount(0),
		   m_ColumnCount(0), m_ColumnIndex(0),
		   m_pFile(new T)
{}

/*! \brief CsvWriter destructor */
template <typename T>CsvWriter<T>::~CsvWriter()
{
	Cleanup();
}

/*! \brief Performs cleanup operations */
template <typename T> void CsvWriter<T>::Cleanup()
{
	// delete the file object owned by this object, if any
	if (m_pFile != NULL)
	{
		delete m_pFile;
		m_pFile = NULL;
	}

	if (m_bFileOpened)
	{
		WriteLine();
		CloseFile();
	}
}

/*! \brief Opens and creates a file given its filename
	\param[in] pFilename_in : the path of the file
	\param[in] Format_in : the desired format for the CSV file
	\param[in] ColumnCount_in : the number of columns in the file
	\param[in] OpenFlags_in : flags to be passed to CFile::Open(...)
	\return true if the file was created successfully; false otherwise
			a CFileException will be thrown if the file creation failed
*/
template <typename T> bool CsvWriter<T>::CreateFile(const TCHAR *pFilename_in, UINT ColumnCount_in, DWORD Format_in, int OpenFlags_in)
{
	if (m_bFileOpened == false && m_pFile != NULL)
	{
		CFileException *pExcept = new CFileException;

		m_bFileOpened = (m_pFile->Open(pFilename_in, OpenFlags_in, pExcept) != FALSE);
		m_LineCount = m_ColumnIndex = 0;
		m_ColumnCount = ColumnCount_in;

		if (m_bFileOpened)
		{
			SetDialectFromFormat(Format_in);
			m_Filename = pFilename_in;
			pExcept->Delete();
			WriteHeader();
		}
		else
			throw pExcept;
	}

	return m_bFileOpened;
}

/*! \brief Closes the CSV file 
	\return true if the file is closed; false otherwise
*/
template <typename T> bool CsvWriter<T>::CloseFile(void)
{
	if (m_pFile != NULL)
	{
		m_bFileOpened = (m_pFile->m_hFile != INVALID_HANDLE_VALUE);

		if (m_bFileOpened)
		{
			m_pFile->Flush();
			m_pFile->Close();
			m_bFileOpened = false;
		}
	}

	return !m_bFileOpened;
}

/*! \brief Writes the header of the CSV file
	\return true if the file is opened; false otherwise
*/
template <typename T> bool CsvWriter<T>::WriteHeader(void)
{
	if (m_bFileOpened == false || m_pFile == NULL)
		return false;

	if (m_Dialect.m_Header[0] != NULL && m_Dialect.m_Header[1] != NULL)
		m_pFile->Write(m_Dialect.m_Header, 2);
	else if (m_Dialect.m_Header[0] != NULL)
		m_pFile->Write(m_Dialect.m_Header, 1);

	return true;
}

/*! \brief Escapes the specified string for use as a CSV column value
	\param[in,out] String_in_out : the string to be escaped
	\return true if the string was modified; false otherwise
*/
template <typename T> bool CsvWriter<T>::EscapeString(CString &String_in_out)
{
	TCHAR Replace[3] = { m_Dialect.m_Delimiter, m_Dialect.m_Delimiter, 0x0 };
	TCHAR Delimiter[2] = { m_Dialect.m_Delimiter, 0x0 };

	return (String_in_out.Replace(Delimiter, Replace) != 0);
}

/*! \brief Adds a column to the file
	\param[in] pValue_in : the value of the column
	\param[in] bEscaped_in : a flag specifying if the value is already escaped
	\return a reference to the writer to chain methods
*/
template <typename T> CsvWriter<T>& CsvWriter<T>::AddColumn(const TCHAR *pValue_in, bool bEscaped_in)
{
	ASSERT(m_bFileOpened);

	if (m_bFileOpened)
	{
		ASSERT(m_ColumnCount > 0);

		bool bLineBreak = (++m_ColumnIndex >= m_ColumnCount);
		CString strEscapedValue = pValue_in;

		if (bEscaped_in == false)
			EscapeString(strEscapedValue);

		if (bLineBreak)
		{
			// add the column and the line break
			m_CurrentLine.AppendFormat(_T("%c%s%c%s"), m_Dialect.m_Delimiter, strEscapedValue,
									   m_Dialect.m_Delimiter, m_Dialect.m_LineBreak);
			// write the current line
			WriteLine();
		}
		else
		{
			// add the column
			m_CurrentLine.AppendFormat(_T("%c%s%c%c"), m_Dialect.m_Delimiter, strEscapedValue,
									   m_Dialect.m_Delimiter, m_Dialect.m_Separator);
		}
	}

	return *this;
}

/*! \brief Adds a formatted column to the file
	\param[in] pFormat_in : the value of the column
	\param[in] ... : the variables needed for the string formatting
	\return a reference to the writer to chain methods
*/
template <typename T> CsvWriter<T>& CsvWriter<T>::AddColumnFormat(const TCHAR *pFormat_in, ...)
{
	int MessageLength;
	va_list ArgList;
	TCHAR *pColumn;

	// retrieve the arguments list
	va_start(ArgList, pFormat_in);
	// compute the length of the formatted string
	MessageLength = _vsctprintf(pFormat_in, ArgList) + 1;
	// allocate the memory
	pColumn = (TCHAR*)malloc(MessageLength * sizeof(TCHAR));
	// format the string
	_vstprintf_s(pColumn, MessageLength, pFormat_in, ArgList);
	va_end(ArgList);
	// add the column
	AddColumn(pColumn);
	// cleanup
	free(pColumn);

	return *this;
}

/*! \brief Writes the current line in the file
	\param[in] bAutocomplete_in : flag specifying if the missing columns
			   should be created before writing the line
	\return a reference to the writer to chain methods
*/
template <typename T> CsvWriter<T>& CsvWriter<T>::WriteLine(bool bAutocomplete_in)
{
	ASSERT(m_bFileOpened);

	if (m_bFileOpened && m_pFile != NULL && m_CurrentLine.IsEmpty() == false)
	{
		ASSERT(m_ColumnCount > 0);

		if (bAutocomplete_in)
		{
			int EmptyColumns = m_ColumnCount - m_ColumnIndex;

			// complete the line if it's lacking columns
			if (EmptyColumns > 0)
				AddBlank(EmptyColumns);
		}
		// write the current line
		m_pFile->Write(m_CurrentLine.GetBuffer(0), m_CurrentLine.GetLength() * sizeof(TCHAR));
		// empty the current line
		m_CurrentLine.Empty();
		m_ColumnIndex = 0;
		m_LineCount++;
	}

	return *this;
}

/*! \brief Adds a specified number of empty columns 
	\param[in] ColumnCount_in : the number of empty columns to add
	\return a reference to the writer to chain methods
*/
template <typename T> CsvWriter<T>& CsvWriter<T>::AddBlank(UINT ColumnCount_in)
{
	ASSERT(m_bFileOpened);

	// check if the file is opened
	if (m_bFileOpened && ColumnCount_in > 0)
	{
		ASSERT(m_ColumnCount > 0);

		// avoid looping for 1 column
		if (ColumnCount_in == 1)
		{
			m_CurrentLine += m_Dialect.m_Separator;
			m_ColumnIndex++;
		}
		else
		{
			// 
			for (UINT Index = 0; Index < ColumnCount_in; ++Index)
			{
				if (++m_ColumnIndex < m_ColumnCount)
				{
					m_CurrentLine += m_Dialect.m_Separator;
				}
				else
				{
					// the number of columns in the file has been reached
					m_CurrentLine += m_Dialect.m_LineBreak;
					// write the current line
					m_ColumnIndex = 0;
					WriteLine(false);
				}
			}
		}
	}

	return *this;
}

/*! \brief Sets the format of the CSV file
	\param[in] Format_in : the desired format for the CSV file
*/
template <typename T> void CsvWriter<T>::SetDialectFromFormat(DWORD Format_in)
{
	// set the common default values
	m_Dialect.m_Delimiter = '"';
	m_Dialect.m_Separator = '\t';
	m_Dialect.m_LineBreak[0] = '\r';
	m_Dialect.m_LineBreak[1] = '\n';
	m_Dialect.m_LineBreak[2] = '\0';

	// modify the default dialect depending on the format
	switch(Format_in)
	{
		case CSV_FORMAT_EXCEL_5_97:
			// Add the UNICODE low bit version header only for "recent" Excel
			// because the old version does not support the standard UNICODE header !
			m_Dialect.m_Header[0] = NULL;
			m_Dialect.m_Header[1] = NULL;
		break;
		case CSV_FORMAT_EXCEL_2000_XP:
			m_Dialect.m_Header[0] = 0xFF;
			m_Dialect.m_Header[1] = 0xFE;			
		break;
	}
}

template CsvWriter<CFile>;