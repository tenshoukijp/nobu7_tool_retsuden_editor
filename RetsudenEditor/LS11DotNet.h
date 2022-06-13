// LS11DotNet.h

#pragma once

using namespace System;
using namespace System::Collections;
using namespace System::Runtime::InteropServices;

namespace LS11DotNet {

	public ref class Ls11 {
	public:
		//  LS11��***.n6p�����A�o���o���ɕ������f�[�^��ϐ��Ɋi�[����BLS11 Archiever��z������Ηǂ��B
		static int DecodePack(String^ csFilename, ArrayList^ csArrayList);

		// �t�@�C���A���邢�́A�ϐ��̃f�[�^���A�P��LS11�t�@�C���Ƀp�b�N������B
		static int EncodePack(String^ csFilename, ArrayList^ csArrayList);

		// �P�̈�����o�C�i���f�[�^���A�Ӗ��P�ʂŕ�������
		static int SplitData(ArrayList^ csSrcJoinedData, ArrayList^ csDstSplittedData);

		// �����������̂����ւƖ߂��B
		static int JoinData(ArrayList^ csSrcSplittedData, ArrayList^ csvDstJoinedData);
	};
}
