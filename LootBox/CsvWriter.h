#ifndef __CSV_WRITER_H__
#define __CSV_WRITER_H__

class IFile;

/*! \brief CSV file format */
typedef struct _CsvDialect
{
	BYTE	m_Header[2];	//!< file header (for BOM support)
	TCHAR	m_Delimiter;	//!< CSV delimiter
	TCHAR	m_Separator;	//!< CSV separator
	TCHAR   m_LineBreak[3]; //!< CSV line break

	/*! \brief CsvDialect constructor */
	_CsvDialect() : m_Delimiter('"'), m_Separator(',')
	{
		m_LineBreak[0] = '\r';
		m_LineBreak[1] = '\n';
		m_LineBreak[2] = '\0';
		m_Header[0] = 0xFF;
		m_Header[1] = 0xFE;
	}

} CsvDialect;

/*! \brief CSV writer */
template <typename T> class CsvWriter
{
public:
	//! Excel format -- indexes matches the open file dialog file type combo
	enum CsvFormat
	{
		CSV_FORMAT_UNSUPPORTED = 0,
		CSV_FORMAT_EXCEL_2000_XP,
		CSV_FORMAT_EXCEL_5_97,
		CSV_FORMAT_COUNT
	};

	/*! \brief CsvWriter constructor */
	CsvWriter();
	/*! \brief CsvWriter destructor */
	~CsvWriter();

	/*! \brief Opens and creates a file given its filename
		\param[in] pFilename_in : the path of the file
		\param[in] ColumnCount_in : the number of columns in the file
		\param[in] Format_in : the desired format for the CSV file
		\param[in] OpenFlags_in : flags to be passed to CFile::Open(...)
		\return true if the file was created successfully; false otherwise
				a CFileException will be thrown if the file creation failed
	*/
	bool CreateFile(const TCHAR *pFilename_in, UINT ColumnCount_in = 1, 
					DWORD Format_in = CSV_FORMAT_EXCEL_2000_XP, 
					int OpenFlags_in = CFile::modeCreate | CFile::modeWrite);
	/*! \brief Closes the CSV file 
		\return true if the file is closed; false otherwise
	*/
	bool CloseFile(void);
	/*! \brief Escapes the specified string for use as a CSV column value
		\param[in,out] String_in_out : the string to be escaped
		\return true if the string was modified; false otherwise
	*/
	bool EscapeString(CString &String_in);
	/*! Sets the number of columns in the CSV file
		\param[in] ColumnCount_in : the number of columns
	*/
	void SetColumnCount(UINT ColumnCount_in = 0) { m_ColumnCount = ColumnCount_in; }
	/*! \brief Returns a pointer to the file object
		\return a pointer to the file object
	*/
	T* GetFile(void) { return m_pFile; }
	/*! \brief Adds a column to the file
		\param[in] pValue_in : the value of the column
		\param[in] bEscaped_in : a flag specifying if the value is already escaped
		\return a reference to the writer to chain methods
	*/
	CsvWriter& AddColumn(const TCHAR *pValue_in, bool bEscaped_in = false);
	/*! \brief Adds a formatted column to the file
		\param[in] pFormat_in : the value of the column
		\param[in] ... : the variables used in the formatted string
		\return a reference to the writer to chain methods
	*/
	CsvWriter& AddColumnFormat(const TCHAR *pFormat_in, ...);
	/*! \brief Adds a specified number of empty columns 
		\param[in] ColumnCount_in : the number of empty columns to add
		\return a reference to the writer to chain methods
	*/
	CsvWriter& AddBlank(UINT ColumnCount_in = 1);
	/*! \brief Writes the current line in the file
		\param[in] bAutocomplete_in : flag specifying if the missing columns
				   should be created before writing the line
		\return a reference to the writer to chain methods
	*/
	CsvWriter& WriteLine(bool bAutocomplete_in = true);
	/*! \brief Returns the dialect of the CSV file 
		\return a reference to the dialect of the CSV file
	*/
	CsvDialect& GetDialect(void) { return m_Dialect; }
	/*! \brief Returns the number of lines written so far in the CSV file
		\return the current number of lines in the CSV file
	*/
	UINT GetLineCount(void) { return m_LineCount; }


protected:
	/*! a flag specifying if the CSV file is opened */
	bool		m_bFileOpened;
	/*! a string holding the content of the current line */
	CString		m_CurrentLine;
	/*! a string holding the path of the CSV file */
	CString		m_Filename;
	/*! a dialect object containing the parameters of the CSV file format */
	CsvDialect	m_Dialect;
	/*! a file object used to write the data to a file */
	typename T *m_pFile;
	/*! the number of columns in the CSV file */
	UINT		m_ColumnCount;
	/*! the index of the current column in the current line */
	UINT		m_ColumnIndex;
	/*! the number of lines written in the file */
	UINT		m_LineCount;

	/*! \brief Writes the header of the CSV file
		\return true if the file is opened; false otherwise
	*/
	bool WriteHeader(void);
	/*! \brief Sets the format of the CSV file
		\param[in] Format_in : the desired format for the CSV file
	*/
	void SetDialectFromFormat(DWORD Format_in = CSV_FORMAT_EXCEL_2000_XP);
	/*! \brief Performs cleanup operations */
	void Cleanup();
};

#endif//__CSV_WRITER_H__