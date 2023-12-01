#pragma once;

#include "BushouData.h"
#include "HimeData.h"
#include "KahouData.h"
#include "CastleData.h"
#include "KanniData.h"

// RetsudenEditor.cpp : メイン プロジェクト ファイルです。
#include <windows.h>
#include <string>

#include "DotNetTestToMultiByte.h"

using namespace System;
using namespace Microsoft::Win32;
using namespace System::Drawing;
using namespace System::Windows::Forms;

ref class RetsudenEditorForm : public Form {

//-----------------------------------------------フォームウィンドウ系

	TabControl^ tcRE;	// 全体で１つになってるタブコントロール。TabPage型を追加してく

	static System::Drawing::Text::PrivateFontCollection^ pfc;
	static cli::array<String^>^ aryStrFontCandidate = {"将星 明朝", "ＭＳ 明朝", "ＭＳ ゴシック" }; // フォントリスト

public:
	RetsudenEditorForm() {
		// フォームにかかわるものの初期化等
		this->Text = "列伝エディタ";
		this->Width = 530;
		this->Height = 360;

		LoadPrivateFont();

		SetFormIcon();

		SetTabControl();
	}

	void LoadPrivateFont() {
		// システムではなく、プライベートフォントとして、ユーザー専用としてインストールしたフォントを検索する
		pfc = gcnew System::Drawing::Text::PrivateFontCollection();
		RegistryKey^ key = nullptr;

		// 比較対象のフォント。
		try
		{
			key = Registry::CurrentUser->OpenSubKey(R"(Software\Microsoft\Windows NT\CurrentVersion\Fonts)");
			cli::array<String^>^ valueNames = key == nullptr ? nullptr : key->GetValueNames();
			if (valueNames != nullptr)
			{
				String^ targetFontName = "将星 明朝";
				for each (String ^ valueName in valueNames)
				{
					String^ path = key->GetValue(valueName)->ToString();
					if (valueName->Contains(targetFontName)) {
						// フォント名が指定のフォント名を含んでいる場合は、フォントを追加する
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
		// このプロジェクトのアセンブリのタイプを取得。
		System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
		System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.RetsudenEditorRes", prj_assebmly->GetName()->Name), prj_assebmly);

		System::Drawing::Icon^ iconform = (System::Drawing::Icon^)(r->GetObject("icon"));
		this->Icon = iconform;
	}

	// タブコントロールの初期化。間に、各データグリッドビューへのデータ流し込み処理が入り込む
	void SetTabControl() {
		tcRE = gcnew TabControl();
		tcRE->Size = this->ClientSize;
		
		// 武将列伝のタブページ追加
		tpBushou_Init();
		Bushou_SetEventHander();

		// 姫列伝のタブページ追加
		tpHime_Init();
		Hime_SetEventHander();

		// 家宝列伝のタブページ追加
		tpKahou_Init();
		Kahou_SetEventHander();

		// 城列伝のタブページ追加
		tpCastle_Init();
		Castle_SetEventHander();

		// 官位列伝のタブページ追加
		tpKanni_Init();
		Kanni_SetEventHander();

		tcRE->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::tcRE_SelectedIndexChanged);
		this->Controls->Add(tcRE);
	}

	// タブを切り替えた時
	void tcRE_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
	}


	int GetStringMeasure(String^ str) {
		string sstr = DotNetStringToMultiByte(str);
		int iByteNum = strlen(sstr.data());
		return iByteNum * 8 + 8;
	}

//-----------------------------------------------武将列伝系
private:
	TabPage^ tpBushou;
	BushouData^ bdBushouData;

	TextBox^ tbBushouSearch; // 武将検索
	Button^ btnBushouSearch; // 検索ボタン
	Button^ btnBushouImport; // 取込ボタン
	OpenFileDialog^ ofdBushouImport;
	Button^ btnBushouExport; // 出力ボタン
	Button^ btnBushouAllSave; // 全て保存

	ListBox^ lbBushouList; // 武将リストのコンボボックス
	int iLastBushouListSelected;

	Panel^ plBushouRetsuden;	 // 列伝情報用のパネル
	TextBox^ tbBushouFullName; // 武将姓名のテキストボックス
	TextBox^ tbBushouRubyYomi; // ふりがなのるび
	TextBox^ tbBushouBornEtc;  // 生年などの情報
	TextBox^ tbBushouRetsuden; // 列伝情報


	// フォント
	System::Drawing::Font^ fontBushou;
	ComboBox^ cbFontBushou;	// フォントリスト

	Button^ btnBushouAdds; // 50枠追加
	Button^ btnBushouDels; // 50枠追加


private:

	void tpBushou_Init() {

		tpBushou = gcnew TabPage();

		tpBushou->Text = "武将列伝";
		tpBushou->Size = tcRE->ClientSize;

		// 武将検索
		tbBushouSearch = gcnew TextBox();
		tbBushouSearch->Width = 140;
		tbBushouSearch->Height = 16;
		tbBushouSearch->Left = 10;
		tbBushouSearch->Top = 10;

		// 検索ボタン
		btnBushouSearch = gcnew Button();
		btnBushouSearch->Text = "検索";
		btnBushouSearch->Width = 60;
		btnBushouSearch->Height = tbBushouSearch->Height;
		btnBushouSearch->Left = tbBushouSearch->Right + 1;
		btnBushouSearch->Top = tbBushouSearch->Top;

		// 取込ボタン
		btnBushouImport = gcnew Button();
		btnBushouImport->Text = "単独取込";
		btnBushouImport->Width = 70;
		btnBushouImport->Height = tbBushouSearch->Height;
		btnBushouImport->Left = btnBushouSearch->Right + 30;
		btnBushouImport->Top = tbBushouSearch->Top;

		// 出力ボタン
		btnBushouExport = gcnew Button();
		btnBushouExport->Text = "単独出力";
		btnBushouExport->Width = 70;
		btnBushouExport->Height = tbBushouSearch->Height;
		btnBushouExport->Left = btnBushouImport->Right;
		btnBushouExport->Top = tbBushouSearch->Top;

		// 全て保存
		btnBushouAllSave = gcnew Button();
		btnBushouAllSave->Text = "武将列伝を保存";
		btnBushouAllSave->Width = 120;
		btnBushouAllSave->Height = tbBushouSearch->Height;
		btnBushouAllSave->Left = tpBushou->Right - (btnBushouAllSave->Width+20);
		btnBushouAllSave->Top = tbBushouSearch->Top;

		// 武将知ると
		lbBushouList = gcnew ListBox();
		lbBushouList->Width = tbBushouSearch->Width;
		lbBushouList->Left = tbBushouSearch->Left;
		lbBushouList->Top = tbBushouSearch->Bottom + 10;
		lbBushouList->Height = 200;

		// 列伝情報用のパネル
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

		// フォントリスト
		cbFontBushou = gcnew ComboBox();
		cbFontBushou->Width = 100;
		cbFontBushou->Left = tpBushou->Right - (cbFontBushou->Width+20);
		cbFontBushou->Top = plBushouRetsuden->Bottom + 10;
		cbFontBushou->Height = tbBushouSearch->Height;
		Bushou_GetValidTenshoukiFontList();

		Bushou_cbFont_SelectedIndexChanged(nullptr, nullptr);

		/*
		// 武将枠50人追加
		btnBushouAdds = gcnew Button();
		btnBushouAdds->Text = "50枠追加";
		btnBushouAdds->Width = 70;
		btnBushouAdds->Height = tbBushouSearch->Height;
		btnBushouAdds->Left = tbBushouSearch->Left;
		btnBushouAdds->Top = lbBushouList->Bottom + 10;

		// 武将枠50人削除
		btnBushouDels = gcnew Button();
		btnBushouDels->Text = "50枠削除";
		btnBushouDels->Width = 70;
		btnBushouDels->Height = tbBushouSearch->Height;
		btnBushouDels->Left = btnBushouAdds->Right;
		btnBushouDels->Top = lbBushouList->Bottom + 10;
		*/

		// 武将列伝のタブにコンポーネント追加
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
		
		// タブをフォームに追加
		tcRE->TabPages->Add(tpBushou);

		// 武将データを作成して、リストに流しこむ
		bdBushouData = gcnew BushouData();
		// リストボックスに足し込み
		for ( int i=0; i<bdBushouData->lstStrFullName->Count; i++ ) {
			String^ strLine = String::Format( "{0:0000} - {1}", i, bdBushouData->lstStrFullName[i]);
			lbBushouList->Items->Add( strLine );
		}

		// 最初のものを選択しておく
		lbBushouList->SelectedIndex = 0;
		iLastBushouListSelected = 0;

		// 武将データ→テキストボックス
		Bushou_BDataToTextBox();
		Bushou_TextBoxWidthUpdate();
	}

	// パネルの背景画像設定
	void Bushou_SetPanelBackImg() {
		// このプロジェクトのアセンブリのタイプを取得。
		System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
		System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.RetsudenEditorRes", prj_assebmly->GetName()->Name), prj_assebmly);

		// パネルの背景
		plBushouRetsuden->BackgroundImage = (System::Drawing::Image^)(r->GetObject("retsuden_back"));
		// 背景画像の大きさにパネルの大きさを合わせる
		plBushouRetsuden->Size = plBushouRetsuden->BackgroundImage->Size;

	}

	// 各種ＧＵＩのイベントハンドラ設定
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

	// 武将検索ボタンを押した時
	void Bushou_btnSearch_Click(Object^ sender, EventArgs^ e) {
		if ( tbBushouSearch->Text == "" ) { return; } // テキストエリアが空っぽなら何もしない。

		int iStart = GetBushouCBSelectedIndex() + 1; // 現在選択しているもののから次の候補

		Regex^ regex = gcnew Regex(tbBushouSearch->Text);

		bool isExist = false;
		// iStart以降を全部サーチして、
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

	// 武将検索テキストボックスでキーを押した時
	void Bushou_tbSearch_KeyDown(Object^ sender, KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Return ) {
			Bushou_btnSearch_Click(sender, e);
		}
		if (e->KeyCode == Keys::F3 ) {
			Bushou_btnSearch_Click(sender, e);
		}
	}

	// 武将出力ボタンを押した時
	void Bushou_btnExport_Click(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetBushouCBSelectedIndex();
		std::string unmanagedData = bdBushouData->GetOneBushouExportData(iSelectedIndex);

		std::string filepath = DotNetStringToMultiByte(bdBushouData->lstStrFullName[iSelectedIndex]);
		if ( filepath == "" ) { filepath = "ななし"; }
		filepath += ".n6b";

		FILE *fp = fopen(filepath.c_str(), "wb");
		if ( fp ) {
			fwrite( unmanagedData.c_str(), unmanagedData.size(), 1, fp);
			fclose(fp);
			System::Windows::Forms::MessageBox::Show("データを出力しました。", "完了", ::MessageBoxButtons::OK, ::MessageBoxIcon::Information);
		} else {
			System::Windows::Forms::MessageBox::Show("データの出力に失敗しました。", "エラー", MessageBoxButtons::OK, ::MessageBoxIcon::Error);

		}
	}


	// 武将入力ボタンを押した時
	void Bushou_btnImport_Click(Object^ sender, EventArgs^ e) {

		ofdBushouImport = gcnew OpenFileDialog();

		ofdBushouImport->InitialDirectory = "."; // ツールと同じディレクトリがデフォルト

		ofdBushouImport->Filter = "将星録 メッセージファイル(*.nb7)|*.nb7";

		ofdBushouImport->FilterIndex = 1;

		ofdBushouImport->Title = "とある武将の列伝ファイルを選択してください";

		//ダイアログボックスを閉じる前に現在のディレクトリを復元するようにする
		ofdBushouImport->RestoreDirectory = true;

		//ダイアログを表示する
		if (ofdBushouImport->ShowDialog() == ::DialogResult::OK) {
			//OKボタンがクリックされたとき
			if ( ofdBushouImport->FileName ) {
				BOOL success = bdBushouData->GetOneBushouImportData(ofdBushouImport->FileName, GetBushouCBSelectedIndex());

				//　成否にかかわらず更新
				Bushou_BDataToTextBox();
			}
		}
	}

	// 全て保存ボタンを押した時
	void Bushou_btnAllSave_Click(Object^ sender, EventArgs^ e) {
		bdBushouData->AllSaveToBFileN6P();
	}

	// フォントリストの選択項目が変更された時
	void Bushou_cbFont_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
		// フォントの設定
		fontBushou = gcnew System::Drawing::Font((String^)cbFontBushou->Items[cbFontBushou->SelectedIndex], 12, FontStyle::Regular);
		tbBushouFullName->Font = fontBushou;
		tbBushouRubyYomi->Font = fontBushou;
		tbBushouBornEtc->Font  = fontBushou;
		tbBushouRetsuden->Font = fontBushou;
	}

	// インストールされているフォントにしたがって、フォントリストボックスに追加
	void Bushou_GetValidTenshoukiFontList() {

		//InstalledFontCollectionオブジェクトの取得
		System::Drawing::Text::InstalledFontCollection^ ifc = gcnew System::Drawing::Text::InstalledFontCollection();

		//インストールされているすべてのフォントファミリアを取得
		cli::array<FontFamily^>^ ffs = ifc->Families;

		//ここでは候補となりえるフォント名のみ
		for ( int i=0; i<aryStrFontCandidate->Length; i++) {

			// フォントリスト
			for each (FontFamily^ ff in ffs) {

				// 候補フォントがあった。
				if (ff->Name == aryStrFontCandidate[i]) {
					// コンボボックスに追加
					cbFontBushou->Items->Add( ff->Name );
				}
			}
		}

		// １つ以上フォントがあったら、最初のものを選択しておく。
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
		// 焦点を見失っているようなら、最後に選択していたものを宛がう。
		int iSelectIndex = GetBushouCBSelectedIndex();
		if ( iSelectIndex == 0xFFFFFFFF ) {
			lbBushouList->SelectedIndex = iLastBushouListSelected;
		} else {
			iLastBushouListSelected = iSelectIndex;
		}
	}


	// 武将リストを選択変更すると、画像の上の列伝各種の値が入れ替わる
	void Bushou_lbBushouList_SelectedIndexChanged(Object^ sender, EventArgs^ e ) {
		ModifyCBBushouSelectedIndex();
		// 新たなindexのデータを取得。
		Bushou_BDataToTextBox();

		ModifyCBBushouSelectedIndex();
	}


	// 武将データ→テキストボックスへ転写
	void Bushou_BDataToTextBox() {
		int iSelectedIndex = GetBushouCBSelectedIndex();
		tbBushouFullName->Text = bdBushouData->lstStrFullName[iSelectedIndex];
		tbBushouRubyYomi->Text = bdBushouData->lstStrRubyYomi[iSelectedIndex];
		tbBushouBornEtc->Text  = bdBushouData->lstStrBornEtc[iSelectedIndex];
		tbBushouRetsuden->Text = bdBushouData->lstStrRetsuden[iSelectedIndex];
	}

	// テキストボックスの位置や幅を中身のテキスト量に従って変更
	void Bushou_TextBoxWidthUpdate() {
		tbBushouFullName->Width = GetStringMeasure(tbBushouFullName->Text);

		tbBushouRubyYomi->Left = tbBushouFullName->Right + 8;
		tbBushouRubyYomi->Width = GetStringMeasure(tbBushouRubyYomi->Text);
		
		tbBushouBornEtc->Left = tbBushouRubyYomi->Right + 8;
		tbBushouBornEtc->Width = GetStringMeasure(tbBushouBornEtc->Text);
	}

	// テキストボックスの、「武将姓名」が変更された。
	void Bushou_tbFullName_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetBushouCBSelectedIndex();
		bdBushouData->lstStrFullName[iSelectedIndex] = tbBushouFullName->Text;
		String^ strLine = String::Format( "{0:0000} - {1}", iSelectedIndex, tbBushouFullName->Text);
		// SelectIndexChangeが反応しないようにする。
		lbBushouList->SelectedIndexChanged -= gcnew EventHandler(this, &RetsudenEditorForm::Bushou_lbBushouList_SelectedIndexChanged);

		// 反応しないようにしたので、値を書き換える。SelectedIndexChangeメソッドが反応しないのでエラーが起きない。

		lbBushouList->Items[iSelectedIndex] = gcnew String(strLine); // 武将リストの名前の方を更新

		Bushou_TextBoxWidthUpdate();

		// 更新したので、反応を戻す。
		// SelectIndexChangeが反応するようにする。
		lbBushouList->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Bushou_lbBushouList_SelectedIndexChanged);
	}

	// テキストボックスの、「ふりがな」が変更された。
	void Bushou_tbRubyYomi_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetBushouCBSelectedIndex();
		bdBushouData->lstStrRubyYomi[iSelectedIndex] = tbBushouRubyYomi->Text;
		Bushou_TextBoxWidthUpdate();
	}

	// テキストボックスの、「生年等」が変更された。
	void Bushou_tbBornEtc_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetBushouCBSelectedIndex();
		bdBushouData->lstStrBornEtc[iSelectedIndex]  = tbBushouBornEtc->Text;
		Bushou_TextBoxWidthUpdate();
	}

	// テキストボックスの、「列伝」が変更された。
	void Bushou_tbRetsuden_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetBushouCBSelectedIndex();

		bdBushouData->lstStrRetsuden[iSelectedIndex] = tbBushouRetsuden->Text;
	}

	// テキストボックスから去った
	void Bushou_textBox_Leave(Object^ sender, EventArgs^ e ) {
	}


	// 50枠追加ボタン
	void Bushou_btnAddBushous_Click(Object^ sender, EventArgs^ e) {
		int iStart = lbBushouList->Items->Count;
		for ( int i = iStart; i < iStart+50; i++ ) {
			String^ bname = String::Format("登録{0:0000}", i);
			// 先に元データから増やす
			bdBushouData->lstStrFullName->Add( bname );
			bdBushouData->lstStrRubyYomi->Add( "ﾄｳﾛｸ"+i );
			bdBushouData->lstStrBornEtc->Add( "????～????" );
			bdBushouData->lstStrRetsuden->Add( "未入力" );
			String^ strLine = String::Format( "{0:0000} - {1}", i, bname);
			lbBushouList->Items->Add( strLine );
		}

	}

	// 50枠削除ボタン
	void Bushou_btnDelBushous_Click(Object^ sender, EventArgs^ e) {
		
		int iStart = lbBushouList->Items->Count-1;

		// 選択インデックスが削除予定の所にあったら、ギリギリのところまで退避
		if ( GetBushouCBSelectedIndex() > iStart - 50 ) {
			lbBushouList->SelectedIndex = iStart - 50;
		}

		for ( int i = iStart; i > iStart-50; i-- ) {
			// 先に武将リストからカット
			lbBushouList->Items->RemoveAt(i);

			// 次いで元データを減らす
			bdBushouData->lstStrFullName->RemoveAt( i );
			bdBushouData->lstStrRubyYomi->RemoveAt( i );
			bdBushouData->lstStrBornEtc->RemoveAt( i );
			bdBushouData->lstStrRetsuden->RemoveAt( i );
		}
	}




