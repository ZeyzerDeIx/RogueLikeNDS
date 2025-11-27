#pragma once

#include "pch.h"

class AssetManager
{
public:
	static bool loadBin(std::string const& assetPath, void* dest, std::size_t size);
	
};