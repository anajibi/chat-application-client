#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include "cJSON_Mine.c"
#include <string.h>
#include <conio.h>
#include <stdbool.h>

#define BLUE 60
#define RED 4
#define DEFAULTCOLOR 9
#define LOGIN 3
#define REGISTRATION 11
#define  OUTPUTFONT 50
#define PORT 12345
#define SA struct sockaddr
#define LTARROW 0x4B
#define RTARROW 0x4D
#define UPARROW 0x48
#define DNARROW 0x50
#define ESC 0x1b

typedef struct {
    char username[50];
    char password[20];
} person;

void SetColor(int );
int Registration();
int Login();
void MenuMaker(int, const char**, int);
int socketMaker();
int LoginMenu();
int chatting();
void ErrorPrinter(cJSON *serverResponse);
void gotoxy(int,int);
void ChangeCursorStatus(bool);
char serverToken[32] = {0};
person newPerson;
int rowNumber = 25 ;
int main(void)
{
    SetColor(163);
   gotoxy(43, 3);
    printf("%c",218);
    for(int i=0;i<40;i++)
    {
        printf("%c",196);
    }
    printf("%c\n",191);
    gotoxy(43, 4);
    printf("%c     WELCOME TO MY CHAT APPLICATION     ",179);
    printf("%c\n",179);
    gotoxy(43, 5);
    printf("%c",192);
    for(int i=0;i<40;i++)
    {
        printf("%c",196);
    }
    printf("%c\n",217);
    printf("\n");
    SetColor(155);
    gotoxy(30, 7);
    printf("%c",218);
    for(int i=0;i<65;i++)
    {
        printf("%c",196);
    }
    printf("%c\n",191);
    gotoxy(30, 8);
    printf("%c           You can use arrow keys to move between menus          ",179);
    printf("%c\n",179);
    gotoxy(30, 9);
    printf("%c  and use the enter key or right arrow key to select an options  ",179);
    printf("%c\n",179);
    gotoxy(30, 10);
    printf("%c",192);
    for(int i=0;i<65;i++)
    {
        printf("%c",196);
    }
    printf("%c\n",217);
    printf("\n");
    ChangeCursorStatus(false);
    int option;
    int MenuChoice = 0;
    const char* menu[5] ={"Main menu:"
            ,"1: Register           "
            ,"2: Login              "
            ,"3: Exit               "
            ,"                       "};
    while (1) {
        gotoxy(20, rowNumber + 1);
        MenuMaker(MenuChoice, menu, 5);
        while(1) {

            option = getch();
            if(option == RTARROW ||  option == '\r')
                break;
            switch (option) {
                case UPARROW: // if the entered key is the 'up arrow' notice that its equal to 'P' (capital)
                    MenuChoice--; //then we will increment the cursor by one.
                    MenuChoice < 0 ? MenuChoice += 3 : MenuChoice;
                    MenuMaker(abs(MenuChoice %= 3), menu, 5);
                    break;
                case DNARROW: // same but with 'down arrow' and decrement the cursor.
                    MenuChoice++;
                    MenuMaker(abs(MenuChoice %= 3), menu, 5);
                    break;
                case LTARROW: // same but with 'down arrow' and decrement the cursor.
                    exit(0);
                case ESC:
                    exit(0);
            }

        }
        switch (MenuChoice + 1) {
            case 1:
                while(Registration());
                break;
            case 2:
                while(Login());
                break;
            case 3:
                return 0;
        }
    }
}

void MenuMaker(int optionNumber , const char** menu, int numberOfOptions){
    SetColor(DEFAULTCOLOR);

    for (int i = 0; i < numberOfOptions; ++i) {
        gotoxy(30,17+i);
        optionNumber== i - 1 ? printf(" -> ") : printf("    ");
        printf("%s",menu[i]);
    }
    printf("\n\n\n\n");
    gotoxy(20, 7);
}

int Login(){
    int socket;
    char message[120];
    char *response;
    gotoxy(20, ++rowNumber);
    SetColor(LOGIN);
    printf("Enter your username\n");
    gotoxy(20, ++rowNumber);
    scanf("%s", newPerson.username);
    gotoxy(20, ++rowNumber);
    printf("Enter your password\n");
    gotoxy(20, ++rowNumber);
    scanf("%s", newPerson.password);
    //---------Contacting server----------//
    socket = socketMaker();
    snprintf(message, sizeof(message),"login %s, %s\n",newPerson.username,newPerson.password);
    send(socket, message, sizeof(message), 0);
    memset(message, 0, sizeof(message));
    recv(socket, message, sizeof(message), 0);
    cJSON *serverResponse = cJSON_Parse(message);
    response = cJSON_GetObjectItem(serverResponse, "type") -> valuestring;
    //show the response from server
    if(strcmp(response, "Error") == 0){
        gotoxy(20, ++rowNumber);
        ErrorPrinter(serverResponse);
        return 0;
    } else{
        strcpy(serverToken, cJSON_GetObjectItem(serverResponse, "content") -> valuestring);
        //going to login Menu
        while(LoginMenu());
        return 0;
    }
}

