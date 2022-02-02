

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include <Windows.h>

struct user             /*The user's account and password*/
{
    char num[40];       /*account*/
    char code[30];      /*password*/
    int score44[100];   /*The score of 4*4*/
    int time44[100];
    int index44;
    int score66[100];   /*The score of 6*6*/
    int time66[100];
    int index66;
    int score88[100];   /*The score of 8*8*/
    int time88[100];
    int index88;
    int score1010[100];   /*The score of 10*10*/
    int time1010[100];
    int index1010;
};

int readFile(struct user users[]);/*read the file*/
void saveFile(struct user users[], int usersLen); /*Save the file*/
int loginUser(struct user users[], int usersLen); /*users login*/
int createUser(struct user users[], int* usersLen); /*Creat new account*/
void clearScore(struct user users[], int index);  /*Clear the history*/
void gamePlay(struct user users[], int* usersLen, int index, int size);/*The game*/
void checkGame(struct user users[], int* usersLen, int *index);  /* The history of game*/
void logoutUser(struct user users[], int* usersLen, int index);  /* Logout the ccount*/

/*Clear the history*/
void clearScore(struct user users[], int index) {

    memset(users[index].score44, 0, 100);
    memset(users[index].score66, 0, 100);
    memset(users[index].score88, 0, 100);
    memset(users[index].time44, 0, 100);
    memset(users[index].time66, 0, 100);
    memset(users[index].time88, 0, 100);
    users[index].index44 = 0;
    users[index].index66 = 0;
    users[index].index88 = 0;
}

/*Creat new account*/
int createUser(struct user users[],int *usersLen) {
    char num[40], code1[30], code2[30];
    int i, j;
    int flag;
    system("cls");
    printf("        Registration Page\n\n");
    printf("        Please enter the account name you would like to use:");
    fflush(stdin);
    scanf("%s", num);
    printf("\n\n\nSearching database this may take a moment, please wait...\n\n");
    while (1) {
        flag = 0;
        for (i = 0; i < *usersLen; i++) {
            if (strcmp(users[i].num, num) == 0) {
                flag = 1;
                printf("The user name already exists, please re-create:");
                fflush(stdin);
                scanf("%s", num);
                break;
            }
        }
        if (flag == 0) {
            break;
        }
    }
    printf("Please enter the password you would like to use:");
    fflush(stdin);
    scanf("%s", code1);
    printf("\n\n\nPlease enter your password again:");
    fflush(stdin);
    scanf("%s", code2);
    printf("\n\n\nyou have successfully created and credited your account, you are now logged in...");

    /*The processing of new user data*/
    clearScore(users, *usersLen);
    strcpy(users[*usersLen].num, num);
    strcpy(users[*usersLen].code, code1);
    (*usersLen)++;

    _getch();
    return (*usersLen) - 1;
}

 /*read the file*/
int readFile(struct user users[]) {
    FILE* fp = NULL;   /* The file pointer*/
    int len = 0;    /*Storage structure length*/
    struct user tmp;

    /*Determine whether the account password file exists. If it exists, open the file; otherwise, create a file*/
    if ((fp = fopen("account_code.bin", "rb")) == NULL) {
        if ((fp = fopen("account_code.bin", "wb")) == NULL) {
            printf("Failed to create account password file!\n");
            exit(-1);
        }
    }
    /*Read the file contents into the user structure array and closes the file*/
    else {
        while (1) {  /*Read the entire contents of the file into the user structure array*/
            fread(&tmp, sizeof(struct user), 1, fp);
            if (feof(fp) == 0) {
                fseek(fp, -sizeof(struct user), SEEK_CUR);
                fread(&users[len++], sizeof(struct user), 1, fp);
            }
            else {
                break;
            }

        }
    }
    fclose(fp);
    return len;
}