//-----------------------------------------------武将列伝系

private:
	TabPage^ tpHime;
	HimeData^ bdHimeData;

	TextBox^ tbHimeSearch; // 武将検索
	Button^ btnHimeSearch; // 検索ボタン
	Button^ btnHimeImport; // 取込ボタン
	OpenFileDialog^ ofdHimeImport;
	Button^ btnHimeExport; // 出力ボタン
	Button^ btnHimeAllSave; // 全て保存

	ListBox^ lbHimeList; // 武将リストのコンボボックス
	int iLastHimeListSelected;

	Panel^ plHimeRetsuden;	 // 列伝情報用のパネル
	TextBox^ tbHimeFullName; // 武将姓名のテキストボックス
	TextBox^ tbHimeRubyYomi; // ふりがなのるび
	TextBox^ tbHimeBornEtc;  // 生年などの情報
	TextBox^ tbHimeRetsuden; // 列伝情報


	// フォント
	System::Drawing::Font^ fontHime;
	ComboBox^ cbFontHime;	// フォントリスト

	Button^ btnHimeAdds; // 50枠追加
	Button^ btnHimeDels; // 50枠追加


private:

	void tpHime_Init() {

		tpHime = gcnew TabPage();

		tpHime->Text = "姫列伝";
		tpHime->Size = tcRE->ClientSize;

		// 武将検索
		tbHimeSearch = gcnew TextBox();
		tbHimeSearch->Width = 140;
		tbHimeSearch->Height = 16;
		tbHimeSearch->Left = 10;
		tbHimeSearch->Top = 10;

		// 検索ボタン
		btnHimeSearch = gcnew Button();
		btnHimeSearch->Text = "検索";
		btnHimeSearch->Width = 60;
		btnHimeSearch->Height = tbHimeSearch->Height;
		btnHimeSearch->Left = tbHimeSearch->Right + 1;
		btnHimeSearch->Top = tbHimeSearch->Top;

		// 取込ボタン
		btnHimeImport = gcnew Button();
		btnHimeImport->Text = "単独取込";
		btnHimeImport->Width = 70;
		btnHimeImport->Height = tbHimeSearch->Height;
		btnHimeImport->Left = btnHimeSearch->Right + 30;
		btnHimeImport->Top = tbHimeSearch->Top;

		// 出力ボタン
		btnHimeExport = gcnew Button();
		btnHimeExport->Text = "単独出力";
		btnHimeExport->Width = 70;
		btnHimeExport->Height = tbHimeSearch->Height;
		btnHimeExport->Left = btnHimeImport->Right;
		btnHimeExport->Top = tbHimeSearch->Top;

		// 全て保存
		btnHimeAllSave = gcnew Button();
		btnHimeAllSave->Text = "姫列伝を保存";
		btnHimeAllSave->Width = 120;
		btnHimeAllSave->Height = tbHimeSearch->Height;
		btnHimeAllSave->Left = tpHime->Right - (btnHimeAllSave->Width + 20);
		btnHimeAllSave->Top = tbHimeSearch->Top;

		// 武将知ると
		lbHimeList = gcnew ListBox();
		lbHimeList->Width = tbHimeSearch->Width;
		lbHimeList->Left = tbHimeSearch->Left;
		lbHimeList->Top = tbHimeSearch->Bottom + 10;
		lbHimeList->Height = 200;

		// 列伝情報用のパネル
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

		// フォントリスト
		cbFontHime = gcnew ComboBox();
		cbFontHime->Width = 100;
		cbFontHime->Left = tpHime->Right - (cbFontHime->Width + 20);
		cbFontHime->Top = plHimeRetsuden->Bottom + 10;
		cbFontHime->Height = tbHimeSearch->Height;
		Hime_GetValidTenshoukiFontList();

		Hime_cbFont_SelectedIndexChanged(nullptr, nullptr);

		/*
		// 武将枠50人追加
		btnHimeAdds = gcnew Button();
		btnHimeAdds->Text = "50枠追加";
		btnHimeAdds->Width = 70;
		btnHimeAdds->Height = tbHimeSearch->Height;
		btnHimeAdds->Left = tbHimeSearch->Left;
		btnHimeAdds->Top = lbHimeList->Bottom + 10;

		// 武将枠50人削除
		btnHimeDels = gcnew Button();
		btnHimeDels->Text = "50枠削除";
		btnHimeDels->Width = 70;
		btnHimeDels->Height = tbHimeSearch->Height;
		btnHimeDels->Left = btnHimeAdds->Right;
		btnHimeDels->Top = lbHimeList->Bottom + 10;
		*/

		// 武将列伝のタブにコンポーネント追加
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

		// タブをフォームに追加
		tcRE->TabPages->Add(tpHime);

		// 武将データを作成して、リストに流しこむ
		bdHimeData = gcnew HimeData();
		// リストボックスに足し込み
		for (int i = 0; i < bdHimeData->lstStrFullName->Count; i++) {
			String^ strLine = String::Format("{0:0000} - {1}", i, bdHimeData->lstStrFullName[i]);
			lbHimeList->Items->Add(strLine);
		}

		// 最初のものを選択しておく
		lbHimeList->SelectedIndex = 0;
		iLastHimeListSelected = 0;

		// 武将データ→テキストボックス
		Hime_BDataToTextBox();
		Hime_TextBoxWidthUpdate();
	}

	// パネルの背景画像設定
	void Hime_SetPanelBackImg() {
		// このプロジェクトのアセンブリのタイプを取得。
		System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
		System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.RetsudenEditorRes", prj_assebmly->GetName()->Name), prj_assebmly);

		// パネルの背景
		plHimeRetsuden->BackgroundImage = (System::Drawing::Image^)(r->GetObject("retsuden_back"));
		// 背景画像の大きさにパネルの大きさを合わせる
		plHimeRetsuden->Size = plHimeRetsuden->BackgroundImage->Size;

	}

	// 各種ＧＵＩのイベントハンドラ設定
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

	// 武将検索ボタンを押した時
	void Hime_btnSearch_Click(Object^ sender, EventArgs^ e) {
		if (tbHimeSearch->Text == "") { return; } // テキストエリアが空っぽなら何もしない。

		int iStart = GetHimeCBSelectedIndex() + 1; // 現在選択しているもののから次の候補

		Regex^ regex = gcnew Regex(tbHimeSearch->Text);

		bool isExist = false;
		// iStart以降を全部サーチして、
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

	// 武将検索テキストボックスでキーを押した時
	void Hime_tbSearch_KeyDown(Object^ sender, KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Return) {
			Hime_btnSearch_Click(sender, e);
		}
		if (e->KeyCode == Keys::F3) {
			Hime_btnSearch_Click(sender, e);
		}
	}

	// 武将出力ボタンを押した時
	void Hime_btnExport_Click(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetHimeCBSelectedIndex();
		std::string unmanagedData = bdHimeData->GetOneHimeExportData(iSelectedIndex);

		std::string filepath = DotNetStringToMultiByte(bdHimeData->lstStrFullName[iSelectedIndex]);
		if (filepath == "") { filepath = "ななし"; }
		filepath += ".n6b";

		FILE* fp = fopen(filepath.c_str(), "wb");
		if (fp) {
			fwrite(unmanagedData.c_str(), unmanagedData.size(), 1, fp);
			fclose(fp);
			System::Windows::Forms::MessageBox::Show("データを出力しました。", "完了", ::MessageBoxButtons::OK, ::MessageBoxIcon::Information);
		}
		else {
			System::Windows::Forms::MessageBox::Show("データの出力に失敗しました。", "エラー", MessageBoxButtons::OK, ::MessageBoxIcon::Error);

		}
	}


	// 武将入力ボタンを押した時
	void Hime_btnImport_Click(Object^ sender, EventArgs^ e) {

		ofdHimeImport = gcnew OpenFileDialog();

		ofdHimeImport->InitialDirectory = "."; // ツールと同じディレクトリがデフォルト

		ofdHimeImport->Filter = "将星録 メッセージファイル(*.nb7)|*.nb7";

		ofdHimeImport->FilterIndex = 1;

		ofdHimeImport->Title = "とある武将の列伝ファイルを選択してください";

		//ダイアログボックスを閉じる前に現在のディレクトリを復元するようにする
		ofdHimeImport->RestoreDirectory = true;

		//ダイアログを表示する
		if (ofdHimeImport->ShowDialog() == ::DialogResult::OK) {
			//OKボタンがクリックされたとき
			if (ofdHimeImport->FileName) {
				BOOL success = bdHimeData->GetOneHimeImportData(ofdHimeImport->FileName, GetHimeCBSelectedIndex());

				//　成否にかかわらず更新
				Hime_BDataToTextBox();
			}
		}
	}

	// 全て保存ボタンを押した時
	void Hime_btnAllSave_Click(Object^ sender, EventArgs^ e) {
		bdHimeData->AllSaveToBFileN6P();
	}

	// フォントリストの選択項目が変更された時
	void Hime_cbFont_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
		// フォントの設定
		fontHime = gcnew System::Drawing::Font((String^)cbFontHime->Items[cbFontHime->SelectedIndex], 12, FontStyle::Regular);
		tbHimeFullName->Font = fontHime;
		tbHimeRubyYomi->Font = fontHime;
		tbHimeBornEtc->Font = fontHime;
		tbHimeRetsuden->Font = fontHime;
	}

	// インストールされているフォントにしたがって、フォントリストボックスに追加
	void Hime_GetValidTenshoukiFontList() {

		//InstalledFontCollectionオブジェクトの取得
		System::Drawing::Text::InstalledFontCollection^ ifc = gcnew System::Drawing::Text::InstalledFontCollection();

		//インストールされているすべてのフォントファミリアを取得
		cli::array<FontFamily^>^ ffs = ifc->Families;

		//ここでは候補となりえるフォント名のみ
		for (int i = 0; i < aryStrFontCandidate->Length; i++) {

			// フォントリスト
			for each (FontFamily ^ ff in ffs) {

				// 候補フォントがあった。
				if (ff->Name == aryStrFontCandidate[i]) {
					// コンボボックスに追加
					cbFontHime->Items->Add(ff->Name);
				}
			}
		}

		// １つ以上フォントがあったら、最初のものを選択しておく。
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
		// 焦点を見失っているようなら、最後に選択していたものを宛がう。
		int iSelectIndex = GetHimeCBSelectedIndex();
		if (iSelectIndex == 0xFFFFFFFF) {
			lbHimeList->SelectedIndex = iLastHimeListSelected;
		}
		else {
			iLastHimeListSelected = iSelectIndex;
		}
	}


	// 武将リストを選択変更すると、画像の上の列伝各種の値が入れ替わる
	void Hime_lbHimeList_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
		ModifyCBHimeSelectedIndex();
		// 新たなindexのデータを取得。
		Hime_BDataToTextBox();

		ModifyCBHimeSelectedIndex();
	}


	// 武将データ→テキストボックスへ転写
	void Hime_BDataToTextBox() {
		int iSelectedIndex = GetHimeCBSelectedIndex();
		tbHimeFullName->Text = bdHimeData->lstStrFullName[iSelectedIndex];
		tbHimeRubyYomi->Text = bdHimeData->lstStrRubyYomi[iSelectedIndex];
		tbHimeBornEtc->Text = bdHimeData->lstStrBornEtc[iSelectedIndex];
		tbHimeRetsuden->Text = bdHimeData->lstStrRetsuden[iSelectedIndex];
	}

	// テキストボックスの位置や幅を中身のテキスト量に従って変更
	void Hime_TextBoxWidthUpdate() {
		tbHimeFullName->Width = GetStringMeasure(tbHimeFullName->Text);

		tbHimeRubyYomi->Left = tbHimeFullName->Right + 8;
		tbHimeRubyYomi->Width = GetStringMeasure(tbHimeRubyYomi->Text);

		tbHimeBornEtc->Left = tbHimeRubyYomi->Right + 8;
		tbHimeBornEtc->Width = GetStringMeasure(tbHimeBornEtc->Text);
	}

	// テキストボックスの、「武将姓名」が変更された。
	void Hime_tbFullName_TextChanged(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetHimeCBSelectedIndex();
		bdHimeData->lstStrFullName[iSelectedIndex] = tbHimeFullName->Text;
		String^ strLine = String::Format("{0:0000} - {1}", iSelectedIndex, tbHimeFullName->Text);
		// SelectIndexChangeが反応しないようにする。
		lbHimeList->SelectedIndexChanged -= gcnew EventHandler(this, &RetsudenEditorForm::Hime_lbHimeList_SelectedIndexChanged);

		// 反応しないようにしたので、値を書き換える。SelectedIndexChangeメソッドが反応しないのでエラーが起きない。

		lbHimeList->Items[iSelectedIndex] = gcnew String(strLine); // 武将リストの名前の方を更新

		Hime_TextBoxWidthUpdate();

		// 更新したので、反応を戻す。
		// SelectIndexChangeが反応するようにする。
		lbHimeList->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Hime_lbHimeList_SelectedIndexChanged);
	}

	// テキストボックスの、「ふりがな」が変更された。
	void Hime_tbRubyYomi_TextChanged(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetHimeCBSelectedIndex();
		bdHimeData->lstStrRubyYomi[iSelectedIndex] = tbHimeRubyYomi->Text;
		Hime_TextBoxWidthUpdate();
	}

	// テキストボックスの、「生年等」が変更された。
	void Hime_tbBornEtc_TextChanged(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetHimeCBSelectedIndex();
		bdHimeData->lstStrBornEtc[iSelectedIndex] = tbHimeBornEtc->Text;
		Hime_TextBoxWidthUpdate();
	}

	// テキストボックスの、「列伝」が変更された。
	void Hime_tbRetsuden_TextChanged(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetHimeCBSelectedIndex();

		bdHimeData->lstStrRetsuden[iSelectedIndex] = tbHimeRetsuden->Text;
	}

	// テキストボックスから去った
	void Hime_textBox_Leave(Object^ sender, EventArgs^ e) {
	}


	// 50枠追加ボタン
	void Hime_btnAddHimes_Click(Object^ sender, EventArgs^ e) {
		int iStart = lbHimeList->Items->Count;
		for (int i = iStart; i < iStart + 50; i++) {
			String^ bname = String::Format("登録{0:0000}", i);
			// 先に元データから増やす
			bdHimeData->lstStrFullName->Add(bname);
			bdHimeData->lstStrRubyYomi->Add("ﾄｳﾛｸ" + i);
			bdHimeData->lstStrBornEtc->Add("????～????");
			bdHimeData->lstStrRetsuden->Add("未入力");
			String^ strLine = String::Format("{0:0000} - {1}", i, bname);
			lbHimeList->Items->Add(strLine);
		}

	}

	// 50枠削除ボタン
	void Hime_btnDelHimes_Click(Object^ sender, EventArgs^ e) {

		int iStart = lbHimeList->Items->Count - 1;

		// 選択インデックスが削除予定の所にあったら、ギリギリのところまで退避
		if (GetHimeCBSelectedIndex() > iStart - 50) {
			lbHimeList->SelectedIndex = iStart - 50;
		}

		for (int i = iStart; i > iStart - 50; i--) {
			// 先に武将リストからカット
			lbHimeList->Items->RemoveAt(i);

			// 次いで元データを減らす
			bdHimeData->lstStrFullName->RemoveAt(i);
			bdHimeData->lstStrRubyYomi->RemoveAt(i);
			bdHimeData->lstStrBornEtc->RemoveAt(i);
			bdHimeData->lstStrRetsuden->RemoveAt(i);
		}
	}



