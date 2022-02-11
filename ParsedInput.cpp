#include "include/ParsedInput.h"

ParsedInput::ParsedInput(std::string executable, std::string arguments, std::string outputFile, bool overwriteOutput, bool runInBackground)
	: executable(executable)
	, arguments(arguments)
	, outputFile(outputFile)
	, overwriteOutput(overwriteOutput)
	, runInBackground(runInBackground)
{
}

std::string ParsedInput::getExecutable() const
{
	return executable;
}

std::string ParsedInput::getArguments() const
{
	return arguments;
}

std::string ParsedInput::getOutputFile() const
{
	return outputFile;
}

bool ParsedInput::shouldOverwriteOutputFile() const
{
	return overwriteOutput;
}

bool ParsedInput::shouldRunInBackground() const
{
	return runInBackground;
}
