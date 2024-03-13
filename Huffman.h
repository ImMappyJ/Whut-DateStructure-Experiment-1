#pragma once
#include<iostream>
#include"Huffman.h"

using namespace std;

struct HTNode //���������
{
	int weight;//Ȩֵ
	int parent;//Ȩֵ
	int lchild;//Ȩֵ
	int rchild;//Ȩֵ
};

typedef char** HuffmanCode;//�洢huffman����

typedef HTNode* HuffmanTree;//��̬����洢Huffman��

int MakeHuffmanTree(HuffmanTree& pHT, int* w, int n);//����Huffman��

int Select(HuffmanTree pHT, int Size);//������СȨֵ

void MakeHuffmanCode(HuffmanTree pHT, HuffmanCode& pHC, int n);//����

void PrintHuffmanTree(HuffmanTree pHT);//����Huffman��

void PrintHuffmanCode(int root, HuffmanTree pHT, HuffmanCode pHC);//����Huffman������