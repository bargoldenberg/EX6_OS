#include<stdio.h>
#include<string.h>
#include "Queue.h"
void ceaser_cypher(char* data,int len){
    int i;
    for(i=0;i<len;i++){
        printf("in here %c\n",data[0]);
        if(data[i]>='a' && data[i]<='z'){
            data[i] = (data[i]-'a'+1)%26+'a';
        }
        else if(data[i]>='A' && data[i]<='Z'){
            char c = data[i];
            data[i] = (c-'A'+1)%26+'A';
        }
    }
}

void switchCase(char* data){
    int i;
    for(i=0;i<strlen(data);i++){
        if(data[i]>='a' && data[i]<='z'){
            data[i] = (data[i]-'a')+'A';
        }
        else if(data[i]>='A' && data[i]<='Z'){
            data[i] = (data[i]-'A')+'a';
        }
    }
}


int main(){
    // queue *q = createQ();
    // enQ(q, "Hello");
    // enQ(q, "World");
    // enQ(q, "GG");
    // printf("%s\n", (char*)deQ(q));
    // printf("%s\n", (char*)deQ(q));
    // printf("%s\n", (char*)deQ(q));
    // destroyQ(q);
    // char str[4] = {'H','a','L','\0'};
    // ceaser_cypher(str,strlen(str));
    // printf("%s\n", str);
    // switchCase(str);
    printf("%s\n", "hi");
}