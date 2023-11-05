// FOR_YOU:
// This is the main source of your plugin.
// It's in this source you'll type all your assembly or c++ code and all 
// the variables and memory zones you need.

// ************  Top/Header section ************
#include "stdafx.h"
#include <stdio.h>
#include "bass.h"		// prototypes for extra sound library: bass.dll
	// Following header files will be updated for every new version of 
	// the tomb_NextGeneration.dll, so it's better you don't change them
	//  because they will be replaced for any new update.

#include "Tomb_NextGeneration.h" // mnemonic constants defined in tomb_NextGeneration.dll
#include "structures.h" // structure of tomb4 program and trng dll
#include "DefTomb4Funct.h" // defines of tomb4 procedure prototypes
#include "functions.h"  // assigments of real tomb4 address to each tomb4 procedures
#include "macros.h"  // definition of macros

	// FOR_YOU:
	// While the followings are the headers you can use 
	// to type your structures, constants and new tomb4 procedures you 
	// discovered. 
	// Following files are only yours and trng will not ever change them:
#include "macros_mine.h"  // here you define your own macros
#include "constants_mine.h" // here you define your mnemonic constants
#include "structures_mine.h" // here you type your structure definitions
#include "Tomb4Discoveries_mine.h" // here type tomb4 procedures you discovered

#include "trng.h" // list of trng functions imported from trng.cpp source.

#pragma warning( error : 4706 )
#pragma warning(disable: 4996)

// ************  Early function declarations ************
void SetTextParameters(int);

// ************  Global Variables Section *************

// FOR_YOU:
// here you type the variables or memory zones you wish use in different
// C++ procedures.
// If you use the assembly you'll have to type ALL your variables in this
// section.
// for example if you wish have a 32 bits signed variable to store big
// numbers you can type:
// TYPE_HERE:
// int MyNumber;
// and then you can use it in asm in this way:
//		mov  eax, [MyNumber]
// or
//      mov eax, dword ptr [MyNumber]
// same speech for wide memory zones.
// If you wish having a memory zone of 13000 bytes you can type:
//  BYTE MyZone[13000];
// and then you can use it in this way:
//     mov  al, MyNumber[ecx]  ; read the ECXth byte and copy it in AL
// or also:
//     mov al, [MyNumber+ecx] ; same result of above instruction

// Variables and memory zone to TYPE_HERE:

HINSTANCE MyDllInstance=NULL;  // the instance handle of your dll

extern char BufferLog[4096]; // temporary global buffer to host error and warning messages

// FOR_YOU: If you mean create code patch (changes in tomb4 code to call your code in the plugin dll,
// you MUST change the MyTomb4PatcherAddress value (here below) 
// You can discover a free tomb4 zone using TrngPatcher program, with the 
// menu command "Tools->Get Assignment of Free Memory range"
// Note: choose an address, in the given range, terminating with hex 
// digits: 0,4,8, or C
// because it's usefull that the address was a multiple by 4
// If you chose an address used from other plugins you'll get an error and
// the game will be aborted
// note: if you don't mean use code patches you can let 0x0 in following line
DWORD MyTomb4PatcherAddress = 0x0;//0x6708E0; // <- TYPE_HERE: the new address you chose
								
// this text will contain your plugin name (omitting .dll extension).
// it will be used to give a title to messagebox for error messages or warnings
char TexMyPluginName[80];  
// in this MyData structure you can store all global variables for your plugin
// you have to define them in structures_mine.h source inside structure "StrMyData" or for variable
// to save and restore in savegames, in "StrSavegameGlobalData" or "StrSavegameLocalData" structures
TextTicket ticket;
StrMyData MyData;
int MessageDisplay;
char Subs[16];
// ************  Utilities section  ****************


// ************  Patcher Functions section  ***************
// Note: please, don't change or remove the C++ procedure you find here,
// because they are necessary to do work the plugin with the trng dll
// Anyway in many of these functions you can add your code


// FOR_YOU: In this function you insert the callings of functions used to change tomb4 code
// You can get these functions, in the correct format, using Trng Core -> Asm Souce Editor -> Debugging menu
// program
// note: if there is a conflict the function will return "false"
// and the tomb4 program will abort with an error message
bool CreateMyCodePatches(void)
{
	// the call of the code patch to TYPE_HERE:
	// example: 
	// SET_PATCH(Path_RedirCollision)
	// to call the function Patch_RedirCollision() created with TrngPatcher program (command Assmembly->Create Dynamic Patch Generator)
	//if (SetReservedDataZone(0x6708E0, 128) != APPC_OK) return false;

	return true;
}

