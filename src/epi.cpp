// epi² v_0.203
// @author Blas Fernández
// @date 31-07-24
// @brief Nice programming language

#include "noerrorfile.cpp"
#include "license.cpp"
#include "downloadbar-utils.cpp"
#include "utils/calc.cpp"

int hdasljdsaj = 0;
bool debug = false;

using namespace std::filesystem;

bool fullDebug = false;
ofstream fileObject("epi2.debug.log");

// Variables for storing variables
vector<vector<String>> functions;

class Variable {
private:
	String type = "";
	String content = "";
	String name = "";
public:
	Variable(String t, String n, String c) {
		type = t;
		content = c;
		name = n;
	}

	String getType() {
		return type;
	}
	String getContent() {
		return content;
	}
	void setType(String newtype) {
		type = newtype;
	}
	void setContent(String newcontent) {
		content = newcontent;
	}
	String getName() {
		return name;
	}
};
class File {
private:
	String filename;
	String varName;
	bool opened = false;
public:
	File(String n, String f) {
		filename = f;
		varName = n;

		if(f != "") {
			opened = true;
		}
	}

	String getName() {
		return varName;
	}
	String getFilename() {
		return filename;
	}
	void write(String c) {
		ofstream f(filename);
		f << c;
		f.close();
	}
	void deletef() {
		remove(filename.c_str());
	}
	String read() {
		fstream f(filename);

		String line, content;
		while(getline(f, line)) {
			content += line;
		}

		f.close();

		return content;
	}
	bool isOpen() {
		return opened;
	}
	void open(String f) {
		filename = f;
	}
};

vector<Variable> variables;
vector<Variable> values;
vector<File> files;

// Variables for utils
bool writingFunction = false;
String functionToWrite = "";
bool writingTry = false;
bool writingCatch = false;
String tryCode = "";
String catchCode = "";

void throwError(String error, String message, String& exceptionN, int line, bool onFunction = false, String functionName = "", int& line2 = hdasljdsaj, bool isOnTry = false) {
	if(fullDebug) {
		String result = "";

		if(isOnTry) {
			result += "Handled exception at line: ";
			if(onFunction) {
				result += "#" + functionName + ":" + to_string(line2) + ":" + to_string(line);
			} else {
				result += to_string(line);
			}
			result += "; Caused by: " + error;
		} else {
			result += "Unhandled exception at line: ";
			if(onFunction) {
				result += "#" + functionName + ":" + to_string(line2) + ":" + to_string(line);
			} else {
				result += to_string(line);
			}
			result += "; Caused by: " + error;
		}
		
		fileObject << result + "\n";
	}
	
	exceptionN = error;

	if(debug) {
		if(!onFunction) {
			cout << "\t" << ASCII_BOLD << ASCII_UNDERLINE << ASCII_BLACK << ASCII_BG_GREEN << line << ASCII_RESET << ASCII_WHITE << ":\n";
		} else {
			cout << "\t" << ASCII_BOLD << ASCII_UNDERLINE << ASCII_BLACK << ASCII_BG_GREEN << line << " #" << functionName << ":" << line2 << ASCII_RESET << ASCII_WHITE << ":\n";
		}
		cout << ASCII_UNDERLINE << "\t\tVariables" << ASCII_RESET << ":\n" << ASCII_WHITE;
		for(Variable v : variables) {
			cout << ASCII_BOLD << v.getName() << "@" << v.getType() << ":" << ASCII_RESET << "" << v.getContent() << "\n";
		}
		cout << ASCII_UNDERLINE << "\t\tFunctions" << ASCII_RESET << ":\n" << ASCII_RESET << ASCII_WHITE;
		for(const auto& s : functions) {
			cout << ASCII_BOLD << s[0] << ":" << ASCII_RESET << ASCII_WHITE << s[1] << "\n";
		}
	}

	if(!isOnTry) {
		if(!onFunction) {
			cout << ASCII_BOLD << BRIGHT_WHITE << "Error at line [" << line << "] " << ASCII_RESET << ASCII_BOLD << ASCII_RED << error << ASCII_RESET << " - " << ASCII_RED << message << "\n" << ASCII_RESET;
		} else {
			cout << ASCII_BOLD << BRIGHT_WHITE << "Error at line [" << line << "] while running #" << functionName << ":" << line2 << ASCII_RESET << ASCII_BOLD << ASCII_RED << " " << error << ASCII_RESET << " - " << ASCII_RED << message << "\n" << ASCII_RESET;
		}
	}
}

int run(String& command, String& returnS, string& exceptionN, int& line, int line2 = 0);
int compile(String& command, String& returnS, String& exceptionN, int& line);

String typeIn(String content);

void translateString(String& s, int line, String& exceptionN);
string translateSingleString(string s, int line, string exceptionN);

