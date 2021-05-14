#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

const size_t PHRASE_BEGINNING_OFFSET = 0;
const float  PLAY_SPEED = 1.5; 
const size_t INITIAL_SAY_BUFFER_SIZE = 512; 


enum GVRLK_MODE{
    GVRLK_END = 0,
    SINGLE_PHR = 1,
    SUPPLEMENT_PHR = 2,
    FINISH_PHR = 3,
};

enum GVRLK_RESULT{
    GVRLK_OK = 'o',
    GVRLK_ATTEMPT_SAY_SINGLE_WHILE_IS_FILLED = '1',
    GVRLK_ATTEMPT_TO_SAY_BLANK_PHRASE = '2',
    GVRLK_ATTEMPT_TO_USE_FREE_SAY_BUFFER = 0
};

enum GVRLK_PHR_STATUS{
    PHR_EMPTY,
    PHR_BLANK,
    PHR_FILLED,
    PHR_NO_LONGER_AVAILABLE
};

char* EndOfString(char* str);
GVRLK_RESULT vGvrlkSay(GVRLK_MODE mode, const char* format, va_list args);
GVRLK_RESULT GvrlkSay(GVRLK_MODE mode, const char* format, ...);
GVRLK_RESULT GvrlkSayPrint(GVRLK_MODE mode, const char* format, ...);