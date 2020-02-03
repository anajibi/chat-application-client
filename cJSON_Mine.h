//
// Created by alin2 on 02-Feb-20.
//

#ifndef CHAT_APPLICATION_PHASE1_CJSON_MINE_H
#define CHAT_APPLICATION_PHASE1_CJSON_MINE_H

#endif //CHAT_APPLICATION_PHASE1_CJSON_MINE_H

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


#define cJSON_False 0
#define cJSON_True 1
#define cJSON_NULL 2
#define cJSON_Number 3
#define cJSON_String 4
#define cJSON_Array 5
#define cJSON_Object 6
#define cJSON_IsReference 256
#define cJSON_StringIsConst 512
#define DBL_EPSILON	__DBL_EPSILON__

typedef struct cJSON {
    struct cJSON *next,*prev;	/* next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem */
    struct cJSON *child;		/* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */

    int type;					/* The type of the item, as above. */

    char *valuestring;			/* The item's string, if type==cJSON_String */
    int valueint;				/* The item's number, if type==cJSON_Number */
    double valuedouble;			/* The item's number, if type==cJSON_Number */

    char *string;				/* The item's name string, if this item is the child of, or is in the list of subitems of an object. */
} cJSON;
typedef struct {
    char *buffer;
    int length;
    int offset;
} printbuffer;
static void *(*cJSON_malloc)(size_t sz) = malloc;
static cJSON *cJSON_New_Item(void);
static char* cJSON_strdup(const char* str);
cJSON *cJSON_Parse(const char *value);
cJSON *cJSON_GetObjectItem(cJSON *object,const char *string);
static cJSON *cJSON_New_Item(void);
static char* cJSON_strdup(const char* );
static char *print_string_ptr(const char *,printbuffer *);
static int cJSON_strcasecmp(const char *s1,const char *s2);
int cJSON_GetArraySize(cJSON *array)	;
cJSON *cJSON_GetArrayItem(cJSON *array,int item);
int childCounter(const char separator , char* value);