string translateSingleString(string s, int line, string exceptionN) {
	String result = "";
	if (s.front() == '\"' && s.back() == '\"') {
		result += s.substr(1, s.length() - 2);
	} else if(s == "true") {
		result += "true";
	} else if(s == "false") {
		result += "false";
	} else {
		try {
			stoi(s);
			result += s;
		} catch(invalid_argument&) {
			if(s.front() == '-') {
				try {
					stoi(s.substr(1));

					result += s;
				} catch(invalid_argument&) {}
			}

			std::string call = s;
			bool found = false;

			if(call.substr(0,1) == "#") {
				String functionName = call.substr(1);

				int i = 0;
				
				for(const auto& vec : functions) {
					if(functionName == vec[0]) {
						for(String& s : splitString(vec[1], '\n')) {
							String exc = "", ret = "";
							compile(s, ret, exc, line);
							
							if(ret != "") {
								result += ret;
							}
						}
						found = true;

						return result;
					}
					i++;
				}
				String exceptionN = "";
				throwError("epi2.error.function.unknownfunction", "That function doesn't exist.", exceptionN, line);   
			}

			for(Variable var : values) {
				if(s == var.getName()) {
					result += var.getContent();
					found = true;
				}
			}

			for(Variable var : variables) {
				if(findFirstIndexOutsideQuotes(call, ".") != -1) {
					if(call.substr(0, findFirstIndexOutsideQuotes(call, ".")) == var.getName()) {
						if(call.substr(var.getName().length(), 1) == ".") {
							string modifiers = call.substr(var.getName().length() + 1);
							
							if(modifiers == "Value") {
								result += var.getContent();
								found = true;
							} else if(modifiers == "Type") {
								result += typeIn(var.getContent());
								found = true;
							}

							try {
								if(modifiers.substr(0, 7) == "Equals(") {
									string args = modifiers.substr(7);
									args.pop_back();

									translateString(args, line, exceptionN);
									
									if(args == var.getContent()) {
										result += "true";
									} else {
										result += "false";
									}
									found = true;
								}
							} catch(exception&) {}

							if(var.getType() == "string") {
								if(modifiers == "Length") {
									result += to_string(var.getContent().length());
									found = true;
								} else if(modifiers == "Empty") {
									if(var.getContent().empty()) {
										result += "true";
									} else {
										result += "false";
									}
									found = true;
								} else if(modifiers == "IsNumber") {
									if(typeIn(var.getContent()) == "number") {
										result += "true";
									} else {
										result += "false";
									}
									found = true;
								} else if(modifiers == "IsBoolean") {
									if(typeIn(var.getContent()) == "boolean") {
										result += "true";
									} else {
										result += "false";
									}
									found = true;
								}

								try {
									if(modifiers.substr(0, 10) == "Substring(") {
										string args = modifiers.substr(10);
										args.pop_back();

										if(findFirstIndexOutsideQuotes(args, ",") != -1) {
											removeSpacesOutsideQuotes(args);

											string firstnum = splitOutsideQuotes(args, ',')[0];
											string secondnum = splitOutsideQuotes(args, ',')[1];

											translateString(firstnum, line, exceptionN);
											translateString(secondnum, line, exceptionN);

											if(typeIn(firstnum) == "number" && typeIn(secondnum) == "number") {
												try {
													result += var.getContent().substr(stoi(firstnum), stoi(secondnum));
													found = true;
												} catch(invalid_argument&) {
													// Throw invalid_argument
													throwError("epi2.error.invalid_argument", "The type that you introduced for making a substring of a string is not a number.", exceptionN, line);
													exit(-1);
												} catch(out_of_range&) {
													// Throw invalid_argument
													throwError("epi2.error.out_of_bounds", "The number that you introduced is invalid on that operation", exceptionN, line);
													exit(-1);
												}
											} else {
												// Throw invalid_argument
												throwError("epi2.error.invalid_argument", "The type that you introduced for making a substring of a string is not a number.", exceptionN, line);
												exit(-1);
											}
										} else {
											throwError("epi2.error.lang.missingargs", "Some arguments are missing.", exceptionN, line);
										}
									} else if(modifiers.substr(0, 5) == "Find(") {
										string args = modifiers.substr(5);
										args.pop_back();

										translateString(args, line, exceptionN);

										result += to_string(findFirstIndexOutsideQuotes(var.getContent(), args));

										found = true;
									} else if(modifiers.substr(0, 9) == "Contains(") {
										string args = modifiers.substr(9);
										args.pop_back();

										translateString(args, line, exceptionN);

										if(var.getContent().find(args) != string::npos) {
											result += "true";
										} else {
											result += "false";
										}
										found = true;
									} else if(modifiers.substr(0, 4) == "Add(") {
										string args = modifiers.substr(4);
										args.pop_back();

										translateString(args, line, exceptionN);

										result = var.getContent() + args;
										found = true;
									} else if(modifiers.substr(0, 7) == "Insert(") {
										string args = modifiers.substr(7);
										args.pop_back();

										if(findFirstIndexOutsideQuotes(args, ",") != -1) {
											removeSpacesOutsideQuotes(args);

											string firstnum = splitOutsideQuotes(args, ',')[0];
											string text = splitOutsideQuotes(args, ',')[1];

											translateString(firstnum, line, exceptionN);
											translateString(text, line, exceptionN);

											if(typeIn(firstnum) == "number") {
												try {
													string part1 = var.getContent().substr(0, stoi(firstnum)), part2 = var.getContent().substr(stoi(firstnum));

													result += part1 + text + part2;
													found = true;
												} catch(invalid_argument&) {
													// Throw invalid_argument
													throwError("epi2.error.invalid_argument", "The type that you introduced for making a substring of a string is not a number.", exceptionN, line);
													exit(-1);
												} catch(out_of_range&) {
													// Throw invalid_argument
													throwError("epi2.error.out_of_bounds", "The number that you introduced is invalid on that operation", exceptionN, line);
													exit(-1);
												}
											} else {
												// Throw invalid_argument
												throwError("epi2.error.invalid_argument", "The type that you introduced for making a substring of a string is not a number.", exceptionN, line);
												exit(-1);
											}
										} else {
											throwError("epi2.error.lang.missingargs", "Some arguments are missing.", exceptionN, line);
										}
									} else if(modifiers.substr(0, 3) == "At(") {
										string args = modifiers.substr(3);
										args.pop_back();

										translateString(args, line, exceptionN);

										if(typeIn(args) == "number") {
											try {
												result += var.getContent().at(stoi(args));
												found = true;
											} catch(invalid_argument&) {
												// Throw invalid_argument
												throwError("epi2.error.invalid_argument", "The type that you introduced for making a substring of a string is not a number.", exceptionN, line);
												exit(-1);
											} catch(out_of_range&) {
												// Throw invalid_argument
												throwError("epi2.error.out_of_bounds", "The number that you introduced is invalid on that operation", exceptionN, line);
												exit(-1);
											}
										} else {
											// Throw invalid_argument
											throwError("epi2.error.invalid_argument", "The type that you introduced for making a substring of a string is not a number.", exceptionN, line);
											exit(-1);
										}
									} else if(modifiers.substr(0,6) == "Split(") {
										string args = modifiers.substr(6);
										args.pop_back();

										if(findFirstIndexOutsideQuotes(args, ",") != -1) {
											removeSpacesOutsideQuotes(args);

											string delimeter = splitOutsideQuotes(args, ',')[0];
											string index = splitOutsideQuotes(args, ',')[1];

											translateString(delimeter, line, exceptionN);
											translateString(index, line, exceptionN);

											if(typeIn(index) == "number") {
												try {
													result += splitStringUsingString(var.getContent(), delimeter)[stoi(index)];
													found = true;
												} catch(invalid_argument&) {
													// Throw invalid_argument
													throwError("epi2.error.invalid_argument", "The type that you introduced for making a substring of a string is not a number.", exceptionN, line);
													exit(-1);
												} catch(out_of_range&) {
													// Throw invalid_argument
													throwError("epi2.error.out_of_bounds", "The number that you introduced is invalid on that operation", exceptionN, line);
													exit(-1);
												}
											} else {
												// Throw invalid_argument
												throwError("epi2.error.invalid_argument", "The type that you introduced for making a substring of a string is not a number.", exceptionN, line);
												exit(-1);
											}
										} else {
											throwError("epi2.error.lang.missingargs", "Some arguments are missing.", exceptionN, line);
										}
									} else if(modifiers.substr(0, 6) == "Crypt(") {
										string args = modifiers.substr(6);
										args.pop_back();

										translateString(args, line, exceptionN);

										result += encryptText(var.getContent(), args);
										found = true;
									} else if(modifiers.substr(0, 13) == "ReplaceIndex(") {
										string args = modifiers.substr(13);
										args.pop_back();

										if(findFirstIndexOutsideQuotes(args, ",") != -1) {
											removeSpacesOutsideQuotes(args);

											string index = splitOutsideQuotes(args, ',')[0];
											string match = splitOutsideQuotes(args, ',')[1];
											string replaceWith = splitOutsideQuotes(args, ',')[2];

											translateString(index, line, exceptionN);
											translateString(match, line, exceptionN);
											translateString(replaceWith, line, exceptionN);

											if(typeIn(index) == "number") {
												try {
													result += replaceNthOccurrence(var.getContent(), match, replaceWith, stoi(index) + 1);
													found = true;
												} catch(invalid_argument&) {
													// Throw invalid_argument
													throwError("epi2.error.invalid_argument", "The type that you introduced for making a substring of a string is not a number.", exceptionN, line);
													exit(-1);
												} catch(out_of_range&) {
													// Throw invalid_argument
													throwError("epi2.error.out_of_bounds", "The number that you introduced is invalid on that operation", exceptionN, line);
													exit(-1);
												}
											} else {
												// Throw invalid_argument
												throwError("epi2.error.invalid_argument", "The type that you introduced for making a substring of a string is not a number.", exceptionN, line);
												exit(-1);
											}
										} else {
											throwError("epi2.error.lang.missingargs", "Some arguments are missing.", exceptionN, line);
										}
									} else if(modifiers.substr(0,11) == "ReplaceAll(") {
										string args = modifiers.substr(11);
										args.pop_back();

										if(findFirstIndexOutsideQuotes(args, ",") != -1) {
											removeSpacesOutsideQuotes(args);

											string match = splitOutsideQuotes(args, ',')[0];
											string replaceWith = splitOutsideQuotes(args, ',')[1];

											translateString(match, line, exceptionN);
											translateString(replaceWith, line, exceptionN);

											try {
												String original = var.getContent();
												replaceAll(original, match, replaceWith);
												result += original;
												found = true;
											} catch(invalid_argument&) {
												// Throw invalid_argument
												throwError("epi2.error.invalid_argument", "The type that you introduced for making a substring of a string is not a number.", exceptionN, line);
												exit(-1);
											} catch(out_of_range&) {
												// Throw invalid_argument
												throwError("epi2.error.out_of_bounds", "The number that you introduced is invalid on that operation", exceptionN, line);
												exit(-1);
											}
										} else {
											throwError("epi2.error.lang.missingargs", "Some arguments are missing.", exceptionN, line);
										}
									}
								} catch(exception&) {}
							}
						} else {
							result += var.getContent();
							found = true;
						}
					}
				} else {
					if(call == var.getName()) {
						result += var.getContent();
						found = true;
					}
				}
			}

			if(call.substr(0, 4) == "read") {
				String varName = s.substr(4);

				for(File file : files) {
					if(file.getName() == varName) {
						if(!file.isOpen()) {
							throwError("epi2.file.notopen", "The file isn't opened, you should define the file to write before writing it.", exceptionN, line);
						} else {
							result += file.read();
							found = true;
						}
					}
				}
			} else if(s.substr(0, 4) == "open") {
				String varName = s.substr(4);

				for(File file : files) {
					if(file.getName() == varName) {
						if(file.isOpen()) {
							result += "true";
						} else {
							result += "false";
						}
						found = true;
					}
				}
			}

			if (!found) {
				result += "undefined";
			}
		}
	}

	return result;
}

