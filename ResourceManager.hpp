#pragma once
#include <iostream>
#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics.hpp>
#include <filesystem>

using namespace std;
using namespace sf;
namespace fs = filesystem;

class ResourceManager 
{
protected:
	bool isFullscreen = false;
	bool isAmbientPlay = true;
	bool allLoaded = false;
	int i = 0;
	string pathToMusic = "music/";
	vector<string> allMusic;
	map<string,Texture> loadedTextures;
	Music ambient;
	Vector2i resolution = Vector2i(1280, 720);
public:
	static ResourceManager& getInstance();

	void loadTexture(const string &name, const string &path);

	void unloadTexture(const string &name);

	Texture& getTexture(const string &name);
};
	
class Settings : protected ResourceManager 
{
private:
	Settings() {
		for (auto& file : fs::directory_iterator(pathToMusic)) {
			allMusic.push_back(file.path().filename().string());
		}
		i = rand() % (allMusic.size() - 1);
		ambient.setVolume(20);
		allLoaded = true;
}
public:
	static Settings& getInstance();

	Vector2i getResolution();

	bool successfulLoaded();

	void switchFullScreen(RenderWindow& window);

	float getVolume(string music);

	void setVolume(string music, float volume_);

	void switchMusic();

	void updateMusic();
};