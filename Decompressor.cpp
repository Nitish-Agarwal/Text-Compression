#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

vector <string> key;
void Decompress(char*, int);
int power(int, int);

int main(){
	for(int i = 0; i < 256; i++)
		key.push_back(string(1, i));
	ifstream InputFile("CompressedFile.txt", ios :: binary);
	if(!InputFile){
		cout<<"Sorry. Input file is not present.\n\n";
		return 1;
	}
	InputFile.seekg(0, ios::end);
	int sz = InputFile.tellg();
	char* data = new char[sz];
	InputFile.seekg(0, ios::beg);
	InputFile.read(data, sz);
	int count = InputFile.gcount();
	InputFile.close();
	Decompress(data, count);
	return 0;
}

void Decompress(char* data, int size){
	int i;
	for(i = 0; data[i + 1] || data[i + 2] || data[i + 3] || data[i + 4]; i++);
	vector <char> space;
	for(int C = i + 5; C < size; C++){
		space.push_back(((data[C] + 256) % 256) / 64 + 48);
		space.push_back((((data[C] + 256) % 256) / 16) % 4 + 48);
		space.push_back((((data[C] + 256) % 256) / 4) % 4 + 48);
		space.push_back(((data[C] + 256) % 256) % 4 + 48);
	}
	vector <int> code;
	for(int A= 0, B = 0; A < space.size(); A++){
		int temp = 0;
		for(int C = space[A] - 48; C >= 0 && B < i; C--)
			temp += ((data[B++] + 256) % 256) * power(256, C);
		code.push_back(temp);
	}
	ofstream OutputFile("DecompressedFile.txt", ios::binary);
	if(!i)
		return;
	OutputFile<<key[code[0]];
	for(int i = 1; i < code.size(); i++){
		string s;
		if(code[i] == key.size()){
			s = key[code[i - 1]];
			const char* c = s.c_str();
			s += string(1, c[0]);
		}
		else if(code[i] > key.size()){
			cout<<"Sorry, the compressed file cannot be decompressed.\n";
			return;
		}
		else
			s = key[code[i]];
		const char* c = s.c_str();
		key.push_back(key[code[i - 1]] + string(1, c[0]));
		OutputFile<<s;
	}
	OutputFile.close();
	cout<<"The compressed file has been successfully decompressed.\n";
}

int power(int a, int b){
	if(!b)
		return 1;
	return a * power(a, b - 1);
}
