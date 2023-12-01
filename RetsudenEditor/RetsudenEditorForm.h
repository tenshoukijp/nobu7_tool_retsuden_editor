#pragma once;

#include "BushouData.h"
#include "HimeData.h"
#include "KahouData.h"
#include "CastleData.h"
#include "KanniData.h"

// RetsudenEditor.cpp : ���C�� �v���W�F�N�g �t�@�C���ł��B
#include <windows.h>
#include <string>

#include "DotNetTestToMultiByte.h"

using namespace System;
using namespace Microsoft::Win32;
using namespace System::Drawing;
using namespace System::Windows::Forms;

ref class RetsudenEditorForm : public Form {

//-----------------------------------------------�t�H�[���E�B���h�E�n

	TabControl^ tcRE;	// �S�̂łP�ɂȂ��Ă�^�u�R���g���[���BTabPage�^��ǉ����Ă�

	static System::Drawing::Text::PrivateFontCollection^ pfc;
	static cli::array<String^>^ aryStrFontCandidate = {"���� ����", "�l�r ����", "�l�r �S�V�b�N" }; // �t�H���g���X�g

public:
	RetsudenEditorForm() {
		// �t�H�[���ɂ��������̂̏�������
		this->Text = "��`�G�f�B�^";
		this->Width = 530;
		this->Height = 360;

		LoadPrivateFont();

		SetFormIcon();

		SetTabControl();
	}

	void LoadPrivateFont() {
		// �V�X�e���ł͂Ȃ��A�v���C�x�[�g�t�H���g�Ƃ��āA���[�U�[��p�Ƃ��ăC���X�g�[�������t�H���g����������
		pfc = gcnew System::Drawing::Text::PrivateFontCollection();
		RegistryKey^ key = nullptr;

		// ��r�Ώۂ̃t�H���g�B
		try
		{
			key = Registry::CurrentUser->OpenSubKey(R"(Software\Microsoft\Windows NT\CurrentVersion\Fonts)");
			cli::array<String^>^ valueNames = key == nullptr ? nullptr : key->GetValueNames();
			if (valueNames != nullptr)
			{
				String^ targetFontName = "���� ����";
				for each (String ^ valueName in valueNames)
				{
					String^ path = key->GetValue(valueName)->ToString();
					if (valueName->Contains(targetFontName)) {
						// �t�H���g�����w��̃t�H���g�����܂�ł���ꍇ�́A�t�H���g��ǉ�����
						pfc->AddFontFile(path);
					}
				}
			}
		}
		catch (Exception^ /*ex1*/) {

		}
		finally
		{
			if (key != nullptr) {
				key->Close();
			}
		}

	}

	void SetFormIcon() {
		// ���̃v���W�F�N�g�̃A�Z���u���̃^�C�v���擾�B
		System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
		System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.RetsudenEditorRes", prj_assebmly->GetName()->Name), prj_assebmly);

		System::Drawing::Icon^ iconform = (System::Drawing::Icon^)(r->GetObject("icon"));
		this->Icon = iconform;
	}

	// �^�u�R���g���[���̏������B�ԂɁA�e�f�[�^�O���b�h�r���[�ւ̃f�[�^�������ݏ��������荞��
	void SetTabControl() {
		tcRE = gcnew TabControl();
		tcRE->Size = this->ClientSize;
		
		// ������`�̃^�u�y�[�W�ǉ�
		tpBushou_Init();
		Bushou_SetEventHander();

		// �P��`�̃^�u�y�[�W�ǉ�
		tpHime_Init();
		Hime_SetEventHander();

		// �ƕ��`�̃^�u�y�[�W�ǉ�
		tpKahou_Init();
		Kahou_SetEventHander();

		// ���`�̃^�u�y�[�W�ǉ�
		tpCastle_Init();
		Castle_SetEventHander();

		// ���ʗ�`�̃^�u�y�[�W�ǉ�
		tpKanni_Init();
		Kanni_SetEventHander();

		tcRE->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::tcRE_SelectedIndexChanged);
		this->Controls->Add(tcRE);
	}

	// �^�u��؂�ւ�����
	void tcRE_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
	}


	int GetStringMeasure(String^ str) {
		string sstr = DotNetStringToMultiByte(str);
		int iByteNum = strlen(sstr.data());
		return iByteNum * 8 + 8;
	}

//-----------------------------------------------������`�n
private:
	TabPage^ tpBushou;
	BushouData^ bdBushouData;

	TextBox^ tbBushouSearch; // ��������
	Button^ btnBushouSearch; // �����{�^��
	Button^ btnBushouImport; // �捞�{�^��
	OpenFileDialog^ ofdBushouImport;
	Button^ btnBushouExport; // �o�̓{�^��
	Button^ btnBushouAllSave; // �S�ĕۑ�

	ListBox^ lbBushouList; // �������X�g�̃R���{�{�b�N�X
	int iLastBushouListSelected;

	Panel^ plBushouRetsuden;	 // ��`���p�̃p�l��
	TextBox^ tbBushouFullName; // ���������̃e�L�X�g�{�b�N�X
	TextBox^ tbBushouRubyYomi; // �ӂ肪�Ȃ̂��
	TextBox^ tbBushouBornEtc;  // ���N�Ȃǂ̏��
	TextBox^ tbBushouRetsuden; // ��`���


	// �t�H���g
	System::Drawing::Font^ fontBushou;
	ComboBox^ cbFontBushou;	// �t�H���g���X�g

	Button^ btnBushouAdds; // 50�g�ǉ�
	Button^ btnBushouDels; // 50�g�ǉ�


