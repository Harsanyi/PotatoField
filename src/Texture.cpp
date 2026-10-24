#include "Texture.h"
#include "SFML/Graphics.hpp"
#include "Log.h"
#include "Utils.h"

Texture::Texture(const std::string& fileName)
{
	if (!loadFromFile(fileName)) {
		Log::log(asString("Error, file not found:",fileName).c_str());
	}
	generateMipmap();
	setSmooth(true);
}

Texture::Texture() {
	generateMipmap();
	setSmooth(true);
}