int Registration(){
    int socket;
    char message[120];
    char *response;
    gotoxy(20, ++rowNumber);
    SetColor(REGISTRATION);
    printf("Enter your username\n");
    gotoxy(20, ++rowNumber);
    scanf("%s", newPerson.username);
    gotoxy(20, ++rowNumber);
    printf("Enter your password\n");
    gotoxy(20, ++rowNumber);
    scanf("%s", newPerson.password);
    //---------Contacting server----------//
    socket = socketMaker();
    snprintf(message, sizeof(message),"register %s, %s\n",newPerson.username,newPerson.password);
    send(socket, message, sizeof(message), 0);
    memset(message, 0, sizeof(message));
    recv(socket, message, sizeof(message), 0);
    cJSON *serverResponse = cJSON_Parse(message);
    response = cJSON_GetObjectItem(serverResponse, "type") -> valuestring;
    if(strcmp(response, "Error") == 0){
        gotoxy(20, ++rowNumber);
        printf("This username has been taken, please retry\n");
        return 0;
    } else{
        gotoxy(20, ++rowNumber);
        printf("Username successfully created\n");
        return 0;
    }
}

int LoginMenu(){
    int socket;
    int option;
    int MenuChoice = 0;
    char requestAndResponse[120];
    char channelName[30];
    char *response;
    int c;
    SetColor(LOGIN);
    const char* menu[5]={"Account Menu:                 "
            ,"1: Create Channel            "
            ,"2: Join Channel              "
            ,"3: Logout                    "
            ,"                             "};
    MenuMaker(MenuChoice, menu, 5);
    while (1) {
        gotoxy(20, rowNumber);
        while(1) {
            option = getch();
            if(option == RTARROW ||  option == '\r')
                break;
            switch (option) {
                case UPARROW: // if the entered key is the 'up arrow' notice that its equal to 'P' (capital)
                    MenuChoice--; //then we will increment the cursor by one.
                    MenuChoice < 0 ? MenuChoice += 3: MenuChoice;
                    MenuMaker(abs(MenuChoice %= 3), menu, 5);
                    break;
                case DNARROW: // same but with 'down arrow' and decrement the cursor.
                    MenuChoice++;
                    MenuMaker(abs(MenuChoice %= 3), menu, 5);
                    break;
            }
        }
        switch (MenuChoice + 1) {
            case 1:
                gotoxy(20, ++rowNumber);
                printf("Please enter channel name.(Your channel name should be 30 character at most.)\n");
                fflush(stdin);
                //-----------------------------------//
                gotoxy(20, ++rowNumber);
                scanf ("%30[^\n ]%*c", channelName);
                snprintf(requestAndResponse, sizeof(requestAndResponse), "create channel %s, %s\n", channelName,
                         serverToken);

                socket = socketMaker();
                send(socket, requestAndResponse, sizeof(requestAndResponse), 0);
                memset(requestAndResponse, 0, sizeof(requestAndResponse));
                recv(socket, requestAndResponse, sizeof(requestAndResponse), 0);
                cJSON *serverResponse = cJSON_Parse(requestAndResponse);
                response = cJSON_GetObjectItem(serverResponse, "type")->valuestring;
                //show the response from server
                if (strcmp(response, "Error") == 0) {
                    char *outputMessage = cJSON_GetObjectItem(serverResponse, "content")->valuestring;
                    gotoxy(20, ++rowNumber);
                    printf("%s\n", outputMessage);
                    return 1;
                } else {
                    gotoxy(20, ++rowNumber);
                    printf("Channel successfully created.\n");
                    while (chatting());
                    return 1;
                }
                break;
            case 2:
                socket = socketMaker();
                memset(requestAndResponse, 0, sizeof(requestAndResponse));
                memset(channelName, 0, sizeof(channelName));
                gotoxy(20, ++rowNumber);
                printf("Please enter channel name.(Your channel name should be 30 character at most.)\n");
                fflush(stdin);
                //-----------------------------------//
                gotoxy(20, ++rowNumber);
                scanf ("%30[^\n ]%*c", channelName);
                snprintf(requestAndResponse, sizeof(requestAndResponse), "join channel %s, %s\n", channelName,
                         serverToken);
                send(socket, requestAndResponse, sizeof(requestAndResponse), 0);
                recv(socket, requestAndResponse, sizeof(requestAndResponse), 0);
                serverResponse = cJSON_Parse(requestAndResponse);
                response = cJSON_GetObjectItem(serverResponse, "type")->valuestring;
                //show the response from server
                if (strcmp(response, "Error") == 0) {
                    char *outputMessage = cJSON_GetObjectItem(serverResponse, "content")->valuestring;
                    gotoxy(20, ++rowNumber);
                    printf("%s\n", outputMessage);
                    return 1;
                } else {
                    gotoxy(20, ++rowNumber);
                    printf("Successfully joined the channel.\n");
                    while (chatting());
                    return 1;
                }
                break;
            case 3:
                socket = socketMaker();
                memset(requestAndResponse, 0, sizeof(requestAndResponse));
                memset(channelName, 0, sizeof(channelName));
                snprintf(requestAndResponse, sizeof(requestAndResponse), "logout %s\n",
                         serverToken);
                send(socket, requestAndResponse, sizeof(requestAndResponse), 0);
                recv(socket, requestAndResponse, sizeof(requestAndResponse), 0);
                serverResponse = cJSON_Parse(requestAndResponse);
                response = cJSON_GetObjectItem(serverResponse, "type")->valuestring;
                //show the response from server
                if (strcmp(response, "Error") == 0) {
                    char *outputMessage = cJSON_GetObjectItem(serverResponse, "content")->valuestring;
                    gotoxy(20, ++rowNumber);
                    printf("%s\n", outputMessage);
                    return 1;
                } else {
                    gotoxy(20, ++rowNumber);
                    printf("Logout successful.\n");
                    return 0;
                }

        }
    }
}

