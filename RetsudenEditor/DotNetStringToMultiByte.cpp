#include <string>

using namespace System;
using namespace System::Text;


// .NET�^�C�v��String^ �� �}���`�o�C�g��C++�^std::string��
std::string DotNetStringToMultiByte(String^ dotnettext) {
	// �ȉ��ϊ��̃C�f�B�I��
	std::string text;  // ��������󂯎~�߂�o�b�t�@

	IntPtr mptr = Runtime::InteropServices::Marshal::StringToHGlobalAnsi(dotnettext);

	// char *�ɕϊ�
	text = (const char*)(mptr.ToPointer());

	// 
	Runtime::InteropServices::Marshal::FreeHGlobal(mptr);

	return text;
}
