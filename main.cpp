#include "config.hpp"
#include "discord.hpp"
#include <iostream>
#include <windows.h>
#include <csignal>
#include <shellapi.h>
#include <tchar.h>
#include <ctime>


#define CONFIG "config.ini"
#define IDI_ICON1                       102

#define APPWM_ICONNOTIFY (WM_APP + 1)
NOTIFYICONDATA nid = {};
HWND hwnd = GetForegroundWindow(); // get handle of currently active window
DWORD dwPID;


//quit for stopping the loop
//firstUpdate to update without changing the config (first loop only)
bool quit = false;
bool firstUpdate = true;
int stateindex = 0;
int detailsindex = 0;
int firsttimestamp = 0;
int timestamp = NULL;


//Break the loop and properly shutdown discord-rp
void shutdown_signal(int sig)
{
	signal(sig, shutdown_signal);
	Shutdown();
	quit = true;
}

bool WindowVisibility(bool hidethewindow) {
	if (hidethewindow){ ::ShowWindow(::GetConsoleWindow(), SW_HIDE); } else { ::ShowWindow(::GetConsoleWindow(), SW_SHOW); }
	return (::IsWindowVisible(::GetConsoleWindow()) != FALSE);
}


std::string animation(std::string texttoanime, int indextouse)
{
	int animelong = texttoanime.length();
	int i;
	if (indextouse == 1) { i = stateindex; } else if (indextouse == 2) { i = detailsindex; }
	char a[1024];
	strcpy_s(a, texttoanime.c_str());
	if((a[i] >= 'a' && a[i] <= 'z') || (a[i] >= 'A' && a[i] <= 'Z')) {
		if (islower(a[i])) { a[i] = toupper(a[i]); } else { a[i] = tolower(a[i]); }
	}
	if (i < animelong && indextouse == 1) { stateindex = stateindex + 1; } else if (indextouse == 1) { stateindex = 0;}
	if (i < animelong && indextouse == 2) { detailsindex = detailsindex + 1; } else if (indextouse == 2) { detailsindex = 0; }
	return std::string(a);
}

//Loop to keep the program running as well as checking the config file for updates
int main(void)
{
	SetConsoleTitle("1337 Rich Presence Discord Tool");

	//Define When to Shutdown
	signal(SIGINT, shutdown_signal);
	signal(SIGTERM, shutdown_signal);
#ifdef SIGBREAK
	signal(SIGBREAK, shutdown_signal);
#endif

	//Grab initial variables from the config 
	setPresenceVariables(CONFIG);
	//Start discord-rp
	InitDiscord(clientID.c_str());

	//Loop to keep program running also to check for updated config
	while (!quit)
	{

		if (configFileChanged(CONFIG) || firstUpdate)
		{
			//Print and set variables for the presence
			printVariables();
			WindowVisibility(hidden);
			UpdatePresence(state.c_str(), details.c_str(), largeImage.c_str(), smallImage.c_str(), smallImageText.c_str(), largeImageText.c_str(), timestamp);
			animatedstate = state.c_str();
			animateddetails = details.c_str();
			stateindex = 0;
			detailsindex = 0;
			if (showingtime == 0 || showingtime > 2 || showingtime < 0) { timestamp = NULL; }
			else if (showingtime == 1) { timestamp = firsttimestamp; }
			else if (showingtime == 2) { timestamp = 1; }

			//Switch firstUpdate as it is not the first update anymore ;)
			if (firstUpdate){
				firstUpdate = false;
				firsttimestamp = time(0);
			}
		} else if (animate) {
			if (animatestate) { animatedstate = animation(animatedstate, 1); }
			if (animatedetails) { animateddetails = animation(animateddetails, 2); }
			printanime();
			UpdatePresence(animatedstate.c_str(), animateddetails.c_str(), largeImage.c_str(), smallImage.c_str(), smallImageText.c_str(), largeImageText.c_str(), timestamp);
		}
		Sleep(15000); // Discord_UpdatePresence() has a rate limit of one update per 15 seconds, that why...
	}
	return 0;
}

