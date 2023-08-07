#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "function.h"

#define MAX_UserCred_LENGTH 100


typedef struct Train_Timings {

    int t_num;
    char *t_name;
    char *time_ranchi;
    char *time_koderma;
    char *time_patna;
    char *time_howrah;
    char *time_delhi;

} Train_Timings;

typedef struct Return_Station_names {

    char *s_station;
    char *d_station;
    int station_choice;

} Return_Station_names;

typedef struct Train_Date {

    char t_date[20];
    

}Train_Date;

typedef struct Seat_Count{

    char t_coach[60];
    int o_i;
    int o_d;
    int i_d;

} Seat_Count;

typedef struct train_file{

    int number;
    int t_index;

} train_file;

typedef struct StoD{

    int sta;
    int train_num[20];

    int check;

    int tr_num;
    char tr_name[50];
    char dateofjourney[50];
    char sta_source[50];
    char time_source[50];
    char sta_destination[50];
    char time_destination[50];
    char class[50];
    char coach[50];
    int seat_num[3];

} StoD;

typedef struct Passenger_Details{

    int check;
    int pas_num;
    char p_name[3][50];
    int p_age[3];
    char p_gender[3];

} Passenger_Details;

// Global Variables
static StoD train;
bool loginStatus = false;
bool adminLoginStatus = false;



// Function to start the system
int systemStart(void) {

    loginStatus = false;
    adminLoginStatus = false;
/*

Creating a file to store user_id and password ----> it will run only once to create a file so that it can store user credentials
FILE *fp;
fp = fopen("t_files/userCredentials.bin", "wb+");
char *str  = "USER_ID\t\t\t\t\t\tPASSWORD";
fprintf(fp, "%s", str);

*/


// updating the seat asF per todays date in files
    seatACdate();
// Display of beginning of the program
wrongChoice: ;
    printf("\nUser Login  ----> PRESS 1\n");
    printf("New User Registration  ----> PRESS 2\n");
    printf("To Check PNR  ----> PRESS 3\n");
    printf("Admin Login ----> PRESS 4\n");
    printf("To QUIT APPLICATION ----> PRESS 0\n");

    int a;
    printf("\nEnter your choice: ");
    scanf("%d%*c", &a);

    if (a == 1)
    {
        uLogPortal();
    }

    else if (a == 2)
    {
        nLogPortal();
    }

    else if (a == 3)
    {
        int k = pnrCheck();
        if(k == 1) {
            systemStart();
        }
    }

    else if (a == 4)
    {
        aLogPortal();
    }

    else if(a == 0) {
        exit(0);
    }

    else
    {
        printf("\nTry again! You entered wrong choice....\n");
        goto wrongChoice;
    }
}


// Function to generate random prime number between 20 and 99
int randomPrime(void)
{
    int primeArr[] = {23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};
    srand(time(0));

    return primeArr[rand() % 17];
}


// Function for hashing
long long hash(char *str)
{
    long long has = 0, hasF = 0;
    int p = randomPrime();
    long long pow = 1;
    long long m = 1e9 + 7;

    for (int i = 0; i < strlen(str); i++)
    {
        has += ((str[i] - 32 + 1) * pow) % m;
        pow *= p % m;
    }
    hasF = ((has % m)*100) + p;

    return hasF;
}


// Function to update the the data of the number of seats in file according to the current date
void seatACdate(void) {

    const char *SEAT_DETAILS_OUT = "%s, %s, %d, %d, %d, %s, %d, %d, %d, %s, %d, %d, %d, %s, %d, %d, %d\n";

    const char *SEAT_DETAILS_IN = "%[^,], %[^,], %d, %d, %d, %[^,], %d, %d, %d, %[^,], %d, %d, %d, %[^,], %d, %d, %d\n";


    
    FILE *fp;
    fp = fopen("t_files/trains_files.bin", "rb");
    if(fp == NULL) {
        printf("\nSomething went wrong...1\n");
        exit(EXIT_FAILURE);
    }
    rewind(fp);

    int tr_n, index;

    while (fscanf(fp, "%d, %d\n", &tr_n, &index) != EOF)
    {
        char n_dt[15] = {0};
        char file2_name_[60] = "t_files/Train_";
        char ch_num_[5] = {0};
        char *lastPart_ = "_seats.bin";
        int cu_ = strlen(file2_name_);

        sprintf(ch_num_, "%d", index);

        for (int i = 0; ch_num_[i] != '\0'; i++)
        {
            file2_name_[cu_] = ch_num_[i];
            cu_++;
        }
        for (int i = 0; lastPart_[i] != '\0'; i++)
        {
            file2_name_[cu_] = lastPart_[i];
            cu_++;
        }
        file2_name_[cu_] = '\0';
        
        FILE *file;
        file = fopen(file2_name_, "rb");
        if(file == NULL) {
            printf("\nSomething went wrong...2\n");
            exit(EXIT_FAILURE);
        }

        char date[3][15];
        Seat_Count tt[3][4];

        rewind(file);
        for (int i = 0; i < 3; i++)
        {
            fscanf(file, SEAT_DETAILS_IN, &date[i], &tt[i][0].t_coach, &tt[i][0].o_i, &tt[i][0].o_d, &tt[i][0].i_d, &tt[i][1].t_coach, &tt[i][1].o_i, &tt[i][1].o_d, &tt[i][1].i_d, &tt[i][2].t_coach, &tt[i][2].o_i, &tt[i][2].o_d, &tt[i][2].i_d, &tt[i][3].t_coach, &tt[i][3].o_i, &tt[i][3].o_d, &tt[i][3].i_d);
        }

        fclose(file);


        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        char day[3];
        sprintf(day, "%d", tm.tm_mday);
        day[2] = '\0';

        char month[3];
        sprintf(month, "%d", tm.tm_mon+1);

        char year[5];
        sprintf(year, "%d", tm.tm_year+1900);

        char dt[15] = {0};

        strcat(dt, day);
        strcat(dt, "-");
        strcat(dt, month);
        strcat(dt, "-");
        strcat(dt, year);

        // put old data to history file 
        if(strcmp(date[0], dt) != 0) {
            
            FILE *file2;

            char f_n[60];
            for (int i = 0; i < strlen(file2_name_)-8; i++)
            {
                f_n[i] = file2_name_[i+8];
            }
            f_n[strlen(f_n)] = '\0';

            char file2_name[60] = "admin/seats_history/_his_";
            int cu = strlen(file2_name);
        
            for (int i = 0; f_n[i] != '\0'; i++)
            {
                file2_name[cu] = f_n[i];
                cu++;
            }
            file2_name[cu] = '\0';

            file2 = fopen(file2_name, "ab");
            if(file2 == NULL) {
                printf("\nSomething went wrong...\n");
                exit(EXIT_FAILURE);
            }

            fprintf(file2, SEAT_DETAILS_OUT, date[0], tt[0][0].t_coach, tt[0][0].o_i, tt[0][0].o_d, tt[0][0].i_d, tt[0][1].t_coach, tt[0][1].o_i, tt[0][1].o_d, tt[0][1].i_d, tt[0][2].t_coach, tt[0][2].o_i, tt[0][2].o_d, tt[0][2].i_d, tt[0][3].t_coach, tt[0][3].o_i, tt[0][3].o_d, tt[0][3].i_d);  

            fclose(file2);

            // update data in the actual file
            FILE *fptr;
            fptr = fopen(file2_name_, "wb");
            if(fptr == NULL) {
                printf("\nSomething went wrong...\n");
                exit(EXIT_FAILURE);
            }


            for (int i = 0; i < 2; i++)
            {
                fprintf(fptr, SEAT_DETAILS_OUT, date[i+1], tt[i+1][0].t_coach, tt[i+1][0].o_i, tt[i+1][0].o_d, tt[i+1][0].i_d, tt[i+1][1].t_coach, tt[i+1][1].o_i, tt[i+1][1].o_d, tt[i+1][1].i_d, tt[i+1][2].t_coach, tt[i+1][2].o_i, tt[i+1][2].o_d, tt[i+1][2].i_d, tt[i+1][3].t_coach, tt[i+1][3].o_i, tt[i+1][3].o_d, tt[i+1][3].i_d);
            }
            

            char n_day[3];
            char n_month[3];
            char n_year[5];

            if(tm.tm_mday+2 == 31) {
                if(tm.tm_mon+1 == 4 || tm.tm_mon+1 == 6 || tm.tm_mon+1 == 9 || tm.tm_mon+1 == 11) {
                    sprintf(n_day, "%d", 1);
                    sprintf(n_month, "%d", tm.tm_mon+2);
                    
                }
            }
            else if(tm.tm_mday+2 > 31) {
                if(tm.tm_mon+1 == 4 || tm.tm_mon+1 == 6 || tm.tm_mon+1 == 9 || tm.tm_mon+1 == 11) {
                    sprintf(n_day, "%d", 2);
                    sprintf(n_month, "%d", tm.tm_mon+2);
                    
                }
            }

            else if(tm.tm_mday+2 == 31) {
                if(tm.tm_mon+1 != 4 && tm.tm_mon+1 != 6 && tm.tm_mon+1 != 9 && tm.tm_mon+1 != 11 && tm.tm_mon+1 != 2) {

                    
                    sprintf(n_day, "%d", 31);
                    sprintf(n_month, "%d", tm.tm_mon+1);
                }
            }

            else if(tm.tm_mday+2 > 31) {
                if(tm.tm_mon+1 != 4 && tm.tm_mon+1 != 6 && tm.tm_mon+1 != 9 && tm.tm_mon+1 != 11 && tm.tm_mon+1 != 2) {

                    if(tm.tm_mon+1 != 12) {
                        sprintf(n_day, "%d", 1);
                        sprintf(n_month, "%d", tm.tm_mon+2);
                    }
                    else if(tm.tm_mon+1 == 12) {
                        sprintf(n_day, "%d", 1);
                        sprintf(n_month, "%d", 1);
                        sprintf(n_year, "%d", tm.tm_year+1900+1);
                    }

                }
            }

            else if(tm.tm_mday+2 == 29 && tm.tm_mon+1 == 2) {

                int y = tm.tm_year+1900;

                if (y % 4 == 0 && y % 100 != 0)
                {
                    // leap year
                    sprintf(n_day, "%d", 29);
                    sprintf(n_month, "%d", tm.tm_mon+1);
                }

                else if (y % 4 == 0 && y % 100 == 0 && y % 400 == 0)
                {
                    // leap year
                    sprintf(n_day, "%d", 29);
                    sprintf(n_month, "%d", tm.tm_mon+1);
                }

                else
                {
                    // not leat year
                    sprintf(n_day, "%d", 1);
                    sprintf(n_month, "%d", tm.tm_mon+2);
                }
                
            }
            else if(tm.tm_mday+2 > 29 && tm.tm_mon+1 == 2) {

                sprintf(n_day, "%d", 1);
                sprintf(n_month, "%d", tm.tm_mon+2);
            }

            // else if(strcmp(date[0], dt) != 0) {

            // }

            else {
                sprintf(n_day, "%d", tm.tm_mday+2);
                sprintf(n_month, "%d", tm.tm_mon+1);
                sprintf(n_year, "%d", tm.tm_year+1900);
            }


            strcat(n_dt, n_day);
            strcat(n_dt, "-");
            strcat(n_dt, n_month);
            strcat(n_dt, "-");
            strcat(n_dt, n_year);
            
            fprintf(fptr, SEAT_DETAILS_OUT, n_dt, tt[0][0].t_coach, 8, 8, 8, tt[0][1].t_coach, 8, 8, 8, tt[0][2].t_coach, 8, 8, 8, tt[0][3].t_coach, 8, 8, 8);  
            

            fclose(fptr);
        
        }
        
    }
    fclose(fp);

}

