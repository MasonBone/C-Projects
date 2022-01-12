/*
 * Name: Mason Bone
 * Date: 11/14/21
 * Class: CSCE3550
 * Assignment: Project2
 * Description: This file takes in data from an input file and preforms an AES
 * encyrption. the output is sent to a file determined by the user.
 *
 */


#include<iostream>
#include<string>
#include<fstream>
#include<algorithm>
#include<vector>
#include<bitset>
#include<bits/stdc++.h>
using namespace std;

string parse(string s);
string substitution(string s, string k);
string getBin(char c);
uint8_t gadd(uint8_t a, uint8_t b);
uint8_t gmul(uint8_t a, uint8_t b);

int main()
{
	//string to ge thte users input and output files
	string temp, input, output, keyfile;


	cout << "Please enter the name of the input plaintext file: ";

	cin >> input;

	cout << "Please enter the name of the input key file: ";

	cin >> keyfile;

	cout << "Please enter the name of the output file: ";

	cin >> output;

	ifstream inf (input);
	ifstream keyf (keyfile);
	ofstream outf (output);

	//checks that all in and out streams have opened
	if(inf.is_open() == false)
	{
		cout << "input file did not open.\n";
		cout << "please try again. \n";
                return 1;
	}
	if(keyf.is_open() == false)
        {
                cout << "key file did not open.\n";
		cout << "please try again. \n";
                return 1;
        }
	if(outf.is_open() == false)
        {
                cout << "output file did not open.\n";
		cout << "please try again. \n";
		return 1;
        }

	//vectors to hold data for each step
	vector<string> temps, parsed, substitutions, padding, shiftRows;
	//int to hold how many lines are in the file
	int count = 0;

	//gets every line from the file
	while(!inf.eof())
	{
		getline(inf, temp);
		temps.push_back(temp);
		count++;
	}

	//gets rid of the last count
	count--;

	//gets the users key from the input key file
	string key;
	getline(keyf, key);

	cout << endl << "Preprocessing:" << endl;
	outf << endl << "Preprocessing:" << endl;

	//parses input plaintext 
	for(int i = 0; i < count; i++)
	{
		parsed.push_back(parse(temps.at(i)));
		cout << parsed.at(i) << endl;
		outf << parsed.at(i) << endl;
	}

	cout << endl << "Substitution:" << endl;
	outf << endl << "Substitution:" << endl;

	//uses the plain text and key to preform a Vigenère cipher
	for(int i = 0; i < count; i++)
	{
		substitutions.push_back(substitution(parsed.at(i), key));
		cout << substitutions.at(i) << endl;
		outf << substitutions.at(i) << endl;
	}

	cout << "Padding:" << endl;
	outf << "Padding:" << endl;

	//makes the format for the 4x4 matricies and adds 'A' to fill in the end of the text
	for(int i = 0; i < count; i++)
	{
		string s;
		int num = 0;
		//for every line in the plaintext
		for(int j = 0; j < substitutions.at(i).length(); j++)
		{
			//for the 4 rows
			for(int k = 0; k < 4; k++)
			{	
				//for the 4 colums
				for(int k = 0; k < 4; k++)
				{
					//adds 'A' at the end of the file
					if(num > substitutions.at(i).length() - 1)
					{
						s.push_back('A');
					}
					else
					{
						s.push_back(substitutions.at(i).at(num));
					}
					num++;
				}	
				padding.push_back(s);
				s.clear();
			}
			j += 15;
		}
	}

	//prints the padding part of the program
	for(int i = 0; i < padding.size(); i++)
	{
		cout << padding.at(i) << endl;
		outf << padding.at(i) << endl;

		if((i+1)%4 == 0)
		{
			cout << endl;
			outf << endl;
		}
	}

	cout << "Shift Rows:" << endl;
	
	//preforms the shift rows
	for(int i = 0; i < padding.size(); i++)
	{
		string s;

		shiftRows.push_back(padding.at(i));
			
		s.push_back(padding.at(i+1).at(1));
		s.push_back(padding.at(i+1).at(2));
		s.push_back(padding.at(i+1).at(3));
		s.push_back(padding.at(i+1).at(0));
		shiftRows.push_back(s);
		s.clear();

		s.push_back(padding.at(i+2).at(2));
                s.push_back(padding.at(i+2).at(3));
                s.push_back(padding.at(i+2).at(0));
                s.push_back(padding.at(i+2).at(1));
                shiftRows.push_back(s);
		s.clear();

		s.push_back(padding.at(i+3).at(3));
                s.push_back(padding.at(i+3).at(0));
                s.push_back(padding.at(i+3).at(1));
                s.push_back(padding.at(i+3).at(2));
                shiftRows.push_back(s);
		s.clear();
		
		i+=3;
	}

	//prints the shift rows
	for(int i = 0; i < shiftRows.size(); i++)
        {
                cout << shiftRows.at(i) << endl;
		outf << shiftRows.at(i) << endl;

                if((i+1)%4 == 0)
                {
                        cout << endl;
			outf << endl;
                }
        }

	//vector of vectors to store the individual hexadecimal values in each collum
	vector<vector<string>> partyBits;

	cout << "Parity Bit:" << endl;
	outf << "Parity Bit:" << endl;

	//Preforms the parity bit operation
	for(int i = 0; i < shiftRows.size(); i++)
	{
		//variables used just for this section
		vector<string> hex;
		string s;
		char c[2];
		bitset<8> bit;
		int n = 0;

		//since there are 4 collums the program does 4 at a time and stores them in a temp vector
		for(int j = 0; j < 4; j++)
		{
			//makes the base 2 version of the int
			bit = int(shiftRows.at(i).at(j));
			n = 0;

			//counts the amount of 1s
			for(int k = 0; k < bit.size(); k++)
			{
				if(bit[k] == 1)
				{
					n++;
				}
			}

			//if odd change the msb to 1
			if(n % 2 != 0)
			{
				bit.set(7, 1);
			}

			//sets new bit to int then to hex
			n = int(bit.to_ulong());
			sprintf(c, "%X", n);
			s = c;
			hex.push_back(s);
		}
		partyBits.push_back(hex);
	}

	//prints the parity bits
	for(int i = 0; i < partyBits.size(); i++)
        {
		for(int j = 0; j < partyBits.at(i).size(); j++)
		{
			cout << partyBits.at(i).at(j) << " ";
			outf << partyBits.at(i).at(j) << " ";
		}
		cout << endl;
		outf << endl;
        }
	cout << endl;
	outf << endl;

	cout << "Mix Columns:" << endl;
	outf << "Mix Columns:" << endl;
	
	vector<int> finalResult;

	int n = 0;
	//preforms mix columns
	for(int i = 0; i < partyBits.size(); i++)
	{
		n = 0;
		bitset<8> bit;

		bit = (int)strtol(partyBits.at(i).at(n).c_str(), NULL, 16);
		
		int cnt = 0;

		for(int j = 0; j < 8; j++)
		{
			if(bit[j] == 1)
				cnt++;
		}

		if(cnt % 2 != 0)
		{
			bit.set(7, 1);
		}

		uint8_t c0 = int(bit.to_ulong());
                n++;

		bit = (int)strtol(partyBits.at(i).at(n).c_str(), NULL, 16);

                cnt = 0;

                for(int j = 0; j < 8; j++)
                {
                        if(bit[j] == 1)
                                cnt++;
                }

                if(cnt % 2 != 0)
                {
                        bit.set(7, 1);
                }

		uint8_t c1 = int(bit.to_ulong());
                n++;

                bit = (int)strtol(partyBits.at(i).at(n).c_str(), NULL, 16);

                cnt = 0;

                for(int j = 0; j < 8; j++)
                {
                        if(bit[j] == 1)
                                cnt++;
                }

                if(cnt % 2 != 0)
                {
                        bit.set(7, 1);
                }

		uint8_t c2 = int(bit.to_ulong());
                n++;

                bit = (int)strtol(partyBits.at(i).at(n).c_str(), NULL, 16);

                cnt = 0;

                for(int j = 0; j < 8; j++)
                {
                        if(bit[j] == 1)
                                cnt++;
                }

                if(cnt % 2 != 0)
                {
                        bit.set(7, 1);
                }

		uint8_t c3 = int(bit.to_ulong());
		n++;

		uint8_t res = 0;
		res = gadd( gadd(gmul(c0, 2), gmul(c1, 3)) ,  gadd(c2, c3) );//Element a0 = rgfMul(c0,2) ⊕ rgfMul(c1,3) ⊕ c2 ⊕ c3
		finalResult.push_back(int(bitset<8>(res).to_ulong()));
		res = gadd( gadd(c0, gmul(c1, 2)) ,  gadd(gmul(c2, 3), c3) );//Element a1 = c0 ⊕ rgfMul(c1,2) ⊕ rgfMul(c2,3) ⊕ c3
		finalResult.push_back(int(bitset<8>(res).to_ulong()));
		res = gadd( gadd(c0,c1) ,  gadd(gmul(c2, 2), gmul(c3, 3)) );//Element a2 = c0 ⊕ c1 ⊕ rgfMul(c2,2) ⊕ rgfMul(c3,3)
		finalResult.push_back(int(bitset<8>(res).to_ulong()));
 		res = gadd( gadd(gmul(c0, 3),c1) ,  gadd(c2,gmul(c3, 2)) );//Element a3 = rgfMul(c0,3) ⊕ c1 ⊕ c2 ⊕ rgfMul(c3,2)
		finalResult.push_back(int(bitset<8>(res).to_ulong()));
	}	
	
	char c[2];
	string s;
	for(int i = 0; i < finalResult.size();)
	{
		for(int j = 0; j < 4; j++)
		{
			n = finalResult.at(i);
			sprintf(c, "%X", n);
			s = c;
			cout << s << " ";
			outf << s << " ";
			i++;
		}
		cout << endl;
		outf << endl;
	}

	inf.close();
	keyf.close();
	outf.close();
}

//function used to parse string and remove any whitespace and punctuation
string parse(string s)
{
	string tmpoutput;

	const string puncts = " [];',./{}:\"?><`~!-_\n";

	for (const auto& ch : s)
        {
                bool found = false;

                for (const auto& p : puncts)
                {
                        if (ch == p)
                        {
                                found = true;
                                break;
                        }
                }

                if (!found)
                        tmpoutput += ch;
        }

	return tmpoutput;
}

//substitutes all plaintext with encryoted text using the v method
string substitution(string s, string k)
{
	string output;
	for (int i = 0, j = 0; i < s.length(); ++i) 
	{
		char c = s[i];
		output += (c + k[j] - 2 * 'A') % 26 + 'A';
		j = (j + 1) % k.length();
	}
	return output;
}

//adds two bianary numbers together using xor
uint8_t gadd(uint8_t a, uint8_t b)
{
	return a ^ b;
}

//multiplys two bianary numbers together using xor
uint8_t gmul(uint8_t a, uint8_t b)
{
	uint8_t p = 0;
	while (b) 
	{
		if (b & 1)
			p ^= a;

		if (a & 0x80)
			a = (a << 1) ^ 0x11b;
		else
			a <<= 1;
		b >>= 1;
	}
	return p;
}
