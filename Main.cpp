#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
#include<fstream>
#include"Huffman.h"
#include"Compress.h"

using namespace std;

int main()
{
	cout << "========Huffman�ļ�ѹ��========" << endl;
	cout << "1.ѹ��" << endl;
	cout << "2.��ѹ" << endl;
	int i;
	cin >> i;
	switch (i) {
	case 1:
		cout << "������Դ�ļ�����";
		char filename[256];
		cin >> filename;

		if (Compress(filename) == 1) {
			cout << "\nѹ����ɣ�" << endl;
		}
		else {
			cout << "\nѹ��ʧ��!" << endl;
		}
		break;
	case 2:
		cout << "������Դ�ļ�����";
		char filename_f[256];
		cin >> filename_f;
		if (Uncompress(filename_f) == 1) {
			cout << "\n��ѹ��ɣ�" << endl;
		}
		else {
			cout << "\n��ѹʧ�ܣ�" << endl;
		}
		break;
	}
	return 0;
}