private:

	void tpBushou_Init() {

		tpBushou = gcnew TabPage();

		tpBushou->Text = "������`";
		tpBushou->Size = tcRE->ClientSize;

		// ��������
		tbBushouSearch = gcnew TextBox();
		tbBushouSearch->Width = 140;
		tbBushouSearch->Height = 16;
		tbBushouSearch->Left = 10;
		tbBushouSearch->Top = 10;

		// �����{�^��
		btnBushouSearch = gcnew Button();
		btnBushouSearch->Text = "����";
		btnBushouSearch->Width = 60;
		btnBushouSearch->Height = tbBushouSearch->Height;
		btnBushouSearch->Left = tbBushouSearch->Right + 1;
		btnBushouSearch->Top = tbBushouSearch->Top;

		// �捞�{�^��
		btnBushouImport = gcnew Button();
		btnBushouImport->Text = "�P�Ǝ捞";
		btnBushouImport->Width = 70;
		btnBushouImport->Height = tbBushouSearch->Height;
		btnBushouImport->Left = btnBushouSearch->Right + 30;
		btnBushouImport->Top = tbBushouSearch->Top;

		// �o�̓{�^��
		btnBushouExport = gcnew Button();
		btnBushouExport->Text = "�P�Əo��";
		btnBushouExport->Width = 70;
		btnBushouExport->Height = tbBushouSearch->Height;
		btnBushouExport->Left = btnBushouImport->Right;
		btnBushouExport->Top = tbBushouSearch->Top;

		// �S�ĕۑ�
		btnBushouAllSave = gcnew Button();
		btnBushouAllSave->Text = "������`��ۑ�";
		btnBushouAllSave->Width = 120;
		btnBushouAllSave->Height = tbBushouSearch->Height;
		btnBushouAllSave->Left = tpBushou->Right - (btnBushouAllSave->Width+20);
		btnBushouAllSave->Top = tbBushouSearch->Top;

		// �����m���
		lbBushouList = gcnew ListBox();
		lbBushouList->Width = tbBushouSearch->Width;
		lbBushouList->Left = tbBushouSearch->Left;
		lbBushouList->Top = tbBushouSearch->Bottom + 10;
		lbBushouList->Height = 200;

		// ��`���p�̃p�l��
		plBushouRetsuden = gcnew Panel();
		plBushouRetsuden->Top = lbBushouList->Top + 20;
		Bushou_SetPanelBackImg();
		plBushouRetsuden->Left = tpBushou->Right - (plBushouRetsuden->Width+20);

		tbBushouFullName = gcnew TextBox();
		tbBushouFullName->Top = 10;
		tbBushouFullName->Left = 10;
		tbBushouFullName->Width = 16 * 6 + 6;
		tbBushouFullName->MaxLength = 37;
		tbBushouFullName->BackColor = Color::FromArgb(0xD0, 0xD0, 0xFA);

		tbBushouRubyYomi = gcnew TextBox();
		tbBushouRubyYomi->Top = tbBushouFullName->Top;
		tbBushouRubyYomi->Left = tbBushouFullName->Right;
		tbBushouRubyYomi->MaxLength = 37;
		tbBushouRubyYomi->Width = 16 * 5 + 6;
		tbBushouRubyYomi->BackColor = Color::FromArgb(0xD0, 0xD0, 0xFA);

		tbBushouBornEtc  = gcnew TextBox();
		tbBushouBornEtc->Top = tbBushouFullName->Top;
		tbBushouBornEtc->Left = tbBushouRubyYomi->Right;
		tbBushouBornEtc->Width = 16 * 5 + 8;
		tbBushouBornEtc->MaxLength = 37;
		tbBushouBornEtc->BackColor = Color::FromArgb(0xD0, 0xD0, 0xFA);

		tbBushouRetsuden = gcnew TextBox();
		tbBushouRetsuden->Top = tbBushouFullName->Bottom + 12;
		tbBushouRetsuden->Left = tbBushouFullName->Left;
		tbBushouRetsuden->Height = 90;
		tbBushouRetsuden->Width = 312;
		tbBushouRetsuden->MaxLength = 2 * 18 * 4;
		tbBushouRetsuden->Multiline = true;
		tbBushouRetsuden->WordWrap = false;
		tbBushouRetsuden->BackColor = Color::FromArgb(0xD0, 0xD0, 0xFA);
		

		plBushouRetsuden->Controls->Add(tbBushouFullName);
		plBushouRetsuden->Controls->Add(tbBushouRubyYomi);
		plBushouRetsuden->Controls->Add(tbBushouBornEtc);
		plBushouRetsuden->Controls->Add(tbBushouRetsuden);

		// �t�H���g���X�g
		cbFontBushou = gcnew ComboBox();
		cbFontBushou->Width = 100;
		cbFontBushou->Left = tpBushou->Right - (cbFontBushou->Width+20);
		cbFontBushou->Top = plBushouRetsuden->Bottom + 10;
		cbFontBushou->Height = tbBushouSearch->Height;
		Bushou_GetValidTenshoukiFontList();

		Bushou_cbFont_SelectedIndexChanged(nullptr, nullptr);

		/*
		// �����g50�l�ǉ�
		btnBushouAdds = gcnew Button();
		btnBushouAdds->Text = "50�g�ǉ�";
		btnBushouAdds->Width = 70;
		btnBushouAdds->Height = tbBushouSearch->Height;
		btnBushouAdds->Left = tbBushouSearch->Left;
		btnBushouAdds->Top = lbBushouList->Bottom + 10;

		// �����g50�l�폜
		btnBushouDels = gcnew Button();
		btnBushouDels->Text = "50�g�폜";
		btnBushouDels->Width = 70;
		btnBushouDels->Height = tbBushouSearch->Height;
		btnBushouDels->Left = btnBushouAdds->Right;
		btnBushouDels->Top = lbBushouList->Bottom + 10;
		*/

		// ������`�̃^�u�ɃR���|�[�l���g�ǉ�
		tpBushou->Controls->Add(tbBushouSearch);
		tpBushou->Controls->Add(btnBushouSearch);
		// tpBushou->Controls->Add(btnBushouImport);
		// tpBushou->Controls->Add(btnBushouExport);
		tpBushou->Controls->Add(btnBushouAllSave);
		tpBushou->Controls->Add(lbBushouList);
		tpBushou->Controls->Add(plBushouRetsuden);
		tpBushou->Controls->Add(cbFontBushou);
		// tpBushou->Controls->Add(btnBushouAdds);
		// tpBushou->Controls->Add(btnBushouDels);
		
		// �^�u���t�H�[���ɒǉ�
		tcRE->TabPages->Add(tpBushou);

		// �����f�[�^���쐬���āA���X�g�ɗ�������
		bdBushouData = gcnew BushouData();
		// ���X�g�{�b�N�X�ɑ�������
		for ( int i=0; i<bdBushouData->lstStrFullName->Count; i++ ) {
			String^ strLine = String::Format( "{0:0000} - {1}", i, bdBushouData->lstStrFullName[i]);
			lbBushouList->Items->Add( strLine );
		}

		// �ŏ��̂��̂�I�����Ă���
		lbBushouList->SelectedIndex = 0;
		iLastBushouListSelected = 0;

		// �����f�[�^���e�L�X�g�{�b�N�X
		Bushou_BDataToTextBox();
		Bushou_TextBoxWidthUpdate();
	}

	// �p�l���̔w�i�摜�ݒ�
	void Bushou_SetPanelBackImg() {
		// ���̃v���W�F�N�g�̃A�Z���u���̃^�C�v���擾�B
		System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
		System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.RetsudenEditorRes", prj_assebmly->GetName()->Name), prj_assebmly);

		// �p�l���̔w�i
		plBushouRetsuden->BackgroundImage = (System::Drawing::Image^)(r->GetObject("retsuden_back"));
		// �w�i�摜�̑傫���Ƀp�l���̑傫�������킹��
		plBushouRetsuden->Size = plBushouRetsuden->BackgroundImage->Size;

	}

	// �e��f�t�h�̃C�x���g�n���h���ݒ�
	void Bushou_SetEventHander() {
		btnBushouSearch->Click += gcnew EventHandler(this, &RetsudenEditorForm::Bushou_btnSearch_Click);
		btnBushouImport->Click += gcnew EventHandler(this, &RetsudenEditorForm::Bushou_btnImport_Click);
		btnBushouExport->Click += gcnew EventHandler(this, &RetsudenEditorForm::Bushou_btnExport_Click);
		btnBushouAllSave->Click += gcnew EventHandler(this, &RetsudenEditorForm::Bushou_btnAllSave_Click);
		tbBushouSearch->KeyDown += gcnew KeyEventHandler(this, &RetsudenEditorForm::Bushou_tbSearch_KeyDown);
		lbBushouList->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Bushou_lbBushouList_SelectedIndexChanged);
		tbBushouFullName->TextChanged += gcnew EventHandler( this, &RetsudenEditorForm::Bushou_tbFullName_TextChanged);
		tbBushouRubyYomi->TextChanged += gcnew EventHandler( this, &RetsudenEditorForm::Bushou_tbRubyYomi_TextChanged);
		tbBushouBornEtc->TextChanged += gcnew EventHandler( this, &RetsudenEditorForm::Bushou_tbBornEtc_TextChanged);
		tbBushouRetsuden->TextChanged += gcnew EventHandler( this, &RetsudenEditorForm::Bushou_tbRetsuden_TextChanged);
		tbBushouFullName->Leave += gcnew EventHandler( this, &RetsudenEditorForm::Bushou_textBox_Leave);
		tbBushouRubyYomi->Leave += gcnew EventHandler( this, &RetsudenEditorForm::Bushou_textBox_Leave);
		tbBushouBornEtc->Leave += gcnew EventHandler( this, &RetsudenEditorForm::Bushou_textBox_Leave);
		tbBushouRetsuden->Leave += gcnew EventHandler( this, &RetsudenEditorForm::Bushou_textBox_Leave);
		cbFontBushou->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Bushou_cbFont_SelectedIndexChanged);
		// btnBushouAdds->Click += gcnew EventHandler(this, &RetsudenEditorForm::Bushou_btnAddBushous_Click);
		// btnBushouDels->Click += gcnew EventHandler(this, &RetsudenEditorForm::Bushou_btnDelBushous_Click);
	}

	// ���������{�^������������
	void Bushou_btnSearch_Click(Object^ sender, EventArgs^ e) {
		if ( tbBushouSearch->Text == "" ) { return; } // �e�L�X�g�G���A������ۂȂ牽�����Ȃ��B

		int iStart = GetBushouCBSelectedIndex() + 1; // ���ݑI�����Ă�����̂̂��玟�̌��

		Regex^ regex = gcnew Regex(tbBushouSearch->Text);

		bool isExist = false;
		// iStart�ȍ~��S���T�[�`���āA
		for ( int i=iStart; i<lbBushouList->Items->Count; i++) {
			Match^ match = regex->Match( (String^)lbBushouList->Items[i] );

			if ( match->Success ) {
				lbBushouList->SelectedIndex = i;
				isExist = true;
				break;
			}
		}

		if ( !isExist ) {
			for ( int i=0; i<iStart; i++) {
				Match^ match = regex->Match( (String^)lbBushouList->Items[i] );

				if ( match->Success ) {
					lbBushouList->SelectedIndex = i;
					break;
				}
			}
		}
	}

	// ���������e�L�X�g�{�b�N�X�ŃL�[����������
	void Bushou_tbSearch_KeyDown(Object^ sender, KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Return ) {
			Bushou_btnSearch_Click(sender, e);
		}
		if (e->KeyCode == Keys::F3 ) {
			Bushou_btnSearch_Click(sender, e);
		}
	}

	// �����o�̓{�^������������
	void Bushou_btnExport_Click(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetBushouCBSelectedIndex();
		std::string unmanagedData = bdBushouData->GetOneBushouExportData(iSelectedIndex);

		std::string filepath = DotNetStringToMultiByte(bdBushouData->lstStrFullName[iSelectedIndex]);
		if ( filepath == "" ) { filepath = "�ȂȂ�"; }
		filepath += ".n6b";

		FILE *fp = fopen(filepath.c_str(), "wb");
		if ( fp ) {
			fwrite( unmanagedData.c_str(), unmanagedData.size(), 1, fp);
			fclose(fp);
			System::Windows::Forms::MessageBox::Show("�f�[�^���o�͂��܂����B", "����", ::MessageBoxButtons::OK, ::MessageBoxIcon::Information);
		} else {
			System::Windows::Forms::MessageBox::Show("�f�[�^�̏o�͂Ɏ��s���܂����B", "�G���[", MessageBoxButtons::OK, ::MessageBoxIcon::Error);

		}
	}


	// �������̓{�^������������
	void Bushou_btnImport_Click(Object^ sender, EventArgs^ e) {

		ofdBushouImport = gcnew OpenFileDialog();

		ofdBushouImport->InitialDirectory = "."; // �c�[���Ɠ����f�B���N�g�����f�t�H���g

		ofdBushouImport->Filter = "�����^ ���b�Z�[�W�t�@�C��(*.nb7)|*.nb7";

		ofdBushouImport->FilterIndex = 1;

		ofdBushouImport->Title = "�Ƃ��镐���̗�`�t�@�C����I�����Ă�������";

		//�_�C�A���O�{�b�N�X�����O�Ɍ��݂̃f�B���N�g���𕜌�����悤�ɂ���
		ofdBushouImport->RestoreDirectory = true;

		//�_�C�A���O��\������
		if (ofdBushouImport->ShowDialog() == ::DialogResult::OK) {
			//OK�{�^�����N���b�N���ꂽ�Ƃ�
			if ( ofdBushouImport->FileName ) {
				BOOL success = bdBushouData->GetOneBushouImportData(ofdBushouImport->FileName, GetBushouCBSelectedIndex());

				//�@���ۂɂ�����炸�X�V
				Bushou_BDataToTextBox();
			}
		}
	}

	// �S�ĕۑ��{�^������������
	void Bushou_btnAllSave_Click(Object^ sender, EventArgs^ e) {
		bdBushouData->AllSaveToBFileN6P();
	}

	// �t�H���g���X�g�̑I�����ڂ��ύX���ꂽ��
	void Bushou_cbFont_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
		// �t�H���g�̐ݒ�
		fontBushou = gcnew System::Drawing::Font((String^)cbFontBushou->Items[cbFontBushou->SelectedIndex], 12, FontStyle::Regular);
		tbBushouFullName->Font = fontBushou;
		tbBushouRubyYomi->Font = fontBushou;
		tbBushouBornEtc->Font  = fontBushou;
		tbBushouRetsuden->Font = fontBushou;
	}

	// �C���X�g�[������Ă���t�H���g�ɂ��������āA�t�H���g���X�g�{�b�N�X�ɒǉ�
	void Bushou_GetValidTenshoukiFontList() {

		//InstalledFontCollection�I�u�W�F�N�g�̎擾
		System::Drawing::Text::InstalledFontCollection^ ifc = gcnew System::Drawing::Text::InstalledFontCollection();

		//�C���X�g�[������Ă��邷�ׂẴt�H���g�t�@�~���A���擾
		cli::array<FontFamily^>^ ffs = ifc->Families;

		//�����ł͌��ƂȂ肦��t�H���g���̂�
		for ( int i=0; i<aryStrFontCandidate->Length; i++) {

			// �t�H���g���X�g
			for each (FontFamily^ ff in ffs) {

				// ���t�H���g���������B
				if (ff->Name == aryStrFontCandidate[i]) {
					// �R���{�{�b�N�X�ɒǉ�
					cbFontBushou->Items->Add( ff->Name );
				}
			}
		}

		// �P�ȏ�t�H���g����������A�ŏ��̂��̂�I�����Ă����B
		if ( cbFontBushou->Items->Count > 0 ) {
			cbFontBushou->SelectedIndex = 0;
		}
	}

	int GetBushouCBSelectedIndex() {
		int iSelectedIndex = lbBushouList->SelectedIndex;
		if ( 0 <= iSelectedIndex && iSelectedIndex < lbBushouList->Items->Count) {
			return iSelectedIndex;
		} else {
			return 0;
		}
	}
	void ModifyCBBushouSelectedIndex() {
		// �œ_���������Ă���悤�Ȃ�A�Ō�ɑI�����Ă������̂��������B
		int iSelectIndex = GetBushouCBSelectedIndex();
		if ( iSelectIndex == 0xFFFFFFFF ) {
			lbBushouList->SelectedIndex = iLastBushouListSelected;
		} else {
			iLastBushouListSelected = iSelectIndex;
		}
	}


	// �������X�g��I��ύX����ƁA�摜�̏�̗�`�e��̒l������ւ��
	void Bushou_lbBushouList_SelectedIndexChanged(Object^ sender, EventArgs^ e ) {
		ModifyCBBushouSelectedIndex();
		// �V����index�̃f�[�^���擾�B
		Bushou_BDataToTextBox();

		ModifyCBBushouSelectedIndex();
	}


	// �����f�[�^���e�L�X�g�{�b�N�X�֓]��
	void Bushou_BDataToTextBox() {
		int iSelectedIndex = GetBushouCBSelectedIndex();
		tbBushouFullName->Text = bdBushouData->lstStrFullName[iSelectedIndex];
		tbBushouRubyYomi->Text = bdBushouData->lstStrRubyYomi[iSelectedIndex];
		tbBushouBornEtc->Text  = bdBushouData->lstStrBornEtc[iSelectedIndex];
		tbBushouRetsuden->Text = bdBushouData->lstStrRetsuden[iSelectedIndex];
	}

	// �e�L�X�g�{�b�N�X�̈ʒu�╝�𒆐g�̃e�L�X�g�ʂɏ]���ĕύX
	void Bushou_TextBoxWidthUpdate() {
		tbBushouFullName->Width = GetStringMeasure(tbBushouFullName->Text);

		tbBushouRubyYomi->Left = tbBushouFullName->Right + 8;
		tbBushouRubyYomi->Width = GetStringMeasure(tbBushouRubyYomi->Text);
		
		tbBushouBornEtc->Left = tbBushouRubyYomi->Right + 8;
		tbBushouBornEtc->Width = GetStringMeasure(tbBushouBornEtc->Text);
	}

	// �e�L�X�g�{�b�N�X�́A�u���������v���ύX���ꂽ�B
	void Bushou_tbFullName_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetBushouCBSelectedIndex();
		bdBushouData->lstStrFullName[iSelectedIndex] = tbBushouFullName->Text;
		String^ strLine = String::Format( "{0:0000} - {1}", iSelectedIndex, tbBushouFullName->Text);
		// SelectIndexChange���������Ȃ��悤�ɂ���B
		lbBushouList->SelectedIndexChanged -= gcnew EventHandler(this, &RetsudenEditorForm::Bushou_lbBushouList_SelectedIndexChanged);

		// �������Ȃ��悤�ɂ����̂ŁA�l������������BSelectedIndexChange���\�b�h���������Ȃ��̂ŃG���[���N���Ȃ��B

		lbBushouList->Items[iSelectedIndex] = gcnew String(strLine); // �������X�g�̖��O�̕����X�V

		Bushou_TextBoxWidthUpdate();

		// �X�V�����̂ŁA������߂��B
		// SelectIndexChange����������悤�ɂ���B
		lbBushouList->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Bushou_lbBushouList_SelectedIndexChanged);
	}

	// �e�L�X�g�{�b�N�X�́A�u�ӂ肪�ȁv���ύX���ꂽ�B
	void Bushou_tbRubyYomi_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetBushouCBSelectedIndex();
		bdBushouData->lstStrRubyYomi[iSelectedIndex] = tbBushouRubyYomi->Text;
		Bushou_TextBoxWidthUpdate();
	}

	// �e�L�X�g�{�b�N�X�́A�u���N���v���ύX���ꂽ�B
	void Bushou_tbBornEtc_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetBushouCBSelectedIndex();
		bdBushouData->lstStrBornEtc[iSelectedIndex]  = tbBushouBornEtc->Text;
		Bushou_TextBoxWidthUpdate();
	}

	// �e�L�X�g�{�b�N�X�́A�u��`�v���ύX���ꂽ�B
	void Bushou_tbRetsuden_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetBushouCBSelectedIndex();

		bdBushouData->lstStrRetsuden[iSelectedIndex] = tbBushouRetsuden->Text;
	}

	// �e�L�X�g�{�b�N�X���狎����
	void Bushou_textBox_Leave(Object^ sender, EventArgs^ e ) {
	}


	// 50�g�ǉ��{�^��
	void Bushou_btnAddBushous_Click(Object^ sender, EventArgs^ e) {
		int iStart = lbBushouList->Items->Count;
		for ( int i = iStart; i < iStart+50; i++ ) {
			String^ bname = String::Format("�o�^{0:0000}", i);
			// ��Ɍ��f�[�^���瑝�₷
			bdBushouData->lstStrFullName->Add( bname );
			bdBushouData->lstStrRubyYomi->Add( "ĳ۸"+i );
			bdBushouData->lstStrBornEtc->Add( "????�`????" );
			bdBushouData->lstStrRetsuden->Add( "������" );
			String^ strLine = String::Format( "{0:0000} - {1}", i, bname);
			lbBushouList->Items->Add( strLine );
		}

	}

	// 50�g�폜�{�^��
	void Bushou_btnDelBushous_Click(Object^ sender, EventArgs^ e) {
		
		int iStart = lbBushouList->Items->Count-1;

		// �I���C���f�b�N�X���폜�\��̏��ɂ�������A�M���M���̂Ƃ���܂őޔ�
		if ( GetBushouCBSelectedIndex() > iStart - 50 ) {
			lbBushouList->SelectedIndex = iStart - 50;
		}

		for ( int i = iStart; i > iStart-50; i-- ) {
			// ��ɕ������X�g����J�b�g
			lbBushouList->Items->RemoveAt(i);

			// �����Ō��f�[�^�����炷
			bdBushouData->lstStrFullName->RemoveAt( i );
			bdBushouData->lstStrRubyYomi->RemoveAt( i );
			bdBushouData->lstStrBornEtc->RemoveAt( i );
			bdBushouData->lstStrRetsuden->RemoveAt( i );
		}
	}




//-----------------------------------------------������`�n

