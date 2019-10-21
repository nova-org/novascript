#include <iostream>
#include <fstream>
#include <vector>
void lex(std::vector<char>);


std::string getEnvVar(std::string const & key ) 
{
    char * val = getenv( key.c_str() );
    return val == NULL ? "" : std::string(val);
}

bool endsWith(const std::string& s, const std::string& suffix)
{
    return s.rfind(suffix) == (s.size()-suffix.size());
}

int main(int argc, char *argv[]) {
	std::string filename = argv[1];

	if(!endsWith(filename, ".ns")) {
		std::cout << "Only novascript files can be run\n";
		return 1;
	}

	std::string debug = getEnvVar("NOVA_DEBUG");
	std::ifstream file(filename);
	std::vector<char> filecontents;
	std::string contents;
	
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

void lex(std::vector<char> contents) {
	int i = 0;
	std::vector<std::string> type = {"integer", "string"};
	std::string token;
	std::string str;
	std::string var;
	std::vector<std::string> toks;
	int state = 0;
	for(char content : contents) {
		token += content;
		if(token == " ") {
			if (state == 1) {
				continue;
			} else if (state == 0) {
				token = "";
			} else if(state == 3) {
				token = "";
				state = 2;
			} else if (state == 2) {
				token = "";
				state = 0;
			}
		}
		if(token == "log") {
			toks.push_back("LOG");
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


		}   else if(token == "set") {
			toks.push_back("DECLARATION");
			token = "";
			state = 3;
		} else if(token == "as") {
			toks.push_back(var);
			toks.push_back("EQUALS");
			var = "";
			token = "";
		} else if(state == 2) {
			var += token;
			token = "";
		} else if(state == 1) {
			str += token;
			token = "";
		}


	}
	for(std::string tok : toks) {
		printf("%s\n", tok.c_str());
	}
}


