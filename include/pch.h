#pragma once

static_assert(__cplusplus >= 202400L, "This project requires C++26 or higher support!");

using uint = unsigned int;
using ushort = unsigned short;

#include <nds.h>
#include <vector>
#include <list>
#include <span>
#include <cstdlib>
#include <ctime>
#include <maxmod9.h>
#include <filesystem.h>
#include <queue>
#include "Constants.h"
#include "NDSMath.h"
#include "Debug.h"
#include "soundbank.h"