// Function to accept only alpha numeric password
char *passTake(void)
{
    const int min_count = 5;

passAplhaNumric: ;
    char *str = (char *)malloc(31 * sizeof(char));

    printf("\n--> Password should be Alphanumeric\n--> Password should have atleast 5 characters\n-->Max Charaters limit is 17");
    printf("\nEnter the Password: ");
    scanf("%s", str);

    int ch = 0, num = 0, sym = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] >= 32 && str[i] <= 47)
        {
            sym += 1;
        }
        else if (str[i] >= 58 && str[i] <= 64)
        {
            sym += 1;
        }
        else if (str[i] >= 48 && str[i] <= 57)
        {
            num += 1;
        }
        else
        {
            ch += 1;
        }
    }

    if ((ch != 0 && num != 0 && sym != 0) && (strlen(str) >= min_count))
    {
        printf("\nPassword Accepted\n");
        return str;
        free(str);
    }
    else
    {
        free(str);
        printf("\nSomething is missing. Password should be Alphanumeric or Password is too short\nTry again.....\n");
        goto passAplhaNumric;
    }
}

// Function to check if the password entered by user is correct or not
int checkPass(char *str, long long pass){
    
    long long c_has = 0, c_hasF = 0;
    int p = pass % 100;
    long long pow = 1;
    long long m = 1e9 + 7;

    for (int i = 0; i < strlen(str); i++)
    {
        c_has += ((str[i] - 32 + 1) * pow) % m;
        pow *= p % m;
    }
    c_hasF = ((c_has % m)*100) + p;

    if(c_hasF == pass){
        return 1;
    }
    else{
        return -1; 
    }
}

// Function to check the userId and to get password from the userCredentials file
long long chk_ID_get_key(char *uid, int az) {

    if(az == 1) {

        FILE *fptr;
        fptr = fopen("t_files/userCredentials.bin", "rb");
        
        if(fptr == NULL){
            printf("\nUnable to load the file\n\tOR\nFile not found\n");
            exit(EXIT_FAILURE);
        }

        char line[MAX_UserCred_LENGTH];
        char pass[30];
        int count = 0;

        while(fgets(line, MAX_UserCred_LENGTH, fptr) != NULL) {
            if(strncmp(line, uid, strlen(uid)) == 0) {
                count += 1;
                strncpy(pass, line + (strlen(uid) + 2) -1, 30);
                return atoll(pass);
            }
        }
        if(count == 0) return (long long)-1;

        fclose(fptr);
    }

    if(az == 2) {

        FILE *fptr;
        fptr = fopen("admin/adminCredentials.bin", "rb");
        
        if(fptr == NULL){
            printf("\nUnable to load the file\n\tOR\nFile not found\n");
            exit(EXIT_FAILURE);
        }

        char line[MAX_UserCred_LENGTH];
        char pass[30];
        int count = 0;

        while(fgets(line, MAX_UserCred_LENGTH, fptr) != NULL) {
            if(strncmp(line, uid, strlen(uid)) == 0) {
                count += 1;
                strncpy(pass, line + (strlen(uid) + 2) -1, 30);
                return atoll(pass);
            }
        }
        if(count == 0) return (long long)-1;

        fclose(fptr);

    }


}

// Function to start booking 
Return_Station_names startBooking(void) {

    if(loginStatus == true) {

        try: ;
        int c;
        printf("\n\nTO BOOK TICKET ----> PRESS 1\nTO Check PNR ----> PRESS 2\nTO LOGOUT ----> PRESS 9\n");
        printf("\n\nEnter your Choice: ");
        scanf("%d", &c);

        if(c == 9) {
            
            loginStatus = false;
            systemStart();

        }

        if(c == 2) {
            int k = pnrCheck();
            if(k == 1 || k == -1) {
                goto try;
            }

        }

        if(c != 1) {
        trychoice: ;
            printf("\n\n-------------------------------------------------------------------------");
            printf("\nTO GOTO PREVIOUS MENU ---> PRESS 2");
            printf("\nTO LOGOUT ---> PRESS 9");

            int a;
            printf("\n\nEnter your Choice: ");
            scanf("%d", &a);
            printf("-------------------------------------------------------------------------\n");

            if(a == 2) goto try;
            if(a == 9) {
                loginStatus = false;
                systemStart();
            }
            else{
                goto trychoice;
            }
        }

        if(c == 1) {

            try_station: ;
            char *s_station = (char *)malloc(20 * sizeof(char));
            char *d_station = (char *)malloc(20 * sizeof(char));

            printf("\n\n----------------YOU CAN SELECT FROM THESE STATIONS BELOW----------------\n\n\t\t1. RANCHI\n\t\t2. KODERMA\n\t\t3. PATNA\n\t\t4. HOWRAH\n\t\t5. DELHI\n");


            FILE *fptr;
            fptr = fopen("t_files/stations.bin", "rb");
        
            if(fptr == NULL) {
            printf("\nUnable to load the file\n\tOR\nFile not found\n");
            exit(EXIT_FAILURE);
            }

            printf("\nEnter Source: ");
            scanf("%s", s_station);

            for (int i = 0; i < strlen(s_station); i++) {
                s_station[i] = tolower(s_station[i]);
            }

            char line[100];
            int count = 0;
        
            while(fgets(line, 100, fptr) != NULL) {

                if(strncmp(line, s_station, strlen(s_station)) == 0) {
                    count += 1;
                }

            }

            if(count == 0) {

            trychoice2: ;
                int z;

                printf("\nStation not reached till now...\n");
                printf("\nTO GO TO PREVIOUS MENU ---> PRESS 2");
                printf("\nTO LOGOUT ---> PRESS 9");

                printf("\n\nEnter your Choice: ");
                scanf("%d", &z);

                if(z == 2) {
                    free(s_station);
                    free(d_station);
                    goto try_station;
                }

                if(z == 9) {
                    loginStatus = false;
                    systemStart();
                }
                else {
                    goto trychoice2;
                }

            }

            else {

                printf("\nEnter Destination: ");
                scanf("%s", d_station);

                for (int i = 0; i < strlen(d_station); i++) {
                    d_station[i] = tolower(d_station[i]);
                }

                if(strcmp(s_station, d_station) == 0) {
                    printf("\nSource and Destination station can't be same\n");
                    goto try_station;
                }

                rewind(fptr);

                char line2[100];
                int count = 0;
        
                while(fgets(line2, 100, fptr) != NULL) {

                    if(strncmp(line2, d_station, strlen(d_station)) == 0)    {
                        count += 1;
                    }
                }

                if(count == 0) {

                trychoice3: ;
                    int z;

                    printf("\nStation not reached till now...\n");
                    printf("\nTO GO TO PREVIOUS MENU ---> PRESS 2");
                    printf("\nTO LOGOUT ---> PRESS 9");

                    printf("\n\nEnter your Choice: ");
                    scanf("%d", &z);

                    if(z == 2) {
                        free(s_station);
                        free(d_station);
                        goto try_station;
                    }

                    if(z == 9) {
                        loginStatus = false;
                        systemStart();
                    }
                    else {
                        goto trychoice3;
                    }
                }
                else {
                    
                    Return_Station_names retn;
                    retn.s_station = s_station;
                    retn.d_station = d_station;

                    return retn;
                }

            }

            fclose(fptr);

            free(s_station);
            free(d_station);

        }

    }

}

