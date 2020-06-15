#include "LogReader.h"

#define MAX_FILTER	150

int main(int argc, char* argv[])
{
	char sPathToFile[_MAX_PATH + 1] = { NULL };
	char sFilter[MAX_FILTER] = { NULL };

	if (argc < 3)
	{
		std::cout << "Input path: ";
		std::cin.getline(sPathToFile, _MAX_PATH + 1);

		std::cout << "Input filter: ";
		std::cin.getline(sFilter, MAX_FILTER);
	}
	else if (argc == 3)
	{
		strcpy_s(sPathToFile, argv[1]);
		strcpy_s(sFilter, argv[2]);
	}
	else
	{
		std::cout << "Incorrect Input param" << std::endl;
		return -1;
	}

	CLogReader* logReader = new (std::nothrow) CLogReader(sPathToFile);
	if (logReader)
	{
		if (logReader->SetFilter(sFilter) == false)
			std::cout << "Incorrect filter" << std::endl;
		else
		{
			if (logReader->Open() == false)
				std::cout << "Error Open File" << std::endl;
			else
				logReader->WorkWithFile();
		}

		delete logReader;
		logReader = nullptr;
	}
	else 
		std::cout << "Error new memory mainFunc()" << std::endl;

	system("pause");
	return 0;
}

