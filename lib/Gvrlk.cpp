#include "Gvrlk.h"

const char* COMMAND_START_FORMAT = "echo \"";
const char* COMMAND_END_FORMAT = "\" | text2wave -eval '(voice_msu_ru_nsh_clunits)' | play -q - tempo %f"; 

char* EndOfString(char* str){
    return str + strlen(str);
} 

GVRLK_RESULT vGvrlkSay(GVRLK_MODE mode, const char* format, va_list args){

    static char*            say_buffer = (char*)calloc(INITIAL_SAY_BUFFER_SIZE, sizeof(char));
    static GVRLK_PHR_STATUS phr_status = PHR_EMPTY; 
    
    if (phr_status == PHR_NO_LONGER_AVAILABLE) return GVRLK_ATTEMPT_TO_USE_FREE_SAY_BUFFER;

    if (phr_status == PHR_EMPTY){
        sprintf(say_buffer, COMMAND_START_FORMAT);
        phr_status = PHR_BLANK;
    }

    switch(mode){
        
        case SUPPLEMENT_PHR: vsprintf(EndOfString(say_buffer), format, args);
                             phr_status = PHR_FILLED;
                             break;
        
        case FINISH_PHR:     if (phr_status == PHR_BLANK) return GVRLK_ATTEMPT_TO_SAY_BLANK_PHRASE;
                             sprintf(EndOfString(say_buffer), COMMAND_END_FORMAT, PLAY_SPEED);
                             system(say_buffer);
                             phr_status = PHR_EMPTY;
                             break;  

        case SINGLE_PHR:     if (phr_status == PHR_FILLED) return GVRLK_ATTEMPT_SAY_SINGLE_WHILE_IS_FILLED;
                             vGvrlkSay(SUPPLEMENT_PHR, format, args);
                             vGvrlkSay(FINISH_PHR,     format, args);
                             break;
    
        case GVRLK_END:      free(say_buffer);
                             say_buffer = nullptr;
                             phr_status = PHR_NO_LONGER_AVAILABLE;

    }

    return GVRLK_OK;
}

GVRLK_RESULT GvrlkSay(GVRLK_MODE mode, const char* format, ...){

    GVRLK_RESULT result = GVRLK_OK;

    va_list args {};

    va_start(args, format);
    result = vGvrlkSay(mode, format, args);
    va_end(args);

    return result;
}

GVRLK_RESULT GvrlkSayPrint(GVRLK_MODE mode, const char* format, ...){ //Стоит ли делать vGvrlkSayPrint?

    GVRLK_RESULT result = GVRLK_OK;
    
    va_list args_say {};
    va_list args_print {};

    va_start(args_say,   format);
    va_copy (args_print, args_say);
    
    vprintf(format, args_print);
    fflush(stdout);
    result = vGvrlkSay(mode, format, args_say);
    
    va_end(args_say);
    va_end(args_print);

    return result;
}

/*
int main(){

    char* var = (char*)calloc(100, sizeof(char));

    scanf("%s", var);

    GvrlkSayPrint(SINGLE_PHR, "Меня вызвали на строке номер %d.\n", __LINE__);
    GvrlkSayPrint(SINGLE_PHR, "О нет. Мне сказали %s.\n", var);

    GvrlkSayPrint(SUPPLEMENT_PHR, "Первый %s, ", var);
    GvrlkSayPrint(SUPPLEMENT_PHR, "второй %d.\n", __LINE__);
    GvrlkSayPrint(FINISH_PHR, nullptr);

    return 0;
}
*/