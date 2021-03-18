#ifndef PATCH_H
#define PATCH_H

#include "foperations.h"

enum PATCH_RESULTS{
    PATCH_OK,
    PATCH_WRONG_HASH,
    PATCH_WRONG_FILESIZE
};

PATCH_RESULTS DoPatch(MyString* code);

#endif // PATCH_H