int chatting (){
    int option;
   static int MenuChoice = 0;
    int socket;
    char message[400];
    char requestAndResponse[100000];
    char *test;
    cJSON *serverResponse;
    const char* menu[5]={"Chatting Menu"
            ,"1: Send Message                 "
            ,"2: Refresh                      "
            ,"3: Channel Members              "
            ,"4: Leave Channel                "};
    MenuMaker(MenuChoice, menu, 5);
    while (1) {
        gotoxy(20, rowNumber);
        while(1) {
            option = getch();
            if(option == RTARROW ||  option == '\r')
                break;
            switch (option) {
                case UPARROW: // if the entered key is the 'up arrow' notice that its equal to 'P' (capital)
                    MenuChoice--; //then we will increment the cursor by one.
                    MenuChoice < 0 ? MenuChoice += 4: MenuChoice;
                    MenuMaker(abs(MenuChoice %= 4), menu, 5);
                    break;
                case DNARROW: // same but with 'down arrow' and decrement the cursor.
                    MenuChoice++;
                    MenuMaker(abs(MenuChoice %= 4), menu, 5);
                    break;
            }
        }
        switch (MenuChoice + 1) {
            gotoxy(20, ++rowNumber);
            case 1:
                gotoxy(20, ++rowNumber);
                printf("Please enter your message\n");
                //---------- Clearing Buffer----------//
                int c;
                fflush(stdin);
                //-----------------------------------//
                gotoxy(20, ++rowNumber);
                scanf ("%400[^\n]%*c", message);
                socket = socketMaker();
                snprintf(requestAndResponse, sizeof(requestAndResponse), "send %s, %s\n", message,
                         serverToken);
                send(socket, requestAndResponse, sizeof(requestAndResponse), 0);
                memset(requestAndResponse, 0, sizeof(requestAndResponse));
                recv(socket, requestAndResponse, sizeof(requestAndResponse), 0);
                serverResponse = cJSON_Parse(requestAndResponse);
                test = cJSON_GetObjectItem(serverResponse, "type")->valuestring;
                //show the response from server
                if (strcmp(test, "Error") == 0) {
                    gotoxy(20, ++rowNumber);
                    ErrorPrinter(serverResponse);
                    return 1;
                } else {
                    SetColor(BLUE);
                    gotoxy(20, ++rowNumber);
                    printf("Message sent.\n");
                    SetColor(LOGIN);
                    return 1;
                }
            case 2:
                socket = socketMaker();
                memset(requestAndResponse, 0, sizeof(requestAndResponse));
                snprintf(requestAndResponse, sizeof(requestAndResponse), "refresh %s\n", serverToken);
                send(socket, requestAndResponse, sizeof(requestAndResponse), 0);
                memset(requestAndResponse, 0, sizeof(requestAndResponse));
                recv(socket, requestAndResponse, sizeof(requestAndResponse), 0);
                serverResponse = cJSON_Parse(requestAndResponse);
                test = cJSON_GetObjectItem(serverResponse, "type")->valuestring;
                //show the response from server
                if (strcmp(test, "Error") == 0) {
                    gotoxy(20, ++rowNumber);
                    ErrorPrinter(serverResponse);
                    return 1;
                } else {
                    cJSON* name = NULL;
                    cJSON *item = cJSON_GetObjectItem(serverResponse,"content");
                    if(cJSON_GetArraySize(item) == 0){
                        gotoxy(30, ++rowNumber);
                        printf("Nothing to show, you've got all your messages.");
                    }
                    for (int i = 0 ; i < cJSON_GetArraySize(item) ; i++)
                    {
                        cJSON * subitem = cJSON_GetArrayItem(item, i);
                        SetColor(OUTPUTFONT);
                        gotoxy(30, ++rowNumber);
                        printf("%s: %s\n", cJSON_GetObjectItem(subitem, "sender")-> valuestring,
                               cJSON_GetObjectItem(subitem, "content")-> valuestring);
                    }
                    SetColor(LOGIN);
                    return 1;
                }
            case 3:
                socket = socketMaker();
                snprintf(requestAndResponse, sizeof(requestAndResponse), "channel members %s\n", serverToken);
                send(socket, requestAndResponse, sizeof(requestAndResponse), 0);
                memset(requestAndResponse, 0, sizeof(requestAndResponse));
                recv(socket, requestAndResponse, sizeof(requestAndResponse), 0);
                serverResponse = cJSON_Parse(requestAndResponse);
                test = cJSON_GetObjectItem(serverResponse, "type")->valuestring;
                //show the response from server
                if (strcmp(test, "Error") == 0) {
                    gotoxy(20, ++rowNumber);
                    ErrorPrinter(serverResponse);
                    return 1;
                } else {
                    gotoxy(20, ++rowNumber);
                    printf("Channel members are:\n");
                    SetColor(OUTPUTFONT);
                    cJSON *items = cJSON_GetObjectItem(serverResponse, "content");
                    for (int i = 0; i < cJSON_GetArraySize(items); ++i) {
                        gotoxy(30, ++rowNumber);
                        printf("%s\n", cJSON_GetArrayItem(items, i)->valuestring);
                    }
                    SetColor(LOGIN);
                    return 1;
                }
            case 4:

                socket = socketMaker();
                snprintf(requestAndResponse, sizeof(requestAndResponse), "leave %s\n", serverToken);
                send(socket, requestAndResponse, sizeof(requestAndResponse), 0);
                memset(requestAndResponse, 0, sizeof(requestAndResponse));
                recv(socket, requestAndResponse, sizeof(requestAndResponse), 0);
                serverResponse = cJSON_Parse(requestAndResponse);
                test = cJSON_GetObjectItem(serverResponse, "type")->valuestring;
                //show the response from server
                if (strcmp(test, "Error") == 0) {
                    gotoxy(20, ++rowNumber);
                    ErrorPrinter(serverResponse);
                    return 1;
                } else {
                    gotoxy(20, ++rowNumber);
                    printf("You left the channel successfully.\n");
                    return 0;
                }

        }
    }
}

