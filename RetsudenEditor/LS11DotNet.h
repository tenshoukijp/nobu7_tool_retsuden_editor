// LS11DotNet.h

#pragma once

using namespace System;
using namespace System::Collections;
using namespace System::Runtime::InteropServices;

namespace LS11DotNet {

	public ref class Ls11 {
	public:
		//  LS11の***.n6p等を、バラバラに分解しデータを変数に格納する。LS11 Archieverを想像すれば良い。
		static int DecodePack(String^ csFilename, ArrayList^ csArrayList);

		// ファイル、あるいは、変数のデータを、１つのLS11ファイルにパック化する。
		static int EncodePack(String^ csFilename, ArrayList^ csArrayList);

		// １つの扱い難いバイナリデータを、意味単位で分解する
		static int SplitData(ArrayList^ csSrcJoinedData, ArrayList^ csDstSplittedData);

		// 分解したものを元へと戻す。
		static int JoinData(ArrayList^ csSrcSplittedData, ArrayList^ csvDstJoinedData);
	};
}