private:
	TabPage^ tpHime;
	HimeData^ bdHimeData;

	TextBox^ tbHimeSearch; // ��������
	Button^ btnHimeSearch; // �����{�^��
	Button^ btnHimeImport; // �捞�{�^��
	OpenFileDialog^ ofdHimeImport;
	Button^ btnHimeExport; // �o�̓{�^��
	Button^ btnHimeAllSave; // �S�ĕۑ�

	ListBox^ lbHimeList; // �������X�g�̃R���{�{�b�N�X
	int iLastHimeListSelected;

	Panel^ plHimeRetsuden;	 // ��`���p�̃p�l��
	TextBox^ tbHimeFullName; // ���������̃e�L�X�g�{�b�N�X
	TextBox^ tbHimeRubyYomi; // �ӂ肪�Ȃ̂��
	TextBox^ tbHimeBornEtc;  // ���N�Ȃǂ̏��
	TextBox^ tbHimeRetsuden; // ��`���


	// �t�H���g
	System::Drawing::Font^ fontHime;
	ComboBox^ cbFontHime;	// �t�H���g���X�g

	Button^ btnHimeAdds; // 50�g�ǉ�
	Button^ btnHimeDels; // 50�g�ǉ�


private:

	void tpHime_Init() {

		tpHime = gcnew TabPage();

		tpHime->Text = "�P��`";
		tpHime->Size = tcRE->ClientSize;

		// ��������
		tbHimeSearch = gcnew TextBox();
		tbHimeSearch->Width = 140;
		tbHimeSearch->Height = 16;
		tbHimeSearch->Left = 10;
		tbHimeSearch->Top = 10;

		// �����{�^��
		btnHimeSearch = gcnew Button();
		btnHimeSearch->Text = "����";
		btnHimeSearch->Width = 60;
		btnHimeSearch->Height = tbHimeSearch->Height;
		btnHimeSearch->Left = tbHimeSearch->Right + 1;
		btnHimeSearch->Top = tbHimeSearch->Top;

		// �捞�{�^��
		btnHimeImport = gcnew Button();
		btnHimeImport->Text = "�P�Ǝ捞";
		btnHimeImport->Width = 70;
		btnHimeImport->Height = tbHimeSearch->Height;
		btnHimeImport->Left = btnHimeSearch->Right + 30;
		btnHimeImport->Top = tbHimeSearch->Top;

		// �o�̓{�^��
		btnHimeExport = gcnew Button();
		btnHimeExport->Text = "�P�Əo��";
		btnHimeExport->Width = 70;
		btnHimeExport->Height = tbHimeSearch->Height;
		btnHimeExport->Left = btnHimeImport->Right;
		btnHimeExport->Top = tbHimeSearch->Top;

		// �S�ĕۑ�
		btnHimeAllSave = gcnew Button();
		btnHimeAllSave->Text = "�P��`��ۑ�";
		btnHimeAllSave->Width = 120;
		btnHimeAllSave->Height = tbHimeSearch->Height;
		btnHimeAllSave->Left = tpHime->Right - (btnHimeAllSave->Width + 20);
		btnHimeAllSave->Top = tbHimeSearch->Top;

		// �����m���
		lbHimeList = gcnew ListBox();
		lbHimeList->Width = tbHimeSearch->Width;
		lbHimeList->Left = tbHimeSearch->Left;
		lbHimeList->Top = tbHimeSearch->Bottom + 10;
		lbHimeList->Height = 200;

		// ��`���p�̃p�l��
		plHimeRetsuden = gcnew Panel();
		plHimeRetsuden->Top = lbHimeList->Top + 20;
		Hime_SetPanelBackImg();
		plHimeRetsuden->Left = tpHime->Right - (plHimeRetsuden->Width + 20);

		tbHimeFullName = gcnew TextBox();
		tbHimeFullName->Top = 10;
		tbHimeFullName->Left = 10;
		tbHimeFullName->Width = 16 * 6 + 6;
		tbHimeFullName->MaxLength = 37;
		tbHimeFullName->BackColor = Color::FromArgb(0xD0, 0xD0, 0xFA);

		tbHimeRubyYomi = gcnew TextBox();
		tbHimeRubyYomi->Top = tbHimeFullName->Top;
		tbHimeRubyYomi->Left = tbHimeFullName->Right;
		tbHimeRubyYomi->MaxLength = 37;
		tbHimeRubyYomi->Width = 16 * 5 + 6;
		tbHimeRubyYomi->BackColor = Color::FromArgb(0xD0, 0xD0, 0xFA);

		tbHimeBornEtc = gcnew TextBox();
		tbHimeBornEtc->Top = tbHimeFullName->Top;
		tbHimeBornEtc->Left = tbHimeRubyYomi->Right;
		tbHimeBornEtc->Width = 16 * 5 + 8;
		tbHimeBornEtc->MaxLength = 37;
		tbHimeBornEtc->BackColor = Color::FromArgb(0xD0, 0xD0, 0xFA);

		tbHimeRetsuden = gcnew TextBox();
		tbHimeRetsuden->Top = tbHimeFullName->Bottom + 12;
		tbHimeRetsuden->Left = tbHimeFullName->Left;
		tbHimeRetsuden->Height = 90;
		tbHimeRetsuden->Width = 312;
		tbHimeRetsuden->MaxLength = 2 * 18 * 4;
		tbHimeRetsuden->Multiline = true;
		tbHimeRetsuden->WordWrap = false;
		tbHimeRetsuden->BackColor = Color::FromArgb(0xD0, 0xD0, 0xFA);


		plHimeRetsuden->Controls->Add(tbHimeFullName);
		plHimeRetsuden->Controls->Add(tbHimeRubyYomi);
		plHimeRetsuden->Controls->Add(tbHimeBornEtc);
		plHimeRetsuden->Controls->Add(tbHimeRetsuden);

		// �t�H���g���X�g
		cbFontHime = gcnew ComboBox();
		cbFontHime->Width = 100;
		cbFontHime->Left = tpHime->Right - (cbFontHime->Width + 20);
		cbFontHime->Top = plHimeRetsuden->Bottom + 10;
		cbFontHime->Height = tbHimeSearch->Height;
		Hime_GetValidTenshoukiFontList();

		Hime_cbFont_SelectedIndexChanged(nullptr, nullptr);

		/*
		// �����g50�l�ǉ�
		btnHimeAdds = gcnew Button();
		btnHimeAdds->Text = "50�g�ǉ�";
		btnHimeAdds->Width = 70;
		btnHimeAdds->Height = tbHimeSearch->Height;
		btnHimeAdds->Left = tbHimeSearch->Left;
		btnHimeAdds->Top = lbHimeList->Bottom + 10;

		// �����g50�l�폜
		btnHimeDels = gcnew Button();
		btnHimeDels->Text = "50�g�폜";
		btnHimeDels->Width = 70;
		btnHimeDels->Height = tbHimeSearch->Height;
		btnHimeDels->Left = btnHimeAdds->Right;
		btnHimeDels->Top = lbHimeList->Bottom + 10;
		*/

		// ������`�̃^�u�ɃR���|�[�l���g�ǉ�
		tpHime->Controls->Add(tbHimeSearch);
		tpHime->Controls->Add(btnHimeSearch);
		// tpHime->Controls->Add(btnHimeImport);
		// tpHime->Controls->Add(btnHimeExport);
		tpHime->Controls->Add(btnHimeAllSave);
		tpHime->Controls->Add(lbHimeList);
		tpHime->Controls->Add(plHimeRetsuden);
		tpHime->Controls->Add(cbFontHime);
		// tpHime->Controls->Add(btnHimeAdds);
		// tpHime->Controls->Add(btnHimeDels);

		// �^�u���t�H�[���ɒǉ�
		tcRE->TabPages->Add(tpHime);

		// �����f�[�^���쐬���āA���X�g�ɗ�������
		bdHimeData = gcnew HimeData();
		// ���X�g�{�b�N�X�ɑ�������
		for (int i = 0; i < bdHimeData->lstStrFullName->Count; i++) {
			String^ strLine = String::Format("{0:0000} - {1}", i, bdHimeData->lstStrFullName[i]);
			lbHimeList->Items->Add(strLine);
		}

		// �ŏ��̂��̂�I�����Ă���
		lbHimeList->SelectedIndex = 0;
		iLastHimeListSelected = 0;

		// �����f�[�^���e�L�X�g�{�b�N�X
		Hime_BDataToTextBox();
		Hime_TextBoxWidthUpdate();
	}

	// �p�l���̔w�i�摜�ݒ�
	void Hime_SetPanelBackImg() {
		// ���̃v���W�F�N�g�̃A�Z���u���̃^�C�v���擾�B
		System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
		System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.RetsudenEditorRes", prj_assebmly->GetName()->Name), prj_assebmly);

		// �p�l���̔w�i
		plHimeRetsuden->BackgroundImage = (System::Drawing::Image^)(r->GetObject("retsuden_back"));
		// �w�i�摜�̑傫���Ƀp�l���̑傫�������킹��
		plHimeRetsuden->Size = plHimeRetsuden->BackgroundImage->Size;

	}

	// �e��f�t�h�̃C�x���g�n���h���ݒ�
	void Hime_SetEventHander() {
		btnHimeSearch->Click += gcnew EventHandler(this, &RetsudenEditorForm::Hime_btnSearch_Click);
		btnHimeImport->Click += gcnew EventHandler(this, &RetsudenEditorForm::Hime_btnImport_Click);
		btnHimeExport->Click += gcnew EventHandler(this, &RetsudenEditorForm::Hime_btnExport_Click);
		btnHimeAllSave->Click += gcnew EventHandler(this, &RetsudenEditorForm::Hime_btnAllSave_Click);
		tbHimeSearch->KeyDown += gcnew KeyEventHandler(this, &RetsudenEditorForm::Hime_tbSearch_KeyDown);
		lbHimeList->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Hime_lbHimeList_SelectedIndexChanged);
		tbHimeFullName->TextChanged += gcnew EventHandler(this, &RetsudenEditorForm::Hime_tbFullName_TextChanged);
		tbHimeRubyYomi->TextChanged += gcnew EventHandler(this, &RetsudenEditorForm::Hime_tbRubyYomi_TextChanged);
		tbHimeBornEtc->TextChanged += gcnew EventHandler(this, &RetsudenEditorForm::Hime_tbBornEtc_TextChanged);
		tbHimeRetsuden->TextChanged += gcnew EventHandler(this, &RetsudenEditorForm::Hime_tbRetsuden_TextChanged);
		tbHimeFullName->Leave += gcnew EventHandler(this, &RetsudenEditorForm::Hime_textBox_Leave);
		tbHimeRubyYomi->Leave += gcnew EventHandler(this, &RetsudenEditorForm::Hime_textBox_Leave);
		tbHimeBornEtc->Leave += gcnew EventHandler(this, &RetsudenEditorForm::Hime_textBox_Leave);
		tbHimeRetsuden->Leave += gcnew EventHandler(this, &RetsudenEditorForm::Hime_textBox_Leave);
		cbFontHime->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Hime_cbFont_SelectedIndexChanged);
		// btnHimeAdds->Click += gcnew EventHandler(this, &RetsudenEditorForm::Hime_btnAddHimes_Click);
		// btnHimeDels->Click += gcnew EventHandler(this, &RetsudenEditorForm::Hime_btnDelHimes_Click);
	}

	// ���������{�^������������
	void Hime_btnSearch_Click(Object^ sender, EventArgs^ e) {
		if (tbHimeSearch->Text == "") { return; } // �e�L�X�g�G���A������ۂȂ牽�����Ȃ��B

		int iStart = GetHimeCBSelectedIndex() + 1; // ���ݑI�����Ă�����̂̂��玟�̌��

		Regex^ regex = gcnew Regex(tbHimeSearch->Text);

		bool isExist = false;
		// iStart�ȍ~��S���T�[�`���āA
		for (int i = iStart; i < lbHimeList->Items->Count; i++) {
			Match^ match = regex->Match((String^)lbHimeList->Items[i]);

			if (match->Success) {
				lbHimeList->SelectedIndex = i;
				isExist = true;
				break;
			}
		}

		if (!isExist) {
			for (int i = 0; i < iStart; i++) {
				Match^ match = regex->Match((String^)lbHimeList->Items[i]);

				if (match->Success) {
					lbHimeList->SelectedIndex = i;
					break;
				}
			}
		}
	}

	// ���������e�L�X�g�{�b�N�X�ŃL�[����������
	void Hime_tbSearch_KeyDown(Object^ sender, KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Return) {
			Hime_btnSearch_Click(sender, e);
		}
		if (e->KeyCode == Keys::F3) {
			Hime_btnSearch_Click(sender, e);
		}
	}

	// �����o�̓{�^������������
	void Hime_btnExport_Click(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetHimeCBSelectedIndex();
		std::string unmanagedData = bdHimeData->GetOneHimeExportData(iSelectedIndex);

		std::string filepath = DotNetStringToMultiByte(bdHimeData->lstStrFullName[iSelectedIndex]);
		if (filepath == "") { filepath = "�ȂȂ�"; }
		filepath += ".n6b";

		FILE* fp = fopen(filepath.c_str(), "wb");
		if (fp) {
			fwrite(unmanagedData.c_str(), unmanagedData.size(), 1, fp);
			fclose(fp);
			System::Windows::Forms::MessageBox::Show("�f�[�^���o�͂��܂����B", "����", ::MessageBoxButtons::OK, ::MessageBoxIcon::Information);
		}
		else {
			System::Windows::Forms::MessageBox::Show("�f�[�^�̏o�͂Ɏ��s���܂����B", "�G���[", MessageBoxButtons::OK, ::MessageBoxIcon::Error);

		}
	}


	// �������̓{�^������������
	void Hime_btnImport_Click(Object^ sender, EventArgs^ e) {

		ofdHimeImport = gcnew OpenFileDialog();

		ofdHimeImport->InitialDirectory = "."; // �c�[���Ɠ����f�B���N�g�����f�t�H���g

		ofdHimeImport->Filter = "�����^ ���b�Z�[�W�t�@�C��(*.nb7)|*.nb7";

		ofdHimeImport->FilterIndex = 1;

		ofdHimeImport->Title = "�Ƃ��镐���̗�`�t�@�C����I�����Ă�������";

		//�_�C�A���O�{�b�N�X�����O�Ɍ��݂̃f�B���N�g���𕜌�����悤�ɂ���
		ofdHimeImport->RestoreDirectory = true;

		//�_�C�A���O��\������
		if (ofdHimeImport->ShowDialog() == ::DialogResult::OK) {
			//OK�{�^�����N���b�N���ꂽ�Ƃ�
			if (ofdHimeImport->FileName) {
				BOOL success = bdHimeData->GetOneHimeImportData(ofdHimeImport->FileName, GetHimeCBSelectedIndex());

				//�@���ۂɂ�����炸�X�V
				Hime_BDataToTextBox();
			}
		}
	}

	// �S�ĕۑ��{�^������������
	void Hime_btnAllSave_Click(Object^ sender, EventArgs^ e) {
		bdHimeData->AllSaveToBFileN6P();
	}

	// �t�H���g���X�g�̑I�����ڂ��ύX���ꂽ��
	void Hime_cbFont_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
		// �t�H���g�̐ݒ�
		fontHime = gcnew System::Drawing::Font((String^)cbFontHime->Items[cbFontHime->SelectedIndex], 12, FontStyle::Regular);
		tbHimeFullName->Font = fontHime;
		tbHimeRubyYomi->Font = fontHime;
		tbHimeBornEtc->Font = fontHime;
		tbHimeRetsuden->Font = fontHime;
	}

	// �C���X�g�[������Ă���t�H���g�ɂ��������āA�t�H���g���X�g�{�b�N�X�ɒǉ�
	void Hime_GetValidTenshoukiFontList() {

		//InstalledFontCollection�I�u�W�F�N�g�̎擾
		System::Drawing::Text::InstalledFontCollection^ ifc = gcnew System::Drawing::Text::InstalledFontCollection();

		//�C���X�g�[������Ă��邷�ׂẴt�H���g�t�@�~���A���擾
		cli::array<FontFamily^>^ ffs = ifc->Families;

		//�����ł͌��ƂȂ肦��t�H���g���̂�
		for (int i = 0; i < aryStrFontCandidate->Length; i++) {

			// �t�H���g���X�g
			for each (FontFamily ^ ff in ffs) {

				// ���t�H���g���������B
				if (ff->Name == aryStrFontCandidate[i]) {
					// �R���{�{�b�N�X�ɒǉ�
					cbFontHime->Items->Add(ff->Name);
				}
			}
		}

		// �P�ȏ�t�H���g����������A�ŏ��̂��̂�I�����Ă����B
		if (cbFontHime->Items->Count > 0) {
			cbFontHime->SelectedIndex = 0;
		}
	}

	int GetHimeCBSelectedIndex() {
		int iSelectedIndex = lbHimeList->SelectedIndex;
		if (0 <= iSelectedIndex && iSelectedIndex < lbHimeList->Items->Count) {
			return iSelectedIndex;
		}
		else {
			return 0;
		}
	}
	void ModifyCBHimeSelectedIndex() {
		// �œ_���������Ă���悤�Ȃ�A�Ō�ɑI�����Ă������̂��������B
		int iSelectIndex = GetHimeCBSelectedIndex();
		if (iSelectIndex == 0xFFFFFFFF) {
			lbHimeList->SelectedIndex = iLastHimeListSelected;
		}
		else {
			iLastHimeListSelected = iSelectIndex;
		}
	}


	// �������X�g��I��ύX����ƁA�摜�̏�̗�`�e��̒l������ւ��
	void Hime_lbHimeList_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
		ModifyCBHimeSelectedIndex();
		// �V����index�̃f�[�^���擾�B
		Hime_BDataToTextBox();

		ModifyCBHimeSelectedIndex();
	}


	// �����f�[�^���e�L�X�g�{�b�N�X�֓]��
	void Hime_BDataToTextBox() {
		int iSelectedIndex = GetHimeCBSelectedIndex();
		tbHimeFullName->Text = bdHimeData->lstStrFullName[iSelectedIndex];
		tbHimeRubyYomi->Text = bdHimeData->lstStrRubyYomi[iSelectedIndex];
		tbHimeBornEtc->Text = bdHimeData->lstStrBornEtc[iSelectedIndex];
		tbHimeRetsuden->Text = bdHimeData->lstStrRetsuden[iSelectedIndex];
	}

	// �e�L�X�g�{�b�N�X�̈ʒu�╝�𒆐g�̃e�L�X�g�ʂɏ]���ĕύX
	void Hime_TextBoxWidthUpdate() {
		tbHimeFullName->Width = GetStringMeasure(tbHimeFullName->Text);

		tbHimeRubyYomi->Left = tbHimeFullName->Right + 8;
		tbHimeRubyYomi->Width = GetStringMeasure(tbHimeRubyYomi->Text);

		tbHimeBornEtc->Left = tbHimeRubyYomi->Right + 8;
		tbHimeBornEtc->Width = GetStringMeasure(tbHimeBornEtc->Text);
	}

	// �e�L�X�g�{�b�N�X�́A�u���������v���ύX���ꂽ�B
	void Hime_tbFullName_TextChanged(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetHimeCBSelectedIndex();
		bdHimeData->lstStrFullName[iSelectedIndex] = tbHimeFullName->Text;
		String^ strLine = String::Format("{0:0000} - {1}", iSelectedIndex, tbHimeFullName->Text);
		// SelectIndexChange���������Ȃ��悤�ɂ���B
		lbHimeList->SelectedIndexChanged -= gcnew EventHandler(this, &RetsudenEditorForm::Hime_lbHimeList_SelectedIndexChanged);

		// �������Ȃ��悤�ɂ����̂ŁA�l������������BSelectedIndexChange���\�b�h���������Ȃ��̂ŃG���[���N���Ȃ��B

		lbHimeList->Items[iSelectedIndex] = gcnew String(strLine); // �������X�g�̖��O�̕����X�V

		Hime_TextBoxWidthUpdate();

		// �X�V�����̂ŁA������߂��B
		// SelectIndexChange����������悤�ɂ���B
		lbHimeList->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Hime_lbHimeList_SelectedIndexChanged);
	}

	// �e�L�X�g�{�b�N�X�́A�u�ӂ肪�ȁv���ύX���ꂽ�B
	void Hime_tbRubyYomi_TextChanged(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetHimeCBSelectedIndex();
		bdHimeData->lstStrRubyYomi[iSelectedIndex] = tbHimeRubyYomi->Text;
		Hime_TextBoxWidthUpdate();
	}

	// �e�L�X�g�{�b�N�X�́A�u���N���v���ύX���ꂽ�B
	void Hime_tbBornEtc_TextChanged(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetHimeCBSelectedIndex();
		bdHimeData->lstStrBornEtc[iSelectedIndex] = tbHimeBornEtc->Text;
		Hime_TextBoxWidthUpdate();
	}

	// �e�L�X�g�{�b�N�X�́A�u��`�v���ύX���ꂽ�B
	void Hime_tbRetsuden_TextChanged(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetHimeCBSelectedIndex();

		bdHimeData->lstStrRetsuden[iSelectedIndex] = tbHimeRetsuden->Text;
	}

	// �e�L�X�g�{�b�N�X���狎����
	void Hime_textBox_Leave(Object^ sender, EventArgs^ e) {
	}


	// 50�g�ǉ��{�^��
	void Hime_btnAddHimes_Click(Object^ sender, EventArgs^ e) {
		int iStart = lbHimeList->Items->Count;
		for (int i = iStart; i < iStart + 50; i++) {
			String^ bname = String::Format("�o�^{0:0000}", i);
			// ��Ɍ��f�[�^���瑝�₷
			bdHimeData->lstStrFullName->Add(bname);
			bdHimeData->lstStrRubyYomi->Add("ĳ۸" + i);
			bdHimeData->lstStrBornEtc->Add("????�`????");
			bdHimeData->lstStrRetsuden->Add("������");
			String^ strLine = String::Format("{0:0000} - {1}", i, bname);
			lbHimeList->Items->Add(strLine);
		}

	}

	// 50�g�폜�{�^��
	void Hime_btnDelHimes_Click(Object^ sender, EventArgs^ e) {

		int iStart = lbHimeList->Items->Count - 1;

		// �I���C���f�b�N�X���폜�\��̏��ɂ�������A�M���M���̂Ƃ���܂őޔ�
		if (GetHimeCBSelectedIndex() > iStart - 50) {
			lbHimeList->SelectedIndex = iStart - 50;
		}

		for (int i = iStart; i > iStart - 50; i--) {
			// ��ɕ������X�g����J�b�g
			lbHimeList->Items->RemoveAt(i);

			// �����Ō��f�[�^�����炷
			bdHimeData->lstStrFullName->RemoveAt(i);
			bdHimeData->lstStrRubyYomi->RemoveAt(i);
			bdHimeData->lstStrBornEtc->RemoveAt(i);
			bdHimeData->lstStrRetsuden->RemoveAt(i);
		}
	}



