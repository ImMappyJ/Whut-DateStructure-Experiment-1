#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
#include<fstream>
#include"Huffman.h"
#include"Compress.h"

using namespace std;

int main()
{
	cout << "========Huffman文件压缩========" << endl;
	cout << "1.压缩" << endl;
	cout << "2.解压" << endl;
	int i;
	cin >> i;
	switch (i) {
	case 1:
		cout << "请输入源文件名：";
		char filename[256];
		cin >> filename;

		if (Compress(filename) == 1) {
			cout << "\n压缩完成！" << endl;
		}
		else {
			cout << "\n压缩失败!" << endl;
		}
		break;
	case 2:
		cout << "请输入源文件名：";
		char filename_f[256];
		cin >> filename_f;
		if (Uncompress(filename_f) == 1) {
			cout << "\n解压完成！" << endl;
		}
		else {
			cout << "\n解压失败！" << endl;
		}
		break;
	}
	return 0;
}