StoD train_rtn;

StoD train_num_;
// Function to show trains between stations
void showTrains(char *s, char *d) {

    int s_index, d_index;
    // StoD train_num_;
    
    // For conversion of sand d in lowercase
    for (int k = 0; k < strlen(s); k++)
    {
        s[k] = tolower(s[k]);
    }

    for (int k = 0; k < strlen(d); k++)
    {
        d[k] = tolower(d[k]);
    }
    //

    FILE *file2;
    file2 = fopen("t_files/stations.bin", "rb");

    if(file2 == NULL) {
        printf("\nSomething Went Wrong...\n");
        exit(EXIT_FAILURE);
    }

    rewind(file2);

    char line[60];
    int i = 0;
    while(fgets(line, 60, file2)) {
        if(strncmp(line, s, strlen(s)) == 0) {
            s_index = i;
        }
        else {
            if(strncmp(line, d, strlen(d)) == 0) {
                d_index = i;
            }
        }
        i++;
    }

    int sta;
    if((s_index+1) != 2 && (d_index+1) != 2) {
        train_num_.sta = 2;
    }
    else if((s_index+1) == 2) {
        train_num_.sta = 3;
    }
    else {
        train_num_.sta = 1;
    }


    fclose(file2);

    // loading data to station timings in an array
    char arrData[14][7][50]; // array to store the content of the file to be read below

    FILE *file;
    file = fopen("t_files/timings.bin", "rb");

    int m = 0;
    char a_line[200];

    rewind(file);
    while(fgets(a_line, sizeof(a_line)/sizeof(a_line[0]), file)) {

        if(strchr(a_line, '\n') == NULL) {
            printf("\nline too long...\n");
            // return EXIT_FAILURE;
        }

        int j = 0;
        char *ptr1 = strtok(a_line, ",");
        while(ptr1 != NULL) {
            strcpy(arrData[m][j], ptr1);
            ptr1 = strtok(NULL, ",");
            j++;
        }
        m++;
    }

    fclose(file);

    printf("\n\n");

    char arr_fData[14][7][2][50];

    for (int i = 0; i < 14; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            int pos;
            // printf("%s\n", arrData[i][j]);
            for (int k = 0; k < strlen(arrData[i][j]); k++)
            {
                if(arrData[i][j][k] == '>') {
                    pos = k;
                    break;
                }                    
            }
            strncpy(arr_fData[i][j][0], arrData[i][j], pos);
            arr_fData[i][j][0][pos] = '\0';
            strncpy(arr_fData[i][j][1], arrData[i][j] + (pos+1), (strlen(arrData[i][j])-2) - (pos));
            arr_fData[i][j][1][((strlen(arrData[i][j])-2) - (pos))] = '\0';
        }
    }

    // static int train_num[20];
// numTry:
    int arr[15];
    int position;
    char *n = "(null)";
    for (int i = 0; i < 14; i++)
    {
        
        if(strncmp(s, arr_fData[i][s_index+1][0], strlen(s)) == 0 && strncmp(d, arr_fData[i][d_index+1][0], strlen(d)) == 0) {

            if (strncmp(arr_fData[i][s_index+1][1], n, strlen(n)) == 0 || strncmp(arr_fData[i][d_index+1][1], n, strlen(n)) == 0) {

                continue;
            }

            char s_ch_time[5], d_ch_time[5];
            int s_time, d_time;

            s_ch_time[0] = arr_fData[i][s_index+1][1][0];
            s_ch_time[1] = arr_fData[i][s_index+1][1][1];

            d_ch_time[0] = arr_fData[i][d_index+1][1][0];
            d_ch_time[1] = arr_fData[i][d_index+1][1][1];

            s_time = atoi(s_ch_time);
            d_time = atoi(d_ch_time);

            if(s_time >= d_time) {
                continue;
            }


            // For conversion in upper case
            for (int k = 0; k < strlen(arr_fData[i][s_index+1][0]); k++)
            {
                arr_fData[i][s_index+1][0][k] = toupper(arr_fData[i][s_index+1][0][k]);
            }

            for (int k = 0; k < strlen(arr_fData[i][d_index+1][0]); k++)
            {
                arr_fData[i][d_index+1][0][k] = toupper(arr_fData[i][d_index+1][0][k]);
            }
            //
            

            // train_num[i] = atoi(arr_fData[i][0][0]);
            train_num_.train_num[i] = atoi(arr_fData[i][0][0]);

            printf("%s  %s   %s: %s  ", arr_fData[i][0][0], arr_fData[i][0][1], arr_fData[i][s_index+1][0], arr_fData[i][s_index+1][1]);

            printf("%s: %s\n\n", arr_fData[i][d_index+1][0], arr_fData[i][d_index+1][1]);

            arr[i] = i;

           
        }

    }
numTry: ;
    int t_num, count = 0;
        printf("\n\nEnter the Train Number to select that train: ");
        scanf("%d", &t_num);

        for (int j = 0; j < 15; j++)
        {
            if(t_num == atoi(arr_fData[arr[j]][0][0])) {

                count++;

                train_num_.tr_num = t_num;

                strcpy(train_num_.tr_name, arr_fData[arr[j]][0][1]);

                strcpy(train_num_.sta_source, arr_fData[arr[j]][s_index+1][0]);
                strcpy(train_num_.time_source, arr_fData[arr[j]][s_index+1][1]);

                strcpy(train_num_.sta_destination, arr_fData[arr[j]][d_index+1][0]);
                strcpy(train_num_.time_destination, arr_fData[arr[j]][d_index+1][1]);

            }
        }

        if(count == 0) {
            
            printf("\nWrong Train Number entered...\n\nTo Exit--->Press 0\nTo Go Back--->Press 1");
            int c;
            printf("\nYour Choice: ");
            scanf("%d", &c);
            if(c == 0) {
                systemStart();
            } 
            else {
                goto numTry;
            }
        }


}

// Function to update the seats in the main files
void update_in_file(int train_index_in_file, Seat_Count tt[3][4], Train_Date dt[3]) {

    const char *SEAT_DETAILS_OUT = "%s, %s, %d, %d, %d, %s, %d, %d, %d, %s, %d, %d, %d, %s, %d, %d, %d\n";

    FILE *file;

    char f__name_2[60] = "t_files/Train_";
    char *lastPart = "_seats.bin";
    char ch_num[5];
    int cot = strlen(f__name_2);
    
    sprintf(ch_num, "%d", train_index_in_file);
    for (int i = 0; ch_num[i] != '\0'; i++)
    {
        f__name_2[cot] = ch_num[i];
        cot++;
    }

    for (int i = 0; lastPart[i] != '\0'; i++)
    {
        f__name_2[cot] = lastPart[i];
        cot++;
    }

    f__name_2[cot] = '\0';
        
    file = fopen(f__name_2, "wb");
    
    if(file == NULL) {
        printf("\nSomething went wrong...\n");
    }

    for (int i = 0; i < 3; i++)
    {

        fprintf(file, SEAT_DETAILS_OUT, dt[i].t_date, tt[i][0].t_coach, tt[i][0].o_i, tt[i][0].o_d, tt[i][0].i_d, tt[i][1].t_coach, tt[i][1].o_i, tt[i][1].o_d, tt[i][1].i_d, tt[i][2].t_coach, tt[i][2].o_i, tt[i][2].o_d, tt[i][2].i_d, tt[i][3].t_coach, tt[i][3].o_i, tt[i][3].o_d, tt[i][3].i_d);
    }

    fclose(file); 

}

