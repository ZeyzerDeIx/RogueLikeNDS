#include "AssetManager.h"
#include <filesystem.h>

using namespace std;

bool AssetManager::loadBin(string const& assetPath, void* dest, size_t size)
{
	FILE* file = fopen(assetPath.c_str(), "rb");
	if(!file)
	{
		cout << "File load failed: " << assetPath << endl;
		return false;
	}

	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	rewind(file);

	if(fileSize < static_cast<long>(size))
		cout << "Warning: File is smaller than buffer size!" << endl;

	fread(dest, 1, size, file);
	fclose(file);

	return true;
}