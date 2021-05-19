#ifndef __TESTS__
#define __TESTS__

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "coloring.h"
#include "foperations.h"
#include "translator.h"

const size_t TEST_OUTPUT_BUFFER_SIZE = 2048;

#define TESTS_DIRECTORY "tests/"
#define TESTS_IN_FORMAT ".in"
#define TESTS_OUT_FORMAT ".out"

int runTests();
static int runTest(const char* name, void (*func2test)(char*, char*), const char* testfile_in, const char* testfile_out, char* output_buffer, unsigned int* test_number);

void test_push2nasm(char* testdata_in, char* testdata_out);

#endif