#pragma once
#include <Debug.h>

#include <string>
#include <vector>
#include <filesystem>
#include "../../include/Interface_System.h"
#include "../../include/Graphic_gl/glapp.h"

class AssetManager : public ISystems
{
	public:


	virtual void Initialize();
	virtual void Free();
	virtual void Update(float dt);
	virtual std::string SystemName();
	void loadassets();






	// These data shouldn't be modified at all
	private:
	std::filesystem::path pathtexture = "../Asset/Picture";
	//std::filesystem::path audio;
	//std::filesystem::path fonts;
};

extern std::map<std::string, GLuint> textures;





