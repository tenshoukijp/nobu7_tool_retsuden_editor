#include <string>

using namespace System;
using namespace System::Text;


// .NETタイプのString^ → マルチバイトのC++型std::stringへ
std::string DotNetStringToMultiByte(String^ dotnettext) {
	// 以下変換のイディオム
	std::string text;  // 文字列を受け止めるバッファ

	IntPtr mptr = Runtime::InteropServices::Marshal::StringToHGlobalAnsi(dotnettext);

	// char *に変換
	text = (const char*)(mptr.ToPointer());

	// 
	Runtime::InteropServices::Marshal::FreeHGlobal(mptr);

	return text;
}
