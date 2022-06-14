#pragma once

#include <vector>
#include <string>

#include "CnvHankakuZenkaku.h"
#include "OnigRegex.h"

#include "ls11_mod.h"
#include "DotNetTestToMultiByte.h"


using namespace System;
using namespace System::Text;
using namespace System::Drawing;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Text::RegularExpressions;

const int iKanniIndexBgn = 36;
const int iKanniIndexEnd = 40;

char* szKanniTargetFileName = "message.nb7";

// ���傤�� vBufKanniDecodedData[0]=�P�Ԗڂ̕����t�@�C���̃f�[�^��AvBufKanniDecodedData[1]=�Q�Ԗڂ̕����t�@�C���̃f�[�^��A�݂����Ȋ���
vector<vector<byte>> vBufKanniDecodedData;


ref class KanniData {
public:
	List<String^>^ lstStrFullName; // ����
	List<String^>^ lstStrRubyYomi; // ��݂���
	List<String^>^ lstStrRetsuden; // ��`
	List<String^>^ lstStrBornEtc;  // ��`�̌��ɂ���悭�킩��Ȃ��f�[�^

public:
	KanniData() {
		lstStrFullName = gcnew List<String^>();
		lstStrRubyYomi = gcnew List<String^>();
		lstStrBornEtc = gcnew List<String^>();
		lstStrRetsuden = gcnew List<String^>();

		KanniData_Import();
	}

	//  �������u������
	std::string StrReplace(std::string String1, std::string String2, std::string String3)
	{
		std::string::size_type  Pos(String1.find(String2));

		while (Pos != std::string::npos)
		{
			String1.replace(Pos, String2.length(), String3);
			Pos = String1.find(String2, Pos + String3.length());
		}

		return String1;
	}

	std::string HankakuKatakanaToZentakuKatakana(std::string szBufOriginal) {

		std::string szBufCurrent = szBufOriginal;

		Matches matches;

		// ���̓���ȋL���͓V�ċL�ɂ����āA���p�J�^�J�i��S�p�J�^�J�i�ɂ���Ƃ����Ӗ�
		for (int i = 0; OnigMatch(szBufCurrent, "^(.*?)\x1b\x4b(.*?)\x1b\x48(.*?)$", &matches) && i < (int)szBufCurrent.size(); i++) {
			// ��`����v�f�𒊏o����B
			if (!matches[2].empty()) {
				char szBufZenKatakana[256] = "";
				hankana2zenkana((unsigned char*)matches[2].c_str(), (unsigned char*)szBufZenKatakana);
				szBufZenKatakana[255] = NULL; // ����ȏC�����Ă��\�����Ȃ��悤�O�̂���

				szBufCurrent = matches[1] + szBufZenKatakana + matches[3];
			}
		}

		// �Ō�̕���L�����Ȃ��܂܍s�����}����ꍇ������B
		if (OnigMatch(szBufCurrent, "^(.*?)\x1b\x4b(.+)$", &matches)) {

			// ��`����v�f�𒊏o����B
			if (!matches[2].empty()) {
				char szBufZenKatakana[256] = "";
				hankana2zenkana((unsigned char*)matches[2].c_str(), (unsigned char*)szBufZenKatakana);
				szBufZenKatakana[255] = NULL; // ����ȏC�����Ă��\�����Ȃ��悤�O�̂���

				szBufCurrent = matches[1] + szBufZenKatakana;
			}
		}

		return szBufCurrent;

	}


	// ���O�n��Unmanaged��Managed�ւƒ��o�B
	void NameAssignManagedData(char* pUnmanagedLine) {
		string line = string(pUnmanagedLine);
		Matches matches;
		if (OnigMatch(line, "^(.+?)\x81\x40\x1b\x6b(.+?)\x05\x05\x05$", &matches)) {

			matches[1] = HankakuKatakanaToZentakuKatakana(matches[1]);

			String^ strFullName = gcnew String(matches[1].c_str());
			lstStrFullName->Add(strFullName);

			String^ strRubyYomi = gcnew String(matches[2].c_str());
			lstStrRubyYomi->Add(strRubyYomi);

			// �_�~�[�B���ƃR���|�[�l���g�̐������킹�邽�߂����̂���
			lstStrBornEtc->Add(gcnew String(""));
		}
	}

	// ���Add�łł͂Ȃ��āA�㏑���š�� �Ђǂ��\�[�X�ɂȂ��Ă���orz
	BOOL NameOverrideManagedData(char* pUnmanagedLine, int iSelectedIndex) {
		string line = string(pUnmanagedLine);
		Matches matches;
		if (OnigMatch(line, "^(.+?)\x81\x40\x1b\x6b(.+?)\x05\x05\x05$", &matches)) {

			matches[1] = HankakuKatakanaToZentakuKatakana(matches[1]);

			// �I�����Ă���Ƃ���ɏ㏑��
			String^ strFullName = gcnew String(matches[1].c_str());
			lstStrFullName[iSelectedIndex] = strFullName;

			String^ strRubyYomi = gcnew String(matches[2].c_str());
			lstStrRubyYomi[iSelectedIndex] = strRubyYomi;

			lstStrBornEtc->Add(gcnew String(""));

			return TRUE;
		}
		return FALSE;
	}

	// ��`�n��Unmanaged��Managed�ւƒ��o�B
	String^ RetsudenAssignManagedData(char* pUnmanagedLine) {

		string szBufCurrent = string(pUnmanagedLine);

		szBufCurrent = HankakuKatakanaToZentakuKatakana(szBufCurrent);

		Matches matches;
		// �Ō�̏I���L���ȍ~�͏����B
		if (OnigMatch(szBufCurrent, "^(.*?)\x05\x05\x05$", &matches)) {

			if (!matches[1].empty()) {
				// �c��́A���p�J�^�J�i��S�p�Ђ炪�Ȃ�
				char szBufZenHiragana[256] = "";
				hankana2zenhira((unsigned char*)matches[1].c_str(), (unsigned char*)szBufZenHiragana);
				szBufZenHiragana[255] = NULL; // ����ȏC�����Ă��\�����Ȃ��悤�O�̂���

				string strZenHiragana = string(szBufZenHiragana);

				// ���s�I�Ӗ������̂��̂����ȋL���ɒu��������B�������̂��߁B
				for (int i = 0; OnigMatch(strZenHiragana, "^(.*?)\n(.+)", &matches) && i < (int)strZenHiragana.size(); i++) {
					// ��`����v�f�𒊏o����B
					if (!matches[1].empty()) {
						strZenHiragana = matches[1] + "<r><n>" + matches[2];
					}
				}

				// ����ȋL�����A��`�G�f�B�^��ł̌����ڂ̉��s�ւƕϊ��B
				for (int i = 0; OnigMatch(strZenHiragana, "^(.*?)<r><n>(.+)", &matches) && i < (int)strZenHiragana.size(); i++) {
					// ��`����v�f�𒊏o����B
					if (!matches[1].empty()) {
						strZenHiragana = matches[1] + "\r\n" + matches[2];
					}
				}
				return gcnew String(strZenHiragana.c_str());
			}
		}

		return gcnew String("");
	}



	// message.nb7���z��ւƊi�[�B
	int KanniData_Import() {
		if (!System::IO::File::Exists(gcnew String(szKanniTargetFileName))) {
			System::Windows::Forms::MessageBox::Show(gcnew String(szKanniTargetFileName) + "��ǂݍ��߂܂���", "�G���[", MessageBoxButtons::OK, ::MessageBoxIcon::Error);
		}

		// ���k���ꂽLS11�t�H�[�}�b�g���f�R�[�h����֐�
		ls11_DecodePack(szKanniTargetFileName, NULL, &vBufKanniDecodedData);

		int iRetsudenNo = -1;

		// �f�R�[�h�����f�[�^��(�����I)�t�@�C���ł܂킵�Ă����B0�Ԃ͑Ώۂł͂Ȃ��B�ƕ�f�[�^��1�Ԃ���B
		int ivBufDecodedDataSize = vBufKanniDecodedData.size();

		// 1�Ԗڂ���X�^�[�g���Ă���!! ����!!
		for (int ifile = iKanniIndexBgn; ifile <= iKanniIndexEnd && ifile < (int)ivBufDecodedDataSize; ifile++) {

			// ���傤�� vSplittedData[0]=�P�Ԗڂ̗v�f�̃f�[�^��AvSplittedData[1]=�Q�Ԗڂ̗v�f�̃f�[�^��A�݂����Ȋ���
			vector<vector<byte>> vSplittedData;

			ls11_SplitData(vBufKanniDecodedData[ifile], &vSplittedData);

			int ivSplittedDataSize = vSplittedData.size();

			// �e�v�f���Ƃɥ�� �܂����O�A�ǂ݉��� ���v�N�̗�
			for (int ielem = 0; ielem < (int)ivSplittedDataSize; ielem++) {
				if (ielem % 2 == 0) { iRetsudenNo++; }

				if (iRetsudenNo < 0) { iRetsudenNo = 0; }; // �ň��̂��߂̔ԕ�

				// �������ǂ݉����Ƃ�
				if (ielem % 2 == 0) {

					int datasize = vSplittedData[ielem].size();
					if (datasize > 0) {

						char* bytedata = new char[datasize + 1];
						memcpy(bytedata, &vSplittedData[ielem][0], datasize);
						bytedata[datasize] = NULL; // �ԕ�

						NameAssignManagedData(bytedata); // �}�l�[�W�h�f�[�^�ւƑ��

						delete bytedata;
					}


					// ��͗�`�B
				}
				else {

					string szBufCurrent;

					int datasize = vSplittedData[ielem].size();
					if (datasize > 0) {

						char* bytedata = new char[datasize + 1];
						memcpy(bytedata, &vSplittedData[ielem][0], datasize);
						bytedata[datasize] = NULL; // �ԕ�
						szBufCurrent = string(bytedata);

						String^ strRetsuden = RetsudenAssignManagedData(bytedata); // �}�l�[�W�h�f�[�^�ւƑ��
						lstStrRetsuden->Add(strRetsuden);

						delete bytedata;
					}

				}
			}
		}

		// ���v��`����Ԃ��B
		return iRetsudenNo + 1;
	}

	// �Ƃ���P�l�̉ƕ�f�[�^�̏o�͗p�o�C�i���f�[�^�𓾂�
	std::string GetOneKanniExportData(int iSelectedIndex) {
		String^ managedData = String::Format("{0}\x81\x40\x1b\x6b{1}\x05\x05\x05{2}", lstStrFullName[iSelectedIndex], lstStrRubyYomi[iSelectedIndex], lstStrRetsuden[iSelectedIndex]);

		return GetOutputFormatNormalizeData(managedData);

	}

	std::string GetOutputFormatNormalizeData(String^ managedData) {
		// .NET�^�C�v��String^ �� �}���`�o�C�g��C++�^std::string��
		std::string unmanagedData = DotNetStringToMultiByte(managedData);

		Matches matches;
		// ����ȋL�����A��`�G�f�B�^��ł̌����ڂ̉��s�ւƕϊ��B
		for (int i = 0; OnigMatch(unmanagedData, "^(.*?)\r\n(.+)", &matches) && i < (int)unmanagedData.size(); i++) {
			// ��`����v�f�𒊏o����B
			if (!matches[1].empty()) {
				unmanagedData = matches[1] + "\x0A" + matches[2];
			}
		}

		return unmanagedData;
	}

	// �Ƃ���P�l�̉ƕ�f�[�^���t�@�C���������
	BOOL GetOneKanniImportData(String^ managedFilePath, int iSelectedIndex) {
		std::string unmanagedFilePath = DotNetStringToMultiByte(managedFilePath);

		FILE* fp = fopen(unmanagedFilePath.c_str(), "rb");
		if (fp) {
			char pUnmanagedLine[512] = "";
			fread(pUnmanagedLine, sizeof(pUnmanagedLine) - 1, 1, fp); // ��C�ɓǂݍ���
			fclose(fp);


			string line = string(pUnmanagedLine);
			Matches matches;
			if (OnigMatch(line, "^\x1b\x6b(.*?)\x1b\x48(.*?\x05\x05\x05$)", &matches)) {

				BOOL ret1 = NameOverrideManagedData(pUnmanagedLine, iSelectedIndex);

				if (!ret1) {
					System::Windows::Forms::MessageBox::Show("�f�[�^�̎捞�Ɏ��s���܂����B", "�G���[", MessageBoxButtons::OK, ::MessageBoxIcon::Error);
					return FALSE;
				}

				// ��`���������n��
				String^ strRetsuden = RetsudenAssignManagedData((char*)matches[3].c_str());

				lstStrRetsuden[iSelectedIndex] = strRetsuden;

				System::Windows::Forms::MessageBox::Show("�f�[�^����荞�݂܂����B�B", "����", MessageBoxButtons::OK, ::MessageBoxIcon::Information);
				return TRUE;
			}

		}
		else {
			System::Windows::Forms::MessageBox::Show("�t�@�C���̃I�[�v���Ɏ��s���܂����B", "�G���[", MessageBoxButtons::OK, ::MessageBoxIcon::Error);
			return FALSE;
		}

		return FALSE;
	}

	BOOL AllSaveToMessageN6P() {
		// �ƕ��`�̒����𖞂����Ă��Ȃ���΁A�_��
		if (vBufKanniDecodedData.size() < iKanniIndexEnd) {
			return FALSE;
		}
		// �ƕ��`�������Ă���R�̏����N���A�B
		vBufKanniDecodedData[iKanniIndexBgn].clear();
		vBufKanniDecodedData[37].clear();
		vBufKanniDecodedData[38].clear();
		vBufKanniDecodedData[39].clear();
		vBufKanniDecodedData[iKanniIndexEnd].clear();

		// �ƕ��`�S�Ăɂ��ĥ��
		int iFileCnt = iKanniIndexEnd - iKanniIndexBgn + 1; // �쐬����t�@�C����(36,37,38,39,40)

		// ���ꂼ��̉ƕ�ԍ��̊J�n�ԍ��𑗂�Ȃ���A���z�t�@�C���t�@�C�����J��Ԃ��BvBufKanniDecodedData �ɕt���������Ă䂭�B
		for (int f = 0; f < iFileCnt; f++) {
			MakeSplittedDataToJoindData(f * 50, iKanniIndexBgn + f);
		}

		// ���������p�b�N���C���[�W
		int result = ls11_EncodePack(szKanniTargetFileName, NULL, &vBufKanniDecodedData);

		if (result == 0) {
			System::Windows::Forms::MessageBox::Show(gcnew String(szKanniTargetFileName) + "�ւƏo�͂��܂����B", "����", MessageBoxButtons::OK, ::MessageBoxIcon::Information);
			return TRUE;
		}
		else {
			System::Windows::Forms::MessageBox::Show("�\�����ʃG���[���������܂����B", "�G���[", MessageBoxButtons::OK, ::MessageBoxIcon::Error);
			return FALSE;
		}
	}

	// �P�̉��z�t�@�C��(�������ꂽ���̑���)����������ɍ��o���B
	void MakeSplittedDataToJoindData(int iStartIndex, int iAssingIndex) {
		vector<vector<byte>> vSplittedData; // 50 ��l����������ł䂭�f�[�^
		vector<byte> vDstJoinedData; // 50 ��l����������ł䂭�f�[�^

		// 50�l��
		for (int i = iStartIndex; (i < iStartIndex + 50) && (i < lstStrFullName->Count); i++) {
			vSplittedData.push_back(GetNameLineData(i));
			vSplittedData.push_back(GetRetsudenLineData(i));
		}

		// �����������̂����ւƖ߂��B���P�t�@�C������������
		ls11_JoinData(vSplittedData, &vDstJoinedData);

		// �����S�̃f�[�^�ɉ�����B
		vBufKanniDecodedData[iAssingIndex] = vDstJoinedData;
	}

	// �Ƃ���P�l���̉ƕ�̖��O���N�n�̃f�[�^
	vector<byte> GetNameLineData(int iTargetIndex) {

		String^ format = String::Format("{0}\x81\x40\x1b\x6b{1}\x05\x05\x05", lstStrFullName[iTargetIndex], lstStrRubyYomi[iTargetIndex]);

		std::string unmanagedData = GetOutputFormatNormalizeData(format);

		char* psz = (char*)unmanagedData.c_str();

		// ��C��char[]��vector<byte>�ւƃR�s�[
		vector<byte> line(&psz[0], &psz[strlen(psz)]);

		return line;
	}

	// �Ƃ���1�l���̉ƕ�̗�`�f�[�^
	vector<byte> GetRetsudenLineData(int iTargetIndex) {
		String^ format = String::Format("{0}\x05\x05\x05", lstStrRetsuden[iTargetIndex]);

		std::string unmanagedData = GetOutputFormatNormalizeData(format);

		char* psz = (char*)unmanagedData.c_str();

		// ��C��char[]��vector<byte>�ւƃR�s�[
		vector<byte> line(&psz[0], &psz[strlen(psz)]);

		return line;

	}

};