#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include"Huffman.h"
#include"Compress.h"

using namespace std;


#define OK 1
#define ERROR 0

const int SIZE = 256;


char str2byte(const char* pBinStr)
{
	char b = 0x00;
	for (int i = 0; i < 8; i++)
	{
		b = b << 1;//左移一位
		if (pBinStr[i] == '1')
			b = b | 0x01;
	}
	return b;
}

void byte2str(char c, char* str)
{
	if (c >= 0) {
		str[0] = '0';
		for (int i = 0; i < 7; i++) {
			if (c % 2 == 1) {
				str[7 - i] = '1';
			}
			else {
				str[7 - i] = '0';
			}
			c = c / 2;
		}
	}
	else {
		str[0] = '1';
		c = 0 - c - 1;
		for (int i = 0; i < 7; i++) {
			if (c % 2 == 1) {
				str[7 - i] = '0';
			}
			else {
				str[7 - i] = '1';
			}
			c = c / 2;
		}
	}
}

int Encode(const char* pFilename, const HuffmanCode pHC, char* pBuffer, const int nSize)//压缩编码
{
	//打开文件
	FILE* in = fopen(pFilename, "rb");

	//开辟缓冲区
	//pBuffer = (char *)malloc(nSize * sizeof(char));////pBuffer = new char[nSize];//
	//pBuffer[nSize] = { '\0' };

	//if (!pBuffer)
	//{
	//	cerr << "开辟缓冲区失败！EnCode!" << endl;
	//	return 0;//返回0表示失败
	//}

	char cd[SIZE] = { 0 };
	int pos = 0;
	int ch;
	//FILE* test = fopen("log_or.txt", "w+");

	while ((ch = fgetc(in)) != EOF)
	{
		strcat(cd, pHC[ch]);
		for (int i = 0; i < strlen(pHC[ch]); i++) {
			//fputc(pHC[ch][i], test);
		}
		//fputc('\n', test);
		while (strlen(cd) >= 8)
		{
			pBuffer[pos++] = str2byte(cd);
			//fprintf_s(test, "%d\n", pBuffer[pos-1]);
			for (int i = 0; i < SIZE - 8; i++)
			{
				cd[i] = cd[i + 8];
			}
		}
	}
	//fclose(test);
	fclose(in);
	in = NULL;

	if (strlen(cd) > 0)
	{
		pBuffer[pos++] = str2byte(cd);
	}
	cout << pos << endl;
	return OK;
}

int InitHead(const char* pFilename, HEAD& sHead)
{
	strcpy(sHead.type, "HUF");
	sHead.length = 0;
	for (int i = 0; i < SIZE; i++)
	{
		sHead.weight[i] = 0;
	}

	FILE* in = fopen(pFilename, "rb");

	int ch;
	while ((ch = fgetc(in)) != EOF)
	{
		sHead.weight[ch]++;
		sHead.length++;
	}

	fclose(in);
	in = NULL;
	return OK;

}

int WriteFile(const char* pFilename, const HEAD sHead, const char* pBuffer, const int nSize)
{
	char filename[256] = { 0 };
	strcpy(filename, pFilename);
	strcat(filename, ".huf");

	FILE* out = fopen(filename, "wb");

	fwrite(&sHead, sizeof(HEAD), 1, out);
	fwrite(pBuffer, sizeof(char), nSize, out);
	fclose(out);
	out = NULL;

	cout << "生成压缩文件：" << filename << endl;
	int len = sizeof(HEAD) + strlen(pFilename) + 1 + nSize;
	return len;
}

int Compress(const char* pFilename)
{
	HuffmanTree pHT;
	char** pHC = new char* [256];

	cout << "正在读取文件……" << endl;
	int weight[256] = { 0 };
	int ch;

	FILE* in = fopen(pFilename, "rb");
	if (in == NULL)
	{
		cerr << "文件打开失败！" << endl;
		return ERROR;
	}
	while ((ch = getc(in)) != EOF)
	{
		weight[ch]++;
	}

	cout << "文件读取完毕！\n" << endl << endl;

	fclose(in);
	in = NULL;

	//测试得到的权重
	/*
	cout << "显示256种字节出现的次数：" << endl;
	cout << "Byte\t" << "Weight" << endl;
	for (int i = 0; i < 256; i++)
	{
		printf("0x%02X    %d\n", i, weight[i]);
	}*/

	MakeHuffmanTree(pHT, weight, 256);

	MakeHuffmanCode(pHT, pHC, 256);


	int nSize = 0;
	for (int i = 0; i < 256; i++)
	{
		nSize += weight[i] * strlen(pHC[i]);
	}
	nSize = (nSize % 8) ? nSize / 8 + 1 : nSize / 8;

	char* pBuffer = NULL;
	pBuffer = new char[nSize];
	memset(pBuffer, 0, (nSize) * sizeof(char));
	Encode(pFilename, pHC, pBuffer, nSize);
	if (!pBuffer)
	{
		cout << "开辟缓冲区失败！Compress!" << endl;
		return ERROR;
	}

	HEAD sHead;
	InitHead(pFilename, sHead);
	cout << "目标文件大小：" << sHead.length << "字节" << endl;
	int afterlen = WriteFile(pFilename, sHead, pBuffer, nSize);
	cout << "压缩文件大小：" << afterlen << "字节  \n其中文件头sHead大小：" << sizeof(sHead) << "字节" << endl;
	cout << "压缩比率：" << (double)afterlen * 100 / sHead.length << "%" << endl;

	delete pHT;
	//delete pHC;
	delete pBuffer;

	return OK;
}

int Uncompress(const char* pFilename)
{
	char filename[256] = { 0 };
	strcpy(filename, pFilename);
	strcat(filename, ".bmp");
	HEAD head;
	FILE* file = fopen(pFilename, "rb");
	if (file == NULL) {
		cout << "文件不存在" << endl;
		return ERROR;
	}
	fread(&head, sizeof(HEAD), 1, file);
	char* pBuffer = (char*)malloc((head.length)*sizeof(char));
	if (!pBuffer) {
		cout << "开辟缓存区失败" << endl;
		return ERROR;
	}
	int pos = 0;
	HuffmanTree pHT;
	MakeHuffmanTree(pHT, head.weight, 256);
	HTNode temp = pHT[510];
	char str[9];
	char ch;
	int count = 0;
	int count_2 = 0;
	int count_3 = 0;
	while (!feof(file) && pos < head.length) {
		fread(&ch,sizeof(char),1,file);
		count++;
		byte2str(ch, str);
		for (int i = 0; i < 8; i++) {
			count_3++;
			if (str[i] == '0') {
				if (temp.lchild <= 255) {
					pBuffer[pos++] = temp.lchild;
					temp = pHT[510];
					count_2++;
					//fputc('\n', test);
				}
				else {
					temp = pHT[temp.lchild];
				}
			}else{
				if (temp.rchild <= 255) {    
					pBuffer[pos++] = temp.rchild;
					temp = pHT[510];
					count_2++;
					//fputc('\n', test);
				}
				else {
					temp = pHT[temp.rchild];
				}
			}
		}
	}
	FILE* fileo = fopen(filename, "wb");
	fwrite(pBuffer, sizeof(char), head.length, fileo);
	delete pBuffer;
	fclose(file);
	fclose(fileo);
	return OK;
}
