// RetsudenEditor.cpp : メイン プロジェクト ファイルです。
#include <windows.h>
#include <string>
#include <locale> 

#include "RetsudenEditorForm.h"
#include "OnigRegex.h"

using namespace System;
using namespace System::Windows::Forms;


[STAThreadAttribute]
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {

	// 鬼車の初期化
	onig_init();

	// コントロールが作成される前に、Windows XP ビジュアル効果を有効にします
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	RetsudenEditorForm^ rf = gcnew RetsudenEditorForm();

	// メイン ウィンドウを作成して、実行します
	Application::Run(rf);

	// 鬼車の終了
	onig_end();

	return 0;
}