// Function to generate PNR and also save it for the record
int generatePnr(void) {

    int ini_pnr, fin_pnr;

    // To read PNR from the PNR_MAIN file and also update it 
    FILE *file;
    file = fopen("t_files/PNR_MAIN.bin", "rb+");

    if(file == NULL) {
        printf("\nSomething went wrong...\n");
        systemStart();
    }

    rewind(file);
    fscanf(file, "%d\n", &ini_pnr);
    
    fin_pnr = ini_pnr + 1;

    rewind(file);
    fprintf(file, "%d\n", fin_pnr);

    fclose(file);


    // To write the new pnr to the PNR_COUNT file to maintain the record of pnr generated
    FILE *file2;
    file2 = fopen("t_files/PNR_COUNT.bin", "ab");
    if(file2 == NULL) {
        printf("\nSomething went wrong...\n");
        systemStart();
    }

    fprintf(file2, "%d\n", fin_pnr); 
    
    fclose(file2);

    return fin_pnr;

}

// Function to update the Fare_history of a train in its file
void updateFare(int tr_num, char tr_date[], char class[], int fare) {

    int c_choice;
    if(strcmp(class, "Sleeper") == 0) c_choice = 0;
    else if(strcmp(class, "AC-III") == 0) c_choice = 1;
    else if(strcmp(class, "AC-II") == 0) c_choice = 2;
    else if(strcmp(class, "AC-I") == 0) c_choice = 3;


    char f_name[60] = "admin/fare_history/";
    char c_num[7] = {0};
    char *lastPart = ".bin";
    int count = strlen(f_name);

    sprintf(c_num, "%d", tr_num);

    for (int i = 0; c_num[i] != '\0'; i++)
    {
        f_name[count] = c_num[i];
        count++;
    }

    for (int i = 0; lastPart[i] != '\0'; i++)
    {
        f_name[count] = lastPart[i];
        count++;
    }
    f_name[count] = '\0';
    

    char temp_name[60] = "admin/fare_history/temp___";
    char temp_ext[7] = ".bin";
    strcat(temp_name, c_num);
    strcat(temp_name, temp_ext);
    temp_name[strlen(temp_name)] = '\0';


    FILE *file, *temp;
    file = fopen(f_name, "rb");
    temp = fopen(temp_name, "wb");
    if(file == NULL) {
        printf("Something went wrong...");
        systemStart();
    }
    if(temp == NULL) {
        printf("Something went wrong...");
        systemStart();
    }

    rewind(file);
    rewind(temp);

    char date[15] = {0};
    int arr[4];
    int line_num = 0;
    bool status = true;

    while (fscanf(file, "%[^,], %d, %d, %d, %d\n", date, &arr[0], &arr[1], &arr[2], &arr[3]) != EOF) {
        
        if(strcmp(tr_date, date) == 0) {

            switch (c_choice)
            {
                case 0:
                    arr[0] += fare; 
                    break;

                case 1:
                    arr[1] += fare; 
                    break;

                    
                case 2:
                    arr[2] += fare; 
                    break;

                case 3:
                    arr[3] += fare;
                    break;

            }
            fprintf(temp, "%s, %d, %d, %d, %d\n", date, arr[0], arr[1], arr[2], arr[3]);
            status = false;

        }

        else if(strcmp(tr_date, date) != 0) {

            fprintf(temp, "%s, %d, %d, %d, %d\n", date, arr[0], arr[1], arr[2], arr[3]);
        }

    }
    if(status == true) {

        arr[0] = 0, arr[1] = 0, arr[2] = 0, arr[3] = 0;

        switch (c_choice)
            {
                case 0:
                    arr[0] += fare; 
                    break;

                case 1:
                    arr[1] += fare; 
                    break;

                    
                case 2:
                    arr[2] += fare; 
                    break;

                case 3:
                    arr[3] += fare;
                    break;

            }

        fprintf(temp, "%s, %d, %d, %d, %d\n", tr_date, arr[0], arr[1], arr[2], arr[3]);
    }
    
    fclose(file);
    fclose(temp);

    remove(f_name);
    rename(temp_name, f_name);

}

Passenger_Details ps;