// ************  Assembly Procedures section  ******************

// FOR_YOU: In the SubPatchArray you'll type all procedure names of your code in the dll you
// wish were called from tomb4 code.
// type them in the order of ax value. So first asm proc in the list, will be called
// with ax=0, while the second in the list will be called with ax=1 ect.

void *SubPatchArray[] = {
	NULL
};


// ************  MainPatcher() asm procedure  *****************

// FOR_YOU: This is your main patcher procedure.
// All your patch in tomb4 code will call this procedure passing to it, in ax register,
// the number of subpatch to call

BEGIN_ASM_PROC(MainPatcher)
	and eax, 0ffffh
	mov eax, dword ptr [SubPatchArray+eax*4];
	jmp eax
END_ASM_PROC

inline int round(float x)
{
	return (x > 0) ? int(x+0.5f) : int(x-0.5f);
}

// ************  CallBack functions section  *****************

// TYPE_HERE: your callback function

void cbInitProgram(int NumberLoadedPlugins, char *VetPluginNames[]) 
{

	// save number of plugins (enclosed yours and the tomb_nextgeneration.dll ) and their names
	// these data will be used to locate IDs of any loaded plugins using FindPluginID() function
	Trng.TotPlugins = NumberLoadedPlugins;
	Trng.pVetPluginNames = VetPluginNames;

	// clear all my global variables
	ClearMemory(&MyData, sizeof(StrMyData));
	MyData.Save.Global.toggle = true;
}

/*void cbInitGame(void)
{
	// here you can initialize your global data for whole adventure
	// this procedure will be called only once, before loading title level


}
*/


void cbInitLevel(void)
{
	MessageDisplay = 0;
	memcpy(&ticket, &MyData.Save.Global.SaveTicket, sizeof(TextTicket));
	ClearMemory(&MyData.Save.Global.SaveTicket, sizeof(TextTicket));
}


// called everytime player save the game (but also when lara move from a level to another HUB saving). 
// in this procedure your plugin will save its own data in savegame
// SavingType inform about the kind of saving it is happening (SAVT_... values)
// pAdrZone is a byte pointer where this procedure will save the address from where begin data to save in savegame
// this function will return the size of data to save
// note: It's better you don't change anything of this code. It will save byself of data you stored in MyData.Save
// structure. If you wish adding other variables to save, just you add new fields in Structures of MyData.Save 
DWORD cbSaveMyData(BYTE **pAdrZone, int SavingType)
{
	DWORD SizeData;
	int i;
	static WORD *pVetExtras;
	int TotNWords;
	int TotNewActions;

	memcpy(&MyData.Save.Global.SaveTicket, &ticket, sizeof(TextTicket));


	if (SavingType & SAVT_COMPLETED) {
		// this call is not to save data but only it is a confirm that the previous saving has been completed
		// now we can free the temporary memory used to save the data in previous call
		if (pVetExtras != NULL) {
			FreeMemory(pVetExtras);
			pVetExtras=NULL;
		}

		return 0;
	}


	TotNWords=0;
	pVetExtras = (WORD *) GetMemory(16);
	// save id of my plugin in first word

	pVetExtras[TotNWords++] = Trng.IdMyPlugin;

	if (SavingType & SAVT_LOCAL_DATA) {
		// save local data

		// save Local structure
		AddNGToken(NGTAG_LOCAL_DATA, NO_ARRAY, sizeof(StrSavegameLocalData), &MyData.Save.Local, 
						&pVetExtras, &TotNWords);


		// save all (currently enabled) progressive actions
		// before saving, compact progressive action array to remove intermediate free records
		TotNewActions=0;

		for (i=0;i<MyData.TotProgrActions;i++) {
			if (MyData.VetProgrActions[i].ActionType != AXN_FREE) {

				MyData.VetProgrActions[TotNewActions] = MyData.VetProgrActions[i];

				TotNewActions++;
			}
		}
		// update new valuese after recompatting
		MyData.LastProgrActionIndex =0;
		MyData.TotProgrActions= TotNewActions;

		// store all progressive action records
		AddNGToken(NGTAG_PROGRESSIVE_ACTIONS, MyData.TotProgrActions, sizeof(StrProgressiveAction), 
				&MyData.VetProgrActions[0], &pVetExtras, &TotNWords);

	}

	if (SavingType & SAVT_GLOBAL_DATA) {
		// save global data
		AddNGToken(NGTAG_GLOBAL_DATA, NO_ARRAY, sizeof(StrSavegameGlobalData), &MyData.Save.Global , 
						&pVetExtras, &TotNWords);
	}
	// write final sequence
	AddTokenFinalSequence(&pVetExtras, &TotNWords);

	// return to trng the infos about start of memory where there are our data and their size:
	*pAdrZone = (BYTE *) pVetExtras;
	SizeData = TotNWords * 2;

	return SizeData;
}

// called when a savegame will be loaded (but also when lara move from a level to another)
// pAdrZone will point to memory zone with data just loaded from savegame
// SizeData is the size of data pointed by pAdrZone
// note: it's better you don't change anything of this code. It's alread thought to reload all data you saved in MyData.Save 
// structure. There is no need of changes by you
void cbLoadMyData(BYTE *pAdrZone, DWORD SizeData)
{

	WORD *pVetExtras;
	StrParseNGField  ParseField;
	int Indice;
	int i;
	WORD TotActions;

	
	pVetExtras = (WORD*) pAdrZone;

	Indice=0;

	while (ParseNgField(pVetExtras ,Indice, &ParseField)==true) {
		
		// recover different ng token
		switch (ParseField.Type) {
		case NGTAG_LOCAL_DATA:
			// local data
			memcpy(&MyData.Save.Local, ParseField.pData, sizeof(StrSavegameLocalData));
			break;

		case NGTAG_GLOBAL_DATA:
			// global data
			memcpy(&MyData.Save.Global, ParseField.pData, sizeof(StrSavegameGlobalData));
			break;
			
		case NGTAG_PROGRESSIVE_ACTIONS:
			// progressive actions
			i= ParseField.StartDataIndex;
			// read tot actions value
			TotActions = pVetExtras[i++];
			// copy all tot records
			memcpy(&MyData.VetProgrActions[0], &pVetExtras[i], sizeof(StrProgressiveAction) * TotActions);
			MyData.TotProgrActions = TotActions;
			break;
		}
		Indice= ParseField.NextIndex; 
	}
}

// free memory used to store all data about your parameters commands loaded in previous level
void FreeMemoryParameters(void)
{
	int i;

	for (i=0;i<MyData.BaseParametersMine.TotParameters;i++) {
		FreeMemory(MyData.BaseParametersMine.pVetParameters[i].pVetArg);
	}

	if (MyData.BaseParametersMine.TotParameters > 0) {
		FreeMemory(MyData.BaseParametersMine.pVetParameters);
		MyData.BaseParametersMine.TotParameters=0;
	}

	MyData.BaseParametersMine.pVetParameters=NULL;
}

// this procedure will be called at end of any level
// you can type here code to free resources allocated for level (that quits now)
void FreeLevelResources(void)
{

	// free memory used to store all data about your customize commands loaded in previous level
	//FreeMemoryCustomize();
	// free memory used to store all data about your parameters commands loaded in previous level
	FreeMemoryParameters();
	MyData.BaseAssignSlotMine.TotAssign=0;

}


// it will be called before beginning the loading for a new level.
// you can type here code to initialise all variables used for level (to clear old values changed by previous level)
// and to free resources allocated in old level since now we'are going to another new level.*/

void cbInitLoadNewLevel(void)
{
	int i;

	StrProgressiveAction *pAction;

	// clear all LOCAL variables
	ClearMemory(&MyData.Save.Local,sizeof(StrSavegameLocalData));

	// clear progressive actions
	pAction= &MyData.VetProgrActions[0];

	for (i=0;i<MyData.TotProgrActions;i++) {
		if (pAction->ActionType != AXN_FREE) {
			// here you could analise to free resoruce allocated from this specific action

			pAction->ActionType = AXN_FREE;
		}
	}

	MyData.TotProgrActions=0;
	MyData.LastProgrActionIndex=0;

	// here you can initialise other variables of MyData different than Local and progressive actions
	// free resources allocate in previous level

	FreeLevelResources();

	ClearMemory(&ticket, sizeof(TextTicket));
	SetTextParameters(-1);
}


