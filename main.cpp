#include <fstream>
#include <vector>
using namespace std;

void lex(vector<char>);

int main() {

	ifstream file("test.ns");
	vector<char> filecontents;
	string contents;

	if(file.is_open()) {
		while(getline(file, contents)) {
			for(int i = 0; i < contents.length(); i++) {
				filecontents.push_back(contents[i]);
			}
		}
	}
	file.close();
	lex(filecontents);
	return 0;
}

void lex(vector<char> contents) {
	int i = 0;
	vector<string> type = {"integer", "string"};
	string token;
	string str;
	string var;
	vector<string> toks;
	int state = 0;
	for(char content : contents) {
		token += content;
		if(token == " ") {
			if (state == 1) {
				continue;
			} else if (state == 0) {
				token = "";
			}
		}
		if(token == "print") {
			toks.push_back("PRINT");
			token = "";
		} else if(token == "\"") {
			if(state == 0) {
				state = 1;
			} else if(state == 1) {
				state = 0;
				toks.push_back("STRING");
				toks.push_back(str);
				str = "";
			}

			token = "";


		}  else if(token == "set") {

			state = 2;
			token = "";
			toks.push_back("DECLARATION");
		} else if(token == "as") {
			toks.push_back(var);
			toks.push_back("EQUALS");
			var = "";
			token = "";
			state = 0;
		} else if(state == 2) {

			if(token.find(' ') != string::npos) {

				token = "";
				if(i == 1) {
					state = 0;
				}
				i++;
			} else {
				var += token;
				token = "";
			}

		} else if(state == 1) {
			str += token;
			token = "";
		}

		//printf("%s\n", token.c_str());
	}
	for(string tok : toks) {
		printf("%s\n", tok.c_str());
	}
}