//-----------------------------------------------家宝列伝系
private:
	TabPage^ tpKahou;
	KahouData^ bdKahouData;

	TextBox^ tbKahouSearch; // 家宝検索
	Button^ btnKahouSearch; // 検索ボタン
	Button^ btnKahouImport; // 取込ボタン
	Button^ btnKahouExport; // 出力ボタン
	Button^ btnKahouAllSave; // 全て保存

	ListBox^ lbKahouList; // 家宝リストのコンボボックス
	int iLastKahouListSelected;

	Panel^ plKahouRetsuden;	 // 列伝情報用のパネル
	TextBox^ tbKahouFullName; // 家宝姓名のテキストボックス
	TextBox^ tbKahouRubyYomi; // ふりがなのるび
	TextBox^ tbKahouBornEtc;  // 生年などの情報
	TextBox^ tbKahouRetsuden; // 列伝情報


	// フォント
	System::Drawing::Font^ fontKahou;
	ComboBox^ cbFontKahou;	// フォントリスト

	Button^ btnKahouAdds; // 50枠追加
	Button^ btnKahouDels; // 50枠追加



private:

	void tpKahou_Init() {

		tpKahou = gcnew TabPage();

		tpKahou->Text = "家宝列伝";
		tpKahou->Size = tcRE->ClientSize;

		// 家宝検索
		tbKahouSearch = gcnew TextBox();
		tbKahouSearch->Width = 140;
		tbKahouSearch->Height = 16;
		tbKahouSearch->Left = 10;
		tbKahouSearch->Top = 10;

		// 検索ボタン
		btnKahouSearch = gcnew Button();
		btnKahouSearch->Text = "検索";
		btnKahouSearch->Width = 60;
		btnKahouSearch->Height = tbKahouSearch->Height;
		btnKahouSearch->Left = tbKahouSearch->Right + 1;
		btnKahouSearch->Top = tbKahouSearch->Top;

		/*
		// 取込ボタン
		btnKahouImport = gcnew Button();
		btnKahouImport->Text = "単独取込";
		btnKahouImport->Width = 70;
		btnKahouImport->Height = tbKahouSearch->Height;
		btnKahouImport->Left = btnKahouSearch->Right + 30;
		btnKahouImport->Top = tbKahouSearch->Top;

		// 出力ボタン
		btnKahouExport = gcnew Button();
		btnKahouExport->Text = "単独出力";
		btnKahouExport->Width = 70;
		btnKahouExport->Height = tbKahouSearch->Height;
		btnKahouExport->Left = btnKahouImport->Right;
		btnKahouExport->Top = tbKahouSearch->Top;
		*/

		// 全て保存
		btnKahouAllSave = gcnew Button();
		btnKahouAllSave->Text = "家宝列伝を保存";
		btnKahouAllSave->Width = 120;
		btnKahouAllSave->Height = tbKahouSearch->Height;
		btnKahouAllSave->Left = tpKahou->Right - (btnKahouAllSave->Width+20);
		btnKahouAllSave->Top = tbKahouSearch->Top;

		// 家宝知ると
		lbKahouList = gcnew ListBox();
		lbKahouList->Width = tbKahouSearch->Width;
		lbKahouList->Left = tbKahouSearch->Left;
		lbKahouList->Top = tbKahouSearch->Bottom + 10;
		lbKahouList->Height = 200;

		// 列伝情報用のパネル
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

		// フォントリスト
		cbFontKahou = gcnew ComboBox();
		cbFontKahou->Width = 100;
		cbFontKahou->Left = tpKahou->Right - (cbFontKahou->Width+20);
		cbFontKahou->Top = plKahouRetsuden->Bottom + 10;
		cbFontKahou->Height = tbKahouSearch->Height;
		Kahou_GetValidTenshoukiFontList();

		Kahou_cbFont_SelectedIndexChanged(nullptr, nullptr);

		// 家宝枠50人追加
		btnKahouAdds = gcnew Button();
		btnKahouAdds->Text = "50枠追加";
		btnKahouAdds->Width = 70;
		btnKahouAdds->Height = tbKahouSearch->Height;
		btnKahouAdds->Left = tbKahouSearch->Left;
		btnKahouAdds->Top = lbKahouList->Bottom + 10;

		// 家宝枠50人削除
		btnKahouDels = gcnew Button();
		btnKahouDels->Text = "50枠削除";
		btnKahouDels->Width = 70;
		btnKahouDels->Height = tbKahouSearch->Height;
		btnKahouDels->Left = btnKahouAdds->Right;
		btnKahouDels->Top = lbKahouList->Bottom + 10;


		// 家宝列伝のタブにコンポーネント追加
		tpKahou->Controls->Add(tbKahouSearch);
		tpKahou->Controls->Add(btnKahouSearch);
		// tpKahou->Controls->Add(btnKahouImport);
		// tpKahou->Controls->Add(btnKahouExport);
		tpKahou->Controls->Add(btnKahouAllSave);
		tpKahou->Controls->Add(lbKahouList);
		tpKahou->Controls->Add(plKahouRetsuden);
		tpKahou->Controls->Add(cbFontKahou);
		// tpKahou->Controls->Add(btnKahouAdds); // 家宝用にはこのボタンは不要
		// tpKahou->Controls->Add(btnKahouDels); // 家宝用にはこのボタンは不要
		
		// タブをフォームに追加
		tcRE->TabPages->Add(tpKahou);

		// 家宝データを作成して、リストに流しこむ
		bdKahouData = gcnew KahouData();
		// リストボックスに足し込み
		for ( int i=0; i<bdKahouData->lstStrFullName->Count; i++ ) {
			String^ strLine = String::Format( "{0:000} - {1}", i, bdKahouData->lstStrFullName[i]);
			lbKahouList->Items->Add( strLine );
		}

		// 最初のものを選択しておく
		lbKahouList->SelectedIndex = 0;
		iLastKahouListSelected = 0;

		// 家宝データ→テキストボックス
		Kahou_BDataToTextBox();
		Kahou_TextBoxWidthUpdate();
	}

	// パネルの背景画像設定
	void Kahou_SetPanelBackImg() {
		// このプロジェクトのアセンブリのタイプを取得。
		System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
		System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.RetsudenEditorRes", prj_assebmly->GetName()->Name), prj_assebmly);

		// パネルの背景
		plKahouRetsuden->BackgroundImage = (System::Drawing::Image^)(r->GetObject("retsuden_back"));
		// 背景画像の大きさにパネルの大きさを合わせる
		plKahouRetsuden->Size = plKahouRetsuden->BackgroundImage->Size;

	}

	// 各種ＧＵＩのイベントハンドラ設定
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

	// 家宝検索ボタンを押した時
	void Kahou_btnSearch_Click(Object^ sender, EventArgs^ e) {
		if ( tbKahouSearch->Text == "" ) { return; } // テキストエリアが空っぽなら何もしない。

		int iStart = GetKahouCBSelectedIndex() + 1; // 現在選択しているもののから次の候補

		Regex^ regex = gcnew Regex(tbKahouSearch->Text);

		bool isExist = false;
		// iStart以降を全部サーチして、
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

	// 家宝検索テキストボックスでキーを押した時
	void Kahou_tbSearch_KeyDown(Object^ sender, KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Return ) {
			Kahou_btnSearch_Click(sender, e);
		}
		if (e->KeyCode == Keys::F3 ) {
			Kahou_btnSearch_Click(sender, e);
		}
	}

	// 家宝出力ボタンを押した時
	void Kahou_btnExport_Click(Object^ sender, EventArgs^ e) {
	}


	// 家宝入力ボタンを押した時
	void Kahou_btnImport_Click(Object^ sender, EventArgs^ e) {
	}

	// 全て保存ボタンを押した時
	void Kahou_btnAllSave_Click(Object^ sender, EventArgs^ e) {
		bdKahouData->AllSaveToMessageN6P();
	}

	// フォントリストの選択項目が変更された時
	void Kahou_cbFont_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
		// フォントの設定
		fontKahou = gcnew System::Drawing::Font((String^)cbFontKahou->Items[cbFontKahou->SelectedIndex], 12, FontStyle::Regular);
		tbKahouFullName->Font = fontKahou;
		tbKahouRubyYomi->Font = fontKahou;
		tbKahouBornEtc->Font  = fontKahou;
		tbKahouRetsuden->Font = fontKahou;
	}

	// インストールされているフォントにしたがって、フォントリストボックスに追加
	void Kahou_GetValidTenshoukiFontList() {

		//InstalledFontCollectionオブジェクトの取得
		System::Drawing::Text::InstalledFontCollection^ ifc = gcnew System::Drawing::Text::InstalledFontCollection();

		//インストールされているすべてのフォントファミリアを取得
		cli::array<FontFamily^>^ ffs = ifc->Families;

		//ここでは候補となりえるフォント名のみ
		for ( int i=0; i<aryStrFontCandidate->Length; i++) {

			// フォントリスト
			for each (FontFamily^ ff in ffs) {

				// 候補フォントがあった。
				if (ff->Name == aryStrFontCandidate[i]) {
					// コンボボックスに追加
					cbFontKahou->Items->Add( ff->Name );
				}
			}
		}

		// １つ以上フォントがあったら、最初のものを選択しておく。
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
		// 焦点を見失っているようなら、最後に選択していたものを宛がう。
		int iSelectIndex = GetKahouCBSelectedIndex();
		if ( iSelectIndex == 0xFFFFFFFF ) {
			lbKahouList->SelectedIndex = iLastKahouListSelected;
		} else {
			iLastKahouListSelected = iSelectIndex;
		}
	}


	// 家宝リストを選択変更すると、画像の上の列伝各種の値が入れ替わる
	void Kahou_lbKahouList_SelectedIndexChanged(Object^ sender, EventArgs^ e ) {
		ModifyCBKahouSelectedIndex();
		// 新たなindexのデータを取得。
		Kahou_BDataToTextBox();

		ModifyCBKahouSelectedIndex();
	}


	// 家宝データ→テキストボックスへ転写
	void Kahou_BDataToTextBox() {
		int iSelectedIndex = GetKahouCBSelectedIndex();
		tbKahouFullName->Text = bdKahouData->lstStrFullName[iSelectedIndex];
		tbKahouRubyYomi->Text = bdKahouData->lstStrRubyYomi[iSelectedIndex];
		tbKahouBornEtc->Text  = bdKahouData->lstStrBornEtc[iSelectedIndex];
		tbKahouRetsuden->Text = bdKahouData->lstStrRetsuden[iSelectedIndex];
	}

	// テキストボックスの位置や幅を中身のテキスト量に従って変更
	void Kahou_TextBoxWidthUpdate() {
		tbKahouFullName->Width = GetStringMeasure(tbKahouFullName->Text);

		tbKahouRubyYomi->Left = tbKahouFullName->Right + 8;
		tbKahouRubyYomi->Width = GetStringMeasure(tbKahouRubyYomi->Text);
		
		tbKahouBornEtc->Left = tbKahouRubyYomi->Right + 8;
		tbKahouBornEtc->Width = GetStringMeasure(tbKahouBornEtc->Text);
	}

	// テキストボックスの、「家宝姓名」が変更された。
	void Kahou_tbFullName_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetKahouCBSelectedIndex();
		bdKahouData->lstStrFullName[iSelectedIndex] = tbKahouFullName->Text;
		String^ strLine = String::Format( "{0:000} - {1}", iSelectedIndex, tbKahouFullName->Text);
		// SelectIndexChangeが反応しないようにする。
		lbKahouList->SelectedIndexChanged -= gcnew EventHandler(this, &RetsudenEditorForm::Kahou_lbKahouList_SelectedIndexChanged);

		// 反応しないようにしたので、値を書き換える。SelectedIndexChangeメソッドが反応しないのでエラーが起きない。

		lbKahouList->Items[iSelectedIndex] = gcnew String(strLine); // 家宝リストの名前の方を更新

		Kahou_TextBoxWidthUpdate();

		// 更新したので、反応を戻す。
		// SelectIndexChangeが反応するようにする。
		lbKahouList->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Kahou_lbKahouList_SelectedIndexChanged);
	}

	// テキストボックスの、「ふりがな」が変更された。
	void Kahou_tbRubyYomi_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetKahouCBSelectedIndex();
		bdKahouData->lstStrRubyYomi[iSelectedIndex] = tbKahouRubyYomi->Text;
		Kahou_TextBoxWidthUpdate();
	}

	// テキストボックスの、「生年等」が変更された。
	void Kahou_tbBornEtc_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetKahouCBSelectedIndex();
		bdKahouData->lstStrBornEtc[iSelectedIndex]  = tbKahouBornEtc->Text;
		Kahou_TextBoxWidthUpdate();
	}

	// テキストボックスの、「列伝」が変更された。
	void Kahou_tbRetsuden_TextChanged(Object^ sender, EventArgs^ e ) {
		int iSelectedIndex = GetKahouCBSelectedIndex();

		bdKahouData->lstStrRetsuden[iSelectedIndex] = tbKahouRetsuden->Text;
	}

	// テキストボックスから去った
	void Kahou_textBox_Leave(Object^ sender, EventArgs^ e ) {
	}


	// 50枠追加ボタン
	void Kahou_btnAddKahous_Click(Object^ sender, EventArgs^ e) {
	}

	// 50枠削除ボタン
	void Kahou_btnDelKahous_Click(Object^ sender, EventArgs^ e) {
	}

	//-----------------------------------------------城列伝系