/*Save the file*/
void saveFile(struct user users[], int usersLen) {
    FILE* fp = NULL; /* The file pointer*/
    int i;

    if ((fp = fopen("account_code.bin", "rb+")) == NULL) {
        printf("Failed to save account password file!\n");
        exit(-1);
    }
    rewind(fp);  /*The file pointer goes back to the head of the file*/
    for (i = 0; i < usersLen; i++) {  /*Writes the entire contents of the structure array to the user file*/
        fwrite(&users[i], sizeof(struct user), 1, fp);
    }
    fclose(fp); 
}

/*The begin menu*/
int beginMenu() {
    int a,b;
    printf("Welcome to Find the Pairs, Test your memory:\n\n");
    printf("Select from the following options:\n\n");
    printf("      '1' Login to your account\n");
    printf("      '2' Create new account\n");
    printf("      '3' Quit\n\n\n");
    printf("      Option:");
    while (1) {
        b = scanf("%d", &a);    /*Receive user input data*/
        if (b == 1)        /*When input data is valid*/
        {
            if (a >= 1 && a <= 3) {
                return a;  /*Returns the function sequence number selected by the user*/
            }
            else {
                printf("      There is an error in the input data, please re-enter:");
            }
        }
        else
        {
            rewind(stdin);   /*flush*/
            printf("      The input data is not valid. Please re-enter:");
        }
    }
    return a;
}

/*User login*/
int loginUser(struct user users[], int usersLen) {
    char num[40], code[30];
    int i, j;
    printf("        Login screen\n\n");
    printf("        Please enter your account number:");
    fflush(stdin);
    scanf("%s", num);
    printf("        Please enter your password:");
    fflush(stdin);
    scanf("%s", code);
    while (1) {
        for (i = 0; i < usersLen; i++) {
            if (strcmp(users[i].num, num) == 0 && strcmp(users[i].code,code) == 0) {
                return i;
            }
        }
        printf("Incorrect account or password!\n");
        printf("        Please enter your account number:");
        fflush(stdin);
        scanf("%s", num);
        printf("        Please enter your password:");
        fflush(stdin);
        scanf("%s", code);
    }

    return usersLen-1;
}

/*The main menu*/
void mainMenu(struct user users[], int *usersLen,int *index) {

    int a, b,c;

    system("cls");
    printf("Select from the following options:\n\n");
    printf("      1. Start Playing\n\n");
    printf("      2. Check your game history\n\n");
    printf("      3. Save the file and logout\n\n");
    printf("      4. Clear game history\n\n");
    printf("      5. Logout this account without saving \n\n\n");
    printf("      Option:");
    while (1) {
        fflush(stdin);
        b = scanf("%d", &a);    /*Receive user input data*/
        if (b == 1)        /*When input data is valid*/
        {
            if (a >= 1 && a <= 5) {
                break;  /*Return the function sequence number selected by the user*/
            }
            else {
                printf("      There is an error in the input data, please re-enter:");
            }
        }
        else
        {
            rewind(stdin);
            printf("      The input data is not valid. Please re-enter:");
        }
    }
    switch (a) {
    case 1:
        printf("Would you like to play:\n");
        printf("          1 Beginner(4*4)\n");
        printf("          2 Intermediate(6*6)\n");
        printf("          3 Expert(8*8)\n");
        printf("          4 Hell(10*10)\n");
        printf("selection:");
        fflush(stdin);
        scanf("%d", &c);
        if (c == 1) {  /*Beginner(4*4)*/
            gamePlay(users, usersLen, *index, 4);
        }
        else if (c == 2) {  /*Intermediate(6*6)*/
            gamePlay(users, usersLen, *index, 6);
        }
        else if (c == 3) {  /*Expert(8*8)*/
            gamePlay(users, usersLen, *index, 8);
        }
        else if (c == 4) {  /*Hell(10*10)*/
            gamePlay(users, usersLen, *index, 10);
        }
        break;
    case 2:
        checkGame(users, usersLen, index);
        break;
    case 3:
        saveFile(users, *usersLen);
        printf("File saved successfully!");
        printf("Press any key to continue:");
        _getch();
        break;
        exit(0);
    case 4:
        clearScore(users, *index);
        saveFile(users, *usersLen);
        printf("Press any key to continue:");
        _getch();
        mainMenu(users, usersLen, index);
        break;
    case 5:
        saveFile(users, *usersLen);
        logoutUser(users, usersLen, *index);
        printf("Account logout successful, the game is being launched...");
        _getch();
        break;
    }
}

