#include "stdlib.h"
#include "stdio.h" 
#include "assert.h"

void SavePassword(const char* name, char* password);
void StartMenu(int* buf_addr, int* dst_addr, int* value);
char* GeneratePassword(int buf_addr, int dst_addr, int value);
char* GenerateValueBytes(int value);


int main(){
    
    int buf_addr = 0;
    int dst_addr = 0;
    int value    = 0;

    StartMenu(&buf_addr, &dst_addr, &value);
    char* new_password = GeneratePassword(buf_addr, dst_addr, value);
    SavePassword("egor_loh.txt", new_password);
    
    free(new_password);
    return 0;

}


char* GenerateValueBytes(int value){

    char* value_bytes = (char*)calloc(3, sizeof(char));
    assert(value_bytes);

    char higher_byte  = (char)(value >> 8);
    char lower_byte   = (char)(value & 0xFF);

    value_bytes[0] = lower_byte;
    value_bytes[1] = higher_byte;
    value_bytes[2] = '\0';

    return value_bytes;
}


char* GeneratePassword(int buf_addr, int dst_addr, int value){
    
    char* new_password  = (char*)calloc(dst_addr - buf_addr + 4, sizeof(char));
    char* value_bytes   = GenerateValueBytes(value);
    
    assert(new_password);
    assert(value_bytes);
    
    int i = 0;
    for (; i < dst_addr - buf_addr; i++) new_password[i] = '$';
    sprintf(new_password + i, "%c%c\r\0", value_bytes[0], value_bytes[1]);
    printf("value bytes: %X %X\n", value_bytes[0], value_bytes[1]);
    free(value_bytes);
    return new_password;   
}


void StartMenu(int* buf_addr, int* dst_addr, int* value){
    
    assert(buf_addr);
    assert(dst_addr);
    assert(value);

    printf("This is Password Generator for Egor \"Spaghetti\" Dolgodvrodov's program!\n");
    
    printf("Type buffer adress: ");
    scanf("%X", buf_addr);
    
    printf("Type destination adress: ");
    scanf("%X", dst_addr);

    printf("Type desired value: ");
    scanf("%X", value);
    
}


void SavePassword(const char* name, char* password){
    
    assert(password);
    assert(name);
    
    FILE* fp = fopen(name, "w");
    assert(fp);

    fprintf(fp, "%s", password);
    printf("Password successfully saved to %s!\n", name);
    fclose(fp);
}