private:
	TabPage^ tpCastle;
	CastleData^ bdCastleData;

	TextBox^ tbCastleSearch; // 城検索
	Button^ btnCastleSearch; // 検索ボタン
	Button^ btnCastleImport; // 取込ボタン
	Button^ btnCastleExport; // 出力ボタン
	Button^ btnCastleAllSave; // 全て保存

	ListBox^ lbCastleList; // 城リストのコンボボックス
	int iLastCastleListSelected;

	Panel^ plCastleRetsuden;	 // 列伝情報用のパネル
	TextBox^ tbCastleFullName; // 城姓名のテキストボックス
	TextBox^ tbCastleRubyYomi; // ふりがなのるび
	TextBox^ tbCastleBornEtc;  // 生年などの情報
	TextBox^ tbCastleRetsuden; // 列伝情報


	// フォント
	System::Drawing::Font^ fontCastle;
	ComboBox^ cbFontCastle;	// フォントリスト

	Button^ btnCastleAdds; // 50枠追加
	Button^ btnCastleDels; // 50枠追加


private:

	void tpCastle_Init() {

		tpCastle = gcnew TabPage();

		tpCastle->Text = "城列伝";
		tpCastle->Size = tcRE->ClientSize;

		// 城検索
		tbCastleSearch = gcnew TextBox();
		tbCastleSearch->Width = 140;
		tbCastleSearch->Height = 16;
		tbCastleSearch->Left = 10;
		tbCastleSearch->Top = 10;

		// 検索ボタン
		btnCastleSearch = gcnew Button();
		btnCastleSearch->Text = "検索";
		btnCastleSearch->Width = 60;
		btnCastleSearch->Height = tbCastleSearch->Height;
		btnCastleSearch->Left = tbCastleSearch->Right + 1;
		btnCastleSearch->Top = tbCastleSearch->Top;

		/*
		// 取込ボタン
		btnCastleImport = gcnew Button();
		btnCastleImport->Text = "単独取込";
		btnCastleImport->Width = 70;
		btnCastleImport->Height = tbCastleSearch->Height;
		btnCastleImport->Left = btnCastleSearch->Right + 30;
		btnCastleImport->Top = tbCastleSearch->Top;

		// 出力ボタン
		btnCastleExport = gcnew Button();
		btnCastleExport->Text = "単独出力";
		btnCastleExport->Width = 70;
		btnCastleExport->Height = tbCastleSearch->Height;
		btnCastleExport->Left = btnCastleImport->Right;
		btnCastleExport->Top = tbCastleSearch->Top;
		*/

		// 全て保存
		btnCastleAllSave = gcnew Button();
		btnCastleAllSave->Text = "城列伝を保存";
		btnCastleAllSave->Width = 120;
		btnCastleAllSave->Height = tbCastleSearch->Height;
		btnCastleAllSave->Left = tpCastle->Right - (btnCastleAllSave->Width + 20);
		btnCastleAllSave->Top = tbCastleSearch->Top;

		// 城知ると
		lbCastleList = gcnew ListBox();
		lbCastleList->Width = tbCastleSearch->Width;
		lbCastleList->Left = tbCastleSearch->Left;
		lbCastleList->Top = tbCastleSearch->Bottom + 10;
		lbCastleList->Height = 200;

		// 列伝情報用のパネル
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

		// フォントリスト
		cbFontCastle = gcnew ComboBox();
		cbFontCastle->Width = 100;
		cbFontCastle->Left = tpCastle->Right - (cbFontCastle->Width + 20);
		cbFontCastle->Top = plCastleRetsuden->Bottom + 10;
		cbFontCastle->Height = tbCastleSearch->Height;
		Castle_GetValidTenshoukiFontList();

		Castle_cbFont_SelectedIndexChanged(nullptr, nullptr);

		// 城枠50人追加
		btnCastleAdds = gcnew Button();
		btnCastleAdds->Text = "50枠追加";
		btnCastleAdds->Width = 70;
		btnCastleAdds->Height = tbCastleSearch->Height;
		btnCastleAdds->Left = tbCastleSearch->Left;
		btnCastleAdds->Top = lbCastleList->Bottom + 10;

		// 城枠50人削除
		btnCastleDels = gcnew Button();
		btnCastleDels->Text = "50枠削除";
		btnCastleDels->Width = 70;
		btnCastleDels->Height = tbCastleSearch->Height;
		btnCastleDels->Left = btnCastleAdds->Right;
		btnCastleDels->Top = lbCastleList->Bottom + 10;


		// 城列伝のタブにコンポーネント追加
		tpCastle->Controls->Add(tbCastleSearch);
		tpCastle->Controls->Add(btnCastleSearch);
		// tpCastle->Controls->Add(btnCastleImport);
		// tpCastle->Controls->Add(btnCastleExport);
		tpCastle->Controls->Add(btnCastleAllSave);
		tpCastle->Controls->Add(lbCastleList);
		tpCastle->Controls->Add(plCastleRetsuden);
		tpCastle->Controls->Add(cbFontCastle);
		// tpCastle->Controls->Add(btnCastleAdds); // 城用にはこのボタンは不要
		// tpCastle->Controls->Add(btnCastleDels); // 城用にはこのボタンは不要

		// タブをフォームに追加
		tcRE->TabPages->Add(tpCastle);

		// 城データを作成して、リストに流しこむ
		bdCastleData = gcnew CastleData();
		// リストボックスに足し込み
		for (int i = 0; i < bdCastleData->lstStrFullName->Count; i++) {
			String^ strLine = String::Format("{0:000} - {1}", i, bdCastleData->lstStrFullName[i]);
			lbCastleList->Items->Add(strLine);
		}

		// 最初のものを選択しておく
		lbCastleList->SelectedIndex = 0;
		iLastCastleListSelected = 0;

		// 城データ→テキストボックス
		Castle_BDataToTextBox();
		Castle_TextBoxWidthUpdate();
	}

	// パネルの背景画像設定
	void Castle_SetPanelBackImg() {
		// このプロジェクトのアセンブリのタイプを取得。
		System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
		System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.RetsudenEditorRes", prj_assebmly->GetName()->Name), prj_assebmly);

		// パネルの背景
		plCastleRetsuden->BackgroundImage = (System::Drawing::Image^)(r->GetObject("retsuden_back"));
		// 背景画像の大きさにパネルの大きさを合わせる
		plCastleRetsuden->Size = plCastleRetsuden->BackgroundImage->Size;

	}

	// 各種ＧＵＩのイベントハンドラ設定
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

	// 城検索ボタンを押した時
	void Castle_btnSearch_Click(Object^ sender, EventArgs^ e) {
		if (tbCastleSearch->Text == "") { return; } // テキストエリアが空っぽなら何もしない。

		int iStart = GetCastleCBSelectedIndex() + 1; // 現在選択しているもののから次の候補

		Regex^ regex = gcnew Regex(tbCastleSearch->Text);

		bool isExist = false;
		// iStart以降を全部サーチして、
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

	// 城検索テキストボックスでキーを押した時
	void Castle_tbSearch_KeyDown(Object^ sender, KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Return) {
			Castle_btnSearch_Click(sender, e);
		}
		if (e->KeyCode == Keys::F3) {
			Castle_btnSearch_Click(sender, e);
		}
	}

	// 城出力ボタンを押した時
	void Castle_btnExport_Click(Object^ sender, EventArgs^ e) {
	}


	// 城入力ボタンを押した時
	void Castle_btnImport_Click(Object^ sender, EventArgs^ e) {
	}

	// 全て保存ボタンを押した時
	void Castle_btnAllSave_Click(Object^ sender, EventArgs^ e) {
		bdCastleData->AllSaveToMessageN6P();
	}

	// フォントリストの選択項目が変更された時
	void Castle_cbFont_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
		// フォントの設定
		fontCastle = gcnew System::Drawing::Font((String^)cbFontCastle->Items[cbFontCastle->SelectedIndex], 12, FontStyle::Regular);
		tbCastleFullName->Font = fontCastle;
		tbCastleRubyYomi->Font = fontCastle;
		tbCastleBornEtc->Font = fontCastle;
		tbCastleRetsuden->Font = fontCastle;
	}

	// インストールされているフォントにしたがって、フォントリストボックスに追加
	void Castle_GetValidTenshoukiFontList() {

		//InstalledFontCollectionオブジェクトの取得
		System::Drawing::Text::InstalledFontCollection^ ifc = gcnew System::Drawing::Text::InstalledFontCollection();

		//インストールされているすべてのフォントファミリアを取得
		cli::array<FontFamily^>^ ffs = ifc->Families;

		//ここでは候補となりえるフォント名のみ
		for (int i = 0; i < aryStrFontCandidate->Length; i++) {

			// フォントリスト
			for each (FontFamily ^ ff in ffs) {

				// 候補フォントがあった。
				if (ff->Name == aryStrFontCandidate[i]) {
					// コンボボックスに追加
					cbFontCastle->Items->Add(ff->Name);
				}
			}
		}

		// １つ以上フォントがあったら、最初のものを選択しておく。
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
		// 焦点を見失っているようなら、最後に選択していたものを宛がう。
		int iSelectIndex = GetCastleCBSelectedIndex();
		if (iSelectIndex == 0xFFFFFFFF) {
			lbCastleList->SelectedIndex = iLastCastleListSelected;
		}
		else {
			iLastCastleListSelected = iSelectIndex;
		}
	}


	// 城リストを選択変更すると、画像の上の列伝各種の値が入れ替わる
	void Castle_lbCastleList_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
		ModifyCBCastleSelectedIndex();
		// 新たなindexのデータを取得。
		Castle_BDataToTextBox();

		ModifyCBCastleSelectedIndex();
	}


	// 城データ→テキストボックスへ転写
	void Castle_BDataToTextBox() {
		int iSelectedIndex = GetCastleCBSelectedIndex();
		tbCastleFullName->Text = bdCastleData->lstStrFullName[iSelectedIndex];
		tbCastleRubyYomi->Text = bdCastleData->lstStrRubyYomi[iSelectedIndex];
		tbCastleBornEtc->Text = bdCastleData->lstStrBornEtc[iSelectedIndex];
		tbCastleRetsuden->Text = bdCastleData->lstStrRetsuden[iSelectedIndex];
	}

	// テキストボックスの位置や幅を中身のテキスト量に従って変更
	void Castle_TextBoxWidthUpdate() {
		tbCastleFullName->Width = GetStringMeasure(tbCastleFullName->Text);

		tbCastleRubyYomi->Left = tbCastleFullName->Right + 8;
		tbCastleRubyYomi->Width = GetStringMeasure(tbCastleRubyYomi->Text);

		tbCastleBornEtc->Left = tbCastleRubyYomi->Right + 8;
		tbCastleBornEtc->Width = GetStringMeasure(tbCastleBornEtc->Text);
	}

	// テキストボックスの、「城姓名」が変更された。
	void Castle_tbFullName_TextChanged(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetCastleCBSelectedIndex();
		bdCastleData->lstStrFullName[iSelectedIndex] = tbCastleFullName->Text;
		String^ strLine = String::Format("{0:000} - {1}", iSelectedIndex, tbCastleFullName->Text);
		// SelectIndexChangeが反応しないようにする。
		lbCastleList->SelectedIndexChanged -= gcnew EventHandler(this, &RetsudenEditorForm::Castle_lbCastleList_SelectedIndexChanged);

		// 反応しないようにしたので、値を書き換える。SelectedIndexChangeメソッドが反応しないのでエラーが起きない。

		lbCastleList->Items[iSelectedIndex] = gcnew String(strLine); // 城リストの名前の方を更新

		Castle_TextBoxWidthUpdate();

		// 更新したので、反応を戻す。
		// SelectIndexChangeが反応するようにする。
		lbCastleList->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Castle_lbCastleList_SelectedIndexChanged);
	}

	// テキストボックスの、「ふりがな」が変更された。
	void Castle_tbRubyYomi_TextChanged(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetCastleCBSelectedIndex();
		bdCastleData->lstStrRubyYomi[iSelectedIndex] = tbCastleRubyYomi->Text;
		Castle_TextBoxWidthUpdate();
	}

	// テキストボックスの、「生年等」が変更された。
	void Castle_tbBornEtc_TextChanged(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetCastleCBSelectedIndex();
		bdCastleData->lstStrBornEtc[iSelectedIndex] = tbCastleBornEtc->Text;
		Castle_TextBoxWidthUpdate();
	}

	// テキストボックスの、「列伝」が変更された。
	void Castle_tbRetsuden_TextChanged(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetCastleCBSelectedIndex();

		bdCastleData->lstStrRetsuden[iSelectedIndex] = tbCastleRetsuden->Text;
	}

	// テキストボックスから去った
	void Castle_textBox_Leave(Object^ sender, EventArgs^ e) {
	}


	// 50枠追加ボタン
	void Castle_btnAddCastles_Click(Object^ sender, EventArgs^ e) {
	}

	// 50枠削除ボタン
	void Castle_btnDelCastles_Click(Object^ sender, EventArgs^ e) {
	}


	//-----------------------------------------------官位列伝系
