#include "FileManager.h"
#include <iostream>
#include <sstream>
#include "Vector3.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::getline;

bool FileManager::ReadFile(const string myFile)
{
	ifstream file(myFile);
	static int count = 0;
	string data = "";
	string tempData = "";
	int nextData = 0;
	bool skipFirstLine = false;

	if (file.is_open())
	{
		while (getline(file, data))
		{
			if (!skipFirstLine)
			{
				skipFirstLine = true;
				continue;
			}

			for (int i = 0; i < data.size(); ++i)
			{
				if (nextData == 0)
				{
					for (int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
				}
			}
		}
	}

	return false;
}

void FileManager::EditFile(const string myFile)
{
}

Vector3 FileManager::Token2Vector(string token)
{
	Vector3 tempVector(0.0f, 0.0f, 0.0f);

	std::istringstream ss(token);
	std::string aToken = "";
	std::getline(ss, aToken, ',');
	tempVector.x = Token2Double(aToken);
	std::getline(ss, aToken, ',');
	tempVector.y = Token2Double(aToken);
	std::getline(ss, aToken, ',');
	tempVector.z = Token2Double(aToken);

	return tempVector;
}

double FileManager::Token2Double(string token)
{
	return atof(token.c_str());
}

bool FileManager::Token2Bool(string token)
{
	return token.at(0) == '1';
}