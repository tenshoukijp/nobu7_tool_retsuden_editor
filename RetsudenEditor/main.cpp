// RetsudenEditor.cpp : ���C�� �v���W�F�N�g �t�@�C���ł��B
#include <windows.h>
#include <string>
#include <locale> 

#include "RetsudenEditorForm.h"
#include "OnigRegex.h"

using namespace System;
using namespace System::Windows::Forms;


[STAThreadAttribute]
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {

	// �S�Ԃ̏�����
	onig_init();

	// �R���g���[�����쐬�����O�ɁAWindows XP �r�W���A�����ʂ�L���ɂ��܂�
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	RetsudenEditorForm^ rf = gcnew RetsudenEditorForm();

	// ���C�� �E�B���h�E���쐬���āA���s���܂�
	Application::Run(rf);

	// �S�Ԃ̏I��
	onig_end();

	return 0;
}
