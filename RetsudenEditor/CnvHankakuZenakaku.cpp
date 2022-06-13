#include <string.h>

typedef struct
{
	unsigned char   hk[1];
	unsigned char   zh[2];
	unsigned char   zk[2];
}   hz1;

typedef struct
{
	unsigned char   hk[2];
	unsigned char   zh[2];
	unsigned char   zk[2];
}   hz2;


#define HZ2SIZE 26
#define HZ1SIZE 64

static  hz2 t2[HZ2SIZE] =
{
	{   0xB3,0xDE   /* ｳﾞ */,   0x83,0x94   /* ヴ */,   0x83,0x94   /* ヴ */    },
	{   0xB6,0xDE   /* ｶﾞ */,   0x82,0xAA   /* が */,   0x83,0x4B   /* ガ */    },
	{   0xB7,0xDE   /* ｷﾞ */,   0x82,0xAC   /* ぎ */,   0x83,0x4D   /* ギ */    },
	{   0xB8,0xDE   /* ｸﾞ */,   0x82,0xAE   /* ぐ */,   0x83,0x4F   /* グ */    },
	{   0xB9,0xDE   /* ｹﾞ */,   0x82,0xB0   /* げ */,   0x83,0x51   /* ゲ */    },
	{   0xBA,0xDE   /* ｺﾞ */,   0x82,0xB2   /* ご */,   0x83,0x53   /* ゴ */    },
	{   0xBB,0xDE   /* ｻﾞ */,   0x82,0xB4   /* ざ */,   0x83,0x55   /* ザ */    },
	{   0xBC,0xDE   /* ｼﾞ */,   0x82,0xB6   /* じ */,   0x83,0x57   /* ジ */    },
	{   0xBD,0xDE   /* ｽﾞ */,   0x82,0xB8   /* ず */,   0x83,0x59   /* ズ */    },
	{   0xBE,0xDE   /* ｾﾞ */,   0x82,0xBA   /* ぜ */,   0x83,0x5B   /* ゼ */    },
	{   0xBF,0xDE   /* ｿﾞ */,   0x82,0xBC   /* ぞ */,   0x83,0x5D   /* ゾ */    },
	{   0xC0,0xDE   /* ﾀﾞ */,   0x82,0xBE   /* だ */,   0x83,0x5F   /* ダ */    },
	{   0xC1,0xDE   /* ﾁﾞ */,   0x82,0xC0   /* ぢ */,   0x83,0x61   /* ヂ */    },
	{   0xC2,0xDE   /* ﾂﾞ */,   0x82,0xC3   /* づ */,   0x83,0x64   /* ヅ */    },
	{   0xC3,0xDE   /* ﾃﾞ */,   0x82,0xC5   /* で */,   0x83,0x66   /* デ */    },
	{   0xC4,0xDE   /* ﾄﾞ */,   0x82,0xC7   /* ど */,   0x83,0x68   /* ド */    },
	{   0xCA,0xDE   /* ﾊﾞ */,   0x82,0xCE   /* ば */,   0x83,0x6F   /* バ */    },
	{   0xCB,0xDE   /* ﾋﾞ */,   0x82,0xD1   /* び */,   0x83,0x72   /* ビ */    },
	{   0xCC,0xDE   /* ﾌﾞ */,   0x82,0xD4   /* ぶ */,   0x83,0x75   /* ブ */    },
	{   0xCD,0xDE   /* ﾍﾞ */,   0x82,0xD7   /* べ */,   0x83,0x78   /* ベ */    },
	{   0xCE,0xDE   /* ﾎﾞ */,   0x82,0xDA   /* ぼ */,   0x83,0x7B   /* ボ */    },
	{   0xCA,0xDF   /* ﾊﾟ */,   0x82,0xCF   /* ぱ */,   0x83,0x70   /* パ */    },
	{   0xCB,0xDF   /* ﾋﾟ */,   0x82,0xD2   /* ぴ */,   0x83,0x73   /* ピ */    },
	{   0xCC,0xDF   /* ﾌﾟ */,   0x82,0xD5   /* ぷ */,   0x83,0x76   /* プ */    },
	{   0xCD,0xDF   /* ﾍﾟ */,   0x82,0xD8   /* ぺ */,   0x83,0x79   /* ペ */    },
	{   0xCE,0xDF   /* ﾎﾟ */,   0x82,0xDB   /* ぽ */,   0x83,0x7C   /* ポ */    }
};

