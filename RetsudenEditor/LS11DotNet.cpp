// ����� ���C�� DLL �t�@�C���ł��B

#include "ls11_mod.h"
#include "LS11DotNet.h"

#include <windows.h>
#include <vector>

using namespace std;
using namespace LS11DotNet;

int Ls11::DecodePack(String^ csFileName, ArrayList^ csDstArrayList) {

	//--------------------String^ �� char* ��
	// .net��Unicode��filename���}���`�o�C�g��char *�Ƃ���B
	std::string bufFileName;  // ��������󂯎~�߂�o�b�t�@�����˂�B

	// �ȉ��uString^�v�^��string�ɂ��邽�߂̂��܂��Ȃ��ɋ߂��B
	{
		IntPtr mptr = Marshal::StringToHGlobalAnsi(csFileName); // Unicode���}���`�o�C�g��Ansi��
		bufFileName = static_cast<const char*>(mptr.ToPointer()); // IntPtr��char *�փL���X�g
		Marshal::FreeHGlobal(mptr); // mptr�͓��I�m�ۂ������̂ŉ��
	}
	char *szFileName = (char *)bufFileName.c_str();
	vector<vector<byte>> pvecDstDataArray;

	// C++ Native�Ńf�R�[�h
	int result = ls11_DecodePack(szFileName, NULL, &pvecDstDataArray);

	//--------------------vector<vector<byte>> �� ArrayList^<ArrayList^<Byte>>��
	// ���J��œ����B�x�����ȁ` �܂�.net�̂���̓G�f�B�^�쐬�p�Ƃ��p�Ȃ�ł�
	for ( int iPackedIX=0; iPackedIX < (int)pvecDstDataArray.size(); iPackedIX++ ) {
		// ���ɓ����
		ArrayList^ OneData = gcnew ArrayList();
		for ( int i=0; i<(int)pvecDstDataArray[iPackedIX].size(); i++ ) {
			OneData->Add(System::Byte(pvecDstDataArray[iPackedIX][i]));
		}
		csDstArrayList->Add(OneData);
	}

	return result;
}

int Ls11::EncodePack(String^ csFileName, ArrayList^ csSrcArrayList) {

	//--------------------String^ �� char* ��
	// .net��Unicode��filename���}���`�o�C�g��char *�Ƃ���B
	std::string bufFileName;  // ��������󂯎~�߂�o�b�t�@�����˂�B

	// �ȉ��uString^�v�^��string�ɂ��邽�߂̂��܂��Ȃ��ɋ߂��B
	{
		IntPtr mptr = Marshal::StringToHGlobalAnsi(csFileName); // Unicode���}���`�o�C�g��Ansi��
		bufFileName = static_cast<const char*>(mptr.ToPointer()); // IntPtr��char *�փL���X�g
		Marshal::FreeHGlobal(mptr); // mptr�͓��I�m�ۂ������̂ŉ��
	}
	char *szFileName = (char *)bufFileName.c_str();

	//--------------------ArrayList^<ArrayList^<Byte>> �� vector<vector<byte>> ��
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

	// C++ Native�Ńf�R�[�h
	int result = ls11_EncodePack(szFileName, NULL, &pvecDstDataArray);
	return result;
}

// �P�̈�����o�C�i���f�[�^���A�Ӗ��P�ʂŕ�������
int Ls11::SplitData(ArrayList^ csSrcJoinedData, ArrayList^ csDstSplittedData) {

	// CSharp��ArrayList��vector<byte>�ɃR�s�[
	vector<byte> vSrcJoinedData;
	for ( int i=0; i<(int)csSrcJoinedData->Count; i++ ) {
		byte b = static_cast<byte>(csSrcJoinedData->default[i]);
		vSrcJoinedData.push_back(b);
	}

	// 1�̃f�[�^���Ӗ��P�ʂɕ�������
	vector<vector<byte>> vDstSplittedData;
	ls11_SplitData(vSrcJoinedData, &vDstSplittedData);

	//--------------------vector<vector<byte>> �� ArrayList^<ArrayList^<Byte>>��
	// ���J��œ����B�x�����ȁ` �܂�.net�̂���̓G�f�B�^�쐬�p�Ƃ��p�Ȃ�ł�
	for ( int iSplittedIX=0; iSplittedIX < (int)vDstSplittedData.size(); iSplittedIX++ ) {
		// ���ɓ����
		ArrayList^ OneData = gcnew ArrayList();
		for ( int i=0; i<(int)vDstSplittedData[iSplittedIX].size(); i++ ) {
			OneData->Add(System::Byte(vDstSplittedData[iSplittedIX][i]));
		}
		csDstSplittedData->Add(OneData);
	}

	int result = TRUE;
	return TRUE;
}

// �����������̂����ւƖ߂��B
int Ls11::JoinData(ArrayList^ csSrcSplittedData, ArrayList^ csDstJoinedData) {

	// CSharp��ArrayList��vector<vector<byte>>�ɃR�s�[
	vector<vector<byte>> vSrcSplittedData;
	// �܂��̓R�s�[����O�ɐ������킹��
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

	// �����������̂����ւƖ߂��B
	ls11_JoinData(vSrcSplittedData, &vDstJoinedData);

	// vector<byte>��ArrayList<Byte>��
    ArrayList^ OneData = gcnew ArrayList();
	for ( int i=0; i<(int)vDstJoinedData.size(); i++ ) {
		csDstJoinedData->Add(System::Byte(vDstJoinedData[i]));
	}

	int result = TRUE;
	return TRUE;
}
