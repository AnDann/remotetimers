/*
 * Copyright (C) 2008-2011 Frank Schmirler <vdr@schmirler.de>
 *
 * This file is part of VDR Plugin remotetimers.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * Or, point your browser to http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 */

#include <vdr/config.h>
#include "setup.h"
#include "menuitems.h"
#include "watcher.h"
#include "i18n.h"

cRemoteTimersSetup RemoteTimersSetup;

const char *OkKeyMode[2];
const char *RedKeyMode[2];
const char *BlueKeyMode[2];
const char *StartMenuMode[2];

char* copyFilename(char *dest, const char *src, size_t n) {
	while (*src == FOLDERDELIMCHAR)
		src++;
	strn0cpy(dest, src, n);
	for (int i = strlen(dest) - 1; i >= 0 && dest[i] == FOLDERDELIMCHAR; i--)
		dest[i] = '\0';
	return dest;
}

cRemoteTimersSetup::cRemoteTimersSetup() {
	hideMainMenuEntry = 0;
	replaceSchedule = 0;
	replaceTimers = 0;
	replaceRecordings = 0;
	serverIp[0] = 0;
	serverPort = 0;
	useChannelId = 1;
        swapOkBlue = 0;
	showProgressBar = 0;

// Implat from epgsearch
        ReplaseOrgSchedule = 0;
        redkeymode	    = 0;
        bluekeymode       = 1;
        showProgress      = 1;
        showChannelNr     = 0;
        timeShiftValue    = 30;
        toggleGreenYellow = 1;
        StartMenu         = 0;
        showChannelGroups = 0;
        showDaySeparators = 0;
        showEmptyChannels = 0;
        useOkForSwitch = 0;
        maxChannelMenuNow = 0;
        showRadioChannels = 1;
        onePressTimerCreation = 1;
        OkKeyMode[0] = trVDR("Button$Info");
        OkKeyMode[1] = trVDR("Button$Switch");

        RedKeyMode[0] = tr("Standard");
        RedKeyMode[1] = tr("Button$Commands");

        BlueKeyMode[0] = tr("Standard");
        BlueKeyMode[1] = tr("Button$Search");

        StartMenuMode[0] = trVDR("Button$Schedule");
        StartMenuMode[1] = trVDR("Button$Now");
//
	for (int i = 0; i < EPGTIME_LENGTH; ++i)
		epgTime[i] = 0;
	userFilterSchedule = 0;
	userFilterTimers = 0;
	userFilterRecordings = 0;
	skinTimers = 0;
	skinRecordings = 0;
	defaultUser = 0;
	addToRemote = 1;
	remotePause = 0;
	remoteInstant = 0;
	moveBandwidth = 0;
	serverDir[0] = 0;
	watchUpdate = 1;
}

cRemoteTimersSetup& cRemoteTimersSetup::operator=(const cRemoteTimersSetup &Setup) {
	hideMainMenuEntry = Setup.hideMainMenuEntry;
	replaceSchedule = Setup.replaceSchedule;
	replaceTimers = Setup.replaceTimers;
	replaceRecordings = Setup.replaceRecordings;
	strn0cpy(serverIp, Setup.serverIp, sizeof(serverIp));
	serverPort = Setup.serverPort;
	useChannelId = Setup.useChannelId;
	for (int i = 0; i < EPGTIME_LENGTH; ++i)
		epgTime[i] = Setup.epgTime[i];
	userFilterSchedule = Setup.userFilterSchedule;
	userFilterTimers = Setup.userFilterTimers;
	userFilterRecordings = Setup.userFilterRecordings;
	skinTimers = Setup.skinTimers;
	skinRecordings = Setup.skinRecordings;
	defaultUser = Setup.defaultUser;
	addToRemote = Setup.addToRemote;
	remotePause = Setup.remotePause;
	remoteInstant = Setup.remoteInstant;
	moveBandwidth = Setup.moveBandwidth;
	copyFilename(serverDir, Setup.serverDir, sizeof(serverDir));
	watchUpdate = Setup.watchUpdate;
	// epgsearch implant
        ReplaseOrgSchedule = Setup.ReplaseOrgSchedule;
	redkeymode = Setup.redkeymode;
        bluekeymode = Setup.bluekeymode;
        showProgress = Setup.showProgress;
        showChannelNr = Setup.showChannelNr;
        timeShiftValue = Setup.timeShiftValue;
        toggleGreenYellow = Setup.toggleGreenYellow;
        StartMenu = Setup.StartMenu;
        showChannelGroups = Setup.showChannelGroups;
        showDaySeparators = Setup.showDaySeparators;
        showEmptyChannels = Setup.showEmptyChannels;
        showRadioChannels = Setup.showRadioChannels;
        onePressTimerCreation = Setup.onePressTimerCreation;
        maxChannelMenuNow = Setup.maxChannelMenuNow;
        useOkForSwitch = Setup.useOkForSwitch;
	//
	return *this;
}