string get_text_inside_parenthethes(string s) {
	int parenthesesCount = 0;
	string result = "";
	bool insideQuotes = false;

	for(int i = 0; i < s.length(); i++)
	{
		const char c = s[i];
		if(c == '(' && !insideQuotes) 
		{
			if(parenthesesCount >= 1) 
			{
				result += c;
			}
			parenthesesCount++;
		} else if(c == ')' && !insideQuotes)
		{
			if(parenthesesCount == 0) 
			{
				return "";
			} else if(parenthesesCount >= 2)
			{
				result += c;
			}
			parenthesesCount--;
		} else if(c == '\"') 
		{
			if(parenthesesCount >= 1) 
			{
				result += c;
			}
			insideQuotes = !insideQuotes;
		} else if(parenthesesCount >= 1)
		{
			result += c;
		}
	}

	return result;
}

// Util Functions for compile
void translateString(string& s, int line, string& exceptionN) {
	removeSpacesOutsideQuotes(s);
	string result = "";

	if (canBeOperation(s)) {
		result = to_string(calculate(s));
		s = result;
		return;
	}

	if (isCharOutsideQoutes(s, '+')) {
		vector<string> parts = splitOutsideQuotes(s, '+');

		for (const auto& part : parts) {
			// Check if the part is another part
			if(part.c_str()[0] == '(' && part.c_str()[part.length() - 1] == ')') {
				string innerText = get_text_inside_parenthethes(part); // part = "(2 + 2)" > "2 + 2"
				translateString(innerText, line, exceptionN);
				result += innerText;

				continue;
			}
				
			result += translateSingleString(part, line, exceptionN);
		}
	} else {
		result += translateSingleString(s, line, exceptionN);
	}

	s = result;
}

