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
	{   0xB3,0xDE   /* �� */,   0x83,0x94   /* �� */,   0x83,0x94   /* �� */    },
	{   0xB6,0xDE   /* �� */,   0x82,0xAA   /* �� */,   0x83,0x4B   /* �K */    },
	{   0xB7,0xDE   /* �� */,   0x82,0xAC   /* �� */,   0x83,0x4D   /* �M */    },
	{   0xB8,0xDE   /* �� */,   0x82,0xAE   /* �� */,   0x83,0x4F   /* �O */    },
	{   0xB9,0xDE   /* �� */,   0x82,0xB0   /* �� */,   0x83,0x51   /* �Q */    },
	{   0xBA,0xDE   /* �� */,   0x82,0xB2   /* �� */,   0x83,0x53   /* �S */    },
	{   0xBB,0xDE   /* �� */,   0x82,0xB4   /* �� */,   0x83,0x55   /* �U */    },
	{   0xBC,0xDE   /* �� */,   0x82,0xB6   /* �� */,   0x83,0x57   /* �W */    },
	{   0xBD,0xDE   /* �� */,   0x82,0xB8   /* �� */,   0x83,0x59   /* �Y */    },
	{   0xBE,0xDE   /* �� */,   0x82,0xBA   /* �� */,   0x83,0x5B   /* �[ */    },
	{   0xBF,0xDE   /* �� */,   0x82,0xBC   /* �� */,   0x83,0x5D   /* �] */    },
	{   0xC0,0xDE   /* �� */,   0x82,0xBE   /* �� */,   0x83,0x5F   /* �_ */    },
	{   0xC1,0xDE   /* �� */,   0x82,0xC0   /* �� */,   0x83,0x61   /* �a */    },
	{   0xC2,0xDE   /* �� */,   0x82,0xC3   /* �� */,   0x83,0x64   /* �d */    },
	{   0xC3,0xDE   /* �� */,   0x82,0xC5   /* �� */,   0x83,0x66   /* �f */    },
	{   0xC4,0xDE   /* �� */,   0x82,0xC7   /* �� */,   0x83,0x68   /* �h */    },
	{   0xCA,0xDE   /* �� */,   0x82,0xCE   /* �� */,   0x83,0x6F   /* �o */    },
	{   0xCB,0xDE   /* �� */,   0x82,0xD1   /* �� */,   0x83,0x72   /* �r */    },
	{   0xCC,0xDE   /* �� */,   0x82,0xD4   /* �� */,   0x83,0x75   /* �u */    },
	{   0xCD,0xDE   /* �� */,   0x82,0xD7   /* �� */,   0x83,0x78   /* �x */    },
	{   0xCE,0xDE   /* �� */,   0x82,0xDA   /* �� */,   0x83,0x7B   /* �{ */    },
	{   0xCA,0xDF   /* �� */,   0x82,0xCF   /* �� */,   0x83,0x70   /* �p */    },
	{   0xCB,0xDF   /* �� */,   0x82,0xD2   /* �� */,   0x83,0x73   /* �s */    },
	{   0xCC,0xDF   /* �� */,   0x82,0xD5   /* �� */,   0x83,0x76   /* �v */    },
	{   0xCD,0xDF   /* �� */,   0x82,0xD8   /* �� */,   0x83,0x79   /* �y */    },
	{   0xCE,0xDF   /* �� */,   0x82,0xDB   /* �� */,   0x83,0x7C   /* �| */    }
};

