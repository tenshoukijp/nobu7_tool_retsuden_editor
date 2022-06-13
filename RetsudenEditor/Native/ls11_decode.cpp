
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <vector>

#include "ls11_lib.h"

using namespace std;


vector<LS11LOCATIONDATA> ls11_location_data_list;

LS11ONEDATA ls11_onedata;
 



// ビッグエンディアン⇔リトルエンディアン
int ls11_ChangeEndian(int iCng) {
	iCng = iCng<<24 | iCng<<8 & 0x00ff0000 | iCng>>8 & 0x0000ff00 | iCng>>24 & 0x000000ff;
	return iCng;
}


int ls11_memLocationDataArray(const char *inData, int inlen) {
	if(strncmp(inData,"LS11",4)){	/* ヘッダ調査 */
		printf("ファイルヘッダーが違います。\n");
		return -1;
	};

	ls11_location_data_list.clear(); // すべてクリア

	LS11LOCATIONDATA *pLS11SizeData;

	LS11LOCATIONDATA curLS11SizeData;

	// まぁ3000個以上分割されまい的な感じで。
	for (pLS11SizeData = (LS11LOCATIONDATA *)(char *)(inData+0x110); // 0x110のスタート位置から、
		 pLS11SizeData->iDataLenSize;								 // データ長が０でないと指定されている。
		 pLS11SizeData++) {

		// それぞれをビッグエンディアン⇒リトルエンディアンにして格納
		curLS11SizeData.iDataLenSize  = ls11_ChangeEndian( pLS11SizeData->iDataLenSize );
		curLS11SizeData.iExpandedSize = ls11_ChangeEndian( pLS11SizeData->iExpandedSize );
		curLS11SizeData.iStartAddress = ls11_ChangeEndian( pLS11SizeData->iStartAddress );

		// リストに追加
		ls11_location_data_list.push_back( curLS11SizeData );

	}

	return 0;
}

extern int ls11_bit;
extern int ls11_byte;

//LS11伸張
int		ls11_Decode(const char *inData, int inlen, char *outData, int outlen){
	
	ls11_bit = 0;
	ls11_byte = 0;

	char	dict[256]  = "";		/* 辞書 */
	int		dataSize   = 0;			/* データ部サイズ */
	int		writePoint = 0;			/* 書き込み位置 */
	int		serchPoint = 0;			/* コピー元先頭位置 */
	int		i,j,k;					/* 作業用 */
	int		bitcount = 0;			/* 読み取りビット数 */
	int		tempNum = 0;			/* 数値読み取り先 */
	int		checkNum = 0;			/* 辞書orコピーの判定用 */
	int 	isGet1 = TRUE;			/* 読み取り処理判別 */
	int		isLz   = FALSE;			/* コピー処理中か */


	if(strncmp(inData,"LS11",4)){	/* ヘッダ調査 */
		//printf("ファイルヘッダーが違います。");
		return -1;
	}

	for(i=0;i<256;i++){
		dict[i] = inData[0x10+i];
//		printf("%02X ",(unsigned char)dict[i]);
//		if(i%16 == 15){printf("\n");}
	}	/* 辞書登録 */


	/* バイト出力位置設定(0x120~) */
	ls11_getBit((const char*)0,1);
	ls11_getBit((const char*)0,2);
	ls11_getBit((const char*)255,2);
	ls11_getBit((const char*)33,2);
//	printf("Bit :%x\n",ls11_getBit(dict,3));
//	printf("Byte:%x\n",ls11_getBit(dict,4));
//	printf("data:%x\n",(unsigned char)inData[ls11_getBit(dict,4)]);

	for(i=0x120;i<inlen;i++){
		for(j=0;j<8;j++){
			tempNum = tempNum << 1;
			if(isGet1){	/* 前のビットを取得 */
				bitcount++;
				if(	ls11_getBit(inData,0) ){
					tempNum++;
				}else{
					isGet1 = FALSE;
					checkNum = tempNum;
					tempNum = 0;
				}
				continue;
			}else{	/* 後ろのビットを取得 */
				bitcount--;
				if( ls11_getBit(inData,0) ){ tempNum++;}
				if(bitcount>0){	continue;}
			}
			checkNum += tempNum;
			tempNum   = 0;
			isGet1 = TRUE;

			if(isLz){	/* LZ辞書処理中 */
				checkNum += 3;
				for(k=0;k<checkNum;k++){
					outData[writePoint] = (unsigned char)outData[serchPoint];
					writePoint++;
					serchPoint++;
					if(writePoint>outlen+50000){ // ★ここははみ出すことがあるので、パッチ
						return -2;
					}
				}
				isLz = FALSE;
			}else{		/* 辞書処理判別 */
				if(checkNum == 256){
					return -3;	/* 参照位置エラー */
				}
				if(checkNum < 256){	/* 本辞書 */
					outData[writePoint] = dict[checkNum];
					writePoint++;
					if(writePoint>outlen){
						return outlen; 
					}
					
				}else{	/* LZ辞書処理 */
					serchPoint = writePoint - (checkNum - 256);
					if(inData-serchPoint<0){
						printf("err");return -5;
					}
					isLz = TRUE;
				}
			}
		}
	}
	return writePoint;
}

int ls11_bit  = 0;
int ls11_byte = 0;

int	ls11_getBit(const char *inData, int type){

	/*	type0 次のビットを出力
		type1 出力ビットを設定(0:リセット) inDaga[1~7]:加算 [8~255]:リセット
		type2 出力バイトを設定(0:リセット) inData[1~255]:加算
		type3 何ビット目かを出力
		tyoe4 何バイト目かを出力
	*/
	int pick = 0;

	if(type){
		if(type & 1){
			if(type == 1){
				if(inData == 0){
					ls11_bit = 0;
				}else{
					ls11_bit += (int)(unsigned char)inData;
				}
			}
			return ls11_bit;
		}else{
			if(type == 2){
				if(inData == 0){
					ls11_byte = 0;
				}else{
					ls11_byte += (int)(unsigned char)inData;
				}
			}
			return ls11_byte;
		}
	}
	/* ビットの出力 */
	pick = 0x80 >> ls11_bit;
	pick = inData[ls11_byte] & pick;
	/* 後処理 */
	ls11_bit++;
	if(ls11_bit > 7){
		ls11_bit = 0;
		ls11_byte++;
	}
	if(pick){
		return 1;
	}else{
		return 0;
	}
	return -1;
}
