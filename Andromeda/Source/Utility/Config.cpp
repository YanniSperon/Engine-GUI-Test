#include "Config.h"
#include "Console.h"

#include <string>
#include <iostream>
#include <fstream>

//--------------------------------------------
//                 Quality
//--------------------------------------------
//  Value  |    Name    |    Shadow Map Size
//--------------------------------------------
//    0    =  very low  =       256x256
//    1    =    low     =       512x512
//    2    =   medium   =      1024x1024
//    3    =    high    =      2048x2048
//    4    =  very high =      4096x4096

namespace AD {

	static Config* s_Config;

	Config::Config()
		: m_StartingWidth(1280), m_StartingHeight(720), m_HasVSync(0), m_HasRawMouseInput(0), m_FOV(70.0f), m_MouseSensitivity(0.2f), m_RenderDistance(16), m_ParticleQuality(2), m_ShadowQuality(2)
	{
		Load();
	}

	void Config::Save()
	{
		std::ofstream configFile("Resources/Config.cfg");

		if (configFile.is_open()) {
			configFile << "StartingWidth=" << m_StartingWidth << "\n";
			configFile << "StartingHeight=" << m_StartingHeight << "\n";
			configFile << "VSync=" << m_HasVSync << "\n";
			configFile << "HasRawMouseInput=" << m_HasRawMouseInput << "\n";
			configFile << "FOV=" << m_FOV << "\n";
			configFile << "MouseSensitivity=" << m_MouseSensitivity << "\n";
			configFile << "RenderDistance=" << m_RenderDistance << "\n";
			configFile << "ParticleQuality=" << m_ParticleQuality << "\n";
			configFile << "ShadowQuality=" << m_ShadowQuality << "\n";
		}
		else {
			Console::Error("Failed to save config");
		}

		configFile.close();
	}

	void Config::Load()
	{
		std::ifstream configFile("Resources/Config.cfg");

		if (configFile.is_open()) {
			std::string line;
			while (std::getline(configFile, line))
			{
				if (line.find("StartingWidth=") != std::string::npos && line.length() > 14) {
					std::string value = line.substr(14);
					m_StartingWidth = std::stoi(value);
				}
				else if (line.find("StartingHeight=") != std::string::npos && line.length() > 15) {
					std::string value = line.substr(15);
					m_StartingHeight = std::stoi(value);
				}
				else if (line.find("VSync=") != std::string::npos && line.length() > 6) {
					std::string value = line.substr(6);
					m_HasVSync = std::stoi(value);
				}
				else if (line.find("HasRawMouseInput=") != std::string::npos && line.length() > 17) {
					std::string value = line.substr(17);
					m_HasRawMouseInput = std::stoi(value);
				}
				else if (line.find("FOV=") != std::string::npos && line.length() > 4) {
					std::string value = line.substr(4);
					m_FOV = std::stof(value);
				}
				else if (line.find("MouseSensitivity=") != std::string::npos && line.length() > 17) {
					std::string value = line.substr(17);
					m_MouseSensitivity = std::stof(value);
				}
				else if (line.find("RenderDistance=") != std::string::npos && line.length() > 15) {
					std::string value = line.substr(15);
					m_RenderDistance = std::stoi(value);
				}
				else if (line.find("ParticleQuality=") != std::string::npos && line.length() > 16) {
					std::string value = line.substr(16);
					m_RenderDistance = std::stoi(value);
				}
				else if (line.find("ShadowQuality=") != std::string::npos && line.length() > 14) {
					std::string value = line.substr(14);
					m_RenderDistance = std::stoi(value);
				}
			}
		}
		else {
			Console::Error("Failed to load config");
		}

		configFile.close();
	}

	int Config::GetStartingWidth()
	{
		return m_StartingWidth;
	}

	int Config::GetStartingHeight()
	{
		return m_StartingHeight;
	}

	bool Config::GetHasVSync()
	{
		return m_HasVSync;
	}

	float Config::GetFOV()
	{
		return m_FOV;
	}

	float Config::GetMouseSensitivity()
	{
		return m_MouseSensitivity;
	}

	int Config::GetRenderDistance()
	{
		return m_RenderDistance;
	}

	int Config::GetParticleQuality()
	{
		return m_ParticleQuality;
	}

	int Config::GetShadowQuality()
	{
		return m_ShadowQuality;
	}

	bool Config::GetHasRawMouseInput()
	{
		return m_HasRawMouseInput;
	}

	void Config::SetStartingWidth(int width)
	{
		m_StartingWidth = width;
		Save();
	}

	void Config::SetStartingHeight(int height)
	{
		m_StartingHeight = height;
		Save();
	}

	void Config::SetHasVSync(bool hasVSync)
	{
		m_HasVSync = hasVSync;
		Save();
	}

	void Config::SetHasRawMouseInput(bool rawMouseInput)
	{
		m_HasRawMouseInput = rawMouseInput;
		Save();
	}

	void Config::SetFOV(float FOV)
	{
		m_FOV = FOV;
		Save();
	}

	void Config::SetMouseSensitivity(float mouseSensitivity)
	{
		m_MouseSensitivity = mouseSensitivity;
		Save();
	}

	void Config::SetRenderDistance(int renderDistance)
	{
		m_RenderDistance = renderDistance;
		Save();
	}

	void Config::SetParticleQuality(int particleQuality)
	{
		m_ParticleQuality = particleQuality;
		Save();
	}

	void Config::SetShadowQuality(int shadowQuality)
	{
		m_ShadowQuality = shadowQuality;
		Save();
	}

	void Config::InitializeSingleton()
	{
		s_Config = new Config();
	}

	Config* Config::GetInstance()
	{
		return s_Config;
	}

	void Config::CleanupSingleton()
	{
		delete s_Config;
	}
}
