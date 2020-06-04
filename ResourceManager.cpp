#define NOMINMAX
#include<Windows.h>
#include <iostream>
#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics.hpp>
#include "ResourceManager.hpp"
const int FLWIDTH = GetSystemMetrics(SM_CXSCREEN); //Разрешение экрана по X
const int FLHEIGHT = GetSystemMetrics(SM_CYSCREEN); //Разрешение экрана по Y

//const int FLWIDTH = 1920;
//const int FLHEIGHT = 1080;

using namespace std;
using namespace sf;


ResourceManager& ResourceManager::getInstance()
{
	static ResourceManager instance;
	return instance;
}

void ResourceManager::loadTexture(const string &name, const string &path)
{
	loadedTextures.insert({ name, Texture() });
	loadedTextures[name].loadFromFile(path);
}

void ResourceManager::unloadTexture(const string& name)
{
	loadedTextures.erase(name);
}

Texture& ResourceManager::getTexture(const string& name)
{
	return loadedTextures[name];
}

Settings& Settings::getInstance() {
	static Settings instance;
	return instance;
}

Vector2i Settings::getResolution() {
	return resolution;
}

void Settings::switchFullScreen(RenderWindow& window) {
	if (isFullscreen) {
		window.create(VideoMode(1280, 720), "Arkanoid", Style::Default);
		resolution = Vector2i(1280, 720);
	}
	else {
		resolution = Vector2i(FLWIDTH, FLHEIGHT);
		window.create(VideoMode(FLWIDTH, FLHEIGHT), "Arkanoid", Style::Fullscreen);
	}
	isFullscreen = !isFullscreen;
}

bool Settings::successfulLoaded() {
	return allLoaded;
}

float Settings::getVolume(string music) {
	if (music == "ambient") {
		return ambient.getVolume();
	}
	return 100;
}

void Settings::setVolume(string music, float volume_) {
	if (music == "ambient") {
		if (ambient.getVolume() + volume_ >= 0 && ambient.getVolume() + volume_ <= 100) {
			ambient.setVolume(ambient.getVolume() + volume_);
		}
	}
}

void Settings::switchMusic() {
	isAmbientPlay = !isAmbientPlay;
	if (isAmbientPlay)
		ambient.play();
}

void Settings::updateMusic() {
	if (isAmbientPlay) {
		if (ambient.getStatus() == Music::Playing);
		else {
			ambient.openFromFile(pathToMusic + allMusic[i]);
			ambient.play();
			i++;
			if (i >= allMusic.size()) {
				i = 0;
			}
		}
	}
	else {
		ambient.pause();
	}
}