// Function to print the ticket
int printTicket(void) {

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    const char *FARES_IN = "%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d";

    int pnr = generatePnr();
    int fare;
    int fare_arr[3][4];

    FILE *fptr;
    fptr = fopen("admin/fare_details/fares.bin", "rb");
    if(fptr == NULL) {
        printf("\nSomething went Wrong...\n");
        exit(EXIT_FAILURE);
    }

    rewind(fptr);

    int i = 0;
    char line[200];
    while(fgets(line, sizeof(line)/sizeof(line[0]), fptr)) {

        if(strchr(line, '\n') == NULL) {
           continue;
        }

        int j = 0;
        char *ptr1 = strtok(line, ",");
        if (atoi(ptr1) == train_num_.tr_num) {  
            
            while(ptr1 != NULL) {

                ptr1 = strtok(NULL, ",");
                fare_arr[i][j] = atoi(ptr1);
                j++;
                if(j == 4) {
                    i = i + 1;
                    j = 0;
                }
                if(i == 3) {
                    goto leave;
                }
            }
            
        }
    }
leave: ;
    int co;

    if(strcmp(train_rtn.class, "Sleeper") == 0) {
        co = 0;
    }
    else if(strcmp(train_rtn.class, "AC-III") == 0) {
        co = 1;
    }
    else if(strcmp(train_rtn.class, "AC-II") == 0) {
        co = 2;
    }
    else if(strcmp(train_rtn.class, "AC-I") == 0) {
        co = 3;
    }

    fare = (fare_arr[train_num_.sta - 1][co]);

    int fin_fare = ps.pas_num * fare;

    // Calling function to update the fare details of a train in its file
    updateFare(train_num_.tr_num, train_rtn.dateofjourney, train_rtn.class, fin_fare);




    // Generating and saving ticket or user use
    char f_name[30] = "tickets/";
    char *c_pnr = (char *)malloc(50 * sizeof(char));
    char *extension = ".txt";
    int count = strlen(f_name);

    sprintf(c_pnr, "%d", pnr);
    
    for (int i = 0; c_pnr[i] != '\0'; i++)
    {
        f_name[count] = c_pnr[i];
        count++;
    }
    for (int i = 0; extension[i] != '\0'; i++)
    {
        f_name[count] = extension[i];
        count++;
    }
    f_name[count] = '\0';


    FILE *file;
    file = fopen(f_name, "wb");
    if(file == NULL) {
        printf("\nSomething went wrong...\n");
        startBooking();
    }

    fputs("\n", file);
    fputs("\tHAPPY JOURNEY                  TICKET-SLIP                  HAPPY JOURNEY\t\n", file);
    fputs("\t--------------------------------- ^_^ ----------------------------------\t\n\n", file);

    fputs("\t|BOARDING AT|                ===========>>>>>>                |TO|      \t\n", file);
    fprintf(file,
          "\t %s                                                      %s\t\n", train_num_.sta_source, train_num_.sta_destination);
    fprintf(file,
          "\t %s Hrs                                                   %s Hrs\t\n", train_num_.time_source, train_num_.time_destination);
    fprintf(file,
          "\t %s                                                   %s\t\n", train_rtn.dateofjourney, train_rtn.dateofjourney);

    fputs("\t--------------------------------------------------------------------------\t\n\n", file);

    fputs("\t|PNR|                   |Train No./ Train Name|                 |CLASS|   \t\n", file);
    fprintf(file,
         " \t%d                %d/%s                   %s\t\n", pnr, train_num_.tr_num, train_num_.tr_name, train_rtn.class);
    
    fputs("\t--------------------------------------------------------------------------\t\n\n", file);
    fputs("\t-------------------------------Passenger Details--------------------------\t\n", file);
    fputs("\t|Passenger Name|        |Age|    |Gender|        |Coach|    |Seat Number|\t\n", file);

    for (int i = 0; i < ps.pas_num; i++)
    {
    fprintf(file, 
          "\t %s                       %d        %c             %s            %d     \t\n", ps.p_name[i], ps.p_age[i], ps.p_gender[i], train_rtn.coach, train_rtn.seat_num[i]);
    }

    fputs("\t--------------------------------------------------------------------------\t\n\n", file);
    fputs("\t----------------------------------FARE DETAILS----------------------------\t\n", file);

    fprintf(file,
          "\t|FARE|  ---->   %d INR\t\n", fin_fare);
    fprintf(file,
          "\t|Date/Time of Booking|  ---->   %d-%d-%d\t%d:%d:%d Hrs\t\n", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min, tm.tm_sec);


    fclose(file);



    // Saving a copy for admin access and record
    char f_name_2[50] = "admin/t_history/";
    int count2 = strlen(f_name_2);
    char *etion = ".txt";

    sprintf(c_pnr, "%d", pnr);
    
    for (int i = 0; c_pnr[i] != '\0'; i++)
    {
        f_name_2[count2] = c_pnr[i];
        count2++;
    }
    for (int i = 0; etion[i] != '\0'; i++)
    {
        f_name_2[count2] = etion[i];
        count2++;
    }
    f_name_2[count2] = '\0';

    FILE *file2;
    file2 = fopen(f_name_2, "wb");
    if(file2 == NULL) {
        printf("\nSomething went wrong...\n");
        startBooking();
    }


    fputs("\n", file2);
    fputs("\tHAPPY JOURNEY                  TICKET-SLIP                  HAPPY JOURNEY\t\n", file2);
    fputs("\t--------------------------------- ^_^ ----------------------------------\t\n\n", file2);

    fputs("\t|BOARDING AT|                ===========>>>>>>                |TO|      \t\n", file2);
    fprintf(file2,
          "\t %s                                                      %s\t\n", train_num_.sta_source, train_num_.sta_destination);
    fprintf(file2,
          "\t %s Hrs                                                   %s Hrs\t\n", train_num_.time_source, train_num_.time_destination);
    fprintf(file2,
          "\t %s                                                   %s\t\n", train_rtn.dateofjourney, train_rtn.dateofjourney);

    fputs("\t--------------------------------------------------------------------------\t\n\n", file2);

    fputs("\t|PNR|                   |Train No./ Train Name|                 |CLASS|   \t\n", file2);
    fprintf(file2,
         " \t%d                %d/%s                   %s\t\n", pnr, train_num_.tr_num, train_num_.tr_name, train_rtn.class);
    
    fputs("\t--------------------------------------------------------------------------\t\n\n", file2);
    fputs("\t-------------------------------Passenger Details--------------------------\t\n", file2);
    fputs("\t|Passenger Name|        |Age|    |Gender|        |Coach|    |Seat Number|\t\n", file2);

    for (int i = 0; i < ps.pas_num; i++)
    {
    fprintf(file2, 
          "\t %s                       %d        %c             %s            %d     \t\n", ps.p_name[i], ps.p_age[i], ps.p_gender[i], train_rtn.coach, train_rtn.seat_num[i]);
    }

    fputs("\t--------------------------------------------------------------------------\t\n\n", file2);
    fputs("\t----------------------------------FARE DETAILS----------------------------\t\n", file2);

    fprintf(file2,
          "\t|FARE|  ---->   %d INR\t\n", fin_fare);
    fprintf(file2,
          "\t|Date/Time of Booking|  ---->   %d-%d-%d\t%d:%d:%d Hrs\t\n", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

    fclose(file2);


    free(c_pnr);

    return 1;

}


// Function to book the ticket
Passenger_Details bookTicket() {
    

    for (int i = 0; i < ps.pas_num; i++)
    {
        // name_try:
        printf("\nPassenger %d Name: ", i+1);
        scanf("%s", ps.p_name[i]);
        while(getchar() != '\n');
        // if(strcmp(ps[i].p_name, " ") == 0) goto name_try;

        age_try: ;
        printf("\nPassenger %d Age: ", i+1);
        scanf("%d%*c", &ps.p_age[i]);
        if(ps.p_age[i] == 0) goto age_try;

        gender_try: ;
        printf("\nPassenger %d Gender [M/F]: ", i+1);
        scanf("%c", &ps.p_gender[i]);
        while(getchar() != '\n');
        ps.p_gender[i] = toupper(ps.p_gender[i]);

        if(ps.p_gender[i] != 'M' && ps.p_gender[i] != 'F') {
            printf("\nEnter Valid Character....Try Again...\n");
            goto gender_try;
        }
        
        printf("\n");
    }

    ps.check = 1;
    return ps;
    
}

// Function to show the number of seats available in the train selected
int seatAvailability(int sta, int tr_num, StoD train_details_) {



    const char *SEAT_DETAILS_OUT = "%s, %s, %d, %d, %d, %s, %d, %d, %d, %s, %d, %d, %d, %s, %d, %d, %d\n";
    const char *SEAT_DETAILS_IN = "%[^,], %[^,], %d, %d, %d, %[^,], %d, %d, %d, %[^,], %d, %d, %d, %[^,], %d, %d, %d\n";

    const char *SEAT_DETAILS_OUT_s = "%s ||%s: %d||  ||%s: %d||  ||%s: %d||  ||%s: %d||\n\n";

    Seat_Count tt[3][4];
    Train_Date dt[3];
    train_file tf;
    int train_index_in_file;
    int train_number;

    FILE *fptr;
    fptr = fopen("t_files/trains_files.bin", "rb");

    if(fptr == NULL) {
        printf("\nSomething went wrong1...\n");
    }
    rewind(fptr);

    FILE *file;

    for (int i = 0; i < 14; i++)
    {

        fscanf(fptr, "%d, %d\n", &tf.number, &tf.t_index);

        if(tf.number == tr_num) {
            train_index_in_file = tf.t_index;
            train_number = tf.number;

            char f__name[60] = "t_files/Train_";
            char *lastPart = "_seats.bin";
            char ch_num[5];
            int cu = strlen(f__name);
            
            sprintf(ch_num, "%d", train_index_in_file);
    
            for (int i = 0; ch_num[i] != '\0'; i++)
            {
                f__name[cu] = ch_num[i];
                cu++;
            }
            for (int i = 0; lastPart[i] != '\0'; i++)
            {
                f__name[cu] = lastPart[i];
                cu++;
            }
            f__name[cu] = '\0';
    
            file = fopen(f__name, "rb");    

            break;
        }
    }
    fclose(fptr);
    
    if(file == NULL) {
        printf("\nSomething went wrong...\n");
    }

     


    char p_line[200];

    rewind(file);     

   for (int i = 0; i < 3; i++)
   {

        fscanf(file, SEAT_DETAILS_IN, &dt[i].t_date, &tt[i][0].t_coach, &tt[i][0].o_i, &tt[i][0].o_d, &tt[i][0].i_d, &tt[i][1].t_coach, &tt[i][1].o_i, &tt[i][1].o_d, &tt[i][1].i_d, &tt[i][2].t_coach, &tt[i][2].o_i, &tt[i][2].o_d, &tt[i][2].i_d, &tt[i][3].t_coach, &tt[i][3].o_i, &tt[i][3].o_d, &tt[i][3].i_d);

   }

    /*

        // for (int i = 0; i < 3; i++)
        // {
        //     printf("PRESS---->%d  FOR  DATE: ", i+1);
        //     printf(SEAT_DETAILS_OUT, dt[i].t_date, tt[i][0].t_coach, tt[i][0].o_i, tt[i][0].o_d, tt[i][0].i_d, tt[i][1].t_coach, tt[i][1].o_i, tt[i][1].o_d, tt[i][1].i_d, tt[i][2].t_coach, tt[i][2].o_i, tt[i][2].o_d, tt[i][2].i_d, tt[i][3].t_coach, tt[i][3].o_i, tt[i][3].o_d, tt[i][3].i_d);
        // }
    
    */


    // Getting the fare for the respective class of selected train
    int fare[3];
    int fare_arr[3][4];

    FILE *fptr___;
    fptr___ = fopen("admin/fare_details/fares.bin", "rb");
    if(fptr == NULL) {
        printf("\nSomething went Wrong...\n");
        exit(EXIT_FAILURE);
    }

    rewind(fptr);

    int l = 0;
    char line__[200];
    while(fgets(line__, sizeof(line__)/sizeof(line__[0]), fptr)) {

        if(strchr(line__, '\n') == NULL) {
           continue;
        }

        int v = 0;
        char *ptr1 = strtok(line__, ",");
        if (atoi(ptr1) == train_num_.tr_num) {  
            
            while(ptr1 != NULL) {

                ptr1 = strtok(NULL, ",");
                fare_arr[l][v] = atoi(ptr1);
                v++;
                if(v == 4) {
                    l = l + 1;
                    v = 0;
                }
                if(l == 3) {
                    goto leave;
                }
            }
            
        }
    }
leave: ;

    fare[0] = (fare_arr[train_num_.sta - 1][0]);
    fare[1] = (fare_arr[train_num_.sta - 1][1]);
    fare[2] = (fare_arr[train_num_.sta - 1][2]);
    fare[3] = (fare_arr[train_num_.sta - 1][3]);

    fclose(fptr___);


    printf("\n\n\n");
    printf("----------------------------------------SEAT AVAILABILITY--------------------------------------\n\n");
    int b_seat;
    for (int i = 0; i < 3; i++)
    {
        switch (sta)
        {
        case 1:
            printf("PRESS---->%d  FOR  DATE: ", i+1);

            printf(SEAT_DETAILS_OUT_s, dt[i].t_date, tt[i][0].t_coach, tt[i][0].o_i, tt[i][1].t_coach, tt[i][1].o_i, tt[i][2].t_coach, tt[i][2].o_i, tt[i][3].t_coach, tt[i][3].o_i);
            break;

        case 2:
            printf("PRESS---->%d  FOR  DATE: ", i+1);

            printf(SEAT_DETAILS_OUT_s, dt[i].t_date, tt[i][0].t_coach, tt[i][0].o_d, tt[i][1].t_coach, tt[i][1].o_d, tt[i][2].t_coach, tt[i][2].o_d, tt[i][3].t_coach, tt[i][3].o_d);
            break;
            
        case 3:
            printf("PRESS---->%d  FOR  DATE: ", i+1);

            printf(SEAT_DETAILS_OUT_s, dt[i].t_date, tt[i][0].t_coach, tt[i][0].i_d, tt[i][1].t_coach, tt[i][1].i_d, tt[i][2].t_coach, tt[i][2].i_d, tt[i][3].t_coach, tt[i][3].i_d);
            break;
    
        }
        
    }

    fclose(file);


seat_try: ;
    int c;
    printf("\nEnter your choice: ");
    scanf("%d", &c);
    if(c == 1 || c == 2 || c == 3)
    {
        if(c == 1) {
            b_seat = c-1;
            strcpy(train_rtn.dateofjourney, dt[c-1].t_date);
        }
        else if(c == 2) {
            b_seat = c-1;
            strcpy(train_rtn.dateofjourney, dt[c-1].t_date);
        }
        else if(c == 3) {
            b_seat = c-1;
            strcpy(train_rtn.dateofjourney, dt[c-1].t_date);
            
        }

    seat_try_2: ;
        int l;
        printf("\nFor SLEEPER (Fare: %d INR)  PRESS--->1\nFor AC-III (Fare: %d INR)  PRESS--->2\nFor AC-II (Fare: %d INR)  PRESS--->3\nFor AC-I (Fare: %d INR)  PRESS--->4\n\n", fare[0], fare[1], fare[2], fare[3]);

        printf("\nEnter your Choice: ");
        scanf("%d%*c", &l);

    
        switch (l)
        {
                try__: ;
                char ch;
                printf("\nDo you want to confirm booking [y/n]: ");
                scanf("%c%*c", &ch);

                if(ch == 'y' || ch == 'Y') {
                    printf("\n---------------------------Enter Passenger Details-------------------------\n");
                    printf("\nDATE OF JOURNEY: %s\n", train_rtn.dateofjourney);
                    goto proceed;
                }
                else if(ch == 'n' || ch == 'N') {
                    return -1;
                    // goto end_this;
                }
                else {
                    goto try__;
                }

                proceed: ;


            int p_num;

        case 1:

            printf("\nCLASS: Sleeper\n");

        book_try: ;
            printf("\nEnter number of Passengers: ");
            scanf("%d", &p_num);


            if(p_num > 3) {
                printf("\nMaximum number of passengers is 3\n");
                printf("\nTry Again...\n");
                goto book_try;
            }

            ps.pas_num = p_num;

            strcpy(train_rtn.class, "Sleeper");
            strcpy(train_rtn.coach, "SL");

            switch (sta)
            {
                case 1:
                    if(tt[b_seat][0].o_i == 0) {
                        printf("\nSorry, no seats available in %s class. Please go for other class\n", train_rtn.class);
                        goto seat_try_2;
                    }

                    for (int i = 0; i < p_num; i++)
                    {
                        train_rtn.seat_num[i]  = tt[b_seat][0].o_i;
                        train.seat_num[i] = tt[b_seat][0].o_i;
                        tt[b_seat][0].o_i -= 1;
                        tt[b_seat][0].o_d -= 1;
                    }
                    
                    update_in_file(train_index_in_file, tt, dt);
                    return 1;
                    break;



                case 2:
                    if(tt[b_seat][0].o_d == 0) {
                        printf("\nSorry, no seats available in %s class. Please go for other class\n", train_rtn.class);
                        goto seat_try_2;
                    }

                    for (int i = 0; i < p_num; i++)
                    {
                        train_rtn.seat_num[i]  = tt[b_seat][0].o_d;
                        train.seat_num[i] = tt[b_seat][0].o_d;
                        tt[b_seat][0].o_i -= 1;
                        tt[b_seat][0].o_d -= 1;
                        tt[b_seat][0].i_d -= 1;
                    }

                    update_in_file(train_index_in_file, tt, dt);
                    return 1;
                    break;

                case 3:
                    if(tt[b_seat][0].i_d == 0) {
                        printf("\nSorry, no seats available in %s class. Please go for other class\n", train_rtn.class);
                        goto seat_try_2;
                    }

                    for (int i = 0; i < p_num; i++)
                    {
                        train_rtn.seat_num[i]  = tt[b_seat][0].i_d;
                        train.seat_num[i] = tt[b_seat][0].i_d;
                        tt[b_seat][0].o_d -= 1;
                        tt[b_seat][0].i_d -= 1;
                    }
   
                    update_in_file(train_index_in_file, tt, dt);
                    return 1;
                    break;
            }

            break;

        case 2:
            printf("\nCLASS: AC-III\n");

        book_try_2: ;
            printf("\nEnter number of Passengers: ");
            scanf("%d", &p_num);


            if(p_num > 3) {
                printf("\nMaximum number of passengers is 3\n");
                printf("\nTry Again...\n");
                goto book_try_2;
            }

            ps.pas_num = p_num;

            strcpy(train_rtn.class, "AC-III");
            strcpy(train_rtn.coach, "AC-III");

            switch (sta)
            {
                case 1:
                    if(tt[b_seat][1].o_i == 0) {
                        printf("\nSorry, no seats available in %s class. Please go for other class\n", train_rtn.class);
                        goto seat_try_2;
                    }

                    for (int i = 0; i < p_num; i++)
                    {
                        train_rtn.seat_num[i]  = tt[b_seat][1].o_i;
                        train.seat_num[i] = tt[b_seat][1].o_i;
                        tt[b_seat][1].o_i -= 1;
                        tt[b_seat][1].o_d -= 1;
                    }
                    update_in_file(train_index_in_file, tt, dt);
                    return 1;
                    break;

                case 2:
                    if(tt[b_seat][1].o_d == 0) {
                        printf("\nSorry, no seats available in %s class. Please go for other class\n", train_rtn.class);
                        goto seat_try_2;
                    }

                    for (int i = 0; i < p_num; i++)
                    {
                        train_rtn.seat_num[i]  = tt[b_seat][1].o_d;
                        train.seat_num[i] = tt[b_seat][1].o_d;
                        tt[b_seat][1].o_i -= 1;
                        tt[b_seat][1].o_d -= 1;
                        tt[b_seat][1].i_d -= 1;
                    }
                    
                    
                    update_in_file(train_index_in_file, tt, dt);
                    return 1;
                    break;

                case 3:
                    if(tt[b_seat][1].i_d == 0) {
                        printf("\nSorry, no seats available in %s class. Please go for other class\n", train_rtn.class);
                        goto seat_try_2;
                    }

                    for (int i = 0; i < p_num; i++)
                    {
                        train_rtn.seat_num[i]  = tt[b_seat][1].i_d;
                        train.seat_num[i] = tt[b_seat][1].i_d;
                        tt[b_seat][1].o_d -= 1;
                        tt[b_seat][1].i_d -= 1; 
                    }
                    update_in_file(train_index_in_file, tt, dt);
                    return 1;
                    break;
            }
            break;

        case 3:
            printf("\nCLASS: AC-II\n");

        book_try_3: ;
            printf("\nEnter number of Passengers: ");
            scanf("%d", &p_num);


            if(p_num > 3) {
                printf("\nMaximum number of passengers is 3\n");
                printf("\nTry Again...\n");
                goto book_try_3;
            }

            ps.pas_num = p_num;

            strcpy(train_rtn.coach, "AC-II");
            strcpy(train_rtn.class, "AC-II");

            switch (sta)
            {
                case 1:
                    if(tt[b_seat][2].o_i == 0) {
                        printf("\nSorry, no seats available in %s class. Please go for other class\n", train_rtn.class);
                        goto seat_try_2;
                    }

                    for (int i = 0; i < p_num; i++)
                    {
                        train_rtn.seat_num[i]  = tt[b_seat][2].o_i;
                        train.seat_num[i] = tt[b_seat][2].o_i;
                        tt[b_seat][2].o_i -= 1;
                        tt[b_seat][2].o_d -= 1;
                    }
                    update_in_file(train_index_in_file, tt, dt);
                    return 1;
                    break;

                case 2:
                    if(tt[b_seat][2].o_d == 0) {
                        printf("\nSorry, no seats available in %s class. Please go for other class\n", train_rtn.class);
                        goto seat_try_2;
                    }
                    
                    for (int i = 0; i < p_num; i++)
                    {
                        train_rtn.seat_num[i]  = tt[b_seat][2].o_d;
                        train.seat_num[i] = tt[b_seat][2].o_d;
                        tt[b_seat][2].o_i -= 1;
                        tt[b_seat][2].o_d -= 1;
                        tt[b_seat][2].i_d -= 1;
                    }

                    update_in_file(train_index_in_file, tt, dt);
                    return 1;
                    break;

                case 3:
                    if(tt[b_seat][2].i_d == 0) {
                        printf("\nSorry, no seats available in %s class. Please go for other class\n", train_rtn.class);
                        goto seat_try_2;
                    }

                    for (int i = 0; i < p_num; i++)
                    {
                        train_rtn.seat_num[i]  = tt[b_seat][2].i_d;
                        train.seat_num[i] = tt[b_seat][2].i_d;
                        tt[b_seat][2].o_d -= 1;
                        tt[b_seat][2].i_d -= 1;
                    }
                    update_in_file(train_index_in_file, tt, dt);
                    return 1;
                    break;
            }
            break;

        case 4:
            printf("\nCLASS: AC-I\n");

        book_try_4: ;
            printf("\nEnter number of Passengers: ");
            scanf("%d", &p_num);


            if(p_num > 3) {
                printf("\nMaximum number of passengers is 3\n");
                printf("\nTry Again...\n");
                goto book_try_4;
            }
            ps.pas_num = p_num;

            strcpy(train_rtn.class, "AC-I");
            strcpy(train_rtn.coach, "AC-I");

            switch (sta)
            {
                case 1:
                    if(tt[b_seat][3].o_i == 0) {
                        printf("\nSorry, no seats available in %s class. Please go for other class\n", train_rtn.class);
                        goto seat_try_2;
                    }

                    for (int i = 0; i < p_num; i++)
                    {
                        train_rtn.seat_num[i]  = tt[b_seat][3].o_i;
                        train.seat_num[i] = tt[b_seat][3].o_i;
                        tt[b_seat][3].o_i -= 1;
                        tt[b_seat][3].o_d -= 1;
                    }

                    update_in_file(train_index_in_file, tt, dt);
                    return 1;
                    break;

                case 2:
                    if(tt[b_seat][3].o_d == 0) {
                        printf("\nSorry, no seats available in %s class. Please go for other class\n", train_rtn.class);
                        goto seat_try_2;
                    }

                    for (int i = 0; i < p_num; i++)
                    {
                        train_rtn.seat_num[i]  = tt[b_seat][3].o_d;
                        train.seat_num[i] = tt[b_seat][3].o_d;
                        tt[b_seat][3].o_i -= 1;
                        tt[b_seat][3].o_d -= 1;
                        tt[b_seat][3].i_d -= 1;
                    }

                    update_in_file(train_index_in_file, tt, dt);
                    return 1;
                    break;

                case 3:
                if(tt[b_seat][3].i_d == 0) {
                        printf("\nSorry, no seats available in %s class. Please go for other class\n", train_rtn.class);
                        goto seat_try_2;
                    }

                    for (int i = 0; i < p_num; i++)
                    {
                        train_rtn.seat_num[i]  = tt[b_seat][3].i_d;
                        train.seat_num[i] = tt[b_seat][3].i_d;
                        tt[b_seat][3].o_d -= 1;
                        tt[b_seat][3].i_d -= 1;
                    }
                    update_in_file(train_index_in_file, tt, dt);
                    return 1;
                    break;
            }
            break;
        
        default:
            printf("\nWrong Choice....Try Again...\n");
            goto seat_try_2;
        }
    }

    else 
    {
        printf("\nWrong Choice....Try Again...\n");
        goto seat_try;        
    }

    // end_this:

}

// Function to add user details to the User Credential File
void dumpUserCredFile(char *uId, long long uKey)
{

    FILE *fptr;
    fptr = fopen("t_files/userCredentials.bin", "ab+");


    fputs("\n", fptr);
    fputs(uId, fptr);
    // for(int i = 0; i < (40 - strlen(uId)); i++)
    // {
    //     fputs(" ", fptr);
    // }

    fputs(" ", fptr);
    fprintf(fptr, "%lld", uKey);

    fclose(fptr);
}

// Function for New User registarion
void nLogPortal()
{
    char *temp_id = (char *)malloc(31 * sizeof(char));

    printf("Create User ID: ");
    scanf("%s", temp_id);


    char* key = passTake(); 
    long long f_key = hash(key);
    dumpUserCredFile(temp_id, f_key);
    free(temp_id);
    systemStart();
}

// Function for User login portal
int uLogPortal()
{
    // printf("Hello User!\n");
try: ;
    char *uid = (char *)malloc(31 * sizeof(char));
    char *ukey = (char *)malloc(31 * sizeof(char));
    
    printf("Enter User ID: ");
    scanf("%s", uid);
    
    long long chk_key = chk_ID_get_key(uid, 1);

    if(chk_key == (long long)-1) {
        printf("\n------User Not Found!!!------\n\nTo GOTO PREVIOUS MENU--->Press 0\nTo Try Again--->Press 1");

        int c;
        printf("\nYour Choice: ");
        scanf("%d", &c);
        
        if(c == 0) {
            free(uid);
            free(ukey);
            systemStart();
        } 
            
        else {
            free(uid);
            free(ukey);
            goto try;
        }
    }
passTry: ;
    printf("Enter Password: ");
    scanf("%s", ukey);


    int p = checkPass(ukey, chk_key);
    if(p == -1) {
        printf("\n------Incorrect Password!!!------\n\nTo Exit--->Press 0\nTo Re-Enter Password--->Press 1");

        int c;
        printf("\nYour Choice: ");
        scanf("%d", &c);
        if(c == 0) {
            systemStart();
        } 
            
        else {
            goto passTry;
        }
    }
    
    if(p == 1) {
        printf("\n______WELCOME...<3______\n\n");
        printf("\nHello %s!!\n\n", uid);
        loginStatus = true;

        if (loginStatus == true) {

    longback: ;

            // Passenger_Details ps;

            Return_Station_names s_return = startBooking();

            showTrains(s_return.s_station, s_return.d_station);

            int chk = seatAvailability(train_num_.sta,train_num_.tr_num,train_num_);

            if(chk == -1) {
                goto longback;
            }
            if(chk == 1) {
                
            bookTicket();

            if(ps.check == 1) {

                    int p_chk = printTicket();

                    if(p_chk == 1) {

                        printf("\n\n-------------------Congratulations!! Your Ticket Is Booked---------------------\n");
                        printf("\nGO and Print Your Ticket from Folder \"tickets\"\n\n");

                        goto longback;
                    }
            }

            }
        }
    }

    free(uid);
    free(ukey);

}

// Function for Admin login
void aLogPortal()
{
    // printf("Hello Admin!\n");

    try: ;
    char *uid = (char *)malloc(31 * sizeof(char));
    char *ukey = (char *)malloc(31 * sizeof(char));
    
    printf("Enter Admin ID: ");
    scanf("%s", uid);
    
    long long chk_key = chk_ID_get_key(uid, 2);

    if(chk_key == (long long)-1) {
        printf("\n------Admin ID Not Found!!!------\n\nTo GOTO PREVIOUS MENU--->Press 0\nTo Try Again--->Press 1");

        int c;
        printf("\nYour Choice: ");
        scanf("%d", &c);
        
        if(c == 0) {
            free(uid);
            free(ukey);
            systemStart();
        } 
            
        else {
            free(uid);
            free(ukey);
            goto try;
        }
    }
passTry: ;
    printf("Enter Password: ");
    scanf("%s", ukey);


    int p = checkPass(ukey, chk_key);
    if(p == -1) {
        printf("\n------Incorrect Admin Password!!!------\n\nTo Exit--->Press 0\nTo Re-Enter Password--->Press 1");

        int c;
        printf("\nYour Choice: ");
        scanf("%d", &c);
        if(c == 0) {
            adminLoginStatus = false;
            systemStart();
        } 
            
        else {
            goto passTry;
        }
    }
    
    if(p == 1) {
        printf("\n______WELCOME ADMIN...<3______\n\n");
        adminLoginStatus = true;

        if(adminLoginStatus == true) {
        
        _try_: ;
            int c;
            printf("\n\nTO CHECK REVENUE ----> PRESS 1\nTO VIEW SEAT HISTORY ----> PRESS 2\nTO VIEW CURRENT SEAT STATUS ----> PRESS 3\nTO EXIT ----> PRESS 9\n");

            printf("\n\nEnter your Choice: ");
            scanf("%d", &c);

            if(c == 9) {
                
                adminLoginStatus = false;
                systemStart();

            }

            else if(c == 1) {

                int cz = revenueCheck();

                if(cz == -1) {
                    adminLoginStatus = false;
                    systemStart();
                }

                else if(cz == 1) {
                    goto _try_;
                }

            }

            else if(c == 2) {
                int sh = seatHistory();

                if(sh == -1) {
                    adminLoginStatus = false;
                    systemStart();
                }

                else if(sh == 1) {
                    goto _try_;
                }
            }

            else if(c == 3) {
                int ss = seatStatus();

                if(ss == -1) {
                    adminLoginStatus = false;
                    systemStart();
                }

                else if(ss == 1) {
                    goto _try_;
                }
            }

            else {
                printf("\nWrong Choice..");
                goto _try_;
            }

        }

    }

}

// Function to check PNR
int pnrCheck(void) {

    try_pnr: ;
    printf("\n------------------------------PNR Check---------------------------------\n");
    
    int pnr;
    printf("\nEnter the 10 digit PNR: ");
    scanf("%d", &pnr);

    // Checking if entered pnr is correct or not
    FILE * fptr;
    fptr = fopen("t_files/PNR_COUNT.bin", "rb");
    if(fptr == NULL) {
        printf("\nSomething went wrong...\n");
        systemStart();
    }

    int pnr_check;
    int c = 0;
    while (fscanf(fptr, "%d", &pnr_check) != EOF) {
        
        if(pnr == pnr_check) {
            c++;
            break;
        }

    }

    if(c == 0) {

        printf("\nWrong PNR..\n");
        printf("\nTo Try Again ----> PRESS 1\nTO EXIT -----> PRESS 2\n");
        
        again__: ;
        int ch;
        printf("Enter your choice: ");
        scanf("%d", &ch);

        if(ch == 1) {
            fclose(fptr);
            goto try_pnr;
        }
        if(ch == 2) {
            fclose(fptr); // make cookies for this task // task accomplished //

            if(loginStatus == true) {
                return -1;
            }

            else {
                systemStart();
            }
        }
        else {
            printf("\nWrong Choice...Try Again...\n");
            goto again__;
        }

    }

    if(c > 0) {

        fclose(fptr);

        char f_name[60] = "admin/t_history/";
        char c_pnr[15] = {0};
        char *lastPart = ".txt";
        int count = strlen(f_name);

        sprintf(c_pnr, "%d", pnr);

        for (int i = 0; c_pnr[i] != '\0'; i++)
        {
            f_name[count] = c_pnr[i];
            count++;
        }
        for (int i = 0; lastPart[i] != '\0'; i++)
        {
            f_name[count] = lastPart[i];
            count++;
        }
        f_name[count] = '\0';

        FILE *file;
        file = fopen(f_name, "r");

        char line[300];
        while (fgets(line, sizeof(line)/sizeof(line[0]), file)) {
            
            printf("%s\n", line);

        }

        fclose(file);
        
    }
    
    return 1;

}

// Function for admin to check the revenue from train
int revenueCheck(void) {


    printf("\n----------------------------Trains-------------------------------\n");

    FILE *fptr;
    fptr = fopen("t_files/train_names.bin", "rb");
    if(fptr == NULL) {
        printf("\nSomething went wrong...\n");
        return -1;
    }
    rewind(fptr);

    char _line_[300] = {0};
    while(fgets(_line_, sizeof(_line_)/sizeof(_line_[0]), fptr)) {

        printf("%s\n", _line_);

    }

    printf("\n-----------------------------------------------------------------\n");
num_try: ;
    int t_num_;
    printf("\nEnter Train number to view its revenue details: ");
    scanf("%d", &t_num_);

    rewind(fptr);
    
    int num_chk, cnt = 0;
    char tr_name_[60] = {0};

    while(fscanf(fptr, "%d, %s\n", &num_chk, tr_name_) != EOF) {


        if(num_chk == t_num_) {

            char f_name[60] = "admin/fare_history/";
            char c_num[7] = {0};
            char *lastPart = ".bin";
            int count = strlen(f_name);

            sprintf(c_num, "%d", t_num_);

            for (int i = 0; c_num[i] != '\0'; i++)
            {
                f_name[count] = c_num[i];
                count++;
            }

            for (int i = 0; lastPart[i] != '\0'; i++)
            {
                f_name[count] = lastPart[i];
                count++;
            }
            f_name[count] = '\0';


            FILE *file;
            file = fopen(f_name, "rb");
            if(file == NULL) {
                printf("\nSomething went wrong...\n");
                    return -1;
            }
            
            printf("\nRevneue Details:\n\n");
            printf("Train number: %d\n", num_chk);
            printf("Train Name: %s\n\n", tr_name_);

            char line___[300] = {0};
            while(fgets(line___, sizeof(line___)/sizeof(line___[0]), file)) {
                printf("%s\n", line___);
            }

            cnt++;
            fclose(file);
            return 1;
            break;

        }

    }

    if(cnt == 0) {
        printf("\nWrong Train number entered..\n");
        goto num_try;
    }

    fclose(fptr);

}

// Function to view the seatHistory
int seatHistory(void) {

    if(adminLoginStatus == true) {

        printf("\n----------------------------Trains-------------------------------\n");

        FILE *fptr;
        fptr = fopen("t_files/train_names.bin", "rb");
        if(fptr == NULL) {
            printf("\nSomething went wrong...\n");
            return -1;
        }
        rewind(fptr);

        char _line_[300] = {0};
        while(fgets(_line_, sizeof(_line_)/sizeof(_line_[0]), fptr)) {

            printf("%s\n", _line_);

        }

        printf("\n-----------------------------------------------------------------\n");
    num_try: ;

        int t_num_;
        printf("\nEnter Train number to view its Seat Booking History: ");
        scanf("%d", &t_num_);

        rewind(fptr);
    
        int num_chk, cnt = 0;
        char tr_name_[60] = {0};
        int index = 1;

        while(fscanf(fptr, "%d, %s\n", &num_chk, tr_name_) != EOF) {


            if(num_chk == t_num_) {

                char f_name[60] = "admin/seats_history/_his_Train_";
                char c_num[7] = {0};
                char *lastPart = "_seats.bin";
                int count = strlen(f_name);

                sprintf(c_num, "%d", index);

                for (int i = 0; c_num[i] != '\0'; i++)
                {
                    f_name[count] = c_num[i];
                    count++;
                }

                for (int i = 0; lastPart[i] != '\0'; i++)
                {
                    f_name[count] = lastPart[i];
                    count++;
                }
                f_name[count] = '\0';


                FILE *file;
                file = fopen(f_name, "rb");
                if(file == NULL) {
                    printf("\nSomething went wrong...\n");
                        return -1;
                }
                
                printf("\nSeat Booking History:\n\n");
                printf("Train number: %d\n", num_chk);
                printf("Train Name: %s\n\n", tr_name_);

                char line___[300] = {0};
                while(fgets(line___, sizeof(line___)/sizeof(line___[0]), file)) {
                    printf("%s\n", line___);
                }

                cnt++;
                fclose(file);
                return 1;
                break;

            }

            index++;

        }

        if(cnt == 0) {
            printf("\nWrong Train number entered..\n");
            goto num_try;
        }

        fclose(fptr);

    }

}

// Function to view the current the seat status of a train
int seatStatus(void) {

    if(adminLoginStatus == true) {

        printf("\n----------------------------Trains-------------------------------\n");

        FILE *fptr;
        fptr = fopen("t_files/train_names.bin", "rb");
        if(fptr == NULL) {
            printf("\nSomething went wrong...\n");
            return -1;
        }
        rewind(fptr);

        char _line_[300] = {0};
        while(fgets(_line_, sizeof(_line_)/sizeof(_line_[0]), fptr)) {

            printf("%s\n", _line_);

        }

        printf("\n-----------------------------------------------------------------\n");
    num_try: ;

        int t_num_;
        printf("\nEnter Train number to view its Current Seats Avaliable: ");
        scanf("%d", &t_num_);

        rewind(fptr);
    
        int num_chk, cnt = 0;
        char tr_name_[60] = {0};
        int index = 1;

        while(fscanf(fptr, "%d, %s\n", &num_chk, tr_name_) != EOF) {


            if(num_chk == t_num_) {

                char f_name[60] = "t_files/Train_";
                char c_num[7] = {0};
                char *lastPart = "_seats.bin";
                int count = strlen(f_name);

                sprintf(c_num, "%d", index);

                for (int i = 0; c_num[i] != '\0'; i++)
                {
                    f_name[count] = c_num[i];
                    count++;
                }

                for (int i = 0; lastPart[i] != '\0'; i++)
                {
                    f_name[count] = lastPart[i];
                    count++;
                }
                f_name[count] = '\0';


                FILE *file;
                file = fopen(f_name, "rb");
                if(file == NULL) {
                    printf("\nSomething went wrong...\n");
                        return -1;
                }
                
                printf("\nCurrent Seat Available:\n\n");
                printf("Train number: %d\n", num_chk);
                printf("Train Name: %s\n\n", tr_name_);

                char line___[300] = {0};
                while(fgets(line___, sizeof(line___)/sizeof(line___[0]), file)) {
                    printf("%s\n", line___);
                }

                cnt++;
                fclose(file);
                return 1;
                break;

            }

            index++;

        }

        if(cnt == 0) {
            printf("\nWrong Train number entered..\n");
            goto num_try;
        }

        fclose(fptr);

    }

}

// Over--------------------------------------------------------------------------------------- //