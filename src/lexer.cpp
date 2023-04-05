#include <regex>
#include <string>

/// @param code The code to be lexed
/// @param size a reference to an int that will be set to the size of the array
/// @return A pointer to an array of strings that contain the tokens
/// @note The way im currently doing this is pretty messy as it goes from
///       a s_regex_token_iterator to a string list and then in main gets turned to a vector
///       i will change this at a later date as performance and memory safety right now isn't an issue 03/04/2023
std::string *lex(std::string code, int &size)
{
	using namespace std;
	regex pattern(R"(==|!=|<=|>=|/|::|\+|\-|\*|\|&|=|;|:|,|\{|\}|\(|\)|<|>|\[|\]|[a-zA-Z]+([a-zA-Z0-9])*|\d+)");

	sregex_token_iterator it(code.begin(), code.end(), pattern);
	sregex_token_iterator end;

	size		 = distance(it, end);
	auto *tokens = new std::string[size];

	for (int i = 0; it != end; ++it, ++i) {
		tokens[i] = *it;
	}

	return tokens;
}