static  hz1 t1[HZ1SIZE] =
{
	{   0xA0    /*   */,    0x81,0x40   /*    */,   0x81,0x40   /*    */    },
	{   0xA4    /* ､ */,    0x81,0x41   /* 、 */,   0x81,0x41   /* 、 */    },
	{   0xA1    /* ｡ */,    0x81,0x42   /* 。 */,   0x81,0x42   /* 。 */    },
	{   0xA5    /* ･ */,    0x81,0x45   /* ・ */,   0x81,0x45   /* ・ */    },
	{   0xDE    /* ﾞ */,    0x81,0x4A   /* ゛ */,   0x81,0x4A   /* ゛ */    },
	{   0xDF    /* ﾟ */,    0x81,0x4B   /* ゜ */,   0x81,0x4B   /* ゜ */    },
	{   0xB0    /* ｰ */,    0x81,0x5B   /* ー */,   0x81,0x5B   /* ー */    },
	{   0xA2    /* ｢ */,    0x81,0x75   /* 「 */,   0x81,0x75   /* 「 */    },
	{   0xA3    /* ｣ */,    0x81,0x76   /* 」 */,   0x81,0x76   /* 」 */    },
	{   0xA7    /* ｧ */,    0x82,0x9F   /* ぁ */,   0x83,0x40   /* ァ */    },
	{   0xB1    /* ｱ */,    0x82,0xA0   /* あ */,   0x83,0x41   /* ア */    },
	{   0xA8    /* ｨ */,    0x82,0xA1   /* ぃ */,   0x83,0x42   /* ィ */    },
	{   0xB2    /* ｲ */,    0x82,0xA2   /* い */,   0x83,0x43   /* イ */    },
	{   0xA9    /* ｩ */,    0x82,0xA3   /* ぅ */,   0x83,0x44   /* ゥ */    },
	{   0xB3    /* ｳ */,    0x82,0xA4   /* う */,   0x83,0x45   /* ウ */    },
	{   0xAA    /* ｪ */,    0x82,0xA5   /* ぇ */,   0x83,0x46   /* ェ */    },
	{   0xB4    /* ｴ */,    0x82,0xA6   /* え */,   0x83,0x47   /* エ */    },
	{   0xAB    /* ｫ */,    0x82,0xA7   /* ぉ */,   0x83,0x48   /* ォ */    },
	{   0xB5    /* ｵ */,    0x82,0xA8   /* お */,   0x83,0x49   /* オ */    },
	{   0xB6    /* ｶ */,    0x82,0xA9   /* か */,   0x83,0x4A   /* カ */    },
	{   0xB7    /* ｷ */,    0x82,0xAB   /* き */,   0x83,0x4C   /* キ */    },
	{   0xB8    /* ｸ */,    0x82,0xAD   /* く */,   0x83,0x4E   /* ク */    },
	{   0xB9    /* ｹ */,    0x82,0xAF   /* け */,   0x83,0x50   /* ケ */    },
	{   0xBA    /* ｺ */,    0x82,0xB1   /* こ */,   0x83,0x52   /* コ */    },
	{   0xBB    /* ｻ */,    0x82,0xB3   /* さ */,   0x83,0x54   /* サ */    },
	{   0xBC    /* ｼ */,    0x82,0xB5   /* し */,   0x83,0x56   /* シ */    },
	{   0xBD    /* ｽ */,    0x82,0xB7   /* す */,   0x83,0x58   /* ス */    },
	{   0xBE    /* ｾ */,    0x82,0xB9   /* せ */,   0x83,0x5A   /* セ */    },
	{   0xBF    /* ｿ */,    0x82,0xBB   /* そ */,   0x83,0x5C   /* ソ */    },
	{   0xC0    /* ﾀ */,    0x82,0xBD   /* た */,   0x83,0x5E   /* タ */    },
	{   0xC1    /* ﾁ */,    0x82,0xBF   /* ち */,   0x83,0x60   /* チ */    },
	{   0xAF    /* ｯ */,    0x82,0xC1   /* っ */,   0x83,0x62   /* ッ */    },
	{   0xC2    /* ﾂ */,    0x82,0xC2   /* つ */,   0x83,0x63   /* ツ */    },
	{   0xC3    /* ﾃ */,    0x82,0xC4   /* て */,   0x83,0x65   /* テ */    },
	{   0xC4    /* ﾄ */,    0x82,0xC6   /* と */,   0x83,0x67   /* ト */    },
	{   0xC5    /* ﾅ */,    0x82,0xC8   /* な */,   0x83,0x69   /* ナ */    },
	{   0xC6    /* ﾆ */,    0x82,0xC9   /* に */,   0x83,0x6A   /* ニ */    },
	{   0xC7    /* ﾇ */,    0x82,0xCA   /* ぬ */,   0x83,0x6B   /* ヌ */    },
	{   0xC8    /* ﾈ */,    0x82,0xCB   /* ね */,   0x83,0x6C   /* ネ */    },
	{   0xC9    /* ﾉ */,    0x82,0xCC   /* の */,   0x83,0x6D   /* ノ */    },
	{   0xCA    /* ﾊ */,    0x82,0xCD   /* は */,   0x83,0x6E   /* ハ */    },
	{   0xCB    /* ﾋ */,    0x82,0xD0   /* ひ */,   0x83,0x71   /* ヒ */    },
	{   0xCC    /* ﾌ */,    0x82,0xD3   /* ふ */,   0x83,0x74   /* フ */    },
	{   0xCD    /* ﾍ */,    0x82,0xD6   /* へ */,   0x83,0x77   /* ヘ */    },
	{   0xCE    /* ﾎ */,    0x82,0xD9   /* ほ */,   0x83,0x7A   /* ホ */    },
	{   0xCF    /* ﾏ */,    0x82,0xDC   /* ま */,   0x83,0x7D   /* マ */    },
	{   0xD0    /* ﾐ */,    0x82,0xDD   /* み */,   0x83,0x7E   /* ミ */    },
	{   0xD1    /* ﾑ */,    0x82,0xDE   /* む */,   0x83,0x80   /* ム */    },
	{   0xD2    /* ﾒ */,    0x82,0xDF   /* め */,   0x83,0x81   /* メ */    },
	{   0xD3    /* ﾓ */,    0x82,0xE0   /* も */,   0x83,0x82   /* モ */    },
	{   0xAC    /* ｬ */,    0x82,0xE1   /* ゃ */,   0x83,0x83   /* ャ */    },
	{   0xD4    /* ﾔ */,    0x82,0xE2   /* や */,   0x83,0x84   /* ヤ */    },
	{   0xAD    /* ｭ */,    0x82,0xE3   /* ゅ */,   0x83,0x85   /* ュ */    },
	{   0xD5    /* ﾕ */,    0x82,0xE4   /* ゆ */,   0x83,0x86   /* ユ */    },
	{   0xAE    /* ｮ */,    0x82,0xE5   /* ょ */,   0x83,0x87   /* ョ */    },
	{   0xD6    /* ﾖ */,    0x82,0xE6   /* よ */,   0x83,0x88   /* ヨ */    },
	{   0xD7    /* ﾗ */,    0x82,0xE7   /* ら */,   0x83,0x89   /* ラ */    },
	{   0xD8    /* ﾘ */,    0x82,0xE8   /* り */,   0x83,0x8A   /* リ */    },
	{   0xD9    /* ﾙ */,    0x82,0xE9   /* る */,   0x83,0x8B   /* ル */    },
	{   0xDA    /* ﾚ */,    0x82,0xEA   /* れ */,   0x83,0x8C   /* レ */    },
	{   0xDB    /* ﾛ */,    0x82,0xEB   /* ろ */,   0x83,0x8D   /* ロ */    },
	{   0xDC    /* ﾜ */,    0x82,0xED   /* わ */,   0x83,0x8F   /* ワ */    },
	{   0xA6    /* ｦ */,    0x82,0xF0   /* を */,   0x83,0x92   /* ヲ */    },
	{   0xDD    /* ﾝ */,    0x82,0xF1   /* ん */,   0x83,0x93   /* ン */    }
};


