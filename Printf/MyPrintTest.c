
extern "C" void MyPrint(const char*, ...);

void TestPrintfRecursively(int counter){
    
    if (counter == 0){
        MyPrint("%s %x %d%%%c%b\n", "I love", 3802, 100, '!', 511);
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

    TestPrintfRecursively(5);
    return 0;
}