private:
	TabPage^ tpKanni;
	KanniData^ bdKanniData;

	TextBox^ tbKanniSearch; // 官位検索
	Button^ btnKanniSearch; // 検索ボタン
	Button^ btnKanniImport; // 取込ボタン
	Button^ btnKanniExport; // 出力ボタン
	Button^ btnKanniAllSave; // 全て保存

	ListBox^ lbKanniList; // 官位リストのコンボボックス
	int iLastKanniListSelected;

	Panel^ plKanniRetsuden;	 // 列伝情報用のパネル
	TextBox^ tbKanniFullName; // 官位姓名のテキストボックス
	TextBox^ tbKanniRubyYomi; // ふりがなのるび
	TextBox^ tbKanniBornEtc;  // 生年などの情報
	TextBox^ tbKanniRetsuden; // 列伝情報


	// フォント
	System::Drawing::Font^ fontKanni;
	ComboBox^ cbFontKanni;	// フォントリスト

	Button^ btnKanniAdds; // 50枠追加
	Button^ btnKanniDels; // 50枠追加

private:

	void tpKanni_Init() {

		tpKanni = gcnew TabPage();

		tpKanni->Text = "官位列伝";
		tpKanni->Size = tcRE->ClientSize;

		// 官位検索
		tbKanniSearch = gcnew TextBox();
		tbKanniSearch->Width = 140;
		tbKanniSearch->Height = 16;
		tbKanniSearch->Left = 10;
		tbKanniSearch->Top = 10;

		// 検索ボタン
		btnKanniSearch = gcnew Button();
		btnKanniSearch->Text = "検索";
		btnKanniSearch->Width = 60;
		btnKanniSearch->Height = tbKanniSearch->Height;
		btnKanniSearch->Left = tbKanniSearch->Right + 1;
		btnKanniSearch->Top = tbKanniSearch->Top;

		/*
		// 取込ボタン
		btnKanniImport = gcnew Button();
		btnKanniImport->Text = "単独取込";
		btnKanniImport->Width = 70;
		btnKanniImport->Height = tbKanniSearch->Height;
		btnKanniImport->Left = btnKanniSearch->Right + 30;
		btnKanniImport->Top = tbKanniSearch->Top;

		// 出力ボタン
		btnKanniExport = gcnew Button();
		btnKanniExport->Text = "単独出力";
		btnKanniExport->Width = 70;
		btnKanniExport->Height = tbKanniSearch->Height;
		btnKanniExport->Left = btnKanniImport->Right;
		btnKanniExport->Top = tbKanniSearch->Top;
		*/

		// 全て保存
		btnKanniAllSave = gcnew Button();
		btnKanniAllSave->Text = "官位列伝を保存";
		btnKanniAllSave->Width = 120;
		btnKanniAllSave->Height = tbKanniSearch->Height;
		btnKanniAllSave->Left = tpKanni->Right - (btnKanniAllSave->Width + 20);
		btnKanniAllSave->Top = tbKanniSearch->Top;

		// 官位知ると
		lbKanniList = gcnew ListBox();
		lbKanniList->Width = tbKanniSearch->Width;
		lbKanniList->Left = tbKanniSearch->Left;
		lbKanniList->Top = tbKanniSearch->Bottom + 10;
		lbKanniList->Height = 200;

		// 列伝情報用のパネル
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

		// フォントリスト
		cbFontKanni = gcnew ComboBox();
		cbFontKanni->Width = 100;
		cbFontKanni->Left = tpKanni->Right - (cbFontKanni->Width + 20);
		cbFontKanni->Top = plKanniRetsuden->Bottom + 10;
		cbFontKanni->Height = tbKanniSearch->Height;
		Kanni_GetValidTenshoukiFontList();

		Kanni_cbFont_SelectedIndexChanged(nullptr, nullptr);

		// 官位枠50人追加
		btnKanniAdds = gcnew Button();
		btnKanniAdds->Text = "50枠追加";
		btnKanniAdds->Width = 70;
		btnKanniAdds->Height = tbKanniSearch->Height;
		btnKanniAdds->Left = tbKanniSearch->Left;
		btnKanniAdds->Top = lbKanniList->Bottom + 10;

		// 官位枠50人削除
		btnKanniDels = gcnew Button();
		btnKanniDels->Text = "50枠削除";
		btnKanniDels->Width = 70;
		btnKanniDels->Height = tbKanniSearch->Height;
		btnKanniDels->Left = btnKanniAdds->Right;
		btnKanniDels->Top = lbKanniList->Bottom + 10;


		// 官位列伝のタブにコンポーネント追加
		tpKanni->Controls->Add(tbKanniSearch);
		tpKanni->Controls->Add(btnKanniSearch);
		// tpKanni->Controls->Add(btnKanniImport);
		// tpKanni->Controls->Add(btnKanniExport);
		tpKanni->Controls->Add(btnKanniAllSave);
		tpKanni->Controls->Add(lbKanniList);
		tpKanni->Controls->Add(plKanniRetsuden);
		tpKanni->Controls->Add(cbFontKanni);
		// tpKanni->Controls->Add(btnKanniAdds); // 官位用にはこのボタンは不要
		// tpKanni->Controls->Add(btnKanniDels); // 官位用にはこのボタンは不要

		// タブをフォームに追加
		tcRE->TabPages->Add(tpKanni);

		// 官位データを作成して、リストに流しこむ
		bdKanniData = gcnew KanniData();
		// リストボックスに足し込み
		for (int i = 0; i < bdKanniData->lstStrFullName->Count; i++) {
			String^ strLine = String::Format("{0:000} - {1}", i, bdKanniData->lstStrFullName[i]);
			lbKanniList->Items->Add(strLine);
		}

		// 最初のものを選択しておく
		lbKanniList->SelectedIndex = 0;
		iLastKanniListSelected = 0;

		// 官位データ→テキストボックス
		Kanni_BDataToTextBox();
		Kanni_TextBoxWidthUpdate();
	}

	// パネルの背景画像設定
	void Kanni_SetPanelBackImg() {
		// このプロジェクトのアセンブリのタイプを取得。
		System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
		System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.RetsudenEditorRes", prj_assebmly->GetName()->Name), prj_assebmly);

		// パネルの背景
		plKanniRetsuden->BackgroundImage = (System::Drawing::Image^)(r->GetObject("retsuden_back"));
		// 背景画像の大きさにパネルの大きさを合わせる
		plKanniRetsuden->Size = plKanniRetsuden->BackgroundImage->Size;

	}

	// 各種ＧＵＩのイベントハンドラ設定
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

	// 官位検索ボタンを押した時
	void Kanni_btnSearch_Click(Object^ sender, EventArgs^ e) {
		if (tbKanniSearch->Text == "") { return; } // テキストエリアが空っぽなら何もしない。

		int iStart = GetKanniCBSelectedIndex() + 1; // 現在選択しているもののから次の候補

		Regex^ regex = gcnew Regex(tbKanniSearch->Text);

		bool isExist = false;
		// iStart以降を全部サーチして、
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

	// 官位検索テキストボックスでキーを押した時
	void Kanni_tbSearch_KeyDown(Object^ sender, KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Return) {
			Kanni_btnSearch_Click(sender, e);
		}
		if (e->KeyCode == Keys::F3) {
			Kanni_btnSearch_Click(sender, e);
		}
	}

	// 官位出力ボタンを押した時
	void Kanni_btnExport_Click(Object^ sender, EventArgs^ e) {
	}


	// 官位入力ボタンを押した時
	void Kanni_btnImport_Click(Object^ sender, EventArgs^ e) {
	}

	// 全て保存ボタンを押した時
	void Kanni_btnAllSave_Click(Object^ sender, EventArgs^ e) {
		bdKanniData->AllSaveToMessageN6P();
	}

	// フォントリストの選択項目が変更された時
	void Kanni_cbFont_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
		// フォントの設定
		fontKanni = gcnew System::Drawing::Font((String^)cbFontKanni->Items[cbFontKanni->SelectedIndex], 12, FontStyle::Regular);
		tbKanniFullName->Font = fontKanni;
		tbKanniRubyYomi->Font = fontKanni;
		tbKanniBornEtc->Font = fontKanni;
		tbKanniRetsuden->Font = fontKanni;
	}

	// インストールされているフォントにしたがって、フォントリストボックスに追加
	void Kanni_GetValidTenshoukiFontList() {

		//InstalledFontCollectionオブジェクトの取得
		System::Drawing::Text::InstalledFontCollection^ ifc = gcnew System::Drawing::Text::InstalledFontCollection();

		//インストールされているすべてのフォントファミリアを取得
		cli::array<FontFamily^>^ ffs = ifc->Families;

		//ここでは候補となりえるフォント名のみ
		for (int i = 0; i < aryStrFontCandidate->Length; i++) {

			// フォントリスト
			for each (FontFamily ^ ff in ffs) {

				// 候補フォントがあった。
				if (ff->Name == aryStrFontCandidate[i]) {
					// コンボボックスに追加
					cbFontKanni->Items->Add(ff->Name);
				}
			}
		}

		// １つ以上フォントがあったら、最初のものを選択しておく。
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
		// 焦点を見失っているようなら、最後に選択していたものを宛がう。
		int iSelectIndex = GetKanniCBSelectedIndex();
		if (iSelectIndex == 0xFFFFFFFF) {
			lbKanniList->SelectedIndex = iLastKanniListSelected;
		}
		else {
			iLastKanniListSelected = iSelectIndex;
		}
	}


	// 官位リストを選択変更すると、画像の上の列伝各種の値が入れ替わる
	void Kanni_lbKanniList_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
		ModifyCBKanniSelectedIndex();
		// 新たなindexのデータを取得。
		Kanni_BDataToTextBox();

		ModifyCBKanniSelectedIndex();
	}


	// 官位データ→テキストボックスへ転写
	void Kanni_BDataToTextBox() {
		int iSelectedIndex = GetKanniCBSelectedIndex();
		tbKanniFullName->Text = bdKanniData->lstStrFullName[iSelectedIndex];
		tbKanniRubyYomi->Text = bdKanniData->lstStrRubyYomi[iSelectedIndex];
		tbKanniBornEtc->Text = bdKanniData->lstStrBornEtc[iSelectedIndex];
		tbKanniRetsuden->Text = bdKanniData->lstStrRetsuden[iSelectedIndex];
	}

	// テキストボックスの位置や幅を中身のテキスト量に従って変更
	void Kanni_TextBoxWidthUpdate() {
		tbKanniFullName->Width = GetStringMeasure(tbKanniFullName->Text);

		tbKanniRubyYomi->Left = tbKanniFullName->Right + 8;
		tbKanniRubyYomi->Width = GetStringMeasure(tbKanniRubyYomi->Text);

		tbKanniBornEtc->Left = tbKanniRubyYomi->Right + 8;
		tbKanniBornEtc->Width = GetStringMeasure(tbKanniBornEtc->Text);
	}

	// テキストボックスの、「官位姓名」が変更された。
	void Kanni_tbFullName_TextChanged(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetKanniCBSelectedIndex();
		bdKanniData->lstStrFullName[iSelectedIndex] = tbKanniFullName->Text;
		String^ strLine = String::Format("{0:000} - {1}", iSelectedIndex, tbKanniFullName->Text);
		// SelectIndexChangeが反応しないようにする。
		lbKanniList->SelectedIndexChanged -= gcnew EventHandler(this, &RetsudenEditorForm::Kanni_lbKanniList_SelectedIndexChanged);

		// 反応しないようにしたので、値を書き換える。SelectedIndexChangeメソッドが反応しないのでエラーが起きない。

		lbKanniList->Items[iSelectedIndex] = gcnew String(strLine); // 官位リストの名前の方を更新

		Kanni_TextBoxWidthUpdate();

		// 更新したので、反応を戻す。
		// SelectIndexChangeが反応するようにする。
		lbKanniList->SelectedIndexChanged += gcnew EventHandler(this, &RetsudenEditorForm::Kanni_lbKanniList_SelectedIndexChanged);
	}

	// テキストボックスの、「ふりがな」が変更された。
	void Kanni_tbRubyYomi_TextChanged(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetKanniCBSelectedIndex();
		bdKanniData->lstStrRubyYomi[iSelectedIndex] = tbKanniRubyYomi->Text;
		Kanni_TextBoxWidthUpdate();
	}

	// テキストボックスの、「生年等」が変更された。
	void Kanni_tbBornEtc_TextChanged(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetKanniCBSelectedIndex();
		bdKanniData->lstStrBornEtc[iSelectedIndex] = tbKanniBornEtc->Text;
		Kanni_TextBoxWidthUpdate();
	}

	// テキストボックスの、「列伝」が変更された。
	void Kanni_tbRetsuden_TextChanged(Object^ sender, EventArgs^ e) {
		int iSelectedIndex = GetKanniCBSelectedIndex();

		bdKanniData->lstStrRetsuden[iSelectedIndex] = tbKanniRetsuden->Text;
	}

	// テキストボックスから去った
	void Kanni_textBox_Leave(Object^ sender, EventArgs^ e) {
	}


	// 50枠追加ボタン
	void Kanni_btnAddKannis_Click(Object^ sender, EventArgs^ e) {
	}

	// 50枠削除ボタン
	void Kanni_btnDelKannis_Click(Object^ sender, EventArgs^ e) {
	}


};

