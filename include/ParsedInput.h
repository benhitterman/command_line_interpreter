#ifndef PARSED_INPUT_H
#define PARSED_INPUT_H

#include <string>

class ParsedInput
{
public:
	ParsedInput()
		: ParsedInput("")
	{}
	ParsedInput(std::string executable, std::string arguments = "", std::string outputFile = "", bool overwriteOutput = false, bool runInBackground = false);

	std::string getExecutable() const;
	std::string getArguments() const;
	std::string getOutputFile() const;
	bool shouldOverwriteOutputFile() const;
	bool shouldRunInBackground() const;

private:
	std::string executable;
	std::string arguments;
	std::string outputFile;
	bool overwriteOutput;
	bool runInBackground;
};

#endif
