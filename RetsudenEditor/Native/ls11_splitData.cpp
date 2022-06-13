#include "ls11_mod.h"


// LS11系を分解して、１つ１つのファイルにした時、なおもそのファイルが、扱い難いバイナリデータである。
// このバイナリデータは、そのままだと、各データの長さを変えてはいけないのだ。
// よって、これを意味単位で分解し、各々の長さの変更を伴うような修正を可能とする。
// ls11_SplitDataしたものは、ls11_JoinDataすれば、連結することが可能だ。
void ls11_SplitData(vector<byte> &vSrcJoinedData, vector<vector<byte>> *vDstSplittedData) {

	// 消去
	vDstSplittedData->clear();

	// ダミーデータを最後にくっつける。
	// の方で、 &vOrgData[vAddressList[i+1]] とアクセスしてるのが、範囲外アクセスエラーにならないようにするため。
	vSrcJoinedData.push_back( byte(0) );

	WORD *pAddressData = (WORD *)&vSrcJoinedData[0];
	WORD iAddressCnt = (*pAddressData) / 1;   // １つのアドレスあたり2バイトなので、最初のデータ位置指定アドレスである、&vOrgData[0] を２で割ると、アドレス指定が何個あるのかが算出できる。

	// 各データ開始アドレスの一覧
	vector<WORD> vAddressList;
	for (int i=0; i<iAddressCnt; i++) {
		vAddressList.push_back( pAddressData[i+1] );
	}
	// ファイルの長さを入れることで、最後のアドレスとする。
	// こちら、下のvDataListより要素が１つ大きくなるので、このvAddressList.size()を以降のステートメントに使わないこと!!
	vAddressList.push_back( vSrcJoinedData.size()-1 ); // ダミーデータの分マイナス1してる。
	

	// 各データの一覧
	for (int i=0; i<iAddressCnt; i++) {
		// vOrgDataの指定のアドレス～次の指定のアドレスまでを１つのデータとして格納する。
		vector<byte> vData( &vSrcJoinedData[vAddressList[i]], &vSrcJoinedData[vAddressList[i+1]]); 
		vDstSplittedData->push_back( vData );
	}

	// 最初にくっつけたダミーデータを除去
	vSrcJoinedData.pop_back();
}

/*
void main (void) {
	ifstream ifs;
	size_t iFileSize;

	ifs.open("C:\\bfile\\bfile.001", ios::binary);
	iFileSize = (size_t)ifs.seekg(0, std::ios::end).tellg();
	ifs.seekg(0, std::ios::beg);
	
	byte tmp[10000];
    
    ifs.read( ( char * ) tmp, iFileSize );

	vector<byte> v(&tmp[0], &tmp[iFileSize]);

	vector<vector<byte>> vSplittedData;
	ls11_SplitData( v, &vSplittedData );

	char *test = (char *)&vSplittedData[3][0];

	// 何らかの変更をする際には、一度該当のvector要素をクリアした方が良い。
	// そうでないと、長さが短くならないだろう。

	vector<byte> vv;
	ls11_JoinData( vSplittedData, &vv );

	for ( int i=0; i<8000; i++ ) {
		if (tmp[i] != vv[i]) {
			MessageBox(NULL, "OK", "OK", NULL);
		}
	}

	return;
}
*/