bool cRemoteTimersSetup::Parse(const char *Name, const char *Value) {
	if (!strcasecmp(Name, "HideMainMenuEntry"))
		hideMainMenuEntry = atoi(Value);
	else if (!strcasecmp(Name, "ReplaceSchedule"))
		replaceSchedule = atoi(Value);
// Implant from epgsearch
	else if (!strcasecmp(Name, "ReplaseOrgSchedule")) ReplaseOrgSchedule = atoi(Value);
        else if (!strcasecmp(Name, "StartMenu")) StartMenu = atoi(Value);
        else if (!strcasecmp(Name, "RedKeyMode")) redkeymode = atoi(Value);
        else if (!strcasecmp(Name, "BlueKeyMode")) bluekeymode = atoi(Value);
        else if (!strcasecmp(Name, "ShowProgress")) showProgress = atoi(Value);
        else if (!strcasecmp(Name, "ShowChannelNr")) showChannelNr = atoi(Value);
        else if (!strcasecmp(Name, "UseOkForSwitch")) useOkForSwitch = atoi(Value);
        else if (!strcasecmp(Name, "MaxChannelMenuNow")) maxChannelMenuNow = atoi(Value);
        else if (!strcasecmp(Name, "ShowRadioChannels")) showRadioChannels = atoi(Value);
        else if (!strcasecmp(Name, "OnePressTimerCreation")) onePressTimerCreation = atoi(Value);
        else if (!strcasecmp(Name, "ToggleGreenYellow")) toggleGreenYellow = atoi(Value);
        else if (!strcasecmp(Name, "ShowChannelGroups")) showChannelGroups = atoi(Value);
        else if (!strcasecmp(Name, "ShowDaySeparators")) showDaySeparators = atoi(Value);
        else if (!strcasecmp(Name, "ShowEmptyChannels")) showEmptyChannels = atoi(Value);
//
	else if (!strcasecmp(Name, "ReplaceTimers"))
		replaceTimers = atoi(Value);
	else if (!strcasecmp(Name, "ReplaceRecordings"))
		replaceRecordings = atoi(Value);
	else if (!strcasecmp(Name, "ServerIp"))
		strn0cpy(serverIp, Value, sizeof(serverIp));
	else if (!strcasecmp(Name, "ServerPort"))
		serverPort = atoi(Value);
	else if (!strcasecmp(Name, "UseChannelId"))
		useChannelId = atoi(Value);
	else if (!strcasecmp(Name, "SwapOkBlue"))
		swapOkBlue = atoi(Value);
	else if (!strcasecmp(Name, "ShowProgressBar"))
		showProgressBar = atoi(Value);
	else if (!strncasecmp(Name, "EpgTime", 7)) {
		int i = atoi(Name + 7);
		if (0 <= i && i < EPGTIME_LENGTH)
			epgTime[i] = atoi(Value);
	}
	else if (!strcasecmp(Name, "UserFilterSchedule"))
		userFilterSchedule = atoi(Value);
	else if (!strcasecmp(Name, "UserFilterTimers"))
		userFilterTimers = atoi(Value);
	else if (!strcasecmp(Name, "UserFilterRecordings"))
		userFilterRecordings = atoi(Value);
	else if (!strcasecmp(Name, "SkinSchedule"))
		skinSchedule = atoi(Value);
	else if (!strcasecmp(Name, "SkinTimers"))
		skinTimers = atoi(Value);
	else if (!strcasecmp(Name, "SkinRecordings"))
		skinRecordings = atoi(Value);
	else if (!strcasecmp(Name, "DefaultUser"))
		defaultUser = PluginRemoteTimers::cMenuEditUserItem::Parse(Value);
	else if (!strcasecmp(Name, "AddToRemote"))
		addToRemote = atoi(Value);
	else if (!strcasecmp(Name, "RemotePause"))
		remotePause = atoi(Value);
	else if (!strcasecmp(Name, "RemoteInstant"))
		remoteInstant = atoi(Value);
	else if (!strcasecmp(Name, "MoveBandwidth"))
		moveBandwidth = atoi(Value);
	else if (!strcasecmp(Name, "serverDir"))
		copyFilename(serverDir, Value, sizeof(serverDir));
	else if (!strcasecmp(Name, "WatchUpdate"))
		watchUpdate = atoi(Value);
	else
		return false;
	return true;
}

