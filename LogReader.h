#pragma once

#include "windows.h"
#include <iostream>
#include <sstream>

class CLogReader
{
public:
	CLogReader(const char *path);
	~CLogReader();

	bool Open();							 
	void Close() const;						 
	bool SetFilter(const char *filter);		 
	void WorkWithFile();					 

private:
	uint32_t WorkWithLine(char *buf,		 
		const int bufsize, 
		bool isEndLine = false);

	void GetNextLine(char *buf,				 	
		 int bufsize);

	bool isMatch(char *buf, char *filter);
	void PrintStr(char *buf) const;			
		
private:
	const char *filter;						
	const char *path;						

	HANDLE File;						
};