static  hz1 t1[HZ1SIZE] =
{
	{   0xA0    /*   */,    0x81,0x40   /*    */,   0x81,0x40   /*    */    },
	{   0xA4    /* � */,    0x81,0x41   /* �A */,   0x81,0x41   /* �A */    },
	{   0xA1    /* � */,    0x81,0x42   /* �B */,   0x81,0x42   /* �B */    },
	{   0xA5    /* � */,    0x81,0x45   /* �E */,   0x81,0x45   /* �E */    },
	{   0xDE    /* � */,    0x81,0x4A   /* �J */,   0x81,0x4A   /* �J */    },
	{   0xDF    /* � */,    0x81,0x4B   /* �K */,   0x81,0x4B   /* �K */    },
	{   0xB0    /* � */,    0x81,0x5B   /* �[ */,   0x81,0x5B   /* �[ */    },
	{   0xA2    /* � */,    0x81,0x75   /* �u */,   0x81,0x75   /* �u */    },
	{   0xA3    /* � */,    0x81,0x76   /* �v */,   0x81,0x76   /* �v */    },
	{   0xA7    /* � */,    0x82,0x9F   /* �� */,   0x83,0x40   /* �@ */    },
	{   0xB1    /* � */,    0x82,0xA0   /* �� */,   0x83,0x41   /* �A */    },
	{   0xA8    /* � */,    0x82,0xA1   /* �� */,   0x83,0x42   /* �B */    },
	{   0xB2    /* � */,    0x82,0xA2   /* �� */,   0x83,0x43   /* �C */    },
	{   0xA9    /* � */,    0x82,0xA3   /* �� */,   0x83,0x44   /* �D */    },
	{   0xB3    /* � */,    0x82,0xA4   /* �� */,   0x83,0x45   /* �E */    },
	{   0xAA    /* � */,    0x82,0xA5   /* �� */,   0x83,0x46   /* �F */    },
	{   0xB4    /* � */,    0x82,0xA6   /* �� */,   0x83,0x47   /* �G */    },
	{   0xAB    /* � */,    0x82,0xA7   /* �� */,   0x83,0x48   /* �H */    },
	{   0xB5    /* � */,    0x82,0xA8   /* �� */,   0x83,0x49   /* �I */    },
	{   0xB6    /* � */,    0x82,0xA9   /* �� */,   0x83,0x4A   /* �J */    },
	{   0xB7    /* � */,    0x82,0xAB   /* �� */,   0x83,0x4C   /* �L */    },
	{   0xB8    /* � */,    0x82,0xAD   /* �� */,   0x83,0x4E   /* �N */    },
	{   0xB9    /* � */,    0x82,0xAF   /* �� */,   0x83,0x50   /* �P */    },
	{   0xBA    /* � */,    0x82,0xB1   /* �� */,   0x83,0x52   /* �R */    },
	{   0xBB    /* � */,    0x82,0xB3   /* �� */,   0x83,0x54   /* �T */    },
	{   0xBC    /* � */,    0x82,0xB5   /* �� */,   0x83,0x56   /* �V */    },
	{   0xBD    /* � */,    0x82,0xB7   /* �� */,   0x83,0x58   /* �X */    },
	{   0xBE    /* � */,    0x82,0xB9   /* �� */,   0x83,0x5A   /* �Z */    },
	{   0xBF    /* � */,    0x82,0xBB   /* �� */,   0x83,0x5C   /* �\ */    },
	{   0xC0    /* � */,    0x82,0xBD   /* �� */,   0x83,0x5E   /* �^ */    },
	{   0xC1    /* � */,    0x82,0xBF   /* �� */,   0x83,0x60   /* �` */    },
	{   0xAF    /* � */,    0x82,0xC1   /* �� */,   0x83,0x62   /* �b */    },
	{   0xC2    /* � */,    0x82,0xC2   /* �� */,   0x83,0x63   /* �c */    },
	{   0xC3    /* � */,    0x82,0xC4   /* �� */,   0x83,0x65   /* �e */    },
	{   0xC4    /* � */,    0x82,0xC6   /* �� */,   0x83,0x67   /* �g */    },
	{   0xC5    /* � */,    0x82,0xC8   /* �� */,   0x83,0x69   /* �i */    },
	{   0xC6    /* � */,    0x82,0xC9   /* �� */,   0x83,0x6A   /* �j */    },
	{   0xC7    /* � */,    0x82,0xCA   /* �� */,   0x83,0x6B   /* �k */    },
	{   0xC8    /* � */,    0x82,0xCB   /* �� */,   0x83,0x6C   /* �l */    },
	{   0xC9    /* � */,    0x82,0xCC   /* �� */,   0x83,0x6D   /* �m */    },
	{   0xCA    /* � */,    0x82,0xCD   /* �� */,   0x83,0x6E   /* �n */    },
	{   0xCB    /* � */,    0x82,0xD0   /* �� */,   0x83,0x71   /* �q */    },
	{   0xCC    /* � */,    0x82,0xD3   /* �� */,   0x83,0x74   /* �t */    },
	{   0xCD    /* � */,    0x82,0xD6   /* �� */,   0x83,0x77   /* �w */    },
	{   0xCE    /* � */,    0x82,0xD9   /* �� */,   0x83,0x7A   /* �z */    },
	{   0xCF    /* � */,    0x82,0xDC   /* �� */,   0x83,0x7D   /* �} */    },
	{   0xD0    /* � */,    0x82,0xDD   /* �� */,   0x83,0x7E   /* �~ */    },
	{   0xD1    /* � */,    0x82,0xDE   /* �� */,   0x83,0x80   /* �� */    },
	{   0xD2    /* � */,    0x82,0xDF   /* �� */,   0x83,0x81   /* �� */    },
	{   0xD3    /* � */,    0x82,0xE0   /* �� */,   0x83,0x82   /* �� */    },
	{   0xAC    /* � */,    0x82,0xE1   /* �� */,   0x83,0x83   /* �� */    },
	{   0xD4    /* � */,    0x82,0xE2   /* �� */,   0x83,0x84   /* �� */    },
	{   0xAD    /* � */,    0x82,0xE3   /* �� */,   0x83,0x85   /* �� */    },
	{   0xD5    /* � */,    0x82,0xE4   /* �� */,   0x83,0x86   /* �� */    },
	{   0xAE    /* � */,    0x82,0xE5   /* �� */,   0x83,0x87   /* �� */    },
	{   0xD6    /* � */,    0x82,0xE6   /* �� */,   0x83,0x88   /* �� */    },
	{   0xD7    /* � */,    0x82,0xE7   /* �� */,   0x83,0x89   /* �� */    },
	{   0xD8    /* � */,    0x82,0xE8   /* �� */,   0x83,0x8A   /* �� */    },
	{   0xD9    /* � */,    0x82,0xE9   /* �� */,   0x83,0x8B   /* �� */    },
	{   0xDA    /* � */,    0x82,0xEA   /* �� */,   0x83,0x8C   /* �� */    },
	{   0xDB    /* � */,    0x82,0xEB   /* �� */,   0x83,0x8D   /* �� */    },
	{   0xDC    /* � */,    0x82,0xED   /* �� */,   0x83,0x8F   /* �� */    },
	{   0xA6    /* � */,    0x82,0xF0   /* �� */,   0x83,0x92   /* �� */    },
	{   0xDD    /* � */,    0x82,0xF1   /* �� */,   0x83,0x93   /* �� */    }
};