/*
シングルバイト              (0x20 - 0x7E、0xA1 - 0xDF)
マルチバイト文字の先行バイト(0x81 - 0x9F、0xE0 - 0xFC)
マルチバイト文字の後続バイト(0x40 - 0x7E、0x80 - 0xFC)
無効な文字                  (0x20 - 0x7E、0xA1 - 0xDF、
0x81 - 0x9F、0xE0 - 0xFC 以外の文字)
*/



/*
全角カタカナを半角カタカナにする
*/
void zenkana2hankana
(
 unsigned char   *sin,   /* 入力文字列へのポインタ。ヌル終端文字列 */
 unsigned char   *sout   /* 出力文字列へのポインタ。ヌル終端文字列にして返す */
 )
{
	static unsigned char *pin,*pout,c[2],b2;
	static int slen,i;

	slen = strlen((const char *)sin);
	pin  = sin ;
	pout = sout;

	while(1)
	{
		if ( pin >= sin + slen ) break;

		if      ( ( 0x81 <= *pin && *pin <= 0x9F ) ||
			( 0xE0 <= *pin && *pin <= 0xFC ) )    /* 全角文字の１バイト目 */
		{
			c[0] = *pin;
			c[1] = *(pin+1);

			/* 2 → 2 */
			b2   = 0;
			for( i = 0 ; i < HZ2SIZE ; i++ )
			{
				if ( c[0] == t2[i].zk[0] &&
					c[1] == t2[i].zk[1]  )
				{
					*pout++ = t2[i].hk[0];
					*pout++ = t2[i].hk[1];
					pin += 2;
					b2 = 1;
					break;
				}
			}
			if ( b2 ) continue;

			/* 2 → 1 */
			b2 = 0;
			for( i = 0 ; i < HZ1SIZE ; i++ )
			{
				if ( c[0] == t1[i].zk[0] &&
					c[1] == t1[i].zk[1]  )
				{
					if (c[0] == 0x81 && c[1] == 0x40)    /* 全角スペース */
					{
						*pout++ = 0x20;
					}
					else
					{
						*pout++ = t1[i].hk[0];
					}
					pin += 2;
					b2 = 1;
					break;
				}
			}
			if ( b2 ) continue;

			*pout++ = *pin++;
			*pout++ = *pin++;
		}
		else
		{
			*pout++ = *pin++;
		}
	}
	*pout = 0x00;
}




