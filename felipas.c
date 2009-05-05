#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "felipas.h"

__declspec(dllexport) int PollingAndPrintID(void)
{
    POLLING polling;
    CARD_INFORMATION card_information;
    unsigned char system_code[2];
    unsigned char number_of_cards;
    unsigned char card_idm[8];
    unsigned char card_pmm[8]; 

    system_code[0] = 0xff;
    system_code[1] = 0xff;
    polling.system_code = system_code;
    polling.time_slot = 0x00;
    number_of_cards = 0x00;
    card_information.card_idm = card_idm;
    card_information.card_pmm = card_pmm;

    if (!polling_and_get_card_information(&polling, &number_of_cards, &card_information))
    {
        printf("ポーリングに失敗しました。\n");
        return (-1);
    }
    else
    {
        printf("IDm : ");
        print_binary(card_idm, FELICA_IDM_L);
        printf("PMm : ");
        print_binary(card_pmm, FELICA_PMM_L);
    }
   
    return 0;
}

__declspec(dllexport) int PollingOnce(char* idm, char* pmm)
{
    POLLING polling;
    CARD_INFORMATION card_information;
    unsigned char system_code[2];
    unsigned char number_of_cards;
    unsigned char card_idm[8];
    unsigned char card_pmm[8]; 

    system_code[0] = 0xff;
    system_code[1] = 0xff;
    polling.system_code = system_code;
    polling.time_slot = 0x00;
    number_of_cards = 0x00;

    card_information.card_idm = card_idm;
    card_information.card_pmm = card_pmm;

    if (!polling_and_get_card_information(&polling, &number_of_cards, &card_information))
    {
        printf("ポーリングに失敗しました。\n");
        return (-4);
    }

    memcpy(idm, card_idm, FELICA_IDM_L / 2);
    memcpy(pmm, card_pmm, FELICA_PMM_L / 2);

    return 0;
}

__declspec(dllexport) int GetLastCardInfo(char* idm, char* pmm)
{
    CARD_INFORMATION card_information;
    unsigned char card_idm[8];
    unsigned char card_pmm[8]; 
    card_information.card_idm = card_idm;
    card_information.card_pmm = card_pmm;
    if (!get_last_card_information(&card_information))
    {
        printf("情報取得に失敗しました。\n");
        return (-4);
    }
    memcpy(idm, card_idm, FELICA_IDM_L);
    memcpy(pmm, card_pmm, FELICA_PMM_L);

    return 0;
}

__declspec(dllexport) int SetCallback(HWND hWindow)
{
    char message_of_find_card[] = "CardFind";
    char message_of_lose_card[] = "CardLose";

    CALL_BACK_PARAMETERS call_back_parameters;
    call_back_parameters.message_of_card_find = message_of_find_card;
    call_back_parameters.message_of_card_loss = message_of_lose_card;
    call_back_parameters.handle = (int)hWindow;
    call_back_parameters.interval = 1;
    call_back_parameters.retry_count = 1;
    if(!set_call_back_parameters(&call_back_parameters)){
        return FALSE;
    } else {
        return TRUE;
    }
}

__declspec(dllexport) int StartPolling(void)
{
    POLLING polling;
    unsigned char system_code[2];
    system_code[0] = 0xff;
    system_code[1] = 0xff;
    polling.system_code = system_code;
    polling.time_slot = 0x0f;

    if (!start_polling(&polling))
    {
        return FALSE;
    }
    return TRUE;
}

__declspec(dllexport) int StopPolling(void)
{
    if (!stop_polling())
    {
        return FALSE;
    }
    return TRUE;
}

int LoadFelicaLibrary(void)
{
    if ((hInstFelicaDLL=LoadLibrary("felica.dll"))==NULL)
    {
        return (-1);
    }

    initialize_library = (INITIALIZE_LIBRARY)GetProcAddress(
            hInstFelicaDLL, "initialize_library");
    dispose_library = (DISPOSE_LIBRARY)GetProcAddress(
            hInstFelicaDLL, "dispose_library");
    open_reader_writer_auto = (OPEN_READER_WRITER_AUTO)GetProcAddress(
            hInstFelicaDLL,"open_reader_writer_auto");
    polling_and_get_card_information = (POLLING_AND_GET_CARD_INFORMATION)GetProcAddress(
            hInstFelicaDLL,"polling_and_get_card_information");
    get_last_card_information = (GET_LAST_CARD_INFORMATION)GetProcAddress(
            hInstFelicaDLL,"get_last_card_information");
    set_call_back_parameters = (SET_CALL_BACK_PARAMETERS)GetProcAddress(
            hInstFelicaDLL,"set_call_back_parameters");
    start_polling = (START_POLLING)GetProcAddress(
            hInstFelicaDLL,"start_polling");
    stop_polling = (STOP_POLLING)GetProcAddress(
            hInstFelicaDLL,"stop_polling");

    if (!initialize_library())
    {
        printf("FeliCaライブラリを初期化できませんでした。\n");
        return (-1);
    }

    if (!open_reader_writer_auto())
    {
        printf("リーダライタをオープンできませんでした。\n");
        dispose_library();
        return (-3);
    }

    return 0;
}

int FreeFelicaLibrary(void)
{
    if (!dispose_library())
    {
        printf("FeliCaライブラリの開放に失敗しました。\n");
        return (-1);
    }

    if (!FreeLibrary(hInstFelicaDLL))
    {
        return (-1);
    }
    return 0;
}

void print_binary(unsigned char* binary, int length)
{
    int i;
    for (i = 0; i <= length - 1; i++) {
        printf("%02X ", binary[i]);
    }
    printf("\n");
}
