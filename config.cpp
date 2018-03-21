#include <sstream>
#include <iostream>
#include <windows.h>
#include <regex>
#include <fstream>
#include <algorithm>
#include <string>

//Initialize Possible Properties For The Presence
std::string clientID;
std::string state;
std::string details;
std::string smallImage;
std::string largeImage;
std::string largeImageText;
std::string smallImageText;
std::string animatedstate;
std::string animateddetails;
bool animate;
bool animatestate;
bool animatedetails;
bool hidden;
int showingtime;

// Set value of color for cmd
int Black = 0;
int Blue = 1;
int Green = 2;
int Cyan = 3;
int Red = 4;
int Magenta = 5;
int Brown = 6;
int White = 7;
int Gray = 8;
int IBlue = 9;
int IGreen = 10;
int ICyan = 11;
int IRed = 12;
int IMagenta = 13;
int Yellow = 14;
int IWhite = 15;


// Convert string to bool
int to_int(std::string str){
   int a = atoi(str.c_str());
   return a;
}
// Convert string to bool
bool to_bool(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	std::string var(str);
	if (var == "true" || var == "1")
		return true;
	return false;
}

//Regex Patern To Check Certian Variables
bool checkValidValue(std::string value)
{
    std::regex r("^[a-zA-Z-1-9_.-]*");
    std::smatch m;
    if (std::regex_search(value, m, r))
        return true;
    return false;
}

void setcolor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	return;
}

//Check and Set The Global Presence Variables
bool setPresenceVariables(std::string configPath)
{
    //Open Config File
    std::ifstream config(configPath);

    //Parse Config File
    std::string line;
    while(std::getline(config, line))
    {
        std::istringstream is_line(line);
        std::string key;
        if (std::getline(is_line, key, '='))
        {
            std::string value;
            if (std::getline(is_line, value))
            {
                if (key == "ClientID")
                    clientID = value;
                else if (key == "State")
                    state = value;
                else if (key == "Details")
                    details = value;
                else if (key == "LargeImage")
                    largeImage = value;
                else if (key == "SmallImage")
                    smallImage = value;
                else if (key == "LargeImageTooltip")
                    largeImageText = value;
                else if (key == "SmallImageTooltip")
                    smallImageText = value;
				else if (key == "AnimatedState")
					animatestate = to_bool(value);
				else if (key == "AnimatedDetails")
					animatedetails = to_bool(value);
				else if (key == "Hidden")
					hidden = to_bool(value);
				else if (key == "Showtime")
					showingtime = to_int(value);
				animate = (animatestate || animatedetails);

            }
        }
    }

    //Check Certian Variables Against The Same(?) Regex as discord
    std::string checkableValues[2] = {largeImage, smallImage};
    for(std::string value : checkableValues)
    {
        if (!checkValidValue(value))
        {
            std::cout << value + " is NOT valid for a image key" << std::endl;
            return false;
        }
    }
    return true;
}

//Print Values For The Current Settings From The Config File
void printVariables()
{
	system("cls");
	setcolor(Yellow);
    std::cout << "\nCURRENT SETTINGS" << std::endl;
	setcolor(IBlue);
    std::cout << "-------------------------" << std::endl;
	setcolor(IGreen);
	std::cout << "STATE IS: ";
	setcolor(ICyan);
 	std::cout << state;
	setcolor(IGreen);
	std::cout << " ANIMATED: ";
	if (animatestate) { setcolor(IBlue); }
	else { setcolor(IRed); }
	std::cout << std::boolalpha << animatestate << std::endl;
	setcolor(IGreen);
	std::cout << "DETAILS ARE: " ; 
	setcolor(ICyan);
	std::cout << details;
	setcolor(IGreen);
	std::cout << " ANIMATED: ";
	if (animatedetails) { setcolor(IBlue); }
	else { setcolor(IRed); }
	std::cout << std::boolalpha << animatedetails << std::endl;
	setcolor(IGreen);
	std::cout << "LARGE IMAGE IS: ";
	setcolor(ICyan);
	std::cout << largeImage << std::endl;
	setcolor(IGreen);
	std::cout << "SMALL IMAGE IS: "; 
	setcolor(ICyan);
	std::cout << smallImage << std::endl;
	setcolor(IGreen);
	std::cout << "SMALL IMAGE TEXT IS: "; 
	setcolor(ICyan);
	std::cout << smallImageText << std::endl;
	setcolor(IGreen);
	std::cout << "LARGE IMAGE TEXT IS: ";
	setcolor(ICyan);
	std::cout << largeImageText << std::endl;
	setcolor(IGreen);
	std::cout << "IS ANIMATED: ";
	if(animate){ setcolor(IBlue); } else { setcolor(IRed); }
	std::cout << std::boolalpha << animate << std::endl;
	setcolor(IGreen);
	std::cout << "IS HIDDEN: ";
	if (hidden) { setcolor(IBlue); } else { setcolor(IRed); }
	std::cout << std::boolalpha << hidden << std::endl;
	setcolor(IGreen);
	std::cout << "TIME CONFIG: ";
	if (showingtime == 1) { setcolor(IBlue); } else if (showingtime == 2) { setcolor(IMagenta); } else { setcolor(IRed); }
	std::cout <<  showingtime << std::endl;

}

void printanime(){
	printVariables();
	std::cout << std::endl << std::endl;
	setcolor(IGreen);
	std::cout << "Animated State: ";
	setcolor(IMagenta);
	std::cout << animatedstate << std::endl;
	setcolor(IGreen);
	std::cout << "Animated Details: ";
	setcolor(IMagenta);
	std::cout << animateddetails << std::endl;
}

//Check If Config File Updated
bool configFileChanged(std::string configPath)
{
    std::string vars[7] = {clientID, state, details, largeImage, smallImage, smallImageText, largeImageText};
	bool bvars[3] = {animatestate,animatedetails,hidden};
	int ivars[1] = { showingtime };
    if (setPresenceVariables(configPath))
    {
        std::string possibleUpdatedVars[7] = {clientID, state, details, largeImage, smallImage, smallImageText, largeImageText};
		bool possibleUpdatedbVars[3] = { animatestate,animatedetails,hidden };
		int possibleUpdatediVars[1] = { showingtime };
        for(int i = 0; i < 7; i++)
        {
            if (vars[i] != possibleUpdatedVars[i])
                return true;
        }
		for (int i = 0; i < 3; i++)
		{
			if (bvars[i] != possibleUpdatedbVars[i])
				return true;
		}
		for (int i = 0; i < 1; i++)
		{
			if (ivars[i] != possibleUpdatediVars[i])
				return true;
		}
    }
    return false;
}








