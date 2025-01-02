// String Comparision.cpp : This file contains the 'main' function. Program execution begins and ends there.
// my code for finding a string's proximity to another string. 
// aka im going to try to return an int value representing how similar a string is to another string.

#include <iostream>
#include <string>
#include <climits>
#include <vector>
#include <exception>

using namespace std;
//advcomp stands for "advanced comparision"
static double advComp(string s, string comp_s) {
	
	//this is the value that will be returned and represents how alike the two strings are. (set to an abysmally low number on purpose.)
	double bestpts = DBL_MIN;
	double s_points = 0;
	const double EXISTS_WEIGHT = 2.0; //modifies the importance of a letter in each one matching a letter in the other;
	const double LENGTH_WEIGHT = 2.0; //modifies the importance of the difference in string lengths.
	const double MATCH_WEIGHT = 3.0; //modifies the importance of a character in one string matching the character in another string.
	const double PERFECT_WEIGHT = 5.0; //modifies the importance of finding in one of the strings an exact copy of either the other string or a substring of the other string.
	//firstPos becomes relevant soon. I'll explain it when it is.
	int firstPos = 0;

	if (s == comp_s) {
		//pretty simple, if it matches score a point for each character inside of it. unfortunately it is downhill from here.
		bestpts = s.length() * PERFECT_WEIGHT;
		return bestpts;
	}
	else if (s.length() >= comp_s.length()) {
		for (int j = 0; j < comp_s.length(); ++j) {
			firstPos = 0;
			s_points = 0;
			string comp_s_sub = comp_s.substr(j, (comp_s.length() - j));
			for (int sublength = 0; sublength < comp_s_sub.length(); sublength++) {
				if (s.find(comp_s_sub[sublength]) != -1) {
					s_points += EXISTS_WEIGHT;
				}
			}
			if (s_points > bestpts) {
					bestpts = s_points;
			}
			if (s.find(comp_s_sub) != -1) {
				s_points = (s_points + (static_cast<double>(comp_s_sub.length()) * PERFECT_WEIGHT)) - ((static_cast<double>(s.length()) * LENGTH_WEIGHT) - comp_s_sub.length());
				if (s_points > bestpts) {
					bestpts = s_points;
				}
			}
			else {
				while (true) {
					/*so this entire section is a bit of a mess so let me explain.
					* firstPos is almost always used in the context of comp_s_sub[firstPos] as it represents the character in comp_s_sub that the program is looking for.
					* firstPos_if_check is an integer that = -1 if character comp_s_sub[firstPos] does not exist in the string s.
					* However, if the character comp_s_sub[firstPos] does exist, firstPos_if_check = the first position in the string s that comp_s_sub[firstPos] is found at.
					* iter_num serves as the number of iterations that have been */
					int firstPos_if_check = s.find(comp_s_sub[firstPos]);
					if (firstPos_if_check != -1) {
						int iter_num = 0;
							for (int i = firstPos_if_check; i < (firstPos_if_check + (comp_s_sub.length() - firstPos)); ++i) {
								try {
									if (i > s.length()) {
										//throw (i);
										break;
									} else if (s[i] == comp_s_sub[firstPos + iter_num]) {
										s_points += MATCH_WEIGHT;
									}
									++iter_num;
								}
								catch (int error_i) {
									cout << "ERROR | " << s << "[\"" << error_i << "\"] OUT OF SCOPE WHEN COMPARED TO \"" << comp_s_sub << "\". PRINTING VARIABLE INFO." << endl;
									cout << "iter_num = " << iter_num << endl;
									cout << "firstPos_if_check = " << firstPos_if_check << ". firstPos_if_check char = " << s[firstPos_if_check] << endl;
									cout << "i = " << i << endl;
									cout << "s = " << s << endl;
									cout << "comp_s_sub = " << comp_s_sub << endl;
									cout << "comp_s = " << comp_s << endl;
									break;
								}
							}
						break;
					}
					else if (firstPos < comp_s_sub.length()) {
						firstPos++;
					}
					else {
						break;
					}
				}
				s_points = s_points - ((static_cast<double>(s.length()) - comp_s_sub.length()) * LENGTH_WEIGHT);
				if (s_points > bestpts) {
					bestpts = s_points;
				}
			}
		}
		return bestpts;
	}
	else if (s.length() < comp_s.length()) {
		for (int j = 0; j < s.length(); ++j) {
			firstPos = 0;
			s_points = 0;
			string s_sub = s.substr(j, (s.length() - j));
			for (int sublength = 0; sublength < s_sub.length(); sublength++) {
				if (comp_s.find(s_sub[sublength]) != -1) {
					s_points += EXISTS_WEIGHT;
				}
			}
			if (comp_s.find(s_sub) != -1) {
				s_points = (s_points + (static_cast<double>(s_sub.length()) * PERFECT_WEIGHT)) - ((static_cast<double>(comp_s.length()) - s_sub.length()) * LENGTH_WEIGHT);
				if (s_points > bestpts) {
					bestpts = s_points;
				}
			}
			else {
				while (true) {
					//ditto, but the use of s and s_comp is inverted.
					int firstPos_if_check = comp_s.find(s_sub[firstPos]);
					if (firstPos_if_check != -1) {
						int iter_num = 0;
						for (int i = firstPos_if_check; i < (firstPos_if_check + (s_sub.length() - firstPos)); ++i) {
							try {
								if (i > comp_s.length()) {
									//throw (i);
									break;
								} else if (comp_s[i] == s_sub[firstPos + iter_num]) {
									s_points += MATCH_WEIGHT;
								}
								++iter_num;
							}
							catch (int error_i) {
								cout << "ERROR |" << comp_s << "[" << error_i << "] OUT OF SCOPE WHEN COMPARED TO \"" << s_sub << "\". PRINTING VARIABLE INFO." << endl;
								cout << "iter_num = " << iter_num << endl;
								cout << "firstPos_if_check = " << firstPos_if_check << ". firstPos_if_check char = " << comp_s[firstPos_if_check] << endl;
								cout << "i = " << i << endl;
								cout << "s = " << s << endl;
								cout << "s_sub = " << s_sub << endl;
								cout << "comp_s = " << comp_s << endl;
							}
						}
						break;
					}
					else if (firstPos < s_sub.length()) {
						firstPos++;
					}
					else {
						break;
					}
				}
				s_points = s_points - ((static_cast<double>(comp_s.length()) - s_sub.length()) * LENGTH_WEIGHT);
				if (s_points > bestpts) {
					bestpts = s_points;
				}
			}
		}
		return bestpts;
	}
};
static void advComp_many() {
	vector<string> strings;
	int s_num;
	int closestPos = 1;// if none of the strings are similar enough to get scored, it defaults to first string it got compared to.
	double closestScore = DBL_MIN;
	string s_vector;
	cout << "What word would you like to compare other words to?\n";
	cin >> s_vector;
	strings.push_back(s_vector);
	cout << "How many words would you like to compare to it?\n";
	cin >> s_num;
	for (int  iteration = 0; iteration < s_num; iteration++) {
		cout << "please enter word " << iteration + 1 << " of " << s_num << ".\n";
		cin >> s_vector;
		strings.push_back(s_vector);
	}
	for (int iteration = 0; iteration < s_num; iteration++) {
		int relative_string = iteration + 1;
		cout << "Words " << strings.at(0) << " and " << strings.at(relative_string) << " score " << advComp(strings.at(0), strings.at(relative_string)) << endl;
		if (advComp(strings.at(0), strings.at(relative_string)) > static_cast<double>(closestScore)) {
			closestScore = advComp(strings.at(0), strings.at(relative_string));
			closestPos = relative_string;
		} 
	}
	cout << "The closest string to \"" << strings.at(0) << "\" is \"" << strings.at(closestPos) << "\"\n";
}

int main() {
	advComp_many();
	return 0;
}