//-----------------------------------------------�ƕ��`�n
private:
	TabPage^ tpKahou;
	KahouData^ bdKahouData;

	TextBox^ tbKahouSearch; // �ƕ󌟍�
	Button^ btnKahouSearch; // �����{�^��
	Button^ btnKahouImport; // �捞�{�^��
	Button^ btnKahouExport; // �o�̓{�^��
	Button^ btnKahouAllSave; // �S�ĕۑ�

	ListBox^ lbKahouList; // �ƕ󃊃X�g�̃R���{�{�b�N�X
	int iLastKahouListSelected;

	Panel^ plKahouRetsuden;	 // ��`���p�̃p�l��
	TextBox^ tbKahouFullName; // �ƕ󐩖��̃e�L�X�g�{�b�N�X
	TextBox^ tbKahouRubyYomi; // �ӂ肪�Ȃ̂��
	TextBox^ tbKahouBornEtc;  // ���N�Ȃǂ̏��
	TextBox^ tbKahouRetsuden; // ��`���


	// �t�H���g
	System::Drawing::Font^ fontKahou;
	ComboBox^ cbFontKahou;	// �t�H���g���X�g

	Button^ btnKahouAdds; // 50�g�ǉ�
	Button^ btnKahouDels; // 50�g�ǉ�



private:

	void tpKahou_Init() {

		tpKahou = gcnew TabPage();

		tpKahou->Text = "�ƕ��`";
		tpKahou->Size = tcRE->ClientSize;

		// �ƕ󌟍�
		tbKahouSearch = gcnew TextBox();
		tbKahouSearch->Width = 140;
		tbKahouSearch->Height = 16;
		tbKahouSearch->Left = 10;
		tbKahouSearch->Top = 10;

		// �����{�^��
		btnKahouSearch = gcnew Button();
		btnKahouSearch->Text = "����";
		btnKahouSearch->Width = 60;
		btnKahouSearch->Height = tbKahouSearch->Height;
		btnKahouSearch->Left = tbKahouSearch->Right + 1;
		btnKahouSearch->Top = tbKahouSearch->Top;

		/*
		// �捞�{�^��
		btnKahouImport = gcnew Button();
		btnKahouImport->Text = "�P�Ǝ捞";
		btnKahouImport->Width = 70;
		btnKahouImport->Height = tbKahouSearch->Height;
		btnKahouImport->Left = btnKahouSearch->Right + 30;
		btnKahouImport->Top = tbKahouSearch->Top;

		// �o�̓{�^��
		btnKahouExport = gcnew Button();
		btnKahouExport->Text = "�P�Əo��";
		btnKahouExport->Width = 70;
		btnKahouExport->Height = tbKahouSearch->Height;
		btnKahouExport->Left = btnKahouImport->Right;
		btnKahouExport->Top = tbKahouSearch->Top;
		*/

		// �S�ĕۑ�
		btnKahouAllSave = gcnew Button();
		btnKahouAllSave->Text = "�ƕ��`��ۑ�";
		btnKahouAllSave->Width = 120;
		btnKahouAllSave->Height = tbKahouSearch->Height;
		btnKahouAllSave->Left = tpKahou->Right - (btnKahouAllSave->Width+20);
		btnKahouAllSave->Top = tbKahouSearch->Top;

		// �ƕ�m���
		lbKahouList = gcnew ListBox();
		lbKahouList->Width = tbKahouSearch->Width;
		lbKahouList->Left = tbKahouSearch->Left;
		lbKahouList->Top = tbKahouSearch->Bottom + 10;
		lbKahouList->Height = 200;

		// ��`���p�̃p�l��
		plKahouRetsuden = gcnew Panel();
		plKahouRetsuden->Top = lbKahouList->Top + 20;
		Kahou_SetPanelBackImg();
		plKahouRetsuden->Left = tpKahou->Right - (plKahouRetsuden->Width+20);

		tbKahouFullName = gcnew TextBox();
		tbKahouFullName->Top = 10;
		tbKahouFullName->Left = 10;
		tbKahouFullName->Width = 16 * 6 + 6;
		tbKahouFullName->MaxLength = 37;
		tbKahouFullName->BackColor = Color::FromArgb(0xD0, 0xD0, 0xFA);

		tbKahouRubyYomi = gcnew TextBox();
		tbKahouRubyYomi->Top = tbKahouFullName->Top;
		tbKahouRubyYomi->Left = tbKahouFullName->Right;
		tbKahouRubyYomi->MaxLength = 37;
		tbKahouRubyYomi->Width = 16 * 5 + 6;
		tbKahouRubyYomi->BackColor = Color::FromArgb(0xD0, 0xD0, 0xFA);

		tbKahouBornEtc  = gcnew TextBox();
		tbKahouBornEtc->Top = tbKahouFullName->Top;
		tbKahouBornEtc->Left = tbKahouRubyYomi->Right;
		tbKahouBornEtc->Width = 16 * 5 + 8;
		tbKahouBornEtc->MaxLength = 37;
		tbKahouBornEtc->BackColor = Color::FromArgb(0xD0, 0xD0, 0xFA);

		tbKahouRetsuden = gcnew TextBox();
		tbKahouRetsuden->Top = tbKahouFullName->Bottom + 12;
		tbKahouRetsuden->Left = tbKahouFullName->Left;
		tbKahouRetsuden->Height = 90;
		tbKahouRetsuden->Width = 312;
		tbKahouRetsuden->MaxLength = 2 * 18 * 4;
		tbKahouRetsuden->Multiline = true;
		tbKahouRetsuden->WordWrap = false;
		tbKahouRetsuden->BackColor = Color::FromArgb(0xD0, 0xD0, 0xFA);
		

		plKahouRetsuden->Controls->Add(tbKahouFullName);
		plKahouRetsuden->Controls->Add(tbKahouRubyYomi);
		// plKahouRetsuden->Controls->Add(tbKahouBornEtc);
		plKahouRetsuden->Controls->Add(tbKahouRetsuden);

		// �t�H���g���X�g
		cbFontKahou = gcnew ComboBox();
		cbFontKahou->Width = 100;
		cbFontKahou->Left = tpKahou->Right - (cbFontKahou->Width+20);
		cbFontKahou->Top = plKahouRetsuden->Bottom + 10;
		cbFontKahou->Height = tbKahouSearch->Height;
		Kahou_GetValidTenshoukiFontList();

		Kahou_cbFont_SelectedIndexChanged(nullptr, nullptr);

		// �ƕ�g50�l�ǉ�
		btnKahouAdds = gcnew Button();
		btnKahouAdds->Text = "50�g�ǉ�";
		btnKahouAdds->Width = 70;
		btnKahouAdds->Height = tbKahouSearch->Height;
		btnKahouAdds->Left = tbKahouSearch->Left;
		btnKahouAdds->Top = lbKahouList->Bottom + 10;

		// �ƕ�g50�l�폜
		btnKahouDels = gcnew Button();
		btnKahouDels->Text = "50�g�폜";
		btnKahouDels->Width = 70;
		btnKahouDels->Height = tbKahouSearch->Height;
		btnKahouDels->Left = btnKahouAdds->Right;
		btnKahouDels->Top = lbKahouList->Bottom + 10;


		// �ƕ��`�̃^�u�ɃR���|�[�l���g�ǉ�
		tpKahou->Controls->Add(tbKahouSearch);
		tpKahou->Controls->Add(btnKahouSearch);
		// tpKahou->Controls->Add(btnKahouImport);
		// tpKahou->Controls->Add(btnKahouExport);
		tpKahou->Controls->Add(btnKahouAllSave);
		tpKahou->Controls->Add(lbKahouList);
		tpKahou->Controls->Add(plKahouRetsuden);
		tpKahou->Controls->Add(cbFontKahou);
		// tpKahou->Controls->Add(btnKahouAdds); // �ƕ�p�ɂ͂��̃{�^���͕s�v
		// tpKahou->Controls->Add(btnKahouDels); // �ƕ�p�ɂ͂��̃{�^���͕s�v
		
		// �^�u���t�H�[���ɒǉ�
		tcRE->TabPages->Add(tpKahou);

		// �ƕ�f�[�^���쐬���āA���X�g�ɗ�������
		bdKahouData = gcnew KahouData();
		// ���X�g�{�b�N�X�ɑ�������
		for ( int i=0; i<bdKahouData->lstStrFullName->Count; i++ ) {
			String^ strLine = String::Format( "{0:000} - {1}", i, bdKahouData->lstStrFullName[i]);
			lbKahouList->Items->Add( strLine );
		}

		// �ŏ��̂��̂�I�����Ă���
		lbKahouList->SelectedIndex = 0;
		iLastKahouListSelected = 0;

		// �ƕ�f�[�^���e�L�X�g�{�b�N�X
		Kahou_BDataToTextBox();
		Kahou_TextBoxWidthUpdate();
	}

	// �p�l���̔w�i�摜�ݒ�
	void Kahou_SetPanelBackImg() {
		// ���̃v���W�F�N�g�̃A�Z���u���̃^�C�v���擾�B
		System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
		System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.RetsudenEditorRes", prj_assebmly->GetName()->Name), prj_assebmly);

		// �p�l���̔w�i
		plKahouRetsuden->BackgroundImage = (System::Drawing::Image^)(r->GetObject("retsuden_back"));
		// �w�i�摜�̑傫���Ƀp�l���̑傫�������킹��
		plKahouRetsuden->Size = plKahouRetsuden->BackgroundImage->Size;

	}

	// �e��f�t�h�̃C�x���g�n���h���ݒ�
	void Kahou_SetEventHander() {
		btnKahouSearch->Click += gcnew EventHandler(this, &RetsudenEditorForm::Kahou_btnSearch_Click);
		// btnKahouImport->Click += gcnew EventHandler(this, &RetsudenEditorForm::Kahou_btnImport_Click);
		// btnKahouExport->Click += gcnew EventHandler(this, &RetsudenEditorForm::Kahou_btnExport_Click);
		btnKahouAllSave->Click += gcnew EventHandler(this, &RetsudenEditorForm::Kahou_btnAllSave_Click);
		tbKahouSearch->KeyDown += gcnew KeyEventHandler(this, &RetsudenEditorForm::Kahou_tbSearch_KeyDown);
		lbKahouList->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Kahou_lbKahouList_SelectedIndexChanged);
		tbKahouFullName->TextChanged += gcnew EventHandler( this, &RetsudenEditorForm::Kahou_tbFullName_TextChanged);
		tbKahouRubyYomi->TextChanged += gcnew EventHandler( this, &RetsudenEditorForm::Kahou_tbRubyYomi_TextChanged);
		tbKahouBornEtc->TextChanged += gcnew EventHandler( this, &RetsudenEditorForm::Kahou_tbBornEtc_TextChanged);
		tbKahouRetsuden->TextChanged += gcnew EventHandler( this, &RetsudenEditorForm::Kahou_tbRetsuden_TextChanged);
		tbKahouFullName->Leave += gcnew EventHandler( this, &RetsudenEditorForm::Kahou_textBox_Leave);
		tbKahouRubyYomi->Leave += gcnew EventHandler( this, &RetsudenEditorForm::Kahou_textBox_Leave);
		tbKahouBornEtc->Leave += gcnew EventHandler( this, &RetsudenEditorForm::Kahou_textBox_Leave);
		tbKahouRetsuden->Leave += gcnew EventHandler( this, &RetsudenEditorForm::Kahou_textBox_Leave);
		cbFontKahou->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Kahou_cbFont_SelectedIndexChanged);
		// btnKahouAdds->Click += gcnew EventHandler(this, &RetsudenEditorForm::Kahou_btnAddKahous_Click);
		// btnKahouDels->Click += gcnew EventHandler(this, &RetsudenEditorForm::Kahou_btnDelKahous_Click);
	}

	// �ƕ󌟍��{�^������������
	void Kahou_btnSearch_Click(Object^ sender, EventArgs^ e) {
		if ( tbKahouSearch->Text == "" ) { return; } // �e�L�X�g�G���A������ۂȂ牽�����Ȃ��B

		int iStart = GetKahouCBSelectedIndex() + 1; // ���ݑI�����Ă�����̂̂��玟�̌��

		Regex^ regex = gcnew Regex(tbKahouSearch->Text);

		bool isExist = false;
		// iStart�ȍ~��S���T�[�`���āA
		for ( int i=iStart; i<lbKahouList->Items->Count; i++) {
			Match^ match = regex->Match( (String^)lbKahouList->Items[i] );

			if ( match->Success ) {
				lbKahouList->SelectedIndex = i;
				isExist = true;
				break;
			}
		}

		if ( !isExist ) {
			for ( int i=0; i<iStart; i++) {
				Match^ match = regex->Match( (String^)lbKahouList->Items[i] );

				if ( match->Success ) {
					lbKahouList->SelectedIndex = i;
					break;
				}
			}
		}
	}

	// �ƕ󌟍��e�L�X�g�{�b�N�X�ŃL�[����������
	void Kahou_tbSearch_KeyDown(Object^ sender, KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Return ) {
			Kahou_btnSearch_Click(sender, e);
		}
		if (e->KeyCode == Keys::F3 ) {
			Kahou_btnSearch_Click(sender, e);
		}
	}

	// �ƕ�o�̓{�^������������
	void Kahou_btnExport_Click(Object^ sender, EventArgs^ e) {
	}


	// �ƕ���̓{�^������������
	void Kahou_btnImport_Click(Object^ sender, EventArgs^ e) {
	}

	// �S�ĕۑ��{�^������������
	void Kahou_btnAllSave_Click(Object^ sender, EventArgs^ e) {
		bdKahouData->AllSaveToMessageN6P();
	}

	// �t�H���g���X�g�̑I�����ڂ��ύX���ꂽ��
	void Kahou_cbFont_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
		// �t�H���g�̐ݒ�
		fontKahou = gcnew System::Drawing::Font((String^)cbFontKahou->Items[cbFontKahou->SelectedIndex], 12, FontStyle::Regular);
		tbKahouFullName->Font = fontKahou;
		tbKahouRubyYomi->Font = fontKahou;
		tbKahouBornEtc->Font  = fontKahou;
		tbKahouRetsuden->Font = fontKahou;
	}

	// �C���X�g�[������Ă���t�H���g�ɂ��������āA�t�H���g���X�g�{�b�N�X�ɒǉ�
	void Kahou_GetValidTenshoukiFontList() {

		//InstalledFontCollection�I�u�W�F�N�g�̎擾
		System::Drawing::Text::InstalledFontCollection^ ifc = gcnew System::Drawing::Text::InstalledFontCollection();

		//�C���X�g�[������Ă��邷�ׂẴt�H���g�t�@�~���A���擾
		cli::array<FontFamily^>^ ffs = ifc->Families;

		//�����ł͌��ƂȂ肦��t�H���g���̂�
		for ( int i=0; i<aryStrFontCandidate->Length; i++) {

			// �t�H���g���X�g
			for each (FontFamily^ ff in ffs) {

				// ���t�H���g���������B
				if (ff->Name == aryStrFontCandidate[i]) {
					// �R���{�{�b�N�X�ɒǉ�
					cbFontKahou->Items->Add( ff->Name );
				}
			}
		}

		// �P�ȏ�t�H���g����������A�ŏ��̂��̂�I�����Ă����B
		if ( cbFontKahou->Items->Count > 0 ) {
			cbFontKahou->SelectedIndex = 0;
		}
	}

	int GetKahouCBSelectedIndex() {
		int iSelectedIndex = lbKahouList->SelectedIndex;
		if ( 0 <= iSelectedIndex && iSelectedIndex < lbKahouList->Items->Count) {
			return iSelectedIndex;
		} else {
			return 0;
		}
	}
	void ModifyCBKahouSelectedIndex() {
		// �œ_���������Ă���悤�Ȃ�A�Ō�ɑI�����Ă������̂��������B
		int iSelectIndex = GetKahouCBSelectedIndex();
		if ( iSelectIndex == 0xFFFFFFFF ) {
			lbKahouList->SelectedIndex = iLastKahouListSelected;
		} else {
			iLastKahouListSelected = iSelectIndex;
		}
	}


	// �ƕ󃊃X�g��I��ύX����ƁA�摜�̏�̗�`�e��̒l������ւ��
	void Kahou_lbKahouList_SelectedIndexChanged(Object^ sender, EventArgs^ e ) {
		ModifyCBKahouSelectedIndex();
		// �V����index�̃f�[�^���擾�B
		Kahou_BDataToTextBox();

		ModifyCBKahouSelectedIndex();
	}


	// �ƕ�f�[�^���e�L�X�g�{�b�N�X�֓]��
	void Kahou_BDataToTextBox() {
		int iSelectedIndex = GetKahouCBSelectedIndex();
		tbKahouFullName->Text = bdKahouData->lstStrFullName[iSelectedIndex];
		tbKahouRubyYomi->Text = bdKahouData->lstStrRubyYomi[iSelectedIndex];
		tbKahouBornEtc->Text  = bdKahouData->lstStrBornEtc[iSelectedIndex];
		tbKahouRetsuden->Text = bdKahouData->lstStrRetsuden[iSelectedIndex];
	}

	// �e�L�X�g�{�b�N�X�̈ʒu�╝�𒆐g�̃e�L�X�g�ʂɏ]���ĕύX
	void Kahou_TextBoxWidthUpdate() {
		tbKahouFullName->Width = GetStringMeasure(tbKahouFullName->Text);

		tbKahouRubyYomi->Left = tbKahouFullName->Right + 8;
		tbKahouRubyYomi->Width = GetStringMeasure(tbKahouRubyYomi->Text);
		
		tbKahouBornEtc->Left = tbKahouRubyYomi->Right + 8;
		tbKahouBornEtc->Width = GetStringMeasure(tbKahouBornEtc->Text);
	}

	// �e�L�X�g�{�b�N�X�́A�u�ƕ󐩖��v���ύX���ꂽ�B
	void Kahou_tbFullName_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetKahouCBSelectedIndex();
		bdKahouData->lstStrFullName[iSelectedIndex] = tbKahouFullName->Text;
		String^ strLine = String::Format( "{0:000} - {1}", iSelectedIndex, tbKahouFullName->Text);
		// SelectIndexChange���������Ȃ��悤�ɂ���B
		lbKahouList->SelectedIndexChanged -= gcnew EventHandler(this, &RetsudenEditorForm::Kahou_lbKahouList_SelectedIndexChanged);

		// �������Ȃ��悤�ɂ����̂ŁA�l������������BSelectedIndexChange���\�b�h���������Ȃ��̂ŃG���[���N���Ȃ��B

		lbKahouList->Items[iSelectedIndex] = gcnew String(strLine); // �ƕ󃊃X�g�̖��O�̕����X�V

		Kahou_TextBoxWidthUpdate();

		// �X�V�����̂ŁA������߂��B
		// SelectIndexChange����������悤�ɂ���B
		lbKahouList->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Kahou_lbKahouList_SelectedIndexChanged);
	}

	// �e�L�X�g�{�b�N�X�́A�u�ӂ肪�ȁv���ύX���ꂽ�B
	void Kahou_tbRubyYomi_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetKahouCBSelectedIndex();
		bdKahouData->lstStrRubyYomi[iSelectedIndex] = tbKahouRubyYomi->Text;
		Kahou_TextBoxWidthUpdate();
	}

	// �e�L�X�g�{�b�N�X�́A�u���N���v���ύX���ꂽ�B
	void Kahou_tbBornEtc_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetKahouCBSelectedIndex();
		bdKahouData->lstStrBornEtc[iSelectedIndex]  = tbKahouBornEtc->Text;
		Kahou_TextBoxWidthUpdate();
	}

	// �e�L�X�g�{�b�N�X�́A�u��`�v���ύX���ꂽ�B
	void Kahou_tbRetsuden_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetKahouCBSelectedIndex();

		bdKahouData->lstStrRetsuden[iSelectedIndex] = tbKahouRetsuden->Text;
	}

	// �e�L�X�g�{�b�N�X���狎����
	void Kahou_textBox_Leave(Object^ sender, EventArgs^ e ) {
	}


	// 50�g�ǉ��{�^��
	void Kahou_btnAddKahous_Click(Object^ sender, EventArgs^ e) {
	}

	// 50�g�폜�{�^��
	void Kahou_btnDelKahous_Click(Object^ sender, EventArgs^ e) {
	}

	//-----------------------------------------------���`�n
