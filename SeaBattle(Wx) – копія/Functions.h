#pragma once
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct Ammunition
{
	int big_ammo_count;
	int torpedo_count;
};

vector<string> Split(const std::string& input, char delimiter)
{
	istringstream stream(input);
	vector<std::string> tokens;
	string token;

	while (std::getline(stream, token, delimiter))
	{
		tokens.push_back(token);
	}

	return tokens;
}

int StringToInt(const std::string& str)
{
	int result;
	istringstream iss(str);

	iss >> result;

	return result;
}

template<typename T>
string toString(const T thing)
{
	stringstream stream;

	stream << thing;
	string result;
	stream >> result;

	return result;
}

wxPoint GetWindowCenter(wxFrame* ParentFrame, wxFrame* ChildFrame)
{
	if (ParentFrame->GetSize() == ChildFrame->GetSize())
		return ParentFrame->GetPosition();

	auto point = ParentFrame->GetPosition();
	auto size = ParentFrame->GetSize();

	return wxPoint(ParentFrame->GetPosition().x + ParentFrame->GetSize().x / 2.0 - ChildFrame->GetSize().x / 2.0, ParentFrame->GetPosition().y + ParentFrame->GetSize().y / 2.0 - ChildFrame->GetSize().y / 2.0);
}