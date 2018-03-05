#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct dictionary{
	int code;
	string key;
};

int size = 1009;
vector <string*> key;
dictionary** hash = new dictionary*[size];
void Hash();
bool isprime(int);
void computeindex(int&, const string&);
int power(int, int);
void compress(char*, int);
bool indictionary(const string&, int&);

int main(){
	for(int i = 0; i < size; i++)
		hash[i] = 0;
	for(int i = 0; i < 256; i++){
		string* a = new string(string(1, i));
		key.push_back(a);
		Hash();
	}
	ifstream InputFile("InputFile.txt",ios :: binary);
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
	compress(data, count);
}

void compress(char* data, int size){
	ofstream OutputFile("CompressedFile.txt", ios :: binary);
	string space1 = "";
	string space2 = "";
	string a = "";
	for(int i = 0; i < size; i++){
		string b = string(1, data[i]);
		int code;
		if(indictionary(a + b, code))
			a = a + b;
		else{
			indictionary(a, code);
			int j;
			for(j = 3; j >= 0 && !(code / power(256, j)); j--);
			space1 += string(1, j + 48);
			for(int k = j; k >= 0; code %= power(256, k), k--)
				OutputFile<<char(code / power(256, k));
			string* c = new string(a + b);
			key.push_back(c);
			Hash();
			a = b;
		}
	}
	int code;
	indictionary(a, code);
	int j;
	for(j = 3; j >= 0 && !(code / power(256, j)); j--);
	space1 = space1 + string(1, char(j + 48));
	for(int k = j; k >= 0; code %= power(256, k), k--)
		OutputFile<<char(code / power(256, k));
	OutputFile<<"#";
	string s = string(4, 0);
	OutputFile<<s;
	space1 += "000";
	const char* c = space1.c_str();
	for(int i = 0; c[i] && c[i + 1] && c[i + 2] && c[i + 3]; i += 4)
		space2 += string(1, 64 * (c[i] - 48) + 16 * (c[i + 1] - 48) + 4 * (c[i + 2] - 48) + c[i + 3] - 48);
	OutputFile<<space2;
	OutputFile.close();
	cout<<"Your file has been successfully compressed.\n";
}

void Hash(){
	if(key.size() > size / 2){
		int oldsize = size;
		for(size *= 2; !isprime(size); size++);
		dictionary** temp = new dictionary*[size];
		for(int i = 0; i < size; i++)
			temp[i] = 0;
		for(int i = 0; i < oldsize; i++)
			if(hash[i]){
				int hashindex = 0;
				computeindex(hashindex, hash[i]->key);
				for(int i = 1; temp[hashindex]; i++)
					hashindex = (hashindex + 2 * i - 1) % size;
				temp[hashindex] = hash[i];
			}
		delete []hash;
		hash = temp;
	}
	int hashindex;
	computeindex(hashindex, *key.back());
	for(int i = 1; hash[hashindex]; i++)
		hashindex = (hashindex + 2 * i - 1) % size;
	hash[hashindex] = new dictionary ;
	hash[hashindex]->key = *key.back();
	hash[hashindex]->code = key.size() - 1;
}

void computeindex(int& x, const string& s){
	const char* a = s.c_str();
	x = 0;
	for(int i = 0; i < 9 && a[i]; i++)
		x += ((power(i + 1, 8) % size) * (power((a[i] + 256) % 256, 3) % size + power((a[i] + 128), 2) % size)) % size;
	x %= size;
}

bool indictionary(const string& s, int& code){
	int x;
	computeindex(x, s);
	for(int i = 1; hash[x]; i++){
		if(hash[x]->key == s){
			code = hash[x]->code;
			return 1;
		}
		x = (x + 2 * i - 1) % size;
	}
	return 0;
}

bool isprime(int a){
 	int b;
	for(b = 2; b <= a/b; b++)
		if(!(a % b))
			b = a;
	if(b == a + 1)
		return 0;
	else
		return 1;
}

int power(int a, int b){
	if(!b)
		return 1;
	return a * power(a, b - 1);
}
