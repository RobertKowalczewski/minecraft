#pragma once

class Shader {
public:
	unsigned int ID;
	//reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);
	//use/activate the shader
	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
};