int main() {

    struct user users[100];    /*User*/
    int usersLen; /*The number of user*/
    int answer;   /*User's choice in the Start menu*/
    int index;

    usersLen = readFile(users);   /*read the file*/
    answer = beginMenu();
    switch (answer) {   /*User's choice in the Start menu*/
    case 1:
        index = loginUser(users, usersLen);  /*The user login*/
        mainMenu(users, &usersLen, &index);
        break;
    case 2:
        index = createUser(users, &usersLen); /*Creat a new account*/
        mainMenu(users, &usersLen, &index);
        break;
    case 3:
        exit(0);
        break;
    }
}


/*Print the game*/
void printGame(char ch[][10], int size) {
    int i, j;
    system("cls");
    /*Print prompt numbers*/
    for (i = 0; i < size; i++) {
        printf("\n      ");
        for (j = 0; j < size; j++) {
            printf("%-5d", i * size + j + 1);
        }
        printf("\n");
    }
    printf("\n");
    for (i = 0; i < size; i++) {
        printf("\n      ");
        for (j = 0; j < size; j++) {
            printf("%-5c", ch[i][j]);
        }
        printf("\n");
    }
}

/*The game victory*/
BOOL isWin(char ch[][10], int size)     {
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (ch[i][j] == '~') {
                return FALSE;
            }
        }
    }
    return TRUE;
}

/*The game*/
void gamePlay(struct user users[], int* usersLen, int index, int size) {
    int attempts = 0;  /*The number of attempt*/
    int _time;  /*The time*/
    char ch[10][10];
    char ch_rand[10][10];  /*A randomly generated array of cards*/
    time_t start, end;
    /*Initialize the ch array*/
    int i, j;
    int a, b,t;
    int rand_count = 0;
    char card = '!';  /*The value of the card*/
    char choose,k;
    int tx1, ty1, tx2, ty2;
    srand(time(NULL));
    /*Initialize both arrays*/
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            ch[i][j] = '~';
            ch_rand[i][j] = '~';
        }
    }
    while (!isWin(ch_rand,size)) {
    /*Randomly generated column and column subscripts*/
        tx1 = rand() % size;
        ty1 = rand() % size;
        tx2 = rand() % size;
        ty2 = rand() % size;
       /*Make sure it's not the same coordinate*/
        if (tx1 != tx2 || ty1 != ty2) {
            if (ch_rand[tx1][ty1] == '~' && ch_rand[tx2][ty2] == '~') {
                ch_rand[tx1][ty1] = card + rand_count;
                ch_rand[tx2][ty2] = card + rand_count;
                rand_count++;
            }
        }

    }
    printGame(ch_rand, size);
    Sleep(5000);
    printGame(ch, size);
    start = time(NULL);  /*Start the time*/
    while (!isWin(ch, size)) {
        printf("£¨Input 0 0 return mainMenu£©Please select two Numbers:");
        fflush(stdin);
        t = scanf("%d%d", &a, &b);
        while (!(k = getchar()));
        if (a == 0 && b == 0) {   /*Returns the main menu if you enter two zeros*/
            mainMenu(users, usersLen, &index);
        }
        if (t != 2 || (a > size * size) || (b > size * size)||(a == b)||(a<=0)||(b<=0)) {
            printf("\nInput error, please re-enter:\n");
            Sleep(1000);
        }
        else {
            tx1 = (a - 1) / size;
            ty1 = a % size - 1;
            if (a % size == 0) {
                ty1 = size - 1;
            }
            tx2 = (b - 1) / size;
            ty2 = b % size - 1;
            if (b % size == 0) {
                ty2 = size - 1;
            }
            attempts++;
            /*Determine if two cards are equal*/
            if (ch_rand[tx1][ty1] == ch_rand[tx2][ty2]) {
                ch[tx1][ty1] = ch_rand[tx1][ty1];
                ch[tx2][ty2] = ch_rand[tx2][ty2];
                printGame(ch, size);  /*Dispaly the cards*/
                rand_count--;
            }
            else {
                ch[tx1][ty1] = ch_rand[tx1][ty1];
                ch[tx2][ty2] = ch_rand[tx2][ty2];
                printGame(ch, size); /*Display the cards*/
                Sleep(800);  /*Pause for a moment*/
             /*Reduction of the board*/
                ch[tx1][ty1] = '~';
                ch[tx2][ty2] = '~';
                printGame(ch, size); /*Display the cards*/
            }
        }
    }
    end = time(NULL);
    _time = end - start;  /*The time interval*/

    /*Write to file*/
    if (size == 4) {
        users[index].score44[users[index].index44] = attempts;
        users[index].time44[users[index].index44] = _time;
        users[index].index44++;
    }
    else if (size == 6) {
        users[index].score66[users[index].index66] = attempts;
        users[index].time66[users[index].index66] = _time;
        users[index].index66++;
    }
    else if (size == 8) {
        users[index].score88[users[index].index88] = attempts;
        users[index].time88[users[index].index88] = _time;
        users[index].index88++;
    }
     else if (size == 10) {
        users[index].score1010[users[index].index1010] = attempts;
        users[index].time1010[users[index].index1010] = _time;
        users[index].index1010++;
    }
    saveFile(users, *usersLen);  /*Save it to a file*/


    printf("\n\nCongratulations you have completed the board in %d attempts and %d seconds\n\n\n",attempts,_time);
    printf("Press 'n' to exit or any other key to play again:");
    rewind(stdin);
    scanf("%c", &choose);
    if (choose == 'n') {
        mainMenu(users, usersLen, &index);
    }
    else {
        gamePlay(users, usersLen, index, size);
    }
}

