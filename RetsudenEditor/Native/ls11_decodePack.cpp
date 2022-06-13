#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "ls11_lib.h"

using namespace std;



int ls11_DecodePack(char *szInputFileName, char *szOutputPattern, vector<vector<byte>> *pvecDstDataArray) {

	// 渡されたベクターは念のためクリアー
	if ( pvecDstDataArray ) {
		pvecDstDataArray->clear();
	}

	int iTtargetFileSize=0;
	int iKeepableMemSize=0;
	int iOutputFileSize=0;
	char *inData=NULL;
	char *outData=NULL;
	/*
	printf("入力ファイル名:");
	scanf("%s",input);
	printf("出力ファイル名:");
	scanf("%s",output);
	*/
	/* ファイルサイズ取得 */
	iTtargetFileSize = ls11_getFileSize(szInputFileName);
	if(iTtargetFileSize < 0){
		fflush(stdin);
		return -1;
	}
	inData = (char *)calloc(iTtargetFileSize , sizeof(char));
	if(inData == NULL ){
		//printf("メモリ領域取得エラー\n");
		fflush(stdin); 
		return -1;
	}
	iTtargetFileSize = ls11_getFileData(szInputFileName, inData, iTtargetFileSize);
	if(iTtargetFileSize < 0){
		free(inData);
		fflush(stdin);
		return -1;
	}
	//printf("元データ:%d\n",iTtargetFileSize);

	/* 展開処理 */
	int result = ls11_memLocationDataArray(inData, iTtargetFileSize);
	if ( result == -1 ) {
		free(inData);
		fflush(stdin);
		return -1;
	}

	// 辞書部分は共通なのでコピー
	memcpy( &ls11_onedata.dictionary, inData, sizeof(ls11_onedata.dictionary) );
	ls11_onedata.zero = 0;

	// 分割数分について
	for (int i=0; i<(int)ls11_location_data_list.size(); i++ ) {
		// サイズ関連部分をコピーする
		memcpy(&ls11_onedata.locationdata, &ls11_location_data_list[i], sizeof(ls11_location_data_list) );
		memcpy(&ls11_onedata.data, inData+ls11_location_data_list[i].iStartAddress, ls11_location_data_list[i].iDataLenSize);

		//printf("確保サイズ:%d\n",ls11_location_data_list[i].iExpandedSize);

		if(ls11_location_data_list[i].iExpandedSize <= 0){
			//printf("展開後サイズが取得できません\n");
			//fflush(stdin);
			return -1;
		}

		outData = (char *)calloc(ls11_location_data_list[i].iExpandedSize+50000 , sizeof(char)); // この50000て数値は別のところで参照している。

		if(outData == NULL ){
			free(inData);
			//printf("メモリ領域取得エラー\n");
			fflush(stdin); 
			return -1;
		}
		iKeepableMemSize = ls11_Decode((char *)&ls11_onedata, sizeof(ls11_onedata), outData, ls11_location_data_list[i].iExpandedSize );	
		//printf("出力サイズ:%d\n",iKeepableMemSize);
		// メモリ確保できない
		if(iKeepableMemSize < 0){
			//printf("エンコード失敗:%d\n",iKeepableMemSize);
			fflush(stdin); 
			free(outData);
			return -1;
		}

		// ファイル出力パターンがわたってきている場合は、ファイル出力
		if ( szOutputPattern ) {
			// ディレクトリ作成
			_mkdir(szOutputPattern);
			// ファイル名作成。*****.001　など
			char szTmpOutputPattern[256] = "";
			sprintf_s(szTmpOutputPattern, "%s/%s.%03d", szOutputPattern,szOutputPattern, i);
			printf(szTmpOutputPattern);

			/* 変換されたデータを出力 */
			iOutputFileSize = ls11_outFileData(szTmpOutputPattern, outData, ls11_location_data_list[i].iExpandedSize);
			// 出力に失敗した
			if ( iOutputFileSize == -1 ) {
				free(outData);// mallocに対応
				return -1;
			}
		}

		// ベクトルリストがわたってきてる場合は、ベクトルに出力
		if ( pvecDstDataArray ) {
			// vectorに一括コピペ
			vector<byte> buf(&outData[0], &outData[ls11_location_data_list[i].iExpandedSize]);
			// 渡されたベクトル配列にAppend
			pvecDstDataArray->push_back(buf);
		}
		free(outData);// mallocに対応
	}

	free(inData);

	//	}

	//printf("変換終了\n");
	//fflush(stdin);
	return 0;
}

/*
使い方

vector<vector<byte>> v; タイプを渡すことで、各々のvの中に、分割した各バイナリファイル(****.000, ****.001の中身)がそのまま入る

int main(int argc , char *argv[]){
	vector<vector<byte>> vDataArray;
	ls11_DecodePack("hexgrp.nb6", "hexgrp" , &vDataArray);

	return 0;
}
*/