private:
	TabPage^ tpCastle;
	CastleData^ bdCastleData;

	TextBox^ tbCastleSearch; // �錟��
	Button^ btnCastleSearch; // �����{�^��
	Button^ btnCastleImport; // �捞�{�^��
	Button^ btnCastleExport; // �o�̓{�^��
	Button^ btnCastleAllSave; // �S�ĕۑ�

	ListBox^ lbCastleList; // �郊�X�g�̃R���{�{�b�N�X
	int iLastCastleListSelected;

	Panel^ plCastleRetsuden;	 // ��`���p�̃p�l��
	TextBox^ tbCastleFullName; // �鐩���̃e�L�X�g�{�b�N�X
	TextBox^ tbCastleRubyYomi; // �ӂ肪�Ȃ̂��
	TextBox^ tbCastleBornEtc;  // ���N�Ȃǂ̏��
	TextBox^ tbCastleRetsuden; // ��`���


	// �t�H���g
	System::Drawing::Font^ fontCastle;
	ComboBox^ cbFontCastle;	// �t�H���g���X�g

	Button^ btnCastleAdds; // 50�g�ǉ�
	Button^ btnCastleDels; // 50�g�ǉ�


private:

	void tpCastle_Init() {

		tpCastle = gcnew TabPage();

		tpCastle->Text = "���`";
		tpCastle->Size = tcRE->ClientSize;

		// �錟��
		tbCastleSearch = gcnew TextBox();
		tbCastleSearch->Width = 140;
		tbCastleSearch->Height = 16;
		tbCastleSearch->Left = 10;
		tbCastleSearch->Top = 10;

		// �����{�^��
		btnCastleSearch = gcnew Button();
		btnCastleSearch->Text = "����";
		btnCastleSearch->Width = 60;
		btnCastleSearch->Height = tbCastleSearch->Height;
		btnCastleSearch->Left = tbCastleSearch->Right + 1;
		btnCastleSearch->Top = tbCastleSearch->Top;

		/*
		// �捞�{�^��
		btnCastleImport = gcnew Button();
		btnCastleImport->Text = "�P�Ǝ捞";
		btnCastleImport->Width = 70;
		btnCastleImport->Height = tbCastleSearch->Height;
		btnCastleImport->Left = btnCastleSearch->Right + 30;
		btnCastleImport->Top = tbCastleSearch->Top;

		// �o�̓{�^��
		btnCastleExport = gcnew Button();
		btnCastleExport->Text = "�P�Əo��";
		btnCastleExport->Width = 70;
		btnCastleExport->Height = tbCastleSearch->Height;
		btnCastleExport->Left = btnCastleImport->Right;
		btnCastleExport->Top = tbCastleSearch->Top;
		*/

		// �S�ĕۑ�
		btnCastleAllSave = gcnew Button();
		btnCastleAllSave->Text = "���`��ۑ�";
		btnCastleAllSave->Width = 120;
		btnCastleAllSave->Height = tbCastleSearch->Height;
		btnCastleAllSave->Left = tpCastle->Right - (btnCastleAllSave->Width + 20);
		btnCastleAllSave->Top = tbCastleSearch->Top;

		// ��m���
		lbCastleList = gcnew ListBox();
		lbCastleList->Width = tbCastleSearch->Width;
		lbCastleList->Left = tbCastleSearch->Left;
		lbCastleList->Top = tbCastleSearch->Bottom + 10;
		lbCastleList->Height = 200;

		// ��`���p�̃p�l��
		plCastleRetsuden = gcnew Panel();
		plCastleRetsuden->Top = lbCastleList->Top + 20;
		Castle_SetPanelBackImg();
		plCastleRetsuden->Left = tpCastle->Right - (plCastleRetsuden->Width + 20);

		tbCastleFullName = gcnew TextBox();
		tbCastleFullName->Top = 10;
		tbCastleFullName->Left = 10;
		tbCastleFullName->Width = 16 * 6 + 6;
		tbCastleFullName->MaxLength = 37;
		tbCastleFullName->BackColor = Color::FromArgb(0xD0, 0xD0, 0xFA);

		tbCastleRubyYomi = gcnew TextBox();
		tbCastleRubyYomi->Top = tbCastleFullName->Top;
		tbCastleRubyYomi->Left = tbCastleFullName->Right;
		tbCastleRubyYomi->MaxLength = 37;
		tbCastleRubyYomi->Width = 16 * 5 + 6;
		tbCastleRubyYomi->BackColor = Color::FromArgb(0xD0, 0xD0, 0xFA);

		tbCastleBornEtc = gcnew TextBox();
		tbCastleBornEtc->Top = tbCastleFullName->Top;
		tbCastleBornEtc->Left = tbCastleRubyYomi->Right;
		tbCastleBornEtc->Width = 16 * 5 + 8;
		tbCastleBornEtc->MaxLength = 37;
		tbCastleBornEtc->BackColor = Color::FromArgb(0xD0, 0xD0, 0xFA);

		tbCastleRetsuden = gcnew TextBox();
		tbCastleRetsuden->Top = tbCastleFullName->Bottom + 12;
		tbCastleRetsuden->Left = tbCastleFullName->Left;
		tbCastleRetsuden->Height = 90;
		tbCastleRetsuden->Width = 312;
		tbCastleRetsuden->MaxLength = 2 * 18 * 4;
		tbCastleRetsuden->Multiline = true;
		tbCastleRetsuden->WordWrap = false;
		tbCastleRetsuden->BackColor = Color::FromArgb(0xD0, 0xD0, 0xFA);


		plCastleRetsuden->Controls->Add(tbCastleFullName);
		plCastleRetsuden->Controls->Add(tbCastleRubyYomi);
		// plCastleRetsuden->Controls->Add(tbCastleBornEtc);
		plCastleRetsuden->Controls->Add(tbCastleRetsuden);

		// �t�H���g���X�g
		cbFontCastle = gcnew ComboBox();
		cbFontCastle->Width = 100;
		cbFontCastle->Left = tpCastle->Right - (cbFontCastle->Width + 20);
		cbFontCastle->Top = plCastleRetsuden->Bottom + 10;
		cbFontCastle->Height = tbCastleSearch->Height;
		Castle_GetValidTenshoukiFontList();

		Castle_cbFont_SelectedIndexChanged(nullptr, nullptr);

		// ��g50�l�ǉ�
		btnCastleAdds = gcnew Button();
		btnCastleAdds->Text = "50�g�ǉ�";
		btnCastleAdds->Width = 70;
		btnCastleAdds->Height = tbCastleSearch->Height;
		btnCastleAdds->Left = tbCastleSearch->Left;
		btnCastleAdds->Top = lbCastleList->Bottom + 10;

		// ��g50�l�폜
		btnCastleDels = gcnew Button();
		btnCastleDels->Text = "50�g�폜";
		btnCastleDels->Width = 70;
		btnCastleDels->Height = tbCastleSearch->Height;
		btnCastleDels->Left = btnCastleAdds->Right;
		btnCastleDels->Top = lbCastleList->Bottom + 10;


		// ���`�̃^�u�ɃR���|�[�l���g�ǉ�
		tpCastle->Controls->Add(tbCastleSearch);
		tpCastle->Controls->Add(btnCastleSearch);
		// tpCastle->Controls->Add(btnCastleImport);
		// tpCastle->Controls->Add(btnCastleExport);
		tpCastle->Controls->Add(btnCastleAllSave);
		tpCastle->Controls->Add(lbCastleList);
		tpCastle->Controls->Add(plCastleRetsuden);
		tpCastle->Controls->Add(cbFontCastle);
		// tpCastle->Controls->Add(btnCastleAdds); // ��p�ɂ͂��̃{�^���͕s�v
		// tpCastle->Controls->Add(btnCastleDels); // ��p�ɂ͂��̃{�^���͕s�v

		// �^�u���t�H�[���ɒǉ�
		tcRE->TabPages->Add(tpCastle);

		// ��f�[�^���쐬���āA���X�g�ɗ�������
		bdCastleData = gcnew CastleData();
		// ���X�g�{�b�N�X�ɑ�������
		for (int i = 0; i < bdCastleData->lstStrFullName->Count; i++) {
			String^ strLine = String::Format("{0:000} - {1}", i, bdCastleData->lstStrFullName[i]);
			lbCastleList->Items->Add(strLine);
		}

		// �ŏ��̂��̂�I�����Ă���
		lbCastleList->SelectedIndex = 0;
		iLastCastleListSelected = 0;

		// ��f�[�^���e�L�X�g�{�b�N�X
		Castle_BDataToTextBox();
		Castle_TextBoxWidthUpdate();
	}

	// �p�l���̔w�i�摜�ݒ�
	void Castle_SetPanelBackImg() {
		// ���̃v���W�F�N�g�̃A�Z���u���̃^�C�v���擾�B
		System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
		System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.RetsudenEditorRes", prj_assebmly->GetName()->Name), prj_assebmly);

		// �p�l���̔w�i
		plCastleRetsuden->BackgroundImage = (System::Drawing::Image^)(r->GetObject("retsuden_back"));
		// �w�i�摜�̑傫���Ƀp�l���̑傫�������킹��
		plCastleRetsuden->Size = plCastleRetsuden->BackgroundImage->Size;

	}

	// �e��f�t�h�̃C�x���g�n���h���ݒ�
	void Castle_SetEventHander() {
		btnCastleSearch->Click += gcnew EventHandler(this, &RetsudenEditorForm::Castle_btnSearch_Click);
		// btnCastleImport->Click += gcnew EventHandler(this, &RetsudenEditorForm::Castle_btnImport_Click);
		// btnCastleExport->Click += gcnew EventHandler(this, &RetsudenEditorForm::Castle_btnExport_Click);
		btnCastleAllSave->Click += gcnew EventHandler(this, &RetsudenEditorForm::Castle_btnAllSave_Click);
		tbCastleSearch->KeyDown += gcnew KeyEventHandler(this, &RetsudenEditorForm::Castle_tbSearch_KeyDown);
		lbCastleList->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Castle_lbCastleList_SelectedIndexChanged);
		tbCastleFullName->TextChanged += gcnew EventHandler(this, &RetsudenEditorForm::Castle_tbFullName_TextChanged);
		tbCastleRubyYomi->TextChanged += gcnew EventHandler(this, &RetsudenEditorForm::Castle_tbRubyYomi_TextChanged);
		tbCastleBornEtc->TextChanged += gcnew EventHandler(this, &RetsudenEditorForm::Castle_tbBornEtc_TextChanged);
		tbCastleRetsuden->TextChanged += gcnew EventHandler(this, &RetsudenEditorForm::Castle_tbRetsuden_TextChanged);
		tbCastleFullName->Leave += gcnew EventHandler(this, &RetsudenEditorForm::Castle_textBox_Leave);
		tbCastleRubyYomi->Leave += gcnew EventHandler(this, &RetsudenEditorForm::Castle_textBox_Leave);
		tbCastleBornEtc->Leave += gcnew EventHandler(this, &RetsudenEditorForm::Castle_textBox_Leave);
		tbCastleRetsuden->Leave += gcnew EventHandler(this, &RetsudenEditorForm::Castle_textBox_Leave);
		cbFontCastle->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Castle_cbFont_SelectedIndexChanged);
		// btnCastleAdds->Click += gcnew EventHandler(this, &RetsudenEditorForm::Castle_btnAddCastles_Click);
		// btnCastleDels->Click += gcnew EventHandler(this, &RetsudenEditorForm::Castle_btnDelCastles_Click);
	}

	// �錟���{�^������������
	void Castle_btnSearch_Click(Object^ sender, EventArgs^ e) {
		if (tbCastleSearch->Text == "") { return; } // �e�L�X�g�G���A������ۂȂ牽�����Ȃ��B

		int iStart = GetCastleCBSelectedIndex() + 1; // ���ݑI�����Ă�����̂̂��玟�̌��

		Regex^ regex = gcnew Regex(tbCastleSearch->Text);

		bool isExist = false;
		// iStart�ȍ~��S���T�[�`���āA
		for (int i = iStart; i < lbCastleList->Items->Count; i++) {
			Match^ match = regex->Match((String^)lbCastleList->Items[i]);

			if (match->Success) {
				lbCastleList->SelectedIndex = i;
				isExist = true;
				break;
			}
		}

		if (!isExist) {
			for (int i = 0; i < iStart; i++) {
				Match^ match = regex->Match((String^)lbCastleList->Items[i]);

				if (match->Success) {
					lbCastleList->SelectedIndex = i;
					break;
				}
			}
		}
	}

	// �錟���e�L�X�g�{�b�N�X�ŃL�[����������
	void Castle_tbSearch_KeyDown(Object^ sender, KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Return) {
			Castle_btnSearch_Click(sender, e);
		}
		if (e->KeyCode == Keys::F3) {
			Castle_btnSearch_Click(sender, e);
		}
	}

	// ��o�̓{�^������������
	void Castle_btnExport_Click(Object^ sender, EventArgs^ e) {
	}


	// ����̓{�^������������
	void Castle_btnImport_Click(Object^ sender, EventArgs^ e) {
	}

	// �S�ĕۑ��{�^������������
	void Castle_btnAllSave_Click(Object^ sender, EventArgs^ e) {
		bdCastleData->AllSaveToMessageN6P();
	}

	// �t�H���g���X�g�̑I�����ڂ��ύX���ꂽ��
	void Castle_cbFont_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
		// �t�H���g�̐ݒ�
		fontCastle = gcnew System::Drawing::Font((String^)cbFontCastle->Items[cbFontCastle->SelectedIndex], 12, FontStyle::Regular);
		tbCastleFullName->Font = fontCastle;
		tbCastleRubyYomi->Font = fontCastle;
		tbCastleBornEtc->Font = fontCastle;
		tbCastleRetsuden->Font = fontCastle;
	}

	// �C���X�g�[������Ă���t�H���g�ɂ��������āA�t�H���g���X�g�{�b�N�X�ɒǉ�
	void Castle_GetValidTenshoukiFontList() {

		//InstalledFontCollection�I�u�W�F�N�g�̎擾
		System::Drawing::Text::InstalledFontCollection^ ifc = gcnew System::Drawing::Text::InstalledFontCollection();

		//�C���X�g�[������Ă��邷�ׂẴt�H���g�t�@�~���A���擾
		cli::array<FontFamily^>^ ffs = ifc->Families;

		//�����ł͌��ƂȂ肦��t�H���g���̂�
		for (int i = 0; i < aryStrFontCandidate->Length; i++) {

			// �t�H���g���X�g
			for each (FontFamily ^ ff in ffs) {

				// ���t�H���g���������B
				if (ff->Name == aryStrFontCandidate[i]) {
					// �R���{�{�b�N�X�ɒǉ�
					cbFontCastle->Items->Add(ff->Name);
				}
			}
		}

		// �P�ȏ�t�H���g����������A�ŏ��̂��̂�I�����Ă����B
		if (cbFontCastle->Items->Count > 0) {
			cbFontCastle->SelectedIndex = 0;
		}
	}

	int GetCastleCBSelectedIndex() {
		int iSelectedIndex = lbCastleList->SelectedIndex;
		if (0 <= iSelectedIndex && iSelectedIndex < lbCastleList->Items->Count) {
			return iSelectedIndex;
		}
		else {
			return 0;
		}
	}
	void ModifyCBCastleSelectedIndex() {
		// �œ_���������Ă���悤�Ȃ�A�Ō�ɑI�����Ă������̂��������B
		int iSelectIndex = GetCastleCBSelectedIndex();
		if (iSelectIndex == 0xFFFFFFFF) {
			lbCastleList->SelectedIndex = iLastCastleListSelected;
		}
		else {
			iLastCastleListSelected = iSelectIndex;
		}
	}


	// �郊�X�g��I��ύX����ƁA�摜�̏�̗�`�e��̒l������ւ��
	void Castle_lbCastleList_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
		ModifyCBCastleSelectedIndex();
		// �V����index�̃f�[�^���擾�B
		Castle_BDataToTextBox();

		ModifyCBCastleSelectedIndex();
	}


	// ��f�[�^���e�L�X�g�{�b�N�X�֓]��
	void Castle_BDataToTextBox() {
		int iSelectedIndex = GetCastleCBSelectedIndex();
		tbCastleFullName->Text = bdCastleData->lstStrFullName[iSelectedIndex];
		tbCastleRubyYomi->Text = bdCastleData->lstStrRubyYomi[iSelectedIndex];
		tbCastleBornEtc->Text = bdCastleData->lstStrBornEtc[iSelectedIndex];
		tbCastleRetsuden->Text = bdCastleData->lstStrRetsuden[iSelectedIndex];
	}

	// �e�L�X�g�{�b�N�X�̈ʒu�╝�𒆐g�̃e�L�X�g�ʂɏ]���ĕύX
	void Castle_TextBoxWidthUpdate() {
		tbCastleFullName->Width = GetStringMeasure(tbCastleFullName->Text);

		tbCastleRubyYomi->Left = tbCastleFullName->Right + 8;
		tbCastleRubyYomi->Width = GetStringMeasure(tbCastleRubyYomi->Text);

		tbCastleBornEtc->Left = tbCastleRubyYomi->Right + 8;
		tbCastleBornEtc->Width = GetStringMeasure(tbCastleBornEtc->Text);
	}

	// �e�L�X�g�{�b�N�X�́A�u�鐩���v���ύX���ꂽ�B
	void Castle_tbFullName_TextChanged(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetCastleCBSelectedIndex();
		bdCastleData->lstStrFullName[iSelectedIndex] = tbCastleFullName->Text;
		String^ strLine = String::Format("{0:000} - {1}", iSelectedIndex, tbCastleFullName->Text);
		// SelectIndexChange���������Ȃ��悤�ɂ���B
		lbCastleList->SelectedIndexChanged -= gcnew EventHandler(this, &RetsudenEditorForm::Castle_lbCastleList_SelectedIndexChanged);

		// �������Ȃ��悤�ɂ����̂ŁA�l������������BSelectedIndexChange���\�b�h���������Ȃ��̂ŃG���[���N���Ȃ��B

		lbCastleList->Items[iSelectedIndex] = gcnew String(strLine); // �郊�X�g�̖��O�̕����X�V

		Castle_TextBoxWidthUpdate();

		// �X�V�����̂ŁA������߂��B
		// SelectIndexChange����������悤�ɂ���B
		lbCastleList->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Castle_lbCastleList_SelectedIndexChanged);
	}

	// �e�L�X�g�{�b�N�X�́A�u�ӂ肪�ȁv���ύX���ꂽ�B
	void Castle_tbRubyYomi_TextChanged(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetCastleCBSelectedIndex();
		bdCastleData->lstStrRubyYomi[iSelectedIndex] = tbCastleRubyYomi->Text;
		Castle_TextBoxWidthUpdate();
	}

	// �e�L�X�g�{�b�N�X�́A�u���N���v���ύX���ꂽ�B
	void Castle_tbBornEtc_TextChanged(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetCastleCBSelectedIndex();
		bdCastleData->lstStrBornEtc[iSelectedIndex] = tbCastleBornEtc->Text;
		Castle_TextBoxWidthUpdate();
	}

	// �e�L�X�g�{�b�N�X�́A�u��`�v���ύX���ꂽ�B
	void Castle_tbRetsuden_TextChanged(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetCastleCBSelectedIndex();

		bdCastleData->lstStrRetsuden[iSelectedIndex] = tbCastleRetsuden->Text;
	}

	// �e�L�X�g�{�b�N�X���狎����
	void Castle_textBox_Leave(Object^ sender, EventArgs^ e) {
	}


	// 50�g�ǉ��{�^��
	void Castle_btnAddCastles_Click(Object^ sender, EventArgs^ e) {
	}

	// 50�g�폜�{�^��
	void Castle_btnDelCastles_Click(Object^ sender, EventArgs^ e) {
	}


	//-----------------------------------------------���ʗ�`�n