/*Check the history of game*/
void checkGame(struct user users[], int *usersLen, int *index) {
    int i = 0;
    printf("Beginner(4*4):\n");
    printf("      Game      Attempts      Time\n");
    for (i = 0; i < users[*index].index44;i++) {  /*For each play of the game, information will be displayed*/
        printf("      %d            %d            %d\n", i + 1, users[i].score44[*index], users[i].time44[*index]);
        i++;
    }
    printf("Intermediate(6*6):\n");
    printf("      Game      Attempts      Time\n");
    for (i = 0; i < users[*index].index66; i++) {
        printf("      %d            %d            %d\n", i + 1, users[i].score66[*index], users[i].time66[*index]);
        i++;
    }
    printf("Expert(8*8):\n");
    printf("      Game      Attempts      Time\n");
    for (i = 0; i < users[*index].index88; i++) {
        printf("      %d            %d            %d\n", i + 1, users[i].score88[*index], users[i].time88[*index]);
        i++;
    }
    printf("Hell(10*10):\n");
    printf("      Game      Attempts      Time\n");
    for (i = 0; i < users[*index].index1010; i++) {
        printf("      %d            %d            %d\n", i + 1, users[i].score1010[*index], users[i].time1010[*index]);
        i++;
    }
    printf("Press any key to continue:");
    _getch();
    mainMenu(users, usersLen, index);
}

/*Logout the account without saving*/
void logoutUser(struct user users[], int* usersLen, int index) {
    int i = 0;
    (*usersLen)--;
    if ((*usersLen) == 0) {  /*Only one account*/
        strcpy(users[0].num,"");
        strcpy(users[0].code,"");
        clearScore(users, 0);
    }
    else {
        for (i = index; i < *usersLen; i++) {
            users[i] = users[i + 1];
        }
    }
}

