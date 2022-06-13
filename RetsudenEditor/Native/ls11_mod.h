#pragma once

#include "ls11_lib.h"
#include <vector>


using namespace std;

// LS11の***.n6p等を、バラバラに分解する。LS11 Archieverを想像すれば良い。
// ファイルに分解するだけではなく、変数に格納することが出来る。
int ls11_DecodePack(char *szInputFileName, char *szOutputPattern, vector<vector<byte>> *pvecDstDataArray=NULL);

// ファイル、あるいは、変数のデータを、１つのLS11ファイルにパック化する。
int ls11_EncodePack(char *szOutputFileName, char *szInputPattern, vector<vector<byte>> *pvecSrcDataArray=NULL, vector<byte> *pDstLS11PackedData=NULL);


// １つの扱い難いバイナリデータを、意味単位で分解する
void ls11_SplitData(vector<byte> &vSrcJoinedData, vector<vector<byte>> *vDstSplittedData);

// 分解したものを元へと戻す。
void ls11_JoinData(vector<vector<byte>> &vSrcSplittedData, vector<byte> *vDstJoinedData);
