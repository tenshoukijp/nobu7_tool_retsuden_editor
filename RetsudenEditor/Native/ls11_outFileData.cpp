#include <stdio.h>
#include <stdlib.h>
#pragma pack(1)

#pragma warning(push)
#pragma warning(disable:4996)

//ファイルからデータ入力
int ls11_outFileData(const char *filename, char *out, int outlen){
	FILE	*fp = NULL;
	int		dsize = 0;

	//ファイルオープン
	if((fp = fopen(filename,"wb")) == NULL){
		printf("Output File Open Err");
		return -1;
	} 
	//ファイル読込
	dsize = fwrite(out,sizeof(char),outlen,fp);
	//ファイルクローズ
	fclose(fp);
	return dsize;
}

#pragma warning(pop)
