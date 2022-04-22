#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;
using namespace std;



string parseConfig();
void coreMods(string);
void bazar(string);
void cleaner(string);
string lastFolder(string);


int main()
{
	string basePath;
	basePath = parseConfig();
	if (basePath == "empty") {
		return 1;
	}
	//Append "\\" at the end due to possible idiots using the tool
	if (basePath[basePath.length() - 1] != '\\') {
		basePath = basePath + "\\";
	}
	coreMods(basePath);
	cout << "Hit enter to close\n";
	string hold;
	cin.get();
	return 0;
}

/*
Retrieves the user entered path from config.ini, returns the path
*/
string parseConfig() 
{
	string basePath;
	ifstream configFile;
	string line = "";
	configFile.open("config.ini", ios::in);

	bool pathFound = false;
	if (configFile.is_open()) 
	{
		int rows = 0;
			
		while (getline(configFile, line))
		{
			if (line[0] == '#') {
				continue;
			}
			else if (line.substr(0, 4) == "path") {
				basePath = line.substr(5, line.size());
				pathFound = true;
			}
		}
	}
	else {
		cout << "You forgot config.ini you absolute muppet!";
		return "empty";
	}

	if (!pathFound) {
		cout << "You have not included the path, you fuckwit!";
		return "empty";
	}
	return basePath;
}

/*
Iterate through the coreMods folders and each livery for each module version.

TODO: include exceptions for zipped, non-locked nations
*/
void coreMods(string basePath) {
	
	string totalPath = basePath + "CoreMods\\aircraft";
	for (fs::directory_entry modules : fs::directory_iterator(totalPath)) {
		string nextStep = modules.path().string() + "\\Liveries\\";
		cout << "Removing countries from " << lastFolder(modules.path().string()) << "\n";
		for (fs::directory_entry types : fs::directory_iterator(nextStep)) 
		{
			for (fs::directory_entry liveries : fs::directory_iterator(types)) {
				
				cleaner(liveries.path().string() + "\\");
			}
		}
	}


}
void bazar(string basePath) {
	cout << "blah";
}


/*
Remove the countries' entries from the description.lua at the described location
*/
void cleaner(string liveryPath) {
	cout << "   " << lastFolder(liveryPath) << "\n";
	string luaPath = liveryPath + "description.lua";
	fstream lua;
	lua.open(luaPath, ios::in);
	if (lua.is_open())
	{
		string config = "";
		string line;
		bool stillCountries = false;
		while (getline(lua, line)) {
			if (stillCountries) {
				if (line.find("}") != string::npos) {
					config = config + "countries = {}\n";
					stillCountries = false;
				}
			}
			else if (line.find("countries") != string::npos) {
				if (line.find("}") != string::npos) {
					config = config + "countries = {}\n";
				}
				else {
					stillCountries = true;
				}
			}
			else 
			{
				config = config + line + "\n";
			}
		}
		lua.close();
		lua.open(luaPath, ios::out | ios::trunc);
		if (lua.is_open()) {
			lua << config;
			lua.close();
		}
	}

}
/*
Get the last folder name from a path. Returns the folder name.
*/
string lastFolder(string fullPath) {
	string last = "";
	string trimmedPath = "";
	//Clean last slashes.
	if (fullPath.substr(fullPath.size() - 1, fullPath.size()) == "\\") {
		trimmedPath = fullPath.substr(0, fullPath.size() - 1);
	}
	else
	{
		trimmedPath = fullPath;
	}
	int index = trimmedPath.find_last_of("\\");
	last = trimmedPath.substr(index + 1, trimmedPath.size());
	return last;
}