void SetTextParameters(int paramID)
{
	int TextColor = 2;
	int TextBaseY = 972;

	for (int i = 0; i < MyData.BaseParametersMine.TotParameters; ++i)
	{
		StrGenericParameters* params = &MyData.BaseParametersMine.pVetParameters[i];
		if (params->ParamValue == PARAM_SUBTITLES
			&& params->pVetArg[0] == paramID)
		{
			if (params->NArguments > 1 && params->pVetArg[1] >= 0)
				TextColor = params->pVetArg[1];

			if (params->NArguments > 2 && params->pVetArg[2] >= 0)
				TextBaseY = params->pVetArg[2];
		}
	}

	MyData.Save.Global.TextColor = TextColor;
	MyData.Save.Global.TextHeight = TextBaseY/1080.0f;
}

int strcharcount(char* s, char a)
{
	int count = 0;
	char* c = &s[0];

	while (*c != '\0')
	{
		if (*c == a)
			++count;
		++c;
	}

	return count;
}


void GetStringDimensions(char *string, short *w, short *h)
{
	char c;
	int len, y;
	short lo, hi;

	len = GetStringLength(string, &hi, &lo);
	y = (lo - hi) + 2;

	while ((c = *string++) != '\0')
	{
		if (c == '\n')
		{
			int l;

			if ((*string == '\n'))
				y += 16;

			else if (*string)
			{
				l = GetStringLength(string, &hi, &lo);

				y += (lo - hi) + 2;

				if (l > len)
					len = l;
			}
		}
	}

	*w = len;
	*h = y;
}

void DrawSquare(int x1, int y1, int x2, int y2, BYTE alpha1, BYTE alpha2)
{
	clipflags[0] = 0;
	clipflags[1] = 0;
	clipflags[2] = 0;
	clipflags[3] = 0;

	DXVERTEX v[4];

	v[0].tu = v[0].tv = 0.0f;
	v[1].tu = v[1].tv = 0.0f;
	v[2].tu = v[2].tv = 0.0f;
	v[3].tu = v[3].tv = 0.0f;

	DWORD col1 = alpha1 << 24;
	DWORD col2 = alpha2 << 24;

	v[0].sx    = float(0+x1);
	v[0].sy    = float(0+y1);
	v[0].color = col1;
	v[0].specular = col1;

	v[1].sx    = float(0+x2);
	v[1].sy    = float(0+y1);
	v[1].color = col2;
	v[1].specular = col2;

	v[2].sx    = float(0+x2);
	v[2].sy    = float(0+y2);
	v[2].color = col2;
	v[2].specular = col2;

	v[3].sx    = float(0+x1);
	v[3].sy    = float(0+y2);
	v[3].color = col1;
	v[3].specular = col1;

	v[0].sz    = f_mznear-6.0f;
	v[0].rhw   = f_moneoznear-2.0f;

	v[1].sz    = v[0].sz;
	v[1].rhw   = v[0].rhw;
	
	v[2].sz    = v[0].sz;
	v[2].rhw   = v[0].rhw;
	
	v[3].sz    = v[0].sz;
	v[3].rhw   = v[0].rhw;

	TEXTURESTRUCT Tex;
	
	Tex.drawtype = 3;
	Tex.flag = 0;
	Tex.tpage = 0;

	(*AddQuadSorted)(v,0,1,2,3,&Tex,0);
}


void PrintSubtitle()
{
	if (MessageDisplay)
	{
		PrintText (9*Trng.pGlobTomb4->ScreenSizeX/10, 31*Trng.pGlobTomb4->ScreenSizeY/32, Subs, FT_SIZE_ATOMIC_CHAR, 2, FTS_ALIGN_CENTER);
		--MessageDisplay;
	}

	if (!MyData.Save.Global.toggle || !ticket.active)
		return;

	short w, h;
	GetStringDimensions(ticket.text, &w, &h);

	int newlines = strcharcount(ticket.text, '\n');
	int height = round(Trng.pGlobTomb4->ScreenSizeY * MyData.Save.Global.TextHeight) -
		(newlines * 36 * Trng.pGlobTomb4->ScreenSizeY / 1080) / 2;

	int x1, x2, y1, y2;
	int center = phd_centerx;

	float fw = w * 1.075f;
	x1 = center - round(fw/4);
	x2 = round(x1 + fw/2);

	y1 = height - (27 * Trng.pGlobTomb4->ScreenSizeY / 1080);
	y2 = y1 + (h/2);

	const int bordery = 20 * Trng.pGlobTomb4->ScreenSizeY / 1080;
	const int borderx = 150 * Trng.pGlobTomb4->ScreenSizeX / 1920;

	y1 -= bordery;
	y2 += bordery;

	DrawSquare(x1-borderx, y1, x1, y2, 0x0, 0x80);
	DrawSquare(x1, y1, x2, y2, 0x80, 0x80);
	DrawSquare(x2, y1, x2+borderx, y2, 0x80, 0x0);

	int color = (MyData.Save.Global.TextColor > 0) ? MyData.Save.Global.TextColor : FC_WHITE;
	PrintText(center, height, ticket.text, FT_SIZE_ATOMIC_CHAR, color, FTS_ALIGN_CENTER);
}

void SetSubtitle(short ngIndex)
{
	if (ngIndex < 0)
		return;

	strcpy(ticket.text, GetString(ngIndex | STRING_NG));
	ticket.index = ngIndex;
	ticket.active = true;
}

void ClearSubtitle()
{
	//memset(ticket.text, 0, sizeof(ticket.text));
	ticket.active = false;
}


// this procedure will be called everytime a flipeffect of yours will be engaged
// you have to elaborate it and then return a TRET_.. value (most common is TRET_PERFORM_ONCE_AND_GO)
int cbFlipEffectMine(WORD FlipIndex, WORD Timer, WORD Extra, WORD ActivationMode)
{
	int RetValue;
	WORD TimerFull;

	RetValue = enumTRET.PERFORM_ONCE_AND_GO;
	// if the flip has no Extra paremeter you can handle a Timer value with values upto 32767
	// in this case you'll use the following TimerFull variable, where (with following code) we set a unique big number 
	// pasting togheter the timer+extra arguments:
	TimerFull = Timer | (Extra << 8);

	switch (FlipIndex) {
		// here type the "case Number:" for each flipeffect number. At end of the code you'll use the "break;" instruction to signal the code ending
		// Note: when you'll add your first "case Number:" then you can remove the following "case -1: and break;" instructions
	case 1:
		SetSubtitle(TimerFull);
		break;
	case 2:
		ClearSubtitle();
		break;
	case 3:
		SetTextParameters(TimerFull);
		break;
	default:
		SendToLog("WARNING: Flipeffect trigger number %d has not been handled in cbFlipEffectMine() function", FlipIndex);
		break;
	}

	// if there was the one-shot button enabled, return TRET_PERFORM_NEVER_MORE
	if (ActivationMode & enumSCANF.BUTTON_ONE_SHOT) RetValue= enumTRET.PERFORM_NEVER_MORE; 
	return RetValue;
}

// this procedure vill be called for each Parameters=PARAM_... command read from script
// having one of yours PARAM_ constants
// ParameterValue will be the value of your PARAM_ constant
// NumberOfItems will be the number of following Item (signed 16 bit values) following
// the PARAM_ constant in the customize= script command
// pItemArray is the array with all NumberOfItems arguments of Parameter command
void cbParametersMine(WORD ParameterValue, int NumberOfItems, short *pItemArray)
{
	// here you can replace this default management of anonymous parameters commands
	// with your procedure where you can recognize each different Param_ value and 
	// save its arguments in meaningful names fields, or elaboriting them immediatly
	// when it is possible (warning: in this moment nothing of level it has been yet loaded, excepting the script section)

	// ----- default management (optional)----
	// all parameters values will be saved in MyData structure
	DWORD SizeMem;
	StrGenericParameters *pMyParam;
	int TotParam;

	// ask memory to have another (new) record of StrGenericparameters structure
	TotParam= MyData.BaseParametersMine.TotParameters;
	TotParam++;
	SizeMem = TotParam * sizeof(StrGenericParameters);
	MyData.BaseParametersMine.pVetParameters = 
		(StrGenericParameters *) ResizeMemory(MyData.BaseParametersMine.pVetParameters, SizeMem);

	pMyParam = & MyData.BaseParametersMine.pVetParameters[TotParam-1];
	
	// now require memory for all arguments (NumberOfItems) store in pItemArray

	pMyParam->pVetArg = (short *) GetMemory(2 * NumberOfItems);
	// copy data
	pMyParam->NArguments = NumberOfItems;
	pMyParam->ParamValue = ParameterValue;
	memcpy(pMyParam->pVetArg, pItemArray, 2*NumberOfItems);

	MyData.BaseParametersMine.TotParameters= TotParam;
	// ---- end of default managemnt for generic parameters -------------
}


void cbCycleEnd()
{
	if (DX_INPUT[66])
	{
		if (!MyData.debounce)
		{
			MyData.Save.Global.toggle = !MyData.Save.Global.toggle;
			if (MyData.Save.Global.toggle)
				strcpy(Subs, "Subtitles ON");
			else
				strcpy(Subs, "Subtitles OFF");
			MessageDisplay = 60;
		}
		MyData.debounce = true;
	}
	else
		MyData.debounce = false;
}

// FOR_YOU:
// in this function RequireMyCallBacks() you'll type
// a list of:
//		GET_CALLBACK(CB_..., ,)
// one for each callback you need
bool RequireMyCallBacks(void)
{
// ************  RequireMyCallBacks() function  *****************
	// protype of GET_CALLBACK:
	// GET_CALLBACK(CallBackCB, CBT_Flags, Index, MyProcToCall)
	// default callbacks required always 
	GET_CALLBACK(CB_INIT_PROGRAM, 0, 0, cbInitProgram)
	//GET_CALLBACK(CB_INIT_GAME, 0, 0, cbInitGame)
	GET_CALLBACK(CB_INIT_LEVEL, 0,0, cbInitLevel)
	GET_CALLBACK(CB_SAVING_GAME, 0, 0, cbSaveMyData)
	GET_CALLBACK(CB_LOADING_GAME, 0, 0, cbLoadMyData)
	GET_CALLBACK(CB_INIT_LOAD_NEW_LEVEL, 0,0, cbInitLoadNewLevel);
	GET_CALLBACK(CB_FLIPEFFECT_MINE, 0, 0, cbFlipEffectMine);
	//GET_CALLBACK(CB_ACTION_MINE, 0,0, cbActionMine);
	//GET_CALLBACK(CB_CONDITION_MINE,0,0,cbConditionMine);
	//GET_CALLBACK(CB_CUSTOMIZE_MINE, 0,0, cbCustomizeMine);
	GET_CALLBACK(CB_PARAMETER_MINE, 0, 0, cbParametersMine);
	//GET_CALLBACK(CB_ASSIGN_SLOT_MINE, 0,0, cbAssignSlotMine);
	//GET_CALLBACK(CB_CYCLE_BEGIN, 0, 0, cbCycleBegin);
	//GET_CALLBACK(CB_PROGR_ACTION_MINE, 0, 0, cbProgrActionMine);
	GET_CALLBACK(CB_CYCLE_END, 0, 0, cbCycleEnd);
	//GET_CALLBACK(CB_INIT_OBJECTS, 0, 0, cbInitObjects);
	GET_CALLBACK(CB_LARA_DRAW, CBT_AFTER, 0, PrintSubtitle);

	return true;
}
// FOR_YOU:
// This function will be the first code to be executed of your plugin
// It happens when trng will load your plugin.
// In this moment no directX graphic is yet active, so you can show
// message boxes (TryMessageBox) to advise your players or level designer 
// about something
// Note: if you wish abort all (because there is an error or something
// is missing) you have to exit from this function returning: false
bool InitializeAll(void)
{
// ************  InitializeAll() function  ****************
	//  perform all your patches
	CALL_CHECK(CreateMyCodePatches)

	// call the function that requires all callback you need
	CALL_CHECK(RequireMyCallBacks)

	// TYPE_HERE: code to allocate global resource to use in the whole game

	return true;
}

// FOR_YOU: Tyis function will be called when tomb4 game is to be closed.
// you should type in this function further codes to free the global
// resource you had allocated in the InitializeAll() function 
void ReleaseAll(void)
{
// ************  ReleaseAll() function  ******************
	FreeLevelResources();


}


BOOL APIENTRY DllMain( HINSTANCE hInstanceDll, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved)
{

    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			MyDllInstance = hInstanceDll;
			GetTrngInfo();
			// control per check control value about size and alignment with globtomb4 structure
			if (CheckControlGlobTomb4() == false) return FALSE;

			if  (InitializeAll()==false) {
				return FALSE;
			}
			return TRUE;
			

		case DLL_PROCESS_DETACH:
			ReleaseAll();
			break;
    }
    return TRUE;
}
