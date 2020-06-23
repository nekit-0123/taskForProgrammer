#include "LogReader.h"

CLogReader::CLogReader(const char *path)
	: filter(nullptr)
	, File(NULL)
{
	this->path = path;
}

CLogReader::~CLogReader()
{
	Close();
}

bool CLogReader::Open()
{
	File = CreateFile(path, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == File) 
		return false;
	
	return true;
}
void CLogReader::Close() const
{
	CloseHandle(File);
}

bool CLogReader::SetFilter(const char *filter)
{
	if (filter == nullptr || strlen(filter) == 0)
		return false;
	
	char *filterCheck = const_cast<char*> (filter);
	size_t size(strlen(filterCheck));

	for (long i = 1; i < (long)size; ++i)
	{
		if (filterCheck[i - 1] == filterCheck[i] && filterCheck[i] == '*')
		{
			memmove(&filterCheck[i - 1], &filterCheck[i], size - (i - 1));
			--i;
		}
	}

	this->filter = filterCheck;
	return true;
}


void CLogReader::WorkWithFile()
{
	char *buff(nullptr);				
	uint32_t fileRead(0);				
	uint32_t buffOffset(0);				
	uint32_t size(0);					
	DWORD fileReaded(0);				
	uint32_t maxBuffSizeToRd(1 << 20);	

	DWORD fileSz = GetFileSize(File, NULL);
	if (fileSz == 0)
	{
		std::cout << "Error ReadFile code: " << GetLastError() << std::endl;
		return;
	}

	if (fileSz < maxBuffSizeToRd)						
		maxBuffSizeToRd = fileSz;

	buff = new (std::nothrow) char[maxBuffSizeToRd];
	if (buff)
	{
		while (true)
		{
			ZeroMemory(buff, maxBuffSizeToRd);
			fileReaded = 0;

			size = maxBuffSizeToRd - buffOffset;
			if (ReadFile(File, &buff[buffOffset], size, &fileReaded, NULL) == false)
			{
				std::cout << "Error ReadFile code: " << GetLastError() << std::endl;
				break;
			}
	
			if (fileReaded == 0)
				break;
			
			buffOffset += fileReaded;

			if (size > fileReaded)
			{
				WorkWithLine(buff, buffOffset, true);
				break;
			}

			fileRead = WorkWithLine(buff, buffOffset);

			if (fileRead > 0)
			{		
				if (fileRead < buffOffset)
					memmove_s(buff, maxBuffSizeToRd, &buff[fileRead], buffOffset - fileRead);

				buffOffset -= fileRead;
			}
			else
				break;
		}

		delete[] buff;
		buff = nullptr;
	}
	else
		std::cout << "Error new memory WorkWithFile()" << std::endl;
}

uint32_t CLogReader::WorkWithLine(char *buf, const int bufsize, bool isEndLine/* = false*/)
{
	uint32_t pos(0);
	for (long i = 0; i < bufsize; ++i)
	{	
		if (buf[i] == '\n')
		{
			GetNextLine(&buf[pos], i - pos);
			pos = i + 1;
		}
	}


	if (isEndLine)
		GetNextLine(&buf[pos], bufsize - pos);
		
		
	return pos;
}

void CLogReader::GetNextLine(char *buf,  int bufsize)
{
	if (bufsize > 1 && buf[bufsize-1] == '\r')
		bufsize = bufsize - 1;

	char *StrReg = new(std::nothrow) char[bufsize+1];
	if (StrReg)
	{	
		ZeroMemory(StrReg, bufsize + 1);
		memcpy_s(StrReg, bufsize+1, buf, bufsize);

		if (isMatch(StrReg, const_cast<char*>(filter)))
			PrintStr(StrReg);

		delete[] StrReg;
		StrReg = nullptr;
	}
	else
		std::cout << "Error new memory GetNextLine()" << std::endl;
}

void CLogReader::PrintStr(char *buf) const
{	
	fprintf(stdout, "%s \n", buf);
}

bool CLogReader::isMatch(char *buf, char *filter)
{
	if ((*filter == '\0') && (*buf == '\0'))
		return true;

	if (*filter == '\0')
		return(0); 

	if (*filter == '*')
	{
		if (*(filter + 1) == '\0')
			return true; 

		for (long i = 0, size = strlen(buf); i <= size; i++)
			if ((*(buf + i) == *(filter + 1)) ||
				(*(filter + 1) == '?'))
				if (isMatch(buf + i + 1, filter + 2) == 1)
					return true;
	}
	else
	{
		if (*buf == '\0')
			return false;

		if ((*filter == '?') || (*filter == *buf))
			if (isMatch(buf + 1, filter + 1) == 1)
				return true;
	}

	return false;
}