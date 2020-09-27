#include "Texture.h"
#include "Console.h"
#include "TextureManager.h"

#include "stb_image.h"

namespace AD {
	Texture::Texture(const std::string& name)
		: m_ID(0), m_FakeUser(false), m_Path(name)
	{
		LoadTexture(name);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_ID);
		Console::Warning("Deleted texture");
	}

	void Texture::LoadTexture(const std::string& name)
	{
		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		stbi_set_flip_vertically_on_load(1);

		int width, height, nrChannels;
		unsigned char* data = stbi_load(name.c_str(), &width, &height, &nrChannels, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			Console::Error("Failed to load texture \"%s\"", name.c_str());
		}
	}

	void Texture::Bind()
	{
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	GLuint Texture::GetID()
	{
		return m_ID;
	}

	void Texture::SetHasFakeUser(bool fakeUser)
	{
		m_FakeUser = fakeUser;
		if (!fakeUser) {
			TextureManager::GetInstance()->MarkForCleanup();
		}
	}

	bool Texture::GetHasFakeUser()
	{
		return m_FakeUser;
	}

	std::string Texture::GetPath()
	{
		return m_Path;
	}

	void Texture::Unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
