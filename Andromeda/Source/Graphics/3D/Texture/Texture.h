#pragma once

#include <glew.h>
#include <string>

namespace AD {
	class Texture {
	private:
		GLuint m_ID;

		bool m_FakeUser;

		std::string m_Path;

	public:
		Texture(const std::string& name);
		~Texture();

		void LoadTexture(const std::string& name);

		void Bind();

		GLuint GetID();

		void SetHasFakeUser(bool fakeUser);
		bool GetHasFakeUser();

		std::string GetPath();

		// Data must be uninitialized
		// The function will populate the data, width, height, and numberOfChannels variables
		// Expected channels should be 0 if you are unsure
		static void LoadRawData(const std::string& path, unsigned char** data, int* width, int* height, int* numberOfChannels, int expectedChannels);
		static void UnloadRawData(unsigned char* data);

		static void Unbind();
	};
}
