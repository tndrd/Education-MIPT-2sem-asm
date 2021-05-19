#include "tests.h"

int runTests()
{

    printf(H_BAR);

    char* test_output_buf = (char*)calloc(TEST_OUTPUT_BUFFER_SIZE, sizeof(char));
    
    unsigned int test_number = 1; 
    unsigned int failed_tests_quantity = 0;

    if (!test_output_buf)
    {
        printf(RED_CLR "Fatal: failed to create test's output buffer. Shutting down.\n" END_CLR);
        return -1;
    }

    #define TEST(testname, funcname) \
        failed_tests_quantity += runTest(testname, funcname, TESTS_DIRECTORY testname TESTS_IN_FORMAT, TESTS_DIRECTORY testname TESTS_OUT_FORMAT, test_output_buf, &test_number)

    // Put tests here

    TEST("push2nasm", test_push2nasm);


    // Finish putting tests

    free(test_output_buf);


    printf("%d tests failed.\n" H_BAR, failed_tests_quantity);
    return failed_tests_quantity;
}


static int runTest(const char* name, void (*func2test)(char*, char*), const char* testfile_in, const char* testfile_out, char* output_buffer, unsigned int* test_number)
{
    printf("#%d Starting %s test: ", *(test_number++), name);

    long int filesize = 0;

    char* testdata_out = ReadFile(testfile_out, &filesize);
    char* testdata_in  = ReadFile(testfile_in,  &filesize);

    bool do_test = true;

    if (!testdata_in) 
    {
        printf(YLW_CLR "Failed to open %s file!\n" END_CLR, testfile_in);
        do_test = false;
    }

    if (!testdata_out)
    {
        printf(YLW_CLR "Failed to open %s file!\n" END_CLR, testfile_out);
        do_test = false;
    }

    if (!do_test) return 0;

    func2test(testdata_in, output_buffer);
    
    int result = !strncmp(testdata_out, output_buffer, TEST_OUTPUT_BUFFER_SIZE);

    if (result)
    {
        printf(GRN_CLR "OK\n" END_CLR);
        return 0;
    }
    
    printf(RED_CLR "FAILED\n" END_CLR);

    printf(H_BAR "Output is:\n\n");
    printf("%s\n", output_buffer);
    
    printf("But should be:\n");
    printf("%s\n" H_BAR, testdata_out);
    
    free(testdata_in);
    free(testdata_out);

    return 1;
}

void test_push2nasm(char* testdata_in, char* testdata_out)
{
    testdata_in += HEADER_SIZE;
    for (int i = 0; i < 6; i++)
        push2nasm(&testdata_in, &testdata_out);
}