void cRemoteTimersMenuSetup::Store() {
	SetupStore("HideMainMenuEntry", setupTmp.hideMainMenuEntry);
	SetupStore("ReplaceSchedule", setupTmp.replaceSchedule);
	SetupStore("ReplaceTimers", setupTmp.replaceTimers);
	SetupStore("ReplaceRecordings", setupTmp.replaceRecordings);
	SetupStore("ServerIp", setupTmp.serverIp);
	SetupStore("ServerPort", setupTmp.serverPort);
	SetupStore("UseChannelId", setupTmp.useChannelId);
        SetupStore("SwapOkBlue", setupTmp.swapOkBlue);
	SetupStore("ShowProgressBar", setupTmp.showProgressBar);


	//	epgsearch    
        SetupStore("ReplaceOrgSchedule",  setupTmp.ReplaceOrgSchedule);
	SetupStore("StartMenu",  setupTmp.StartMenu);
        SetupStore("RedKeyMode",  setupTmp.redkeymode);
        SetupStore("BlueKeyMode",  setupTmp.bluekeymode);
        SetupStore("ShowProgress",  setupTmp.showProgress);
        SetupStore("ShowChannelNr",  setupTmp.showChannelNr);
        SetupStore("OnePressTimerCreation",  setupTmp.onePressTimerCreation);
        SetupStore("UseOkForSwitch",  setupTmp.useOkForSwitch);
        SetupStore("MaxChannelMenuNow",  setupTmp.maxChannelMenuNow);
	SetupStore("ToggleGreenYellow",  setupTmp.toggleGreenYellow);
	SetupStore("ShowRadioChannels", setupTmp.showRadioChannels);
	SetupStore("ShowChannelGroups",  setupTmp.showChannelGroups);
        SetupStore("ShowDaySeparators",  setupTmp.showDaySeparators);
        SetupStore("ShowEmptyChannels",  setupTmp.showEmptyChannels);
	//
	for (int i = 0; i < EPGTIME_LENGTH; ++i)
		SetupStore(cString::sprintf("EpgTime%d", i), setupTmp.epgTime[i]);
	SetupStore("UserFilterSchedule", setupTmp.userFilterSchedule);
	SetupStore("UserFilterTimers", setupTmp.userFilterTimers);
	SetupStore("UserFilterRecordings", setupTmp.userFilterRecordings);
	SetupStore("SkinSchedule", setupTmp.skinSchedule);
	SetupStore("SkinTimers", setupTmp.skinTimers);
	SetupStore("SkinRecordings", setupTmp.skinRecordings);
	SetupStore("DefaultUser", PluginRemoteTimers::cMenuEditUserItem::ToString(setupTmp.defaultUser));
	SetupStore("AddToRemote", setupTmp.addToRemote);
	SetupStore("RemotePause", setupTmp.remotePause);
	SetupStore("RemoteInstant", setupTmp.remoteInstant);
	SetupStore("MoveBandwidth", setupTmp.moveBandwidth);
	SetupStore("ServerDir", setupTmp.serverDir);
	SetupStore("WatchUpdate", setupTmp.watchUpdate);
	bool serverDirUpdated = RemoteTimersSetup.watchUpdate != setupTmp.watchUpdate ||
			strcmp(RemoteTimersSetup.serverDir, setupTmp.serverDir) != 0;
	RemoteTimersSetup = setupTmp;
	if (serverDirUpdated)
		cUpdateWatcher::GetInstance()->Reconfigure();
		
}

