
extern "C" void MyPrint(const char*, ...);

void TestPrintfRecursively(int counter){
    if (counter == 0){
        MyPrint("Some stuff be like %c, %s, %o, %b, %d, %x\n", '@', "Wee", 100, 100, 100, 100);
    }
    else{
        for (int i = counter; i > 0; i--) MyPrint(" "); // could be printf ("%*s", i, "") but I use MyPrint instead to test it
        MyPrint("Counter is %d\n", counter);
        
        TestPrintfRecursively(counter - 1);
        
        for (int i = counter; i > 0; i--) MyPrint(" ");
        MyPrint("Counter is %d\n", counter);
    }
}

int main(){

    char testchar = '!';
    TestPrintfRecursively(5);
    return 0;
}