Variable analyze(String s, int line, String& exceptionN) {
	String i = s;
	String type, content;
	translateString(i, line, exceptionN);

	if(i == "true" || i == "false") {
		type = "boolean";
	} else {
		try {
			stoi(i);
			type = "number";
		} catch(invalid_argument&) {
			type = "string";
		}
	}

	Variable var(type, "", i);

	return var;
}

String typeIn(String content) {
	if(content == "true" || content == "false") {
		return "boolean";
	} else {
		try {
			stoi(content);
			return "number";
		} catch(invalid_argument&) {
			return "string";
		}
	}

	return "string";
}

// Included statements
bool dirGUIIncluded = false;

// Utils for reading files
bool nameAlreadyUsed(String name) {
	for(Variable var : variables) {
		if(name == var.getName()) {
			return true;
		}
	}
	for(Variable var : values) {
		if(name == var.getName()) {
			return true;
		}
	}
	return false;
}

String file = "";

void logThings(String command, int line, bool onFunction, String functionName, int line2, String runnedFunctionAs, bool onTry) {
	String result = "Running command: " + command + "; at line: ";
	if(onFunction) {
		result += "#" + functionName + ":" + to_string(line2) + ":" + to_string(line);

		result += "; Function runned as: " + runnedFunctionAs;
	} else {
		result += to_string(line);
	}

	result += &"; On try: " [ onTry];
	fileObject << result + "\n";
}

bool doingSwitch = false;
bool doingCase = false;
String switchText = "";

bool skipSwitch = false;

bool doingExternal = false;
string externalLang = "";
string externalCode = "";

int compile(String& command, String& returnS, String& exceptionN, int& line) {
	if(writingFunction) {
		if(command.substr(0,4) == "	" && functionToWrite != "") {
			command = command.substr(4);
			int i = 0;
			for(const auto& vec : functions) {
				if(vec[0] == functionToWrite) {
					functions[i][1] += command + "\n";
					return 0;
				}
				i++;
			}
			writingFunction = false;
			functionToWrite = "";
		} else {
			writingFunction = false;
			functionToWrite = "";
			run(command, returnS, exceptionN, line);
		}
	} else if(writingTry) {
		if(command.substr(0,4) == "	") {
			command = command.substr(4);
			tryCode = command + "\n";
			return 0;
		} else {
			writingTry = false;
			if(command.substr(0, 5) == "catch") {
				catchCode = "";
				std::string errorToCatch = command.substr(6);

				for(String& l : splitOutsideQuotes(tryCode, '\n')) {
					String excepN = "";
					run(l, returnS, excepN, line);
					if(excepN == errorToCatch || (errorToCatch == "*" && excepN != "")) {
						writingCatch = true;
						writingTry = false;
						tryCode = "";
						return 0;
					}
				}
				return 0;
			} else {
				throwError("epi2.error.lang.nocatchaftertry", "You should put a catch statemet after a try statement.", exceptionN, line);
				return 1;
			}
		}
	} else if(writingCatch) {
		if(command.substr(0,4) == "	") {
			command = command.substr(4);
			
			catchCode += command + "\n";

			try {
				String s = splitOutsideQuotes(file, '\n')[line+1];
			} catch(exception&) {}
			
			return 0;
		} else {
			writingCatch = false;
			for(String s : splitOutsideQuotes(catchCode, '\n')) {
				run(s,returnS, exceptionN, line);
			}
			catchCode = "";
			run(command,returnS,exceptionN,line);
			return 0;
		}
	} else if(doingCase) {
		if(command.substr(0,8) == "	") {
			string com = command.substr(8);
			
			run(command, returnS, exceptionN, line);
		} else {
			doingCase = false;
			skipSwitch = true;
		}
	} else if(doingSwitch) {
		if(command.substr(0,4) == "	") {
			if(!skipSwitch) {
				if(command.substr(4,4) == "case") {
					string match = command.substr(9);

					translateString(match, line, exceptionN);

					if(switchText == match) {
						doingCase = true;
						skipSwitch = true;
					}
				} else if(command.substr(4) == "default") {
					doingCase = true;

				}
			} else {
				return 0;
			}
		} else {
			doingSwitch = false;
			skipSwitch = false;
			switchText = "";

			run(command, returnS, exceptionN, line);
		}
	} else if(doingExternal) {
		if(command.substr(0,4) == "	") {
			string com = command.substr(4);
			
			externalCode += com + "\n";
		} else {
			doingExternal = false;

			if(externalLang == "python") {
				ofstream f("temp.py");
				f << externalCode;
				f.close();
				system("python temp.py");
				remove("temp.py");
			} else if(externalLang == "bat") {
				ofstream f("temp.bat");
				f << "@echo off" << "\n";
				f << externalCode;
				f.close();
				system(".\\temp.bat");
				remove("temp.bat");
			}

			externalCode = "";
			externalLang = "";
			run(command, returnS, exceptionN, line);
		}
	} else {
		run(command, returnS, exceptionN, line);
	}

	return 0;
}

