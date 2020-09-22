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

		static void Unbind();
	};
}
