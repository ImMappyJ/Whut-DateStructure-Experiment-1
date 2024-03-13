#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
#include<stdlib.h>
#include"Huffman.h"

using namespace std;

void InitHuffmanTree(HuffmanTree& pHT, int m)//m为所有结点数
{
	pHT = new HTNode[m];

	for (int i = 0; i < m; i++)
	{
		pHT[i].weight = 0;
		pHT[i].parent = -1;
		pHT[i].lchild = -1;
		pHT[i].rchild = -1;
	}

}

int Select(HuffmanTree pHT, int nSize)//查找最小权值
{
	int minValue = 0x7FFFFFFF;
	int min = 0;

	for (int i = 0; i < nSize; i++)
	{
		if (pHT[i].parent == -1 && pHT[i].weight < minValue)
		{
			minValue = pHT[i].weight;
			min = i;
		}
	}

	return min;
}

int MakeHuffmanTree(HuffmanTree& pHT, int* w, int n)
{
	int min1;
	int min2;
	int m;

	m = 2 * n - 1;

	InitHuffmanTree(pHT, m);

	for (int i = 0; i < n; i++)
	{
		pHT[i].weight = w[i];//权值初始化
	}

	for (int i = n; i < m; i++)
	{
		min1 = Select(pHT, i);
		pHT[min1].parent = i;
		pHT[i].lchild = min1;//最小值为左孩子
		min2 = Select(pHT, i);
		pHT[min2].parent = i;
		pHT[i].rchild = min2;//次小值为右孩子
		pHT[i].weight = pHT[min1].weight + pHT[min2].weight;
	}

	return 0;
}

void MakeHuffmanCode(HuffmanTree pHT, HuffmanCode& pHC, int n)
{
	int start;
	char* Code;
	int temp_c = 0, temp_p = 0;

	Code = new char[n];
	Code[n - 1] = '\0';

	for (int i = 0; i < 256; i++)
	{
		start = n - 1;
		temp_c = i;
		temp_p = pHT[temp_c].parent;

		while (temp_p >= 0)
		{
			--start;
			if (pHT[temp_p].lchild == temp_c)
				Code[start] = '0';
			else
				Code[start] = '1';
			temp_c = temp_p;
			temp_p = pHT[temp_c].parent;
		}
		pHC[i] = new char[n - start];
		strcpy(pHC[i], &Code[start]);
	}

	delete Code;
}

void PrintHuffmanCode(int root, HuffmanTree pHT, HuffmanCode pHC)
{
	if (pHT[root].lchild == -1 && pHT[root].rchild == -1)
	{
		printf("0x%02X %s\n", root, pHC[root]);
	}
	if (pHT[root].lchild != -1)
	{
		PrintHuffmanCode(pHT[root].lchild, pHT, pHC);
	}
	if (pHT[root].rchild != -1)
	{
		PrintHuffmanCode(pHT[root].rchild, pHT, pHC);
	}
}

void PrintHuffmanTree(HuffmanTree pHT)
{
	for (int i = 0; i < 511; i++)
	{
		printf("pHT[%d]\t%d\t%d\t%d\t%d\n", i, pHT[i].weight, pHT[i].parent, pHT[i].lchild, pHT[i].rchild);
	}
}