bool prevLineComment = false;

int run(String& command, String& returnS, string& exceptionN, int& line, int line2) {
	if(command.empty()) { return 0; }

	while(command.substr(0,4) == "	") {
		command = command.substr(4);
	}
	if(command.find('@') == std::string::npos) {
		command = splitOutsideQuotes(command, '@')[0];
	}

	if(prevLineComment && command.at(0) == '(') {
		return 0;
	}

	if(command.at(0) == '@') {
		prevLineComment = true;
		return 0;
	} else {
		prevLineComment = false;
	}

	String desCommand;
	if(command.find(' ') != 0) {
		desCommand = command.substr(0, command.find(' '));
	} else {
		desCommand = command;
	}
	if(	   desCommand == "print" || desCommand == "out") {
		// print function is for printing text into the screen
		// @example print "Hello, " + username + "!"
		// @since v_0.1
		String s = command.substr(desCommand.length() + 1);
		translateString(s, line, exceptionN);
		cout << s << "\n";
	} else if(desCommand == "printc" || desCommand == "outc") {
		// print function is for printing text into the screen but without new line
		// @example printc "Hello, " + username + "!"
		// @since v_0.1
		String s = command.substr(desCommand.length() + 1);
		translateString(s, line, exceptionN);
		cout << s;
	} else if(desCommand == "cmd") {
		// cmd function is for running a command line command
		// @example cmd "winver"
		// @since v_0.1
		String s = command.substr(desCommand.length() + 1);
		translateString(s, line, exceptionN);
		system(s.c_str());
	} else if(desCommand == "var") {
		String s = command.substr(desCommand.length() + 1);
		String name = s.substr(0, s.find_first_of(' '));

		if(nameAlreadyUsed(name)) {
			throwError("epi2.error.variables.redefinition", "That name is already used for a variable.", exceptionN, line);
			return -1;
		}

		if(findFirstIndexOutsideQuotes(s, "=") != -1) {
			if(s.substr(s.find_first_of(name) + 1 + name.length(), 1) == "=") {
				removeSpacesOutsideQuotes(s);
				String content = s.substr(s.find_first_of("=") + 1);

				Variable t = analyze(content, line, exceptionN);

				Variable var(t.getType(), name, t.getContent());
				variables.push_back(var);
			} else {
				throwError("epi2.syntax.variables.definition", "Missing equal expression for definig a variable.", exceptionN, line);
			}
		} else {
			if(s.substr(s.find_first_of(' ') + 1, 2) == "as") {
				try {
					String t = s.substr(s.find_last_of(' ') + 1);
					
					if(t == "string" || t == "number" || t == "boolean") {
						Variable var(t, name, "");
						variables.push_back(var);
					} else {
						throwError("epi2.variables.undefinedtype", "If you define an empty variable you should put the type of the variable.", exceptionN, line);
					}
				} catch(out_of_range&) {
					throwError("epi2.variables.undefinedtype", "If you define an empty variable you should put the type of the variable.", exceptionN, line);
				}
			} else {
				throwError("epi2.variables.undefinedtype", "If you define an empty variable you should put the type of the variable.", exceptionN, line);
			}
		}
	} else if(desCommand == "function") {
		// function keyword is for creating a function for storing code
		// @example function helloWorld
		// @since v_0.1
		String args = command.substr(desCommand.length() + 1);

		if(args.back() != ':') {
			throwError("epi2.functions.baddefinition", "A function should always be defined with code.", exceptionN, line);
			return 1;
		}

		String name = args.substr(0, args.length() - 1);

		if(!validateVariableName(name)) {
			throwError("epi2.functions.baddeclaration", "The name you choose for the function isn't valid.", exceptionN, line);
			return 1;
		}

		vector<String> v = {name, ""};
			
		writingFunction = true;
		functionToWrite = name;

		functions.push_back(v);

		return 0;
	} else if(desCommand.substr(0,1) == "$") {
		// $ keyword is for editing a variable
		// @example $hello "Hola"
		// @since v_0.1
		String varToWrite = desCommand.substr(1);

		for(Variable& var : variables) {
			if(varToWrite == var.getName()) {
				String content = command.substr(desCommand.length() + 1);
				Variable var2(analyze(content, line, exceptionN));
				if(var2.getType() == var.getType()) {
					translateString(content, line, exceptionN);
					var.setContent(content);
					return 0;
				} else {
					if(var2.getType() == "number" && var.getType() == "string") {
						translateString(content, line, exceptionN);
						var.setContent(content);
						return 0;	
					}
					throwError("epi2.error.invalid_argument", "The type that you introduced for updating the variable (" + var2.getType() + ") can't be used for updating a variable with type (" + var.getType() + ")", exceptionN, line);
					return -1;
				}
			}
		}
		throwError("epi2.variables.unknownvariable", "That variable doesn't exist", exceptionN, line);
	} else if(command.length() >= 1 && command.substr(0,1) == "@") {
		return 0;
	} else if(desCommand.length() >= 1 && desCommand.substr(0,1) == "#") {
		int i = 0;
		line2 = 1;

		for(const auto& vec : functions) {
			if(desCommand == "#" + vec[0]) {
				for(String& s : splitString(vec[1], '\n')) {
					String exc = "", ret = "";
					compile(s, ret, exc, line);
					if(exc != "") {
						exceptionN = exc;
						return 1;
					} else if(ret != "") {
						returnS = ret;
						return 0;
					}
					line2++;
				}
				return 0;
			}
			i++;
		}
		throwError("epi2.error.function.unknownfunction", "That function doesn't exist.", exceptionN, line);
		return 1;
	} else if(desCommand == "return") {
		if(command == "return") {
			return 0;
		} else {
			try {
				String retText = command.substr(desCommand.length() + 1);
				translateString(retText, line, exceptionN);
				returnS = retText;
				return 0;
			} catch(exception&) {
				return 0;
			}
		}
	} else if(desCommand == "in") {
		String s = command.substr(desCommand.length() + 1);

		String r = "";
		getline(cin, r);

		for(Variable& var : variables) {
			if(s == var.getName()) {
				if(typeIn(r) == var.getType()) {
					var.setContent(r);
					return 0;
				} else {
					if(typeIn(r) == "number" && typeIn(r) == "string") {
						var.setContent(r);
						return 0;	
					}
					
					throwError("epi2.error.invalid_argument", "The type that you introduced for updating the variable (" + analyze(r, line, exceptionN).getType() + ") can't be used for updating a variable with type (" + var.getType() + ")", exceptionN, line);
					return -1;
				}
			}
		}

		// The variable doesn't exist so we create a new variable.
		String t = typeIn(r);
		Variable var(t, s, r);
		variables.push_back(var);
		return 0;
	} else if(desCommand == "~include") {
		String s = command.substr(desCommand.length() + 1);
		if(s == "gui") {
			dirGUIIncluded = true;
		} else {
			if(exists(s)) {
				fstream f(s);
				String lineN, exc, ret;
				while(getline(f, lineN)) {
					compile(lineN, exc, ret, line);
					if(exc != "") {
						return 1;
					}
				}
				f.close();
			} else {
				cout << ASCII_BOLD << BRIGHT_WHITE << "[" << line << "] " << ASCII_RESET << ASCII_BOLD<< ASCII_RED << "epi2.error.lang.unknownlibrary" << ASCII_RESET << " - " << ASCII_RED << "That library doesn't exist or the file doesn't exist.\n" << ASCII_RESET;
				exceptionN = "epi2.error.lang.unknownlibrary";
				return 1;
			}
		}
	} else if(desCommand == "msgbox") {
		if(dirGUIIncluded) {
			if(command.find(",") == std::string::npos) {
				String s = command.substr(desCommand.length() + 1);
				String t = splitOutsideQuotes(s, ',')[1];
				String m = splitOutsideQuotes(s, ',')[0];
				translateString(t, line, exceptionN);
				translateString(m, line, exceptionN);
				showDialog(m, t);
			} else {
				String s = command.substr(desCommand.length() + 1);
				translateString(s, line, exceptionN);
				showDialog(s, "epi2");
			}
		} else {
			cout << ASCII_BOLD << BRIGHT_WHITE << "[" << line << "] " << ASCII_RESET << ASCII_BOLD<< ASCII_RED << "epi2.error.lang.librarynotimported" << ASCII_RESET << " - " << ASCII_RED << "The library which contain the function isn't initialized.\n" << ASCII_RESET;
			exceptionN = "epi2.error.lang.librarynotimported";
			return 1;
		}
	} else if(desCommand == "inn") {
		String s = command.substr(desCommand.length() + 1);

		String r = takePasswdFromUser("");

		for(Variable& var : variables) {
			if(s == var.getName()) {
				if(typeIn(r) == var.getType()) {
					var.setContent(r);
					return 0;
				} else {
					if(typeIn(r) == "number" && var.getType() == "string") {
						translateString(r, line, exceptionN);
						var.setContent(r);
						return 0;	
					}
					throwError("epi2.error.invalid_argument", "The type that you introduced for updating the variable (" + typeIn(r) + ") can't be used for updating a variable with type (" + var.getType() + ")", exceptionN, line);
					return -1;
				}
			}
		}
		// The variable doesn't exist so we create a new variable.
		String t = typeIn(r);
		Variable var(t, s, r);
		variables.push_back(var);
		return 0;
	} else if(desCommand == "inp") {
		String s = command.substr(desCommand.length() + 1);
		if(s.find(" ") == std::string::npos) {
			throwError("epi2.error.lang.missingargs", "Some arguments are missing.", exceptionN, line);
			exceptionN = "epi2.error.lang.missingargs";
			return 1;
		}
		String vName = splitOutsideQuotes(s, ' ')[0];
		String text = s.substr(vName.length() + 1);
		translateString(text, line, exceptionN);

		String r = takePasswdFromUser(text);

		for(Variable& var : variables) {
			if(vName == var.getName()) {
				if(typeIn(r) == var.getType()) {
					var.setContent(r);
					return 0;
				} else {
					if(typeIn(r) == "number" && var.getType() == "string") {
						translateString(r, line, exceptionN);
						var.setContent(r);
						return 0;	
					}
					throwError("epi2.error.invalid_argument", "The type that you introduced for updating the variable (" + typeIn(r) + ") can't be used for updating a variable with type (" + var.getType() + ")", exceptionN, line);
					return -1;
				}
			}
		}
		// The variable doesn't exist so we create a new variable.
		String t = typeIn(r);
		Variable var(t, s, r);
		variables.push_back(var);
		return 0;
	} else if(   command == "try") {
		writingTry = true;
	} else if(desCommand == "File") {
		String args = command.substr(desCommand.length() + 1);
		removeSpacesOutsideQuotes(args);
		
		if(args.find("is") != string::npos) {
			// The code defined a filename
			String varName = args.substr(0, findFirstIndexOutsideQuotes(args, "is"));
			String filename = args.substr(varName.length() + 2);

			translateString(filename, line, exceptionN);

			File file(varName, filename);

			files.push_back(file);
		} else {
			// The code didn't defined a filename
			String varName = args;

			File file(varName, "");

			files.push_back(file);
		}
	} else if(desCommand == "append") {
		String text, varName, args = command.substr(desCommand.length() + 1);

		removeSpacesOutsideQuotes(args);

		text = args.substr(0, findFirstIndexOutsideQuotes(args, "to"));
		varName = args.substr(text.length() + 2);
		translateString(text, line, exceptionN);

		for(File file : files) {
			if(file.getName() == varName) {
				if(!file.isOpen()) {
					throwError("epi2.file.notopen", "The file isn't opened, you should define the file to write before writing it.", exceptionN, line);
					return -1;
				}

				file.write(file.read() + text);
				return 0;
			}
		}

		throwError("epi2.lang.fileundefined", "The file isn't defined.", exceptionN, line);
		return -1;
	} else if(desCommand == "write") {
		String text, varName, args = command.substr(desCommand.length() + 1);

		removeSpacesOutsideQuotes(args);

		text = args.substr(0, findFirstIndexOutsideQuotes(args, "to"));
		varName = args.substr(text.length() + 2);
		translateString(text, line, exceptionN);

		for(File file : files) {
			if(file.getName() == varName) {
				if(!file.isOpen()) {
					throwError("epi2.file.notopen", "The file isn't opened, you should define the file to write before writing it.", exceptionN, line);
					return -1;
				}

				file.write(text);
				return 0;
			}
		}

		throwError("epi2.lang.fileundefined", "The file isn't defined.", exceptionN, line);
		return -1;
	} else if(desCommand == "open") {
		String varName, filename, args = command.substr(desCommand.length() + 1);

		removeSpacesOutsideQuotes(args);

		varName = args.substr(0, findFirstIndexOutsideQuotes(args, "at"));
		filename = args.substr(varName.length() + 2);

		translateString(filename, line, exceptionN);

		for(File file : files) {
			if(file.getName() == varName) {
				file.open(filename);
				return 0;
			}
		}

		throwError("epi2.lang.fileundefined", "The file isn't defined.", exceptionN, line);
		return -1;
	} else if(desCommand == "delete") {
		String varName, args = command.substr(desCommand.length() + 1);

		removeSpacesOutsideQuotes(args);
		varName = args;

		for(File file : files) {
			if(file.getName() == varName) {
				if(!file.isOpen()) {
					throwError("epi2.file.notopen", "The file isn't opened, you should define the file to write before writing it.", exceptionN, line);
					return -1;
				}

				file.deletef();
				return 0;
			}
		}
		for(Variable& var : variables) {
			if(var.getType() == "string" && var.getName() == varName) {
				var.getContent().clear();
				return 0;
			}
		}

		throwError("epi2.lang.variable undefined", "The variable isn't defined.", exceptionN, line);
		return -1;
	} else if(desCommand == "switch") {
		String args = command.substr(7);
		translateString(args, line, exceptionN);
		switchText = args;
		doingSwitch = true;
	} else if(desCommand == "val") {
		String s = command.substr(desCommand.length() + 1);
		String name = s.substr(0, s.find_first_of(' '));

		if(nameAlreadyUsed(name)) {
			throwError("epi2.error.variables.redefinition", "That name is already used for a variable.", exceptionN, line);
			return -1;
		}

		if(findFirstIndexOutsideQuotes(s, "=") != -1) {
			if(s.substr(s.find_first_of(name) + 1 + name.length(), 1) == "=") {
				removeSpacesOutsideQuotes(s);
				String content = s.substr(s.find_first_of("=") + 1);

				Variable t = analyze(content, line, exceptionN);

				Variable var(t.getType(), name, t.getContent());
				values.push_back(var);
			} else {
				throwError("epi2.syntax.variables.definition", "Missing equal expression for definig a value.", exceptionN, line);
			}
		} else {
			throwError("epi2.values.empty_value", "You should initialize the value for using it.", exceptionN, line);		
		}
	} else if(desCommand == "external") {
		string extLang = command.substr(desCommand.length() + 1);
		translateString(extLang, line, exceptionN);
		externalLang = extLang;
		doingExternal = true;
	} else {
		removeSpacesOutsideQuotes(command);

		try {
			for(Variable& var : variables) {
				if(command.substr(0, var.getName().length() + 1) == var.getName() + "=") {
					String newtext = splitString(command, '=')[1];

					translateString(newtext, line, exceptionN);

					var.setContent(newtext);
					return 0;
				}


					if(var.getType() == "string" && command.substr(0, var.getName().length() + 2) == var.getName() + "+=") {
						String args = command.substr(var.getName().length() + 2);

					translateString(args, line, exceptionN);

					var.setContent(var.getContent() + args);
					return 0;
					}

				if(var.getType() == "number" && command == var.getName() + "++") {
					var.setContent(to_string(stoi(var.getContent()) + 1));
					return 0;
				} else if(var.getType() == "number" && command == var.getName() + "--") {
					var.setContent(to_string(stoi(var.getContent()) - 1));
					return 0;
				} else if(var.getType() == "number" && command == var.getName() + "**") {
					var.setContent(to_string(stoi(var.getContent()) * stoi(var.getContent())));
					return 0;
				} else if(var.getType() == "number" && command.substr(0,var.getName().length() + 2) == var.getName() + "+=") {
					String args = command.substr(var.getName().length() + 2);

					translateString(args, line, exceptionN);

					var.setContent(to_string(stoi(var.getContent()) + stoi(args)));
					return 0;
				}
			}
		} catch(exception&) {}

		throwError("epi2.lang.unexpected.notacommand", "I think the command is wrong.", exceptionN, line);
		exceptionN = "epi2.lang.unexpected.notacommand";
		return 1;
	}
	return 1;
}