private:
	TabPage^ tpKanni;
	KanniData^ bdKanniData;

	TextBox^ tbKanniSearch; // ���ʌ���
	Button^ btnKanniSearch; // �����{�^��
	Button^ btnKanniImport; // �捞�{�^��
	Button^ btnKanniExport; // �o�̓{�^��
	Button^ btnKanniAllSave; // �S�ĕۑ�

	ListBox^ lbKanniList; // ���ʃ��X�g�̃R���{�{�b�N�X
	int iLastKanniListSelected;

	Panel^ plKanniRetsuden;	 // ��`���p�̃p�l��
	TextBox^ tbKanniFullName; // ���ʐ����̃e�L�X�g�{�b�N�X
	TextBox^ tbKanniRubyYomi; // �ӂ肪�Ȃ̂��
	TextBox^ tbKanniBornEtc;  // ���N�Ȃǂ̏��
	TextBox^ tbKanniRetsuden; // ��`���


	// �t�H���g
	System::Drawing::Font^ fontKanni;
	ComboBox^ cbFontKanni;	// �t�H���g���X�g

	Button^ btnKanniAdds; // 50�g�ǉ�
	Button^ btnKanniDels; // 50�g�ǉ�

private:

	void tpKanni_Init() {

		tpKanni = gcnew TabPage();

		tpKanni->Text = "���ʗ�`";
		tpKanni->Size = tcRE->ClientSize;

		// ���ʌ���
		tbKanniSearch = gcnew TextBox();
		tbKanniSearch->Width = 140;
		tbKanniSearch->Height = 16;
		tbKanniSearch->Left = 10;
		tbKanniSearch->Top = 10;

		// �����{�^��
		btnKanniSearch = gcnew Button();
		btnKanniSearch->Text = "����";
		btnKanniSearch->Width = 60;
		btnKanniSearch->Height = tbKanniSearch->Height;
		btnKanniSearch->Left = tbKanniSearch->Right + 1;
		btnKanniSearch->Top = tbKanniSearch->Top;

		/*
		// �捞�{�^��
		btnKanniImport = gcnew Button();
		btnKanniImport->Text = "�P�Ǝ捞";
		btnKanniImport->Width = 70;
		btnKanniImport->Height = tbKanniSearch->Height;
		btnKanniImport->Left = btnKanniSearch->Right + 30;
		btnKanniImport->Top = tbKanniSearch->Top;

		// �o�̓{�^��
		btnKanniExport = gcnew Button();
		btnKanniExport->Text = "�P�Əo��";
		btnKanniExport->Width = 70;
		btnKanniExport->Height = tbKanniSearch->Height;
		btnKanniExport->Left = btnKanniImport->Right;
		btnKanniExport->Top = tbKanniSearch->Top;
		*/

		// �S�ĕۑ�
		btnKanniAllSave = gcnew Button();
		btnKanniAllSave->Text = "���ʗ�`��ۑ�";
		btnKanniAllSave->Width = 120;
		btnKanniAllSave->Height = tbKanniSearch->Height;
		btnKanniAllSave->Left = tpKanni->Right - (btnKanniAllSave->Width + 20);
		btnKanniAllSave->Top = tbKanniSearch->Top;

		// ���ʒm���
		lbKanniList = gcnew ListBox();
		lbKanniList->Width = tbKanniSearch->Width;
		lbKanniList->Left = tbKanniSearch->Left;
		lbKanniList->Top = tbKanniSearch->Bottom + 10;
		lbKanniList->Height = 200;

		// ��`���p�̃p�l��
		plKanniRetsuden = gcnew Panel();
		plKanniRetsuden->Top = lbKanniList->Top + 20;
		Kanni_SetPanelBackImg();
		plKanniRetsuden->Left = tpKanni->Right - (plKanniRetsuden->Width + 20);

		tbKanniFullName = gcnew TextBox();
		tbKanniFullName->Top = 10;
		tbKanniFullName->Left = 10;
		tbKanniFullName->Width = 16 * 6 + 6;
		tbKanniFullName->MaxLength = 37;
		tbKanniFullName->BackColor = Color::FromArgb(0xD0, 0xD0, 0xFA);

		tbKanniRubyYomi = gcnew TextBox();
		tbKanniRubyYomi->Top = tbKanniFullName->Top;
		tbKanniRubyYomi->Left = tbKanniFullName->Right;
		tbKanniRubyYomi->MaxLength = 37;
		tbKanniRubyYomi->Width = 16 * 5 + 6;
		tbKanniRubyYomi->BackColor = Color::FromArgb(0xD0, 0xD0, 0xFA);

		tbKanniBornEtc = gcnew TextBox();
		tbKanniBornEtc->Top = tbKanniFullName->Top;
		tbKanniBornEtc->Left = tbKanniRubyYomi->Right;
		tbKanniBornEtc->Width = 16 * 5 + 8;
		tbKanniBornEtc->MaxLength = 37;
		tbKanniBornEtc->BackColor = Color::FromArgb(0xD0, 0xD0, 0xFA);

		tbKanniRetsuden = gcnew TextBox();
		tbKanniRetsuden->Top = tbKanniFullName->Bottom + 12;
		tbKanniRetsuden->Left = tbKanniFullName->Left;
		tbKanniRetsuden->Height = 90;
		tbKanniRetsuden->Width = 312;
		tbKanniRetsuden->MaxLength = 2 * 18 * 4;
		tbKanniRetsuden->Multiline = true;
		tbKanniRetsuden->WordWrap = false;
		tbKanniRetsuden->BackColor = Color::FromArgb(0xD0, 0xD0, 0xFA);


		plKanniRetsuden->Controls->Add(tbKanniFullName);
		plKanniRetsuden->Controls->Add(tbKanniRubyYomi);
		// plKanniRetsuden->Controls->Add(tbKanniBornEtc);
		plKanniRetsuden->Controls->Add(tbKanniRetsuden);

		// �t�H���g���X�g
		cbFontKanni = gcnew ComboBox();
		cbFontKanni->Width = 100;
		cbFontKanni->Left = tpKanni->Right - (cbFontKanni->Width + 20);
		cbFontKanni->Top = plKanniRetsuden->Bottom + 10;
		cbFontKanni->Height = tbKanniSearch->Height;
		Kanni_GetValidTenshoukiFontList();

		Kanni_cbFont_SelectedIndexChanged(nullptr, nullptr);

		// ���ʘg50�l�ǉ�
		btnKanniAdds = gcnew Button();
		btnKanniAdds->Text = "50�g�ǉ�";
		btnKanniAdds->Width = 70;
		btnKanniAdds->Height = tbKanniSearch->Height;
		btnKanniAdds->Left = tbKanniSearch->Left;
		btnKanniAdds->Top = lbKanniList->Bottom + 10;

		// ���ʘg50�l�폜
		btnKanniDels = gcnew Button();
		btnKanniDels->Text = "50�g�폜";
		btnKanniDels->Width = 70;
		btnKanniDels->Height = tbKanniSearch->Height;
		btnKanniDels->Left = btnKanniAdds->Right;
		btnKanniDels->Top = lbKanniList->Bottom + 10;


		// ���ʗ�`�̃^�u�ɃR���|�[�l���g�ǉ�
		tpKanni->Controls->Add(tbKanniSearch);
		tpKanni->Controls->Add(btnKanniSearch);
		// tpKanni->Controls->Add(btnKanniImport);
		// tpKanni->Controls->Add(btnKanniExport);
		tpKanni->Controls->Add(btnKanniAllSave);
		tpKanni->Controls->Add(lbKanniList);
		tpKanni->Controls->Add(plKanniRetsuden);
		tpKanni->Controls->Add(cbFontKanni);
		// tpKanni->Controls->Add(btnKanniAdds); // ���ʗp�ɂ͂��̃{�^���͕s�v
		// tpKanni->Controls->Add(btnKanniDels); // ���ʗp�ɂ͂��̃{�^���͕s�v

		// �^�u���t�H�[���ɒǉ�
		tcRE->TabPages->Add(tpKanni);

		// ���ʃf�[�^���쐬���āA���X�g�ɗ�������
		bdKanniData = gcnew KanniData();
		// ���X�g�{�b�N�X�ɑ�������
		for (int i = 0; i < bdKanniData->lstStrFullName->Count; i++) {
			String^ strLine = String::Format("{0:000} - {1}", i, bdKanniData->lstStrFullName[i]);
			lbKanniList->Items->Add(strLine);
		}

		// �ŏ��̂��̂�I�����Ă���
		lbKanniList->SelectedIndex = 0;
		iLastKanniListSelected = 0;

		// ���ʃf�[�^���e�L�X�g�{�b�N�X
		Kanni_BDataToTextBox();
		Kanni_TextBoxWidthUpdate();
	}

	// �p�l���̔w�i�摜�ݒ�
	void Kanni_SetPanelBackImg() {
		// ���̃v���W�F�N�g�̃A�Z���u���̃^�C�v���擾�B
		System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
		System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.RetsudenEditorRes", prj_assebmly->GetName()->Name), prj_assebmly);

		// �p�l���̔w�i
		plKanniRetsuden->BackgroundImage = (System::Drawing::Image^)(r->GetObject("retsuden_back"));
		// �w�i�摜�̑傫���Ƀp�l���̑傫�������킹��
		plKanniRetsuden->Size = plKanniRetsuden->BackgroundImage->Size;

	}

	// �e��f�t�h�̃C�x���g�n���h���ݒ�
	void Kanni_SetEventHander() {
		btnKanniSearch->Click += gcnew EventHandler(this, &RetsudenEditorForm::Kanni_btnSearch_Click);
		// btnKanniImport->Click += gcnew EventHandler(this, &RetsudenEditorForm::Kanni_btnImport_Click);
		// btnKanniExport->Click += gcnew EventHandler(this, &RetsudenEditorForm::Kanni_btnExport_Click);
		btnKanniAllSave->Click += gcnew EventHandler(this, &RetsudenEditorForm::Kanni_btnAllSave_Click);
		tbKanniSearch->KeyDown += gcnew KeyEventHandler(this, &RetsudenEditorForm::Kanni_tbSearch_KeyDown);
		lbKanniList->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Kanni_lbKanniList_SelectedIndexChanged);
		tbKanniFullName->TextChanged += gcnew EventHandler(this, &RetsudenEditorForm::Kanni_tbFullName_TextChanged);
		tbKanniRubyYomi->TextChanged += gcnew EventHandler(this, &RetsudenEditorForm::Kanni_tbRubyYomi_TextChanged);
		tbKanniBornEtc->TextChanged += gcnew EventHandler(this, &RetsudenEditorForm::Kanni_tbBornEtc_TextChanged);
		tbKanniRetsuden->TextChanged += gcnew EventHandler(this, &RetsudenEditorForm::Kanni_tbRetsuden_TextChanged);
		tbKanniFullName->Leave += gcnew EventHandler(this, &RetsudenEditorForm::Kanni_textBox_Leave);
		tbKanniRubyYomi->Leave += gcnew EventHandler(this, &RetsudenEditorForm::Kanni_textBox_Leave);
		tbKanniBornEtc->Leave += gcnew EventHandler(this, &RetsudenEditorForm::Kanni_textBox_Leave);
		tbKanniRetsuden->Leave += gcnew EventHandler(this, &RetsudenEditorForm::Kanni_textBox_Leave);
		cbFontKanni->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Kanni_cbFont_SelectedIndexChanged);
		// btnKanniAdds->Click += gcnew EventHandler(this, &RetsudenEditorForm::Kanni_btnAddKannis_Click);
		// btnKanniDels->Click += gcnew EventHandler(this, &RetsudenEditorForm::Kanni_btnDelKannis_Click);
	}

	// ���ʌ����{�^������������
	void Kanni_btnSearch_Click(Object^ sender, EventArgs^ e) {
		if (tbKanniSearch->Text == "") { return; } // �e�L�X�g�G���A������ۂȂ牽�����Ȃ��B

		int iStart = GetKanniCBSelectedIndex() + 1; // ���ݑI�����Ă�����̂̂��玟�̌��

		Regex^ regex = gcnew Regex(tbKanniSearch->Text);

		bool isExist = false;
		// iStart�ȍ~��S���T�[�`���āA
		for (int i = iStart; i < lbKanniList->Items->Count; i++) {
			Match^ match = regex->Match((String^)lbKanniList->Items[i]);

			if (match->Success) {
				lbKanniList->SelectedIndex = i;
				isExist = true;
				break;
			}
		}

		if (!isExist) {
			for (int i = 0; i < iStart; i++) {
				Match^ match = regex->Match((String^)lbKanniList->Items[i]);

				if (match->Success) {
					lbKanniList->SelectedIndex = i;
					break;
				}
			}
		}
	}

	// ���ʌ����e�L�X�g�{�b�N�X�ŃL�[����������
	void Kanni_tbSearch_KeyDown(Object^ sender, KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Return) {
			Kanni_btnSearch_Click(sender, e);
		}
		if (e->KeyCode == Keys::F3) {
			Kanni_btnSearch_Click(sender, e);
		}
	}

	// ���ʏo�̓{�^������������
	void Kanni_btnExport_Click(Object^ sender, EventArgs^ e) {
	}


	// ���ʓ��̓{�^������������
	void Kanni_btnImport_Click(Object^ sender, EventArgs^ e) {
	}

	// �S�ĕۑ��{�^������������
	void Kanni_btnAllSave_Click(Object^ sender, EventArgs^ e) {
		bdKanniData->AllSaveToMessageN6P();
	}

	// �t�H���g���X�g�̑I�����ڂ��ύX���ꂽ��
	void Kanni_cbFont_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
		// �t�H���g�̐ݒ�
		fontKanni = gcnew System::Drawing::Font((String^)cbFontKanni->Items[cbFontKanni->SelectedIndex], 12, FontStyle::Regular);
		tbKanniFullName->Font = fontKanni;
		tbKanniRubyYomi->Font = fontKanni;
		tbKanniBornEtc->Font = fontKanni;
		tbKanniRetsuden->Font = fontKanni;
	}

	// �C���X�g�[������Ă���t�H���g�ɂ��������āA�t�H���g���X�g�{�b�N�X�ɒǉ�
	void Kanni_GetValidTenshoukiFontList() {

		//InstalledFontCollection�I�u�W�F�N�g�̎擾
		System::Drawing::Text::InstalledFontCollection^ ifc = gcnew System::Drawing::Text::InstalledFontCollection();

		//�C���X�g�[������Ă��邷�ׂẴt�H���g�t�@�~���A���擾
		cli::array<FontFamily^>^ ffs = ifc->Families;

		//�����ł͌��ƂȂ肦��t�H���g���̂�
		for (int i = 0; i < aryStrFontCandidate->Length; i++) {

			// �t�H���g���X�g
			for each (FontFamily ^ ff in ffs) {

				// ���t�H���g���������B
				if (ff->Name == aryStrFontCandidate[i]) {
					// �R���{�{�b�N�X�ɒǉ�
					cbFontKanni->Items->Add(ff->Name);
				}
			}
		}

		// �P�ȏ�t�H���g����������A�ŏ��̂��̂�I�����Ă����B
		if (cbFontKanni->Items->Count > 0) {
			cbFontKanni->SelectedIndex = 0;
		}
	}

	int GetKanniCBSelectedIndex() {
		int iSelectedIndex = lbKanniList->SelectedIndex;
		if (0 <= iSelectedIndex && iSelectedIndex < lbKanniList->Items->Count) {
			return iSelectedIndex;
		}
		else {
			return 0;
		}
	}
	void ModifyCBKanniSelectedIndex() {
		// �œ_���������Ă���悤�Ȃ�A�Ō�ɑI�����Ă������̂��������B
		int iSelectIndex = GetKanniCBSelectedIndex();
		if (iSelectIndex == 0xFFFFFFFF) {
			lbKanniList->SelectedIndex = iLastKanniListSelected;
		}
		else {
			iLastKanniListSelected = iSelectIndex;
		}
	}


	// ���ʃ��X�g��I��ύX����ƁA�摜�̏�̗�`�e��̒l������ւ��
	void Kanni_lbKanniList_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
		ModifyCBKanniSelectedIndex();
		// �V����index�̃f�[�^���擾�B
		Kanni_BDataToTextBox();

		ModifyCBKanniSelectedIndex();
	}


	// ���ʃf�[�^���e�L�X�g�{�b�N�X�֓]��
	void Kanni_BDataToTextBox() {
		int iSelectedIndex = GetKanniCBSelectedIndex();
		tbKanniFullName->Text = bdKanniData->lstStrFullName[iSelectedIndex];
		tbKanniRubyYomi->Text = bdKanniData->lstStrRubyYomi[iSelectedIndex];
		tbKanniBornEtc->Text = bdKanniData->lstStrBornEtc[iSelectedIndex];
		tbKanniRetsuden->Text = bdKanniData->lstStrRetsuden[iSelectedIndex];
	}

	// �e�L�X�g�{�b�N�X�̈ʒu�╝�𒆐g�̃e�L�X�g�ʂɏ]���ĕύX
	void Kanni_TextBoxWidthUpdate() {
		tbKanniFullName->Width = GetStringMeasure(tbKanniFullName->Text);

		tbKanniRubyYomi->Left = tbKanniFullName->Right + 8;
		tbKanniRubyYomi->Width = GetStringMeasure(tbKanniRubyYomi->Text);

		tbKanniBornEtc->Left = tbKanniRubyYomi->Right + 8;
		tbKanniBornEtc->Width = GetStringMeasure(tbKanniBornEtc->Text);
	}

	// �e�L�X�g�{�b�N�X�́A�u���ʐ����v���ύX���ꂽ�B
	void Kanni_tbFullName_TextChanged(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetKanniCBSelectedIndex();
		bdKanniData->lstStrFullName[iSelectedIndex] = tbKanniFullName->Text;
		String^ strLine = String::Format("{0:000} - {1}", iSelectedIndex, tbKanniFullName->Text);
		// SelectIndexChange���������Ȃ��悤�ɂ���B
		lbKanniList->SelectedIndexChanged -= gcnew EventHandler(this, &RetsudenEditorForm::Kanni_lbKanniList_SelectedIndexChanged);

		// �������Ȃ��悤�ɂ����̂ŁA�l������������BSelectedIndexChange���\�b�h���������Ȃ��̂ŃG���[���N���Ȃ��B

		lbKanniList->Items[iSelectedIndex] = gcnew String(strLine); // ���ʃ��X�g�̖��O�̕����X�V

		Kanni_TextBoxWidthUpdate();

		// �X�V�����̂ŁA������߂��B
		// SelectIndexChange����������悤�ɂ���B
		lbKanniList->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Kanni_lbKanniList_SelectedIndexChanged);
	}

	// �e�L�X�g�{�b�N�X�́A�u�ӂ肪�ȁv���ύX���ꂽ�B
	void Kanni_tbRubyYomi_TextChanged(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetKanniCBSelectedIndex();
		bdKanniData->lstStrRubyYomi[iSelectedIndex] = tbKanniRubyYomi->Text;
		Kanni_TextBoxWidthUpdate();
	}

	// �e�L�X�g�{�b�N�X�́A�u���N���v���ύX���ꂽ�B
	void Kanni_tbBornEtc_TextChanged(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetKanniCBSelectedIndex();
		bdKanniData->lstStrBornEtc[iSelectedIndex] = tbKanniBornEtc->Text;
		Kanni_TextBoxWidthUpdate();
	}

	// �e�L�X�g�{�b�N�X�́A�u��`�v���ύX���ꂽ�B
	void Kanni_tbRetsuden_TextChanged(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetKanniCBSelectedIndex();

		bdKanniData->lstStrRetsuden[iSelectedIndex] = tbKanniRetsuden->Text;
	}

	// �e�L�X�g�{�b�N�X���狎����
	void Kanni_textBox_Leave(Object^ sender, EventArgs^ e) {
	}


	// 50�g�ǉ��{�^��
	void Kanni_btnAddKannis_Click(Object^ sender, EventArgs^ e) {
	}

	// 50�g�폜�{�^��
	void Kanni_btnDelKannis_Click(Object^ sender, EventArgs^ e) {
	}


};

