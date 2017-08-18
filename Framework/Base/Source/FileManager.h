#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "SingletonTemplate.h"
#include <string>
#include <iostream>
#include <fstream>
#include "Vector3.h"
#include <vector>
#include "Attributes.h"

using std::vector;
using std::string;

class FileManager : public Singleton <FileManager>, public CAttributes
{
	friend Singleton<FileManager>;

	//string eqAtk;
	//string eqDef;
	//string eqSpeed;
	//string eqId;
	//string eqType;
	//string eqName;

	int pressCountX;
	int pressCountY;

	struct EQ_Info
	{
		string name;
		string eqAtk;
		string eqDef;
		string eqSpeed;
		string eqId;
		string eqType;
		bool isEquipped;
	}theEQInfo;
	vector<EQ_Info>masterList;

public:
	FileManager() {};
	~FileManager() {};

	void init();

	bool ReadPlayerFile(const string myFile);
	bool ReadWeaponFile( string myFile);
	bool ReadAABBFile(const string myFile);
	bool ReadLevelFile(const string myFile);
	
	//bool ReadFile(const string myFile);

	void EditFile(const string myFile);
	void RenderWeapon();
	void DeleteWeapon();

	void update(double dt);

	void PrintWeaponFile();
	void CreateWeapon();

	Vector3 Token2Vector(string token);
	double Token2Double(string token);
	bool Token2Bool(string token);

	vector<EQ_Info> returnMasterList();
};

#endif