cRemoteTimersMenuSetup::cRemoteTimersMenuSetup() {
	setupTmp = RemoteTimersSetup;
	lastServerDir = strdup(setupTmp.serverDir);
	swapOkBlueFalse = strdup(cString::sprintf("%s/%s", tr("Button$Info"), tr("Button$Switch")));
	swapOkBlueTrue = strdup(cString::sprintf("%s/%s", tr("Button$Switch"), tr("Button$Info")));
	Set();
}

cRemoteTimersMenuSetup::~cRemoteTimersMenuSetup() {
	free((void *) lastServerDir);
	free((void *) swapOkBlueFalse);
	free((void *) swapOkBlueTrue);
}

void cRemoteTimersMenuSetup::Set() {
	int current = Current();
	Clear();

	Add(new cMenuEditStrItem(trREMOTETIMERS("Server IP"), setupTmp.serverIp, sizeof(setupTmp.serverIp), ".1234567890"));
	Add(new cMenuEditIntItem(trREMOTETIMERS("Server port"), &setupTmp.serverPort, 0, 65535, trREMOTETIMERS("from svdrpservice")));
	Add(new cMenuEditBoolItem(trREMOTETIMERS("Map channels using"), &setupTmp.useChannelId, trREMOTETIMERS("channel number"), trREMOTETIMERS("channel ID")));
	Add(new cMenuEditBoolItem(trREMOTETIMERS("Hide mainmenu entry"), &setupTmp.hideMainMenuEntry));
	
	Add(new cOsdItem(trREMOTETIMERS("Defaults for new timers"), osUnknown, false));
	Add(new cMenuEditBoolItem(trREMOTETIMERS("Location"), &setupTmp.addToRemote, trREMOTETIMERS("local"), trREMOTETIMERS("remote")));
#ifdef REMOTEINSTANTVERSION
	Add(new cMenuEditBoolItem(trREMOTETIMERS("Instant recordings"), &setupTmp.remoteInstant, trREMOTETIMERS("local"), trREMOTETIMERS("remote")));
	Add(new cMenuEditBoolItem(trREMOTETIMERS("Pause recordings"), &setupTmp.remotePause, trREMOTETIMERS("local"), trREMOTETIMERS("remote")));
#endif
	Add(new PluginRemoteTimers::cMenuEditUserItem(trREMOTETIMERS("User ID"), &setupTmp.defaultUser, tr("Setup.Replay$Resume ID")));
	
	Add(new cOsdItem(cString::sprintf(trREMOTETIMERS("Settings for menu \"%s\""), tr("Schedule")), osUnknown, false));
#ifdef MAINMENUHOOKSVERSNUM
	Add(new cMenuEditBoolItem(trREMOTETIMERS("Replace mainmenu"), &setupTmp.replaceSchedule));
#endif
	Add(new cMenuEditBoolItem(trREMOTETIMERS("List style"), &setupTmp.skinSchedule, tr("Plugin"), tr("Setup.OSD$Skin")));
	Add(new cMenuEditBoolItem(trREMOTETIMERS("Show progress bar"), &setupTmp.showProgressBar));
	for (int i = 0; i < EPGTIME_LENGTH; ++i)
		Add(new cMenuEditTimeItem(cString::sprintf("%s %d", *cString::sprintf(trREMOTETIMERS("What's on at %s?"), "..."), i + 1), &setupTmp.epgTime[i]));
	Add(new cMenuEditBoolItem(cString::sprintf(trREMOTETIMERS("Key binding of %s/%s"), tr("Key$Ok"), tr("Key$Blue")), &setupTmp.swapOkBlue, swapOkBlueFalse, swapOkBlueTrue));
        Add(new cMenuEditIntItem(trREMOTETIMERS("User ID filter"), &setupTmp.userFilterSchedule, -1, MAX_USER, tr("Setup.Replay$Resume ID")));

 //	Start IMPLANT from epgsearch
 	Add(new cOsdItem(cString::sprintf(trREMOTETIMERS("Settings for epgsearch \"%s\""), tr("Schedule")), osUnknown, false));
        Add(new cMenuEditBoolItem(tr("Replace original schedule"),    &setupTmp.ReplaceOrgSchedule, trVDR("no"),      trVDR("yes")));
        Add(new cMenuEditStraItem(tr("Ok key"), &setupTmp.useOkForSwitch, 2, OkKeyMode));
        Add(new cMenuEditStraItem(tr("Red key"), &setupTmp.redkeymode, 2, RedKeyMode));
        Add(new cMenuEditStraItem(tr("Blue key"), &setupTmp.bluekeymode, 2, BlueKeyMode));
        Add(new cMenuEditBoolItem(tr("Show progress in 'Now'"), &setupTmp.showProgress, trVDR("no"), trVDR("yes")));
        Add(new cMenuEditBoolItem(tr("Show channel numbers"), &setupTmp.showChannelNr,       trVDR("no"),      trVDR("yes")));
        Add(new cMenuEditBoolItem(tr("Show channel separators"), &setupTmp.showChannelGroups,       trVDR("no"),      trVDR("yes")));
        Add(new cMenuEditBoolItem(tr("Show day separators"), &setupTmp.showDaySeparators,       trVDR("no"),      trVDR("yes")));
        Add(new cMenuEditBoolItem(tr("Show radio channels"), &setupTmp.showRadioChannels,       trVDR("no"),      trVDR("yes")));
        Add(new cMenuEditIntItem(tr("Limit channels from 1 to"), &setupTmp.maxChannelMenuNow, 0, 9999));
        Add(new cMenuEditBoolItem(tr("'One press' timer creation"), &setupTmp.onePressTimerCreation,       trVDR("no"),      trVDR("yes")));
        Add(new cMenuEditBoolItem(tr("Show channels without EPG"), &setupTmp.showEmptyChannels,       trVDR("no"),      trVDR("yes")));
        Add(new cMenuEditIntItem(tr("Time interval for FRew/FFwd [min]"), &setupTmp.timeShiftValue, 1, 9999));
        Add(new cMenuEditBoolItem(tr("Toggle Green/Yellow"), &setupTmp.toggleGreenYellow,       trVDR("no"),      trVDR("yes")));
//	IMPLANT from epgsearch

        
	Add(new cOsdItem(cString::sprintf(trREMOTETIMERS("Settings for menu \"%s\""), tr("Timers")), osUnknown, false));
#ifdef MAINMENUHOOKSVERSNUM
	Add(new cMenuEditBoolItem(trREMOTETIMERS("Replace mainmenu"), &setupTmp.replaceTimers));
#endif
	Add(new cMenuEditBoolItem(trREMOTETIMERS("List style"), &setupTmp.skinTimers, tr("Plugin"), tr("Setup.OSD$Skin")));
	Add(new cMenuEditIntItem(trREMOTETIMERS("User ID filter"), &setupTmp.userFilterTimers, -1, MAX_USER, tr("Setup.Replay$Resume ID")));

	Add(new cOsdItem(cString::sprintf(trREMOTETIMERS("Settings for menu \"%s\""), tr("Recordings")), osUnknown, false));
#ifdef MAINMENUHOOKSVERSNUM
	Add(new cMenuEditBoolItem(trREMOTETIMERS("Replace mainmenu"), &setupTmp.replaceRecordings));
#endif
	Add(new cMenuEditBoolItem(trREMOTETIMERS("List style"), &setupTmp.skinRecordings, tr("Plugin"), tr("Setup.OSD$Skin")));
	Add(new cMenuEditIntItem(trREMOTETIMERS("User ID filter"), &setupTmp.userFilterRecordings, -1, MAX_USER, tr("Setup.Replay$Resume ID")));
	Add(new cMenuEditIntItem(trREMOTETIMERS("Move recording bandwidth limit (Mbit/s)"), &setupTmp.moveBandwidth, 0, INT_MAX, trREMOTETIMERS("unlimited")));

	Add(new cOsdItem(trREMOTETIMERS("Remote recordings"), osUnknown, false));
	Add(new cMenuEditStrItem(trREMOTETIMERS("Mounted on subdirectory"), setupTmp.serverDir, sizeof(setupTmp.serverDir), tr(FileNameChars)));
	Add(new cMenuEditBoolItem(trREMOTETIMERS("Monitor update file"), &setupTmp.watchUpdate));
	SetCurrent(Get(current));
	Display();
}

eOSState cRemoteTimersMenuSetup::ProcessKey(eKeys Key) {
	eOSState state = cMenuSetupPage::ProcessKey(Key);
	if (Key == kOk && strcmp(lastServerDir, setupTmp.serverDir) != 0)
	{
		free((void *) lastServerDir);
		lastServerDir = strdup(setupTmp.serverDir);
	}
	return state;
}
