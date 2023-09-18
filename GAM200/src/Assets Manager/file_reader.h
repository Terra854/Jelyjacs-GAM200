#pragma once

#include <string>
#include <vector>

std::vector<std::vector<int>> mapdata;

/*
This function is meant to read a .txt file and only intergers from it and save it into a vector<vector<int>>
I plan to update them to be the serializer thing (maybe) taught in week 3 but ideally works for early on
Returns true if successful
*/
bool importMapFromFile(char* textfile);
bool s_importMapFromFile(std::string textfile);







