/*
 * Name: Mason Bone
 * Date: 12/2/21
 *
 * Assignment: Project 3
 * 
 * Description: This file contains the code for the project 3 assignment. 
 * The code take in the input files cipher text and tries to decryot it 
 * back to plain text as best as possible.
 *
 */



#include <iostream>
#include <fstream> 
#include <sstream>
#include <vector>
#include <string>

//Number of lines
#define LINE 6
//Length of each line
#define LEN 60

using namespace std;

vector<int> getDec(string temp);

int main(int argc, char** argv)
{
	//vecotr to hold all the cipher text
	vector<vector<int>> decCip;

	if(argc != 2)
	{
		cout << "Please make sure to add a single txt file to the execution line" << endl;
		return 0;
	}

	//in stream used to get file content
	ifstream inputFile(argv[1]);

	if(!inputFile.is_open())
	{
		cout << "Please try another input file" << endl;
		return 0;
	}

	//temp string to store from file
	string temp;

	//get every line and send it to function to return list of ascii values
	for(int i = 0; i < LINE; i++)
	{
		getline(inputFile, temp);
        	decCip.push_back(getDec(temp));
	}

	//vector to hold the key
	vector<int> key;

	//sets key to 0
	for(int i = 0; i < LEN; i++)
	{
		key.push_back(0);
	}

	key.at(0) = 0xda;

        inputFile.close();

	//loops through and compares all line of each cipher text to find if there are any that are spaces
	for(int i = 0; i < LINE - 1; i++)
	{
		for(int j = i + 1; j < LINE; j++)
		{
			for(int k = 0; k < LEN; k++)
			{
				//checks if two values xor is 0
				if((decCip.at(i).at(k) ^ decCip.at(j).at(k)) == 0)
				{
					//sets value for key
					key.at(k) = (decCip.at(i).at(k) ^ 32);
				}
			}
		}
	}
	
	//loops through and compares all line of each cipher text to find if there are any that are spaces
	for(int i = 0; i < LINE - 1; i++)
        {
		for(int k = 0; k < LEN; k++)
		{
			for(int j = i + 1; j < LINE; j++)	
			{
				//xors to see if value is greater than 65	
				if((decCip.at(i).at(k) ^ decCip.at(j).at(k)) >= 65)
                              	{
                      			key.at(k) = (decCip.at(i).at(k) ^ 32);
				}
			}
		}
	}

	//set first key vlaue to given keu
	key.at(0) = 0xda;
	
	//prints priinttext and spaces between each charater any charaters out of range are output as a "
	for(int i = 0; i < LINE; i++)
	{
		for (int j = 0; j < decCip.at(i).size(); j++)
        	{
			if((decCip.at(i).at(j) ^ key.at(j)) >= 65 && (decCip.at(i).at(j) ^ key.at(j)) <= 122 || (decCip.at(i).at(j) ^ key.at(j)) == 32)
        			cout << (char) (decCip.at(i).at(j) ^ key.at(j)) << " ";
			else
				cout << "\"" << " ";
		}
		cout << endl;
	}

	cout << endl;

	return 0;
}

//gets a hexadecimal string and returns the converted interger values
vector<int> getDec(string temp)
{
	//stores the ASCII characters
	vector<char> numbers;
	//Stores the Int associated with the ASCII value
	vector<int> num;
	//temporary string to hold 2 chars to form hex pairs
	string buff;
	//temporary char to store each individual hex value
	char current_number = '0';

	//parses string into vector
	for(int i = 0; i < temp.length(); i++)
	{
		current_number = temp.at(i);
		numbers.push_back(current_number);
	}

	//takes 2 hex vaules and converts it to a int
	for(int i = 0; i < numbers.size(); i++)
        {
                buff += numbers.at(i);
                i++;
                buff += numbers.at(i);

                unsigned int x = stoul(buff, nullptr, 16);

                num.push_back((int)(x));

                buff.clear();
        }

	//returns vector with all charater ints
	return num;
}