/*
半角カタカナを全角カタカナにする
*/
void hankana2zenkana
(
 unsigned char   *sin,   /* 入力文字列へのポインタ。ヌル終端文字列 */
 unsigned char   *sout   /* 出力文字列へのポインタ。最大入力文字列の２倍のサイズが必要。ヌル終端文字列にして返す */
 )
{
	static unsigned char *pin,*pout,c[2],b2;
	static int slen,i;

	slen = strlen((const char *)sin);
	pin  = sin ;
	pout = sout;

	while(1)
	{
		if ( pin >= sin + slen ) break;

		if      ( ( 0x81 <= *pin && *pin <= 0x9F ) ||
			( 0xE0 <= *pin && *pin <= 0xFC ) )  /* 全角文字の１バイト目 */
		{
			*pout++ = *pin++;
			*pout++ = *pin++;
		}
		else if (   0xA0 <= *pin && *pin <= 0xDF   )  /* 半角カナ */
		{
			c[0] = *pin;
			c[1] = *(pin+1);
			b2   = 0;
			for( i = 0 ; i < HZ2SIZE ; i++ )
			{
				if ( c[0] == t2[i].hk[0] &&
					c[1] == t2[i].hk[1]  )
				{
					*pout++ = t2[i].zk[0];
					*pout++ = t2[i].zk[1];
					pin += 2;
					b2 = 1;
					break;
				}
			}
			if ( b2 ) continue;
			for( i = 0 ; i < HZ1SIZE ; i++ )
			{
				if ( c[0] == t1[i].hk[0]  )
				{
					*pout++ = t1[i].zk[0];
					*pout++ = t1[i].zk[1];
					pin ++;
					break;
				}
			}
		}
		else    /* 半角カナ以外の半角文字 */
		{
			*pout++ = *pin++;
		}
	}
	*pout = 0x00;
}

/*
半角カタカナを全角ひらがなにする
*/
void hankana2zenhira
(
 unsigned char   *sin,   /* 入力文字列へのポインタ。ヌル終端文字列 */
 unsigned char   *sout   /* 出力文字列へのポインタ。最大入力文字列の２倍のサイズが必要。ヌル終端文字列にして返す */
 )
{
	static unsigned char *pin,*pout,c[2],b2;
	static int slen,i;

	slen = strlen((const char *)sin);
	pin  = sin ;
	pout = sout;

	while(1)
	{
		if ( pin >= sin + slen ) break;

		if      ( ( 0x81 <= *pin && *pin <= 0x9F ) ||
			( 0xE0 <= *pin && *pin <= 0xFC ) )  /* 全角文字の１バイト目 */
		{
			*pout++ = *pin++;
			*pout++ = *pin++;
		}
		else if (   0xA0 <= *pin && *pin <= 0xDF   )  /* 半角カナ */
		{
			c[0] = *pin;
			c[1] = *(pin+1);
			b2   = 0;
			for( i = 0 ; i < HZ2SIZE ; i++ )
			{
				if ( c[0] == t2[i].hk[0] &&
					c[1] == t2[i].hk[1]  )
				{
					*pout++ = t2[i].zh[0];
					*pout++ = t2[i].zh[1];
					pin += 2;
					b2 = 1;
					break;
				}
			}
			if ( b2 ) continue;
			for( i = 0 ; i < HZ1SIZE ; i++ )
			{
				if ( c[0] == t1[i].hk[0]  )
				{
					*pout++ = t1[i].zh[0];
					*pout++ = t1[i].zh[1];
					pin ++;
					break;
				}
			}
		}
		else    /* 半角カナ以外の半角文字 */
		{
			*pout++ = *pin++;
		}
	}
	*pout = 0x00;
}




