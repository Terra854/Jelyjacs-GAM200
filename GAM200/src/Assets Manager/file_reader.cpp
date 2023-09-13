
#include "file_reader.h"
#include <iostream>
#include <fstream>
#include <vector>

/*
This function is meant to read a .txt file and only intergers from it and save it into a vector<vector<int>>
I plan to update them to be the serializer thing (maybe) taught in week 3 but ideally works for early on
Returns true if successful
*/
bool importMapFromFile(char* textfile)
{
	mapdata.clear();
	std::ifstream mapfile;
	mapfile.open(textfile, std::ios::in);

	if (!mapfile.is_open())
	{
		std::cout << "Error opening map file." << std::endl;
		return false;
	}

	int mapwidth{};
	int mapheight{};
	int checker{};

	while (!mapfile.eof())
	{
		char id{};
		mapfile.get(id);
		if (checker == 0 && id >= '0' && id <= '9')
		{
			mapwidth = static_cast<int>(id - '0');
			mapfile.get(id);
			while (id >= '0' && id <= '9')
			{
				mapwidth = mapwidth * 10 + static_cast<int>(id - '0');
				mapfile.get(id);
			}
			checker++;
			continue;
		}

		if (checker == 1 && id >= '0' && id <= '9')
		{
			mapheight = static_cast<int>(id - '0');
			mapfile.get(id);
			while (id >= '0' && id <= '9')
			{
				mapheight = mapheight * 10 + static_cast<int>(id - '0');
				mapfile.get(id);
			}
			checker++;

			if (!(mapheight > 0 && mapwidth > 0))
			{
				std::cout << "Invalid map format: width and height must be > 0." << std::endl;
				return false;
			}
			continue;
		}

		if (checker > 1)
		{
			for (int i{}; i < mapwidth; i++)
			{
				int data{};
				std::vector<int> row;
				for (int j{}; j < mapheight; j++)
				{
					while (mapfile.get(id)) // Cycle till int found
					{
						data = static_cast<int>(id - '0');
						mapfile.get(id);
						while (id >= '0' && id <= '9') // Check consecutive int
						{
							data = data * 10 + static_cast<int>(id - '0');
							mapfile.get(id);
						}
						break; 
					}

					if (mapfile.eof()) // Possible cause of error if directly before eof, the character is an interger and map dimension matches, text file could add a END at the end if this is a problem
					{
						std::cout << "Dimension Mismatch: width or height in the file did not match." << std::endl;
						mapdata.clear();
						return false;
					}

					row.push_back(data);
				}
				mapdata.push_back(row);
			}
		}

	}

	mapfile.close();
	return true;
}

