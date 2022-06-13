#define _CRT_SECURE_NO_WARNINGS

#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <fstream>

#include "ls11_lib.h"

using namespace std;


int ls11_EncodePack(char *szOutputFileName, char *szInputPattern, vector<vector<byte>> *pvecSrcDataArray, vector<byte> *pDstLS11PackedData) {

	ls11_out_loc_data_list.clear();
	vPackOutDataArray.clear();

	// バラバラのそれぞれの名前
	char szCurFileName[256] = "";

	// ファイルを探す。
	for ( int iNo=0; iNo<3000; iNo++ ) {
		sprintf_s(szCurFileName, "%s/%s.%03d", szInputPattern, szInputPattern, iNo );

		int iTargetFileSize=0;
		int iKeepableMemSize=0;
		int iInputFileSize=0;
		int iOutputFileSize=0;
		char *inData=NULL;
		char *outData=NULL;
		/*
		printf("入力ファイル名:");
		scanf("%s",input);
		printf("出力ファイル名:");
		scanf("%s",output);
		*/
		// ファイルの場合
		if ( !pvecSrcDataArray && szInputPattern )  {
			/* ファイルサイズ取得 */
			iInputFileSize = ls11_getFileSize(szCurFileName);
			if(iInputFileSize < 0){
				//fflush(stdin);
				// ファイルが終了した。
				break;
			}
			inData = (char *)calloc(iInputFileSize , sizeof(char));
			if(inData == NULL ){
				//printf("メモリ領域取得エラー\n");
				//fflush(stdin); 
				return -1;
			}
			iInputFileSize = ls11_getFileData(szCurFileName, inData, iInputFileSize);
			if(iInputFileSize < 0){
				free(inData);
				//fflush(stdin);
				return -1;
			}
			// Vectorの場合
		} else if (pvecSrcDataArray) {
			if ( iNo >= (int)(*pvecSrcDataArray).size() ) { break; }
			iInputFileSize = (*pvecSrcDataArray)[iNo].size();
			inData = (char *)calloc(iInputFileSize , sizeof(char));
			memcpy(inData, (char *)&(*pvecSrcDataArray)[iNo][0], iInputFileSize);
		} else {
			return -1;
		}

		iKeepableMemSize = iInputFileSize * 5;
		outData = (char *)calloc(iKeepableMemSize, sizeof(char));
		if(outData == NULL ){
			free(inData);
			//printf("メモリ領域取得エラー\n");
			//fflush(stdin); 
			return -1;
		}

		iOutputFileSize = ls11_Encode(inData, iInputFileSize, outData, iKeepableMemSize);
		free(inData); // malloc

		if(iOutputFileSize < 0){
			// printf("エンコード失敗:%d\n",fsize3);
			free(outData);
			//fflush(stdin); 
			return -1;
		}
		// ヘッダー部分をコピー(これはじつは、ファイルが変わっても固定的)
		memcpy( &ls11_out_pack_header.header, outData, sizeof(ls11_out_pack_header.header));

		// ロケーションの部分をコピー(アディション)
		LS11LOCATIONDATA *p_cur_loc_data = (LS11LOCATIONDATA *)(char *)(outData+0x110);
		// はじめてならば、そのまま足すだけ。
		ls11_out_loc_data_list.push_back( *p_cur_loc_data );

		// データ部分を一気にコピー
		vector<byte> vCurLS11Buf(&outData[0x120], &outData[iOutputFileSize]);
		vPackOutDataArray.push_back(vCurLS11Buf);

		// 用が済んだので解放。次のパック分割ファイルへ
		free(outData);
	}

	// 最後に ls11_out_loc_data_list の一番最初のアドレスが間違っているので、最初のアドレスを修正し、それに基づいて全てのアドレスを修正する必要がある。
	for ( int loc=0; loc < (int)ls11_out_loc_data_list.size(); loc++ ) {
		if ( loc==0 ) {
			ls11_out_loc_data_list[0].iStartAddress = ls11_ChangeEndian(0x110 + 12*ls11_out_loc_data_list.size() + 4); // 複数のロケーションデータがパックされてるので、1つ目の位置も変化する。
		} else {
			// １つ前のアドレス＋１つ前の長さ
			// 元データがビッグエンディアンなので、リトルエンディアンにして、計算。
			int iStartAddress = ls11_ChangeEndian(ls11_out_loc_data_list[loc-1].iStartAddress) + ls11_ChangeEndian(ls11_out_loc_data_list[loc-1].iDataLenSize);
			// リトルエンディアンをビッグエンディアンに戻す
			iStartAddress = ls11_ChangeEndian( iStartAddress );
			ls11_out_loc_data_list[loc].iStartAddress = iStartAddress;
		}
	}

	if ( pDstLS11PackedData ) {
		// 一時バッファをどのぐらい確保するのか計算
		char *tmpBuf = NULL;
		int iCntBufSize = 0;
		for (int loc=0; loc<(int)ls11_out_loc_data_list.size(); loc++ ) {
			iCntBufSize += 12;
		}
		for (int buf=0; buf<(int)vPackOutDataArray.size(); buf++ ) {
			iCntBufSize += vPackOutDataArray[buf].size();
		}
		tmpBuf = (char *)calloc(iCntBufSize+1000, sizeof(char));

		char *pTmpBuf = tmpBuf;

		// ヘッダー＋辞書書き込み
		memcpy( pTmpBuf, (char *)ls11_out_pack_header.header, sizeof(ls11_out_pack_header.header) );
		pTmpBuf += sizeof(ls11_out_pack_header.header);

		// ロケーション関連書き込み
		for (int loc=0; loc<(int)ls11_out_loc_data_list.size(); loc++ ) {
			memcpy( pTmpBuf, (char *)(char *)&ls11_out_loc_data_list[loc].iDataLenSize, 12 );
			pTmpBuf += 12;
		}
		// ロケーション終了の０書き込み。４バイト(int １つ分)
		int iEnd = 0;
		memcpy( pTmpBuf, (char *)&iEnd, 4 );
		pTmpBuf += 4;

		// データ部分書き込み
		for (int buf=0; buf<(int)vPackOutDataArray.size(); buf++ ) {
			memcpy( pTmpBuf, (char *)&vPackOutDataArray[buf][0], vPackOutDataArray[buf].size() );
			pTmpBuf += vPackOutDataArray[buf].size();
		}

		// それを渡された引数のpDstLS11PackedDataに反映
		int cpysize =  (int)pTmpBuf-(int)tmpBuf;
		(*pDstLS11PackedData).resize( cpysize );
		memcpy( (char *)&(*pDstLS11PackedData)[0], tmpBuf, cpysize );

		// 一時バッファの解放
		free(tmpBuf);
	}

	if ( szOutputFileName ) {

		// バイナリでオープン
		FILE *fp = fopen(szOutputFileName, "wb");

		// ヘッダー＋辞書書き込み
		fwrite( (char *)ls11_out_pack_header.header, sizeof(ls11_out_pack_header.header), 1, fp);

		// ロケーション関連書き込み
		for (int loc=0; loc<(int)ls11_out_loc_data_list.size(); loc++ ) {
			fwrite( (char *)&ls11_out_loc_data_list[loc].iDataLenSize, 12, 1, fp);
		}
		// ロケーション終了の０書き込み。４バイト(int １つ分)
		int iEnd = 0;
		fwrite( (char *)&iEnd, 4, 1, fp );

		// データ部分書き込み
		for (int buf=0; buf<(int)vPackOutDataArray.size(); buf++ ) {
			fwrite( (char *)&vPackOutDataArray[buf][0], vPackOutDataArray[buf].size(), 1, fp );
		}
		fclose(fp);
	}

	return 0;
}


