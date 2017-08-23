#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "SingletonTemplate.h"
#include <string>
#include <iostream>
#include <fstream>
#include "Vector3.h"
#include <vector>
#include "Items\Equipment.h"

using std::vector;
using std::string;

class FileManager : public Singleton <FileManager>
{
	friend Singleton<FileManager>;

	//string eqAtk;
	//string eqDef;
	//string eqSpeed;
	//string eqId;
	//string eqType;
	//string eqName;


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

	struct OBJinfo
	{
		string type;
		float posX;
		float posY;
		float posZ;

		float minAABBx;
		float minAABBy;
		float minAABBz;

		float maxAABBx;
		float maxAABBy;
		float maxAABBz;

		float scalex;
		float scaley;
		float scalez;

		float rotateAngle;
	}theOBJinfo;

	struct enemyInfo
	{
		string type;

		float displacementX;
		float displacementY;
		float displacementZ;

		float scaleX;
		float scaleY;
		float scaleZ;
	}theEnemyInfo;

	vector<EQ_Info>masterList;
	vector<OBJinfo>objlist;
	vector<Equipment*> eqlist;

public:
	FileManager() {};
	~FileManager() {};

	void init();

	bool ReadPlayerFile(const string myFile);
	bool ReadWeaponFile( string myFile);
	bool ReadMapFile(const string myFile);
	bool ReadEnemyFile(const string myFile);
	
	//bool ReadFile(const string myFile);

	void EditFile(const string myFile);
	void EditWeaponFile(const string myFile);
	void EditMapFile(const string myFile);
	void EditEnemyFile(const string myFile);

	void PrintWeaponFile();

	void CreateWeapon();

	Vector3 Token2Vector(string token);
	double Token2Double(string token);
	bool Token2Bool(string token);

	vector<EQ_Info> returnMasterList();

	void clearVector();
};

#endif