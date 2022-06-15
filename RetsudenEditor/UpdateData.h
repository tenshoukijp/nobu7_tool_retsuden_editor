#pragma once

#include <vector>
#include <string>

#include "CnvHankakuZenkaku.h"
#include "OnigRegex.h"

#include "ls11_mod.h"
#include "DotNetTestToMultiByte.h"

using namespace std;

extern vector<vector<byte>> vBufBushouDecodedData;
extern vector<vector<byte>> vBufHimeDecodedData;
extern vector<vector<byte>> vBufKahouDecodedData;
extern vector<vector<byte>> vBufCastleDecodedData;
extern vector<vector<byte>> vBufKanniDecodedData;

inline bool UpdateDecodedData(vector<byte>& data, int index) {
	vBufBushouDecodedData[index] = data;
	vBufHimeDecodedData[index] = data;
	vBufKahouDecodedData[index] = data;
	vBufCastleDecodedData[index] = data;
	vBufKanniDecodedData[index] = data;

	return true;
}