#pragma once
/* !
@file	Scenes.h
@author	Tay Sen Chuan
@date	2/11/2023

This file contains the declaration for loading and saving scenes
*//*__________________________________________________________________________*/
#include <string>

// Load all objects from a json scene file
void LoadSceneFromJson(std::string filename, bool isParentScene = true);

// Save all objects into a json file
void SaveScene(std::string);






