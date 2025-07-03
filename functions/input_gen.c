#include<stdio.h>

int main()
{
	char buf[24];
	for(int i = 0; i < 20; i++)
		buf[i] = 'A';
	//0x70	0xdc	0xff	0xff	0xff	0x7f	0x00	0x00
	buf[12] = 0x70;
	buf[13] = 0xdc;
	buf[14] = 0xff;
	buf[15] = 0xff;
	buf[16] = 0xff;
	buf[17] = 0x7f;
	buf[18] = 0x00;
	buf[19] = 0x00;

	buf[20] = 0xd2;
	buf[21] = 0x11;
	buf[22] = 0x40;
	buf[23] = 0x00;

	FILE *f = fopen("file.txt", "wb");
	for(int i = 0; i < 24; i++) {
		fputc(buf[i], f);
		printf("%hhx ", (char) buf[i]);
	}
	fclose(f);
	return 0;
}
