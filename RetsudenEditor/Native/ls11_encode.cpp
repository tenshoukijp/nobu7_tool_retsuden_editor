
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <vector>
#include "ls11_lib.h"

using namespace std;



int		ls11_Encode(const char *inData, int inlen, char *outData, int outlen);
int 	ls11_bitset(char *out,int data,int type);
int		ls11_bitout(char *out,int bit);

typedef struct {	//LZ77処理用(1レコード)
	int point;
	int count;
}	LZ;

typedef struct {	//ハフマン辞書(1レコード)
	int no;
	int count;
}	DICTIONARY;

typedef union {		//INT変換ボックス
	int i;
	char c[4];
	unsigned char uc[4];
}	INTBOX;


LS11OUTPACKHEADER ls11_out_pack_header;

// １つのLS11ファイルに、複数のロケーションデータが入っている。バラバラのファイル⇒出力用
vector<LS11LOCATIONDATA> ls11_out_loc_data_list;

// バラバラのファイルをそれぞれ符号化したデータ部分の配列。
vector<vector<byte>> vPackOutDataArray;

extern int ls11_setPoint;
extern int ls11_bitPoint;


/*///////////////////////////////////////////////////////////////////////////*/
//LS11圧縮
int ls11_Encode(const char *inData, int inlen, char *outData, int outlen){

	ls11_setPoint=0;
	ls11_bitPoint=0;

	LZ	*lzRecord;
	DICTIONARY dict[256];
	INTBOX len;
	int	lzLen;
	int i;

	/* LZ77処理 */
	lzLen = inlen;
	lzRecord = (LZ *)calloc(lzLen , sizeof(LZ));
	if(lzRecord == NULL ){
		printf("メモリ領域取得エラー");
		fflush(stdin); 
		getchar(); 
		return -1;
	}
	for(i=0;i<lzLen;i++){
		lzRecord[i].point = 0;
		lzRecord[i].count = (int)(unsigned char)inData[i];
	}

	/* ハフマン辞書作成 */
	for(i=0;i<256;i++){	//初期化
		dict[i].no    = i;
		dict[i].count = 0;
	}

	
	/* ファイルヘッダ書き込み */
	outData[0] = 'L';
	outData[1] = 'S';
	outData[2] = '1';
	outData[3] = '1';
	for(i=0;i<12;i++){	outData[i+4] = 0x00; }				//Padding
	for(i=0;i<4;i++){	outData[i+0x110] = 0x00; }			//圧縮後データ部サイズ(後で入れる)
	len.i = inlen;
	for(i=0;i<4;i++){	outData[i+0x114] = len.uc[3-i]; }	//圧縮前データ部サイズ
	len.i = 0x120;
	for(i=0;i<4;i++){	outData[i+0x118] = len.uc[3-i]; }	//データ開始位置
	for(i=0;i<4;i++){	outData[i+0x11C] = 0x00; }			//Padding
	ls11_bitset(" ",0x120,1);	//出力先頭位置変更(0x120)


	/* ハフマン出力処理 */
	for(i=0;i<lzLen;i++){
		if(lzRecord[i].point){
			ls11_bitset(outData,(lzRecord[i].point +256),0);
		}
		ls11_bitset(outData,dict[lzRecord[i].count].no,0);
	}
	free(lzRecord);
	outlen = ls11_bitset(" ",0,2);
	if(ls11_bitset(" ",0,4)){ outlen++; }

	/* 展開用ハフマン辞書作成 */
	for(i=0;i<256;i++){	
		dict[i].count = dict[i].no;
		dict[i].no    = i;
	}

	//	printf("辞書テーブル\n");
//	for(i=0;i<256;i++){printf("%02X ",dict[i].no);if(i%16==15)printf("\n");}

	/* ファイルヘッダ書き込み */
	for(i=0;i<256;i++){ outData[i+0x10] = dict[i].no; }			//辞書
	len.i = outlen - 0x120;
	for(i=0;i<4;i++){	outData[i+0x110] = len.uc[3-i];}		//圧縮後データ部サイズ

	return outlen;
}

int ls11_setPoint = 0;
int ls11_bitPoint = 0;

/* ハフマン出力処理 */
int ls11_bitset(char *out,int data,int type){

	unsigned int MASK = 0x02;	//マスク
	unsigned int m_count = 0;	//前bit長さ
	unsigned int num;
	int i;

	/* 情報設定 */
	if(type){
		switch(type){

			case 1:	ls11_setPoint = data;	//出力文字位置設定
			case 2:	return ls11_setPoint;
					break;

			case 3:	if(data > 8){data = 0;}	//出力ビット位置設定
					ls11_bitPoint = data;
			case 4:	return ls11_bitPoint;
					break;

			default:break;
		}
	}

	/* ビット出力処理 */
	num = (unsigned int)data;
	while(num >= ((MASK << (m_count +1)) -2) ){	//ビット長算出
		m_count++;
	}
//	printf("m_count:%d |num:%d ",m_count,num);
	num = num - ((MASK << m_count) -2);
//	printf("- %d = %d\n",(MASK << m_count) -2,num);

	for(i=m_count;i>=0;i--){	//上部出力
//		printf("%d|%03d|",ls11_setPoint,(0x80 >> ls11_bitPoint));
		if(i){	ls11_bitout(&out[ls11_setPoint],ls11_bitPoint);}
//		else{printf("\n");}
		ls11_bitPoint++;
		if(ls11_bitPoint>7){
			ls11_setPoint++;
			ls11_bitPoint = 0;
		}
	}
	for(i=m_count;i>=0;i--){ //下部出力
//		printf("%d|%03d|",ls11_setPoint,(0x80 >> ls11_bitPoint));
		if( num & (0x1 << i)){	ls11_bitout(&out[ls11_setPoint],ls11_bitPoint);}
//		else{printf("\n");}
		ls11_bitPoint++;
		if(ls11_bitPoint>7){
			ls11_setPoint++;
			ls11_bitPoint = 0;
		}
	}
	return 0;
}

/* ビット出力操作(渡されたcharの指定ビットを立てる) */
int ls11_bitout(char *out,int bit){

	unsigned char ch=0x80;
//	printf("%02x:%d:",(unsigned char)*out,bit);
	/* |0|1|2|3|4|5|6|7| */
	if(bit >= 0 || bit < 8){
		*out = *out | (ch >> bit);
//		printf("[%02x]%02x\n",(ch>>bit),(unsigned char)*out);
	}else{
		return -1;
	}
	return 0;
}
