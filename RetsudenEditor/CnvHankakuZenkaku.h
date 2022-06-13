#pragma once


/*
全角カタカナを半角カタカナにする
*/
void zenkana2hankana(
	unsigned char   *sin,   /* 入力文字列へのポインタ。ヌル終端文字列 */
	unsigned char   *sout   /* 出力文字列へのポインタ。ヌル終端文字列にして返す */
 );


/*
半角カタカナを全角カタカナにする
*/
void hankana2zenkana(
	unsigned char   *sin,   /* 入力文字列へのポインタ。ヌル終端文字列 */
	unsigned char   *sout   /* 出力文字列へのポインタ。最大入力文字列の２倍のサイズが必要。ヌル終端文字列にして返す */
 );

/*
半角カタカナを全角ひらがなにする
*/
void hankana2zenhira(
	unsigned char   *sin,   /* 入力文字列へのポインタ。ヌル終端文字列 */
	unsigned char   *sout   /* 出力文字列へのポインタ。最大入力文字列の２倍のサイズが必要。ヌル終端文字列にして返す */
 );
