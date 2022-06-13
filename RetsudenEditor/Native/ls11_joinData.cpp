#include "ls11_mod.h"



// 意味単位で分解されたデータを、元へとつなげた状態に戻す
void ls11_JoinData(vector<vector<byte>> &vSrcSplittedData, vector<byte> *vDstJoinedData) {

	// 消去
	vDstJoinedData->clear();

	/*
	 * アドレス部分の計算と連結
	 */
	// まず、全てのデータに関して、各々のサイズ(バイト数)を求める。
	// それをもとに、データの最初に、データ開始アドレスの配列を書き込んでしまわなければならない。
	// 共用体にしたいのではなくて、int⇒byteのキャストでソースが汚れるの回避ｗ
	union STARTADDRESS {
		WORD iCurDataStartAddress;
		byte szCurDataStartAddress[2];
	};
	STARTADDRESS sa;
	sa.iCurDataStartAddress=0;

	// 最初は全体としてのデータの個数 * 2がアドレスのスタート地点となる。ここの部分にアドレスの一覧が１つあたりWORDの単位で入る。
	// さらに先頭に２バイト分の「個数」が追加されるので+2
	sa.iCurDataStartAddress += (WORD)vSrcSplittedData.size() * 2 + 2;
	vDstJoinedData->push_back( sa.szCurDataStartAddress[0] );
	vDstJoinedData->push_back( sa.szCurDataStartAddress[1] );

	// 最後の１つは、書く必要がないので、-1。最後のひとつはファイル終端のアドレスとなるが、これは、データに含める必要がない。
	for ( int i=0; i<(int)vSrcSplittedData.size()-1; i++) {
		sa.iCurDataStartAddress += (WORD)vSrcSplittedData[i].size();
		vDstJoinedData->push_back( sa.szCurDataStartAddress[0] );
		vDstJoinedData->push_back( sa.szCurDataStartAddress[1] );
	}

	sa.iCurDataStartAddress = (WORD)vSrcSplittedData.size();

	// 先頭にデータの個数を2バイトで挿入(リトルエンディアン)
	vDstJoinedData->insert(vDstJoinedData->begin(), sa.szCurDataStartAddress[1]);
	vDstJoinedData->insert(vDstJoinedData->begin(), sa.szCurDataStartAddress[0]);

	/*
	 * データ部分の連結
	 */
	for ( int i=0; i<(int)vSrcSplittedData.size(); i++ ) {
		vDstJoinedData->insert( vDstJoinedData->end(), vSrcSplittedData[i].begin(), vSrcSplittedData[i].end() ); 
	}

}