/*
�V���O���o�C�g              (0x20 - 0x7E�A0xA1 - 0xDF)
�}���`�o�C�g�����̐�s�o�C�g(0x81 - 0x9F�A0xE0 - 0xFC)
�}���`�o�C�g�����̌㑱�o�C�g(0x40 - 0x7E�A0x80 - 0xFC)
�����ȕ���                  (0x20 - 0x7E�A0xA1 - 0xDF�A
0x81 - 0x9F�A0xE0 - 0xFC �ȊO�̕���)
*/



/*
�S�p�J�^�J�i�𔼊p�J�^�J�i�ɂ���
*/
void zenkana2hankana
(
 unsigned char   *sin,   /* ���͕�����ւ̃|�C���^�B�k���I�[������ */
 unsigned char   *sout   /* �o�͕�����ւ̃|�C���^�B�k���I�[������ɂ��ĕԂ� */
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
			( 0xE0 <= *pin && *pin <= 0xFC ) )    /* �S�p�����̂P�o�C�g�� */
		{
			c[0] = *pin;
			c[1] = *(pin+1);

			/* 2 �� 2 */
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

			/* 2 �� 1 */
			b2 = 0;
			for( i = 0 ; i < HZ1SIZE ; i++ )
			{
				if ( c[0] == t1[i].zk[0] &&
					c[1] == t1[i].zk[1]  )
				{
					if (c[0] == 0x81 && c[1] == 0x40)    /* �S�p�X�y�[�X */
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
���p�J�^�J�i��S�p�J�^�J�i�ɂ���
*/
void hankana2zenkana
(
 unsigned char   *sin,   /* ���͕�����ւ̃|�C���^�B�k���I�[������ */
 unsigned char   *sout   /* �o�͕�����ւ̃|�C���^�B�ő���͕�����̂Q�{�̃T�C�Y���K�v�B�k���I�[������ɂ��ĕԂ� */
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
			( 0xE0 <= *pin && *pin <= 0xFC ) )  /* �S�p�����̂P�o�C�g�� */
		{
			*pout++ = *pin++;
			*pout++ = *pin++;
		}
		else if (   0xA0 <= *pin && *pin <= 0xDF   )  /* ���p�J�i */
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
		else    /* ���p�J�i�ȊO�̔��p���� */
		{
			*pout++ = *pin++;
		}
	}
	*pout = 0x00;
}

/*
���p�J�^�J�i��S�p�Ђ炪�Ȃɂ���
*/
void hankana2zenhira
(
 unsigned char   *sin,   /* ���͕�����ւ̃|�C���^�B�k���I�[������ */
 unsigned char   *sout   /* �o�͕�����ւ̃|�C���^�B�ő���͕�����̂Q�{�̃T�C�Y���K�v�B�k���I�[������ɂ��ĕԂ� */
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
			( 0xE0 <= *pin && *pin <= 0xFC ) )  /* �S�p�����̂P�o�C�g�� */
		{
			*pout++ = *pin++;
			*pout++ = *pin++;
		}
		else if (   0xA0 <= *pin && *pin <= 0xDF   )  /* ���p�J�i */
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
		else    /* ���p�J�i�ȊO�̔��p���� */
		{
			*pout++ = *pin++;
		}
	}
	*pout = 0x00;
}




