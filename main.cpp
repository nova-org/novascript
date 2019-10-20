#include <fstream>
#include <vector>

void lex(std::vector<char>);


std::string getEnvVar(std::string const & key ) 
{
    char * val = getenv( key.c_str() );
    return val == NULL ? std::string("") : std::string(val);
}


int main(int argc, char *argv[]) {
	std::string debug = getEnvVar("DEBUG");
	debug = debug == "" ? false : true;
	std::ifstream file("test.ns");
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

			if(token.find(' ') != std::string::npos) {

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
	for(std::string tok : toks) {
		printf("%s\n", tok.c_str());
	}
}


