#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <map>
#include <string>
#include "SingletonTemplate.h"

struct Vector3;
class Mtx44;

class ShaderProgram : public Singleton<ShaderProgram>
{
public:
	const unsigned int SHADER_ERROR = UINT_MAX;

	ShaderProgram();
	ShaderProgram(unsigned int _programID);
	~ShaderProgram();

	unsigned int GetProgramID();
	void SetProgramID(const unsigned int _programID);

	unsigned int AddUniform(const std::string& _name);
	unsigned int GetUniform(const std::string& _name);
	unsigned int GetOrAddUniform(const std::string& _name);

	void UpdateInt(unsigned int _ID, int _value);
	void UpdateFloat(unsigned int _ID, float _value);
	void UpdateVector3(unsigned int _ID, const Vector3& _value);
	void UpdateVector3(unsigned int _ID, float* _startPtr);
	void UpdateMatrix44(unsigned int _ID, const Mtx44& _value);
	void UpdateMatrix44(unsigned int _ID, float* _startPtr);

	void UpdateInt(const std::string& _name, int _value);
	void UpdateFloat(const std::string& _name, float _value);
	void UpdateVector3(const std::string& _name, const Vector3& _value);
	void UpdateVector3(const std::string& _name, float* _startPtr);
	void UpdateMatrix44(const std::string& _name, const Mtx44& _value);
	void UpdateMatrix44(const std::string& _name, float* _startPtr);

	void RemoveUniformMap(void);

	ShaderProgram* currProg;
	ShaderProgram* m_gPassShaderID;

private:
	unsigned int programID;
	std::map<std::string, unsigned int> uniformMap;
};

#endif // SHADER_PROGRAM_H