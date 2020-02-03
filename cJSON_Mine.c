//
// Created by alin2 on 02-Feb-20.
//
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cJSON_Mine.h"
static const char *ep;

static cJSON *cJSON_New_Item(void)
{
    cJSON* node = (cJSON*)cJSON_malloc(sizeof(cJSON));
    if (node) memset(node,0,sizeof(cJSON));
    return node;
}

static char* cJSON_strdup(const char* str)
{
    size_t len;
    char* copy;

    len = strlen(str) + 1;
    if (!(copy = (char*)cJSON_malloc(len))) return 0;
    memcpy(copy,str,len);
    return copy;
}

cJSON *cJSON_Parse(const char *value) {
    cJSON *item = cJSON_New_Item();
    char temp[100], temp2[100];
    memset(temp,0,100);memset(temp2,0 ,100);
    int counter = 0,counter2 = 0;
    while(*value != '\"')
      value++;
    value++;
    while(*value != '\"')
        temp[counter++] = *(value++);
    value++;
    while(*value != '\"')
        value++;
    value++;
    while(*value != '\"')
        temp2[counter2++] = *(value++);
    cJSON* child = cJSON_New_Item();
    child ->string = cJSON_strdup(temp);
    child ->valuestring = cJSON_strdup(temp2);
    item ->child = child;
    value++;
    counter = 0;counter2 = 0;
    memset(temp,0,100);memset(temp2,0, 100);
    while(*value != '\"')
        value++;
    value++;
    while(*value != '\"')
        temp[counter++] = *(value++);
    cJSON* secondChild = cJSON_New_Item();
    secondChild->string = cJSON_strdup(temp);
    value++;
    while(*value != '\"' && *value != '[')
        value++;
    if (*value == '['){
        secondChild->type = cJSON_Array;
        secondChild ->prev = child;
        child -> next = secondChild;
        int numberOfChildren = 0;
        if(*(++value) == ']')
            secondChild ->valuestring = cJSON_strdup("");
        else{
            if(*value == '{'){
                numberOfChildren = childCounter('{', value);
                for (int i = 0; i < numberOfChildren ; ++i) {
                    cJSON* object = cJSON_New_Item();
                    char test[10000];
                    long int counter = 0;
                    memset(test,0,10000);
                    while (*value != '}'){
                        test[counter++] = *(value++);
                    }
                    test[counter++] = *(value++);
                    object = cJSON_Parse(test);
                    if(secondChild ->child == NULL)
                        secondChild->child = object;
                    else{
                        cJSON* check = secondChild->child;
                        while (check->next )
                            check = check->next;
                        check ->next = object;
                        object->prev = check;
                    }
                    memset(test, 0,100);
                    counter = 0;
                    object = NULL;
                }
            }
            else{
                numberOfChildren = childCounter('\"', value)/2;
                for (int i = 0; i < numberOfChildren ; ++i) {
                    cJSON* object = cJSON_New_Item();
                    char temp1[200];
                    int counter1 = 0 ;
                    while (*(value++) !='\"');
                    while(*value != '\"')
                        temp1[counter1++] = *(value++);
                    value++;
                    object ->valuestring =cJSON_strdup(temp1);
                    if(secondChild ->child == NULL)
                        secondChild->child = object;
                    else{
                        cJSON* check = secondChild->child;
                        while (check->next )
                            check = check->next;
                        check ->next = object;
                        object->prev = check;
                    }
                    memset(temp1, 0,100);
                    counter1 = 0;;
                    object = NULL;
                }
            }
        }
    }
    else {
        secondChild->type = cJSON_Object;
        secondChild ->prev = child;
        child -> next = secondChild;
        if(*(++value) == '\"' )
            secondChild ->valuestring = cJSON_strdup("");
        else{
            while(*value != '\"')
                temp2[counter2++] = *(value++);
            secondChild->valuestring = cJSON_strdup(temp2);
        }
    }
    return item;
}

cJSON *cJSON_GetObjectItem(cJSON *object,const char *string)	{
    cJSON *c=object->child;
    while (c && cJSON_strcasecmp(c->string,string)) c=c->next;
    return c;
}
static int cJSON_strcasecmp(const char *s1,const char *s2){
    if (!s1) return (s1==s2)?0:1;if (!s2) return 1;
    for(; tolower(*s1) == tolower(*s2); ++s1, ++s2)	if(*s1 == 0)	return 0;
    return tolower(*(const unsigned char *)s1) - tolower(*(const unsigned char *)s2);
}
int cJSON_GetArraySize(cJSON *array){
    cJSON *c=array->child;
    int i=0;
    while(c)i++,c=c->next;
    return i;
}
cJSON *cJSON_GetArrayItem(cJSON *array,int item){
    cJSON *c=array->child;
    while (c && item>0) item--,c=c->next;
    return c;
}

int childCounter(const char separator , char* value){
    int output = 0;
    char * temp = value;
    while (*temp != ']'){
        if(*temp == separator)
            output++;
        temp++;
    }

    return output;
}