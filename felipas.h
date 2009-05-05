#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define FELICA_IDM_L 16
#define FELICA_PMM_L 32

typedef struct {
	unsigned char* system_code;
	unsigned char time_slot;
} POLLING;

typedef struct {
	unsigned char* card_idm;
	unsigned char* card_pmm;
} CARD_INFORMATION;

typedef struct {
	int handle;
	unsigned char* message_of_card_find;
	unsigned char* message_of_card_loss;
	unsigned long interval;
	unsigned long retry_count;
} CALL_BACK_PARAMETERS;

typedef BOOL (*INITIALIZE_LIBRARY)(void);
typedef BOOL (*DISPOSE_LIBRARY)(void);
typedef BOOL (*OPEN_READER_WRITER_AUTO)(void);
typedef BOOL (*POLLING_AND_GET_CARD_INFORMATION)(POLLING*, unsigned char*,
		CARD_INFORMATION*);
typedef BOOL (*GET_LAST_CARD_INFORMATION)(CARD_INFORMATION*);
typedef BOOL (*SET_CALL_BACK_PARAMETERS)(CALL_BACK_PARAMETERS*);
typedef BOOL (*START_POLLING)(POLLING*);
typedef BOOL (*STOP_POLLING)(void);

INITIALIZE_LIBRARY initialize_library;
DISPOSE_LIBRARY dispose_library;
OPEN_READER_WRITER_AUTO open_reader_writer_auto;
POLLING_AND_GET_CARD_INFORMATION polling_and_get_card_information;
GET_LAST_CARD_INFORMATION get_last_card_information;
SET_CALL_BACK_PARAMETERS set_call_back_parameters;
START_POLLING start_polling;
STOP_POLLING stop_polling;

HINSTANCE hInstFelicaDLL;

void print_binary(unsigned char* binary, int length);

__declspec(dllexport) int LoadFelicaLibrary(void);
__declspec(dllexport) int FreeFelicaLibrary(void);
__declspec(dllexport) int PollingAndPrintID(void);
__declspec(dllexport) int PollingOnce(char* idm, char* pmm);
__declspec(dllexport) int LastCardInfo(char* idm, char* pmm);
__declspec(dllexport) int SetCallback(HWND hWindow);
__declspec(dllexport) int StartPolling(void);
__declspec(dllexport) int StopPolling(void);
