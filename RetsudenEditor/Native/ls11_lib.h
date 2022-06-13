#pragma once

#include <windows.h>
#include <vector>


using namespace std;




// ビッグエンディアン⇔リトルエンディアン 相互変換
int ls11_ChangeEndian(int iCng);

// *inData = インプットのストリームアドレス, inlen = 該当のストリームの長さ
int ls11_memLocationDataArray(const char *inData, int inlen);

// *inData = インプットのストリームアドレス, inlen = 該当のストリームの長さ
// char* outData outData用のバッファ, outData用の長さ
int	ls11_Decode(const char *inData, int inlen, char *outData, int outlen);
int	ls11_Encode(const char *inData, int inlen, char *outData, int outlen);

int	ls11_getBit(const char *inData, int type);

int ls11_getFileData(const char *filename, char *out, int outlen);
int ls11_getFileSize(const char *filename);
int ls11_outFileData(const char *filename, char *out, int outlen);




// ロケーションデータ１つ分
struct LS11LOCATIONDATA {
	int iDataLenSize;  // 該当のロケーション(iStartAddress)から始まるデータのバイトサイズ
	int iExpandedSize; // 該当のロケーションデータを展開した場合に必要となるバイトサイズ
	int iStartAddress; // 該当のロケーションの開始アドレス(開始位置)
};

// *.nb6は元来複数のファイルであるが、計算がめんどくさいので、１つの辞書＋ロケーションデータ＋０＋データ という形で入っているとみなす。
struct LS11ONEDATA {
	byte dictionary[0x110];
	LS11LOCATIONDATA locationdata;
	int zero;
	byte data[500000];
};

// １つのLS11ファイルに、複数のロケーションデータが入っている。これのリストを覚えておく。１つのパック⇒バラバラのファイル用
extern vector<LS11LOCATIONDATA> ls11_location_data_list;

extern LS11ONEDATA ls11_onedata;



/*
* ls11_out_pack_header
*         +
* ls11_out_loc_data_list
*         +
*      (int)0
*         +
* vPackOutDataArray
*
* で１つのLS11パックファイルとなる。
*/
struct LS11OUTPACKHEADER {
	byte header[0x110];
};

extern LS11OUTPACKHEADER ls11_out_pack_header;

// １つのLS11ファイルに、複数のロケーションデータが入っている。バラバラのファイル⇒出力用
extern vector<LS11LOCATIONDATA> ls11_out_loc_data_list;

// バラバラのファイルをそれぞれ符号化したデータ部分の配列。
extern vector<vector<byte>> vPackOutDataArray;

