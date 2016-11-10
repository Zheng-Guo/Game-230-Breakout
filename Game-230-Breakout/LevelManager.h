#pragma once
#include <vector>
#include <memory>
#include <fstream>
#include <string>
#include "GameConstants.h"
#include "Level.h"

using namespace std;

class LevelManager {
private:
	vector<shared_ptr<Level>> levels;
	vector<shared_ptr<Level>>::iterator currentLevel;
public:
	LevelManager() {
		ifstream ifs(string(Config_Folder) + '/' + string(Config_File_Name));
		string levelConfigFile;
		while (ifs >> levelConfigFile) {
			shared_ptr<Level> level = make_shared<Level>();
			level->loadConfig(string(Config_Folder) + '/'+levelConfigFile);
			levels.push_back(level);
		}
		currentLevel = levels.begin();
	}
	shared_ptr<Level> getFirstLevel();
	shared_ptr<Level> getNextLevel();
};

shared_ptr<Level> LevelManager::getFirstLevel() {
	currentLevel = levels.begin();
	(*currentLevel)->resetBricks();
	return *currentLevel;
}

shared_ptr<Level> LevelManager::getNextLevel() {
	++currentLevel;
	if (currentLevel == levels.end())
		currentLevel = levels.begin();
	auto level = *currentLevel;
	level->resetBricks();
	return level;
}