//#include "OptionsManager.h"
//#include "Controls.h"
//#include <iostream>
//#include <fstream>
//#include <string>
//
//using std::string;
//using std::getline;
//using std::ifstream;
///*Output*/
//using std::ofstream;
//using std::stoi;
//using std::cout;
//using std::endl;
//
//#include "../PlayerInfo/PlayerInfo.h"
//
//OptionsManager::OptionsManager() :
//	editingState(false)
//	, highscore(0)
//{
//}
//
//OptionsManager::~OptionsManager()
//{
//	while (controls.size() > 0)
//	{
//		Controls* object = controls.back();
//		delete object;
//		object = nullptr;
//
//		controls.pop_back();
//	}
//}
//
//
//vector<Controls*>& OptionsManager::getControls(void)
//{
//	return controls;
//}
//
//int OptionsManager::getInput(int _type)
//{
//	for (vector<Controls*>::iterator it = controls.begin(); it != controls.end(); ++it)
//	{
//		Controls* control = (Controls*)*it;
//
//		if (control->checkType(_type))
//			return control->getKey();
//
//	}
//	return NULL;
//}
//
//bool OptionsManager::getEditingState(void)
//{
//	return editingState;
//}
//
//bool OptionsManager::loadConfig(string filePath)
//{
//	while (controls.size() > 0)
//	{
//		Controls* key = controls.back();
//		delete key;
//		key = nullptr;
//
//		controls.pop_back();
//	}
//
//	string data = "";
//	string _type = "";
//	string _key = "";
//	bool storedType = false;
//	ifstream file;
//	/*Prevent player from loading wrong file for now.*/
//	file.open("Data//keyconfig.txt");
//	if (file.is_open())
//	{
//		while (std::getline(file, data)) // To get you all the lines.
//		{
//			for (unsigned int i = 0; i < data.size(); ++i)
//			{
//				if (!storedType && data[i] != ',')
//					_type += data[i];
//				
//
//				else if (!storedType && data[i] == ',')
//				{
//					storedType = true;
//					continue;
//				}
//
//				else if (storedType)
//					_key += data[i];
//
//			}
//			Controls* newInput = new Controls(_type, stoi(_key));
//			//newInput->SetType(_type);
//			//newInput->set_mKey(stoi(_key));
//			_type = "";
//			_key = "";
//			storedType = false;
//
//			controls.push_back(newInput);
//		}
//		file.close();
//		return true;
//	}
//	return false;
//}
//
//bool OptionsManager::saveConfig(void)
//{
//	ofstream output("Data//tempConfig.txt");
//
//	for (vector<Controls*>::iterator it = controls.begin(); it != controls.end(); ++it)
//	{
//		Controls* input = (Controls*)*it;
//		output << input->getStrType() << "," << input->getKey();
//		if (it + 1 != controls.end())
//			output << endl;
//		continue;
//	}
//
//	output.close();
//	remove("Data//keyconfig.txt");
//	rename("Data//tempConfig.txt", "Data//keyconfig.txt");
//	return true;
//}
//
//bool OptionsManager::defaultConfig(void)
//{
//	ifstream infile("Data//default.txt");
//	ofstream outfile("Data//output.txt");
//	string content = "";
//	int i;
//
//	for (i = 0; infile.eof() != true; i++)/*Get content until end of line.*/
//		content += infile.get();
//
//	i--;
//	content.erase(content.end() - 1);/*Erase last.*/
//
//	infile.close();
//
//	outfile << content; /*Save content in.*/
//	outfile.close();
//
//	remove("Data//keyconfig.txt");
//
//	rename("Data//output.txt", "Data//keyconfig.txt");
//	return true;
//}
//
//bool OptionsManager::loadHighscore(string filePath)
//{
//
//	string data = "";
//	ifstream file;
//	/*Prevent player from loading wrong file for now.*/
//	file.open("Data//highscore.txt");
//	if (file.is_open())
//	{
//		while (std::getline(file, data)) // To get you all the lines.
//		{
//			highscore = stoi(data);
//		}
//		file.close();
//		CPlayerInfo::GetInstance()->setGold(highscore);
//		return true;
//	}
//	
//	return false;
//}
//
//bool OptionsManager::saveHighscore(void)
//{
//	ofstream output("Data//tempHighscore.txt");
//	output << highscore;
//	output.close();
//	remove("Data//highscore.txt");
//	rename("Data//tempHighscore.txt", "Data//highscore.txt");
//	return true;
//}
//
//void OptionsManager::setHighscore(int _highscore)
//{
//	highscore = _highscore;
//}
//
//int OptionsManager::getHighscore(void)
//{
//	return highscore;
//}
//
//Controls* &OptionsManager::setInput(int _type)
//{
//	for (vector<Controls*>::iterator it = controls.begin(); it != controls.end(); ++it)
//	{
//		Controls* control = (Controls*)*it;
//
//		if (control->checkType(_type))
//			return control;
//	}
//}
//
//void OptionsManager::setEditingState(bool _editingState)
//{
//	editingState = _editingState;
//}
