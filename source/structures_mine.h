// let below pragma directive at top of this source
#pragma pack(1)
// TYPE_HERE: here you can type your structure definitions like it has been done
// in the "structures.h" file for trng dll

typedef struct
{
	bool active;
	short index;
	char text[240];
}TextTicket;


typedef struct
{
	WORD drawtype;
	WORD tpage;
	WORD flag;
	float u1;
	float v1;
	float u2;
	float v2;
	float u3;
	float v3;
	float u4;
	float v4;
}TEXTURESTRUCT;


typedef struct
{
	WORD tpage;
	WORD offset;
	WORD width;
	WORD height;
	float x1;	//left
	float y1;	//top
	float x2;	//right
	float y2;	//bottom
}SPRITESTRUCT;


typedef struct
{
	union {
	float sx;
	float dvSX;
	};

	union {
	float sy;
	float dvSY;
	};

	union {
	float sz;
	float dvSZ;
	};

	union {
	float rhw;
	float dvRHW;
	};

	union {
	DWORD color;
	DWORD dcColor;
	};

	union {
	DWORD specular;
	DWORD dcSpecular;
	};

	union {
	float tu;
	float dvTU;
	};

	union {
	float tv;
	float dvTV;
	};

}DXVERTEX;

// --------------- PRESET STRUCTURE ZONE -----------------------------
// Please, don't remove structures and fields you find in this "PRESET ZONE". They will be used by some
// preset functions of your plugin sources
// However, you can add new fields and structures in following structures, of course
typedef struct StrSavegameGlobalData {
	// FOR_YOU:
	// define here your variables that you wish were saved (and then restored) to/from savegame in GLOBAL section 
	//           (only one for all levels)
	// note: the size of this structure should be always even (if you add BYTE variable, remember to compensate that 
	//       with another BYTE vairable or placefolder)
	int TextColor;
	float TextHeight, TextSpacing;
	bool toggle;
	TextTicket SaveTicket;
}SavegameGlobalDataFields;

typedef struct StrSavegameLocalData {
	// FOR_YOU:
	// define here your variables that you wish were saved (and then restored) to/from savegame in LOCAL section (one for each different level)
	// note: the size of this structure should be always even (if you add BYTE variable, compensate it with another BYTE vairable or placefolder)
}SavegameLocalDataFields;


typedef struct StrSavegameData {
	StrSavegameGlobalData Global;
	StrSavegameLocalData  Local;
}SavegameDataFields;


typedef struct StrBaseAssignSlotMine {
	int TotAssign;
	StrRecordAssSlot VetAssignSlot[MAX_ASSIGN_SLOT_MINE];
}BaseAssignSlotMineFields;


typedef struct StrMyData {
	StrSavegameData Save;  // variable that it will be saved and restored to/from savegame
	// FOR_YOU:
	// define here all your global variables, i.e. those variables that will be seen from all procedures and keep 
	// their value between all cycles of the game.
	// after you defined a variabile like:
	// int  Alfa;
	// then you'll be able to access to them using syntax like:
	// MyData.Alfa = 3;
	// if (MyData.Alfa == 5) .
	int TotProgrActions;
	int LastProgrActionIndex; // used in case of overloading
	bool debounce;
	StrProgressiveAction VetProgrActions[MAX_MYPROGR_ACTIONS];
	StrBaseGenericCustomize BaseCustomizeMine;  // stored all your customize script commands for current level
	StrBaseGenericParameters BaseParametersMine; // store of all your parameters= script commands of current level
	StrBaseAssignSlotMine BaseAssignSlotMine; // stored all AssignSlot= command script commands of current level
}MyDataFields;
// ----------------- END PRESET STRUCTURE ZONE ---------------------------------------


// let below pragma directive at bottom of this source
#pragma pack(2)