int socketMaker(){
    int client_socket, server_socket;
    struct sockaddr_in servaddr, cli;
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    // Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        return NULL ;
    }

    // Create and verify socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        printf("Socket creation failed...\n");
        exit(0);
    }
    else
        // printf("Socket successfully created..\n");
        // Assign IP and port
        memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // Connect the client socket to server socket
    if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("Connection to the server failed...\n");
        exit(0);
    }
    else
        //printf("Successfully connected to the server..\n");
        // Function for chat
        return client_socket;
    // Close the socket
}

void SetColor(int ForgC)
{
    WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    //We use csbi for the wAttributes word.
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        //Mask out all but the background attribute, and add in the forgournd color
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}

void ErrorPrinter(cJSON *serverResponse){
    SetColor(RED);
    char *outputMessage = cJSON_GetObjectItem(serverResponse, "content")->valuestring;
    printf("%s\n", outputMessage);
    SetColor(LOGIN);
}
void gotoxy(int xpos, int ypos)  // just take this function as it is.
{
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = xpos; scrn.Y = ypos;
    SetConsoleCursorPosition(hOuput,scrn);
}
void clear(int from, int count){
    for (int i = 0; i < count; ++i) {
        gotoxy(0, from + i);
        printf("                                                               ");
    }
}
void ChangeCursorStatus(bool Visible)
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    if (Visible)
    {
        info.bVisible=TRUE;
    }
    else
    {
        info.bVisible = FALSE;
    }

    SetConsoleCursorInfo(consoleHandle, &info);

}
bool getString(char* buffer){
    char test;
    int counter = 0;
    while((test = getche()) != '\r'&& test != ' ' ){
        if(test == ESC)
            return 0;
        else
            buffer[counter++] = test;
    }
    buffer[counter + 1] = '\0';
    return 1;
}
