#pragma once
#include <iostream>
using namespace std;

class Grefenstette
{
public:
	static void coding(int *&str, const int &length)
	{
		int *newCode = new int[length]();
		for (int i = length -1; i >= 0; i--)
		{
			for (int j = length - 1 ; j >= i; j--)
			{
				if (str[j] < str[i])
				{
					newCode[i]++;
				}
			}
		}
		delete[] str;
		str = newCode;
		//cout << "---------------- code ---------------" << endl;
		//for (int i = 0; i < length; i++)
		//{
		//	cout << str[i] << " ";
		//}
		//cout << endl;
		//cout << "---------------- code ---------------" << endl;
	}

	static void Decoding(int *str, const int &length)
	{
		for (int i = length - 1; i >= 0; i--)
		{
			for (int j = i - 1; j >= 0; j--)
			{
				if (str[j] <= str[i]) 
					str[i]++;
			}

		}
		//cout << "---------------- Decode ---------------" << endl;
		//for (int i = 0; i < length; i++)
		//{
		//	cout << str[i] << " ";
		//}
		//cout << endl;
		//cout << "---------------- Decode ---------------" << endl;
	}
};

