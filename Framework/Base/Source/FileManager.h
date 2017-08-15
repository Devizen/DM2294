#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "SingletonTemplate.h"
#include <string>
#include <iostream>
#include <fstream>
#include "Vector3.h"
#include "Attributes.h"

using std::string;

class FileManager : public Singleton <FileManager>, public CAttributes
{
	friend Singleton<FileManager>;

public:
	FileManager() {};
	~FileManager() {};

	bool ReadPlayerFile(const string myFile);
	bool ReadWeaponFile(const string myFile);
	bool ReadAABBFile(const string myFile);
	bool ReadLevelFile(const string myFile);
	bool ReadDirectionFile(const string myFile);
	
	//bool ReadFile(const string myFile);

	void EditFile(const string myFile);
	Vector3 Token2Vector(string token);
	double Token2Double(string token);
	bool Token2Bool(string token);
};

#endif