void processFile(const std::string& s) {
	std::ifstream f(s);
	if (!f.is_open()) return;

	std::string line, ret, exc;
	int li = 0;

	// Pre-allocate memory if you know the rough size
	ret.reserve(256);
	exc.reserve(256);

	// Read file lines into memory to reduce I/O overhead
	std::vector<std::string> lines;
	std::string temp;
	while (getline(f, temp)) {
		lines.emplace_back(std::move(temp));
		file += temp + "\n";
	}

	f.close();

	lines.push_back("");

	// Process lines after reading
	for (String& line : lines) {
		compile(line, ret, exc, li);
		li++;
	}
}

bool cpp_writing_function = false;

string toCpp(string line, string& beforeMain) {
    if(line.substr(0,6) == "print ") {
        String content = line.substr(6);
        translateString(content, 0, line);
        return "cout << \"" + content + "\\n\";\n";
    } else if(line.substr(0,7) == "printc ") {
        String content = line.substr(7);
        translateString(content, 0, line);
        return "cout << \"" + content + "\";\n";
    } else if(line.substr(0,4) == "cmd ") {
        String content = line.substr(4);
        translateString(content, 0, line);
        return "system(\"" + content + "\");\n";
    } else if(line.substr(0,3) == "in") {
        String content = line.substr(3);
        translateString(content, 0, line);
        return "cin >> " + content + ";\n";
    } else if(line.substr(0,9) == "function ") {
        beforeMain += "void " + line.substr(9) + "() {\n";
        cpp_writing_function = true;
    } else {
        return "// " + line + "\n";
    }

	return NULL;
}

// Main
int main(int argc, char** argv) {
	if(isStrOnCharArr("--version", argv, argc)) {
		cout << "epi" << (char) 253 << " v_0.203\n";
		cout << "This program is open-source software licensed with GNU GPL v3.\n";
		cout << "Run \"epi2 --show-license\" for reading it.\n";
		cout << "You can modify this code BUT the name of the new program should contain the \"" << (char) 253 << "\" symbol.\n";
		return 0;
	}
	if(isStrOnCharArr("--show-license", argv, argc)) {
		cout << license << "\n";
		return 0;
	}
	if(isStrOnCharArr("--debug-v", argv, argc)) {
		debug = true;
	}
	if(argc != 1) {
		String s(argv[1]);

		if(s == "install") {
			if(argc != 2) {
				String libname(argv[2]);

				if(fileExistsOnInternet("https://elitees.github.io/epi2-libraries/" + libname + "/libInfo.inf")) {
					if(fileExistsOnInternet("https://elitees.github.io/epi2-libraries/" + libname + "/libFiles.list")) {
						vector<String> libFiles = splitString(readFileFromInternet("https://elitees.github.io/epi2-libraries/" + libname + "/libFiles.list"), '\n');

						system("mkdir lib");

						for(const auto& f : libFiles) {
							downloadFileWithProgress("https://elitees.github.io/epi2-libraries/" + libname + "/" + f, "./lib/" + f);
						}

						return 0;
					} else {
						cout << ASCII_RED << ASCII_BOLD << "Error: " << ASCII_RESET << "Library is corrupt, uneable to locate files of the library..\n";
						return -1;
					}
				} else {
					cout << ASCII_RED << ASCII_BOLD << "Error: " << ASCII_RESET << "Unknown library.\n";
					return -1;
				}
			} else {
				cout << ASCII_RED << ASCII_BOLD << "Error: " << ASCII_RESET << "No lib was specified.\n";
				return -1;
			}
		}

		
		if(isStrOnCharArr("--f-debug", argv, argc)) {
			fullDebug = true;
			time_t rawtime;
			struct tm ltm;
			time(&rawtime);

			#ifdef _WIN32
			localtime_s(&ltm, &rawtime);
			#else
			localtime_r(&rawtime, &ltm);
			#endif
			std::ostringstream ss;
			ss << std::put_time(&ltm, "%d/%m/%Y %H:%M");

			fileObject << "-------------------\nCompilation Started at: " + ss.str() + " Compiling File: " + s + "\n-------------------\n";
		} else {
			fileObject.close();
			remove("epi2.debug.log");
		}
		if(exists(s)) {
			processFile(s);
		} else {
			cout << ASCII_BOLD << BRIGHT_RED << "Fatal Error: The file you gave doesn't exist.\n" << ASCII_RESET;
		}
	} else {
		#ifndef _WIN32
		system("title epi2");
		#endif
		cout << ASCII_BG_GREEN << ASCII_BLACK << ASCII_BOLD << " * " << ASCII_RESET << " epi" << (char) 253 << " v_0.203\nEnter any epi" << (char) 253 << " command or enter \"help\" for get a list of commands.\n";
		String command = "";
		while (command != "exit()") {
			cout << " " << ASCII_BOLD << BRIGHT_WHITE << ">> " << ASCII_RESET;
			getline(cin, command);
			if(command == "exit()") {} else if(command == "help") {
				cout << "List of commands:\n";
				cout << "- \"help\" Show this list.\n";
				cout << "- \"version\" Show the current epi" << (char) 253 << " version.\n";
				cout << "- \"exit()\" Exit\n";
				cout << "Go to " << ASCII_UNDERLINE << ASCII_BOLD << ASCII_CYAN << "https://e--epi.web.app/docs/" << ASCII_RESET << " for getting a full list of epi commands.\n";
			} else if(command == "version") {
				cout << "epi" << (char) 253 << " v_0.203\n";
				cout << "This program is open-source software licensed with GNU GPL v3.\n";
				cout << "Run \"epi2 --show-license\" for reading the license.\n";
				cout << "You can modify this code BUT the name of the new program should contain the \"" << (char) 253 << "\" symbol.\n";
			} else {
				String returnS;
				String exceptionN;
				int l = 0;
				run(command, returnS, exceptionN, l);
				if(!returnS.empty()) {
					cout << ASCII_BOLD << BRIGHT_WHITE << " << " << ASCII_RESET << returnS << "\n";
				}
			}
		}
		cout << ASCII_RESET;
		return 0;
	}
	cout << ASCII_RESET;
	return 0;
}
