#pragma once

#include "pch.h"

class AssetManager
{
public:
	static bool LoadBin(std::string const& assetPath, void* dest, std::size_t size);
	
};