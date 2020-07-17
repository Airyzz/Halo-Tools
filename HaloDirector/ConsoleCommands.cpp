#include "ConsoleCommands.h"
#include "StdInc.h"
#include <unordered_map>
#include <string>
#include <sstream>
std::unordered_map<std::string, ConsoleCommand*> umap;

bool has_key(std::string key)
{
	// Key is not present 
	if (umap.find(key) == umap.end())
		return false;

	return true;
}

std::string convertToString(char* a, int size)
{
	int i;
	std::string s = "";
	for (i = 0; i < size; i++) {
		if (a[i] != (char)0)
			s = s + a[i];
	}
	return s;
}

std::vector<std::string> split(const std::string& str, const std::string& delim)
{
	std::vector<std::string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == std::string::npos) pos = str.length();
		std::string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}

void AddText(const char* buffer, int size)
{
	std::string s = convertToString((char*)buffer, size);
	std::vector<std::string> segments = split(s, " ");

	if (segments.size() > 0)
	{

		if (segments.size() == 1)
		{
			Log::Info("Function: %s", segments[0].c_str());

			if (has_key(segments[0]))
			{
				umap[segments[0]](0);
			}
			else {
				Log::Error("Function not found: %s", segments[0]);
			}
		}
		else if (segments.size() == 2)
		{
			Log::Info("Function: %s", segments[0].c_str());
			Log::Info("Argument: %s", segments[1].c_str());

			if (has_key(segments[0]))
			{

				int arg = 0;

				try {
					arg = std::stoi(segments[1]);
				}
				catch (std::invalid_argument const& e) {
					Log::Error("Console Commands -> Invalid Argument");
				}
				catch (std::out_of_range const& e) {
					Log::Error("Console Commands -> Out of Range");
				}

				umap[segments[0]](arg);


			}
			else {
				Log::Error("Function not found: %s", segments[0].c_str());
			}
		}
		else {
			Log::Error("Invalid Console Command: %s", buffer);
		}
	}
}

DWORD WINAPI CommandLoop(LPVOID Param) {
	while (true)
	{
		char buffer[255];
		size_t sizeOut;
		_cgets_s<255>(buffer, &sizeOut);
		
		AddText(buffer, (int)sizeOut);
	}
}

void ConsoleCommands::Add(std::string name, ConsoleCommand* function)
{
	umap[name] = function;
}

void test(int arg)
{
	Log::Info("Test Console Comamnd! Arg: %d", arg);
}

void ConsoleCommands::Initialise()
{
	Add("cmd_test", &test);

	CreateThread(0, 0, &CommandLoop, 0, 0, 0);
}

