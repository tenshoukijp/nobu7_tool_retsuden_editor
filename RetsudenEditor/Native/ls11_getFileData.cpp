#include <stdio.h>
#include <stdlib.h>
#pragma pack(1)

#pragma warning(push)
#pragma warning(disable:4996)

//ファイルからデータ入力
int 	ls11_getFileData(const char *filename, char *out, int outlen){
	FILE	*fp = NULL;
	fpos_t	fsize = 0;
	int		dsize = 0;

	//ファイルオープン
	if((fp = fopen(filename,"rb")) == NULL){
		printf("Input File Open Err");
		return -1;
	} 
	/* ファイルサイズを調査 */ 
	fseek(fp,0,SEEK_END); 
	fgetpos(fp,&fsize);
	fseek(fp, 0L, SEEK_SET);
	
	if(outlen > fsize){
		outlen = (int)fsize;
	}

	//ファイル読込
	dsize = fread(out,sizeof(char),outlen,fp);
	//ファイルクローズ
	fclose(fp);
	return dsize;
}

//ファイルのサイズ取得
int 	ls11_getFileSize(const char *filename){
	FILE	*fp;
	fpos_t fsize = 0;
	
	if((fp = fopen(filename,"rb")) == NULL){
		printf("Input File Open Err");
		return -1;
	}
 
	/* ファイルサイズを調査 */ 
	fseek(fp,0,SEEK_END); 
	fgetpos(fp,&fsize); 
 
	fclose(fp);
 
	return (int)fsize;
}

#pragma warning(pop)
