// これは メイン DLL ファイルです。

#include "ls11_mod.h"
#include "LS11DotNet.h"

#include <windows.h>
#include <vector>

using namespace std;
using namespace LS11DotNet;

int Ls11::DecodePack(String^ csFileName, ArrayList^ csDstArrayList) {

	//--------------------String^ → char* へ
	// .netのUnicodeのfilenameをマルチバイトのchar *とする。
	std::string bufFileName;  // 文字列を受け止めるバッファを兼ねる。

	// 以下「String^」型をstringにするためのおまじないに近い。
	{
		IntPtr mptr = Marshal::StringToHGlobalAnsi(csFileName); // UnicodeをマルチバイトのAnsiへ
		bufFileName = static_cast<const char*>(mptr.ToPointer()); // IntPtrをchar *へキャスト
		Marshal::FreeHGlobal(mptr); // mptrは動的確保だったので解放
	}
	char *szFileName = (char *)bufFileName.c_str();
	vector<vector<byte>> pvecDstDataArray;

	// C++ Nativeでデコード
	int result = ls11_DecodePack(szFileName, NULL, &pvecDstDataArray);

	//--------------------vector<vector<byte>> → ArrayList^<ArrayList^<Byte>>へ
	// 順繰りで入れる。遅いかな～ まぁ.netのこれはエディタ作成用とか用なんでｗ
	for ( int iPackedIX=0; iPackedIX < (int)pvecDstDataArray.size(); iPackedIX++ ) {
		// 順に入れる
		ArrayList^ OneData = gcnew ArrayList();
		for ( int i=0; i<(int)pvecDstDataArray[iPackedIX].size(); i++ ) {
			OneData->Add(System::Byte(pvecDstDataArray[iPackedIX][i]));
		}
		csDstArrayList->Add(OneData);
	}

	return result;
}

int Ls11::EncodePack(String^ csFileName, ArrayList^ csSrcArrayList) {

	//--------------------String^ → char* へ
	// .netのUnicodeのfilenameをマルチバイトのchar *とする。
	std::string bufFileName;  // 文字列を受け止めるバッファを兼ねる。

	// 以下「String^」型をstringにするためのおまじないに近い。
	{
		IntPtr mptr = Marshal::StringToHGlobalAnsi(csFileName); // UnicodeをマルチバイトのAnsiへ
		bufFileName = static_cast<const char*>(mptr.ToPointer()); // IntPtrをchar *へキャスト
		Marshal::FreeHGlobal(mptr); // mptrは動的確保だったので解放
	}
	char *szFileName = (char *)bufFileName.c_str();

	//--------------------ArrayList^<ArrayList^<Byte>> → vector<vector<byte>> へ
	vector<vector<byte>> pvecDstDataArray;
    for (int iExpandedIX = 0; iExpandedIX < csSrcArrayList->Count; iExpandedIX++)
    {
		vector<byte> pvecOneData;
        ArrayList^ OneData = gcnew ArrayList();
        OneData = static_cast<ArrayList^>(csSrcArrayList[iExpandedIX]);
		for ( int i=0; i<(int)OneData->Count; i++ ) {
			byte b = static_cast<byte>(OneData->default[i]);
			pvecOneData.push_back(b);
		}
		pvecDstDataArray.push_back( pvecOneData );
	}

	// C++ Nativeでデコード
	int result = ls11_EncodePack(szFileName, NULL, &pvecDstDataArray);
	return result;
}

// １つの扱い難いバイナリデータを、意味単位で分解する
int Ls11::SplitData(ArrayList^ csSrcJoinedData, ArrayList^ csDstSplittedData) {

	// CSharpのArrayList→vector<byte>にコピー
	vector<byte> vSrcJoinedData;
	for ( int i=0; i<(int)csSrcJoinedData->Count; i++ ) {
		byte b = static_cast<byte>(csSrcJoinedData->default[i]);
		vSrcJoinedData.push_back(b);
	}

	// 1つのデータを意味単位に分割する
	vector<vector<byte>> vDstSplittedData;
	ls11_SplitData(vSrcJoinedData, &vDstSplittedData);

	//--------------------vector<vector<byte>> → ArrayList^<ArrayList^<Byte>>へ
	// 順繰りで入れる。遅いかな～ まぁ.netのこれはエディタ作成用とか用なんでｗ
	for ( int iSplittedIX=0; iSplittedIX < (int)vDstSplittedData.size(); iSplittedIX++ ) {
		// 順に入れる
		ArrayList^ OneData = gcnew ArrayList();
		for ( int i=0; i<(int)vDstSplittedData[iSplittedIX].size(); i++ ) {
			OneData->Add(System::Byte(vDstSplittedData[iSplittedIX][i]));
		}
		csDstSplittedData->Add(OneData);
	}

	int result = TRUE;
	return TRUE;
}

// 分解したものを元へと戻す。
int Ls11::JoinData(ArrayList^ csSrcSplittedData, ArrayList^ csDstJoinedData) {

	// CSharpのArrayList→vector<vector<byte>>にコピー
	vector<vector<byte>> vSrcSplittedData;
	// まずはコピーする前に数を合わせる
	for ( int iSplittedIX=0; iSplittedIX<(int)csSrcSplittedData->Count; iSplittedIX++ ) {
		vector<byte> srcOneData;
        ArrayList^ OneData = gcnew ArrayList();
        OneData = static_cast<ArrayList^>(csSrcSplittedData[iSplittedIX]);
		for ( int i=0; i<(int)OneData->Count; i++ ) {
			byte b = static_cast<byte>(OneData->default[i]);
			srcOneData.push_back(b);
		}
		vSrcSplittedData.push_back( srcOneData );
	}

	vector<byte> vDstJoinedData;

	// 分解したものを元へと戻す。
	ls11_JoinData(vSrcSplittedData, &vDstJoinedData);

	// vector<byte>→ArrayList<Byte>へ
    ArrayList^ OneData = gcnew ArrayList();
	for ( int i=0; i<(int)vDstJoinedData.size(); i++ ) {
		csDstJoinedData->Add(System::Byte(vDstJoinedData[i]));
	}

	int result = TRUE;
	return TRUE;
}
