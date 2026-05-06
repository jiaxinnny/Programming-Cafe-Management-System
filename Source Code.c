#include <stdio.h>
#include <string.h>

#define Max_Session_Code 10
#define Max_User_Code 10
#define Max_String 130

// Structure
struct sessions {
    char session_code[Max_Session_Code];
    char title[Max_String];
    char day[Max_String];
    char start_time[Max_String];
    char location[Max_String];
    char tutor_code[Max_User_Code];
};

struct enrol_sessions {
    char user_code[Max_User_Code];
    char user_name[Max_String];
    char user_role[Max_String];
    char session_code[Max_Session_Code];
};

struct users {
    char session_title[Max_String];
    char user_role[Max_String];
    char user_code[Max_User_Code];
    char user_name[Max_String];
    char email[Max_String];
    char password[Max_String];
};


// menu function
int main_menu();

int main();

int login_page();

int Session_menu();

//Admin page function
int Admin_menu();

// Def: struct users in the () is to bring over the users' information from login page
void Admin(struct users);

int add_User(struct users session_user);

void Print_View_User_Row(char session_title[Max_String], char user_role[Max_String], char user_code[Max_User_Code],
                         char user_name[Max_String], char email[Max_String], char password[Max_String]);

int View_all_User(struct users session_user);

int add_New_Session(struct users session_user);

int Enroll_Student_to_Session(struct users session_user);

//Tutor page function
int Tutor_menu();

void Tutor(struct users);

int View_Tutor_Session(struct users session_user);

int View_Students_in_Session(struct users session_user);

//Student page function
int Student_menu();

void Student(struct users);

int View_Student_Session(struct users session_user);

int Enroll_into_Session(struct users session_user);

//File function
void PrintTableRow(char session_code[Max_Session_Code], char title[Max_String], char day[Max_String],
                   char start_time[Max_String], char location[Max_String], char tutor_code[Max_User_Code]);

int create_file();

int user_code_parser(char *user_code);


// menu //
int main_menu() {
    int choice;

    printf("\n ---------------------------------------------------- \n");
    printf(" |      APU Programming Cafe Management System      |\n");
    printf(" |--------------------------------------------------| \n");
    printf(" |                 >>  HOME PAGE  <<                |\n");
    printf(" ---------------------------------------------------- \n");
    printf("1. Login \n");
    printf("2. View Available Programming Session\n");
    printf("3. Exit the Programming Cafe Session \n");
    printf("Enter your choice: ");

    scanf("%d", &choice);

    return choice;
}


int main() {
    create_file();

    int flag = 1;
    int choice, authenticated;

    //program start
    while (flag) {
        choice = main_menu();
        switch (choice) {
            case 1:
                authenticated = login_page();
                if (authenticated == 0) {
                    flag = 0;
                }
                break;
            case 2:
                Session_menu();
                break;
            case 3:
                flag = 0;
                printf(" ~~~ Thanks for using APU Programming Cafe Management System ~~~ ");
                break;
            default:
                printf(" !!! Invalid option, Please try again !!! \n");
                printf("--------------------------------------------\n");
        }
    }
    return 0;
}


// login page (3 attempts to log in, otherwise end the program)
int login_page() {

    printf("\n ---------------------------------------------------- \n");
    printf(" |      APU Programming Cafe Management System      |\n");
    printf(" |--------------------------------------------------|\n");
    printf(" |                >>  LOGIN PAGE  <<                |\n");
    printf(" ---------------------------------------------------- \n");

    FILE *users_file = fopen("user_profile.txt", "r");

    int authenticated = 0;
    int login_attempts = 0;
    const int max_login_attempts = 3;

    while (login_attempts < max_login_attempts) {
        //Def: rewind is to let the pointer move back to the top when 1st fscan are done and the pointer is at the last//
        rewind(users_file);
        struct users user;

        char user_code[Max_User_Code];
        char password[Max_String];

        printf("Please Enter Your User Code: ");
        scanf("%s", user_code);
        printf("Password: ");
        scanf("%s", password);

        int response = user_code_parser(user_code);

        if (response) {
            //read file line by line
            while (fscanf(users_file, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];\n", user.session_title,
                          user.user_role, user.user_code, user.user_name, user.email, user.password) != EOF) {

                //Def: strcmp() function is used to compare the string arguments//
                // EXP: user file inside de user_code string compare with user_code that entered by user. //
                if (strcmp(user.user_code, user_code) == 0 && strcmp(user.password, password) == 0) {
                    authenticated = 1;
                    break;
                }
            }
        }

        if (authenticated) {
            fclose(users_file);
            printf("Login successful\n");
            if (strcmp(user.user_role, "admin") == 0) {
                Admin(user);
            } else if (strcmp(user.user_role, "tutor") == 0) {
                Tutor(user);
            } else if (strcmp(user.user_role, "student") == 0) {
                Student(user);
            }
            break;
        } else {
            login_attempts = login_attempts + 1;   //i++ concept//
            printf("Incorrect Password or User Code, Please try again. (%d attempts left) \n\n",
                   (3 - login_attempts));
        }
    }

    if (login_attempts == max_login_attempts) {
        fclose(users_file);
        printf(" Too many attempts, Try again later. \n");
    }

    return authenticated;
}


// 1. Admin page (to register users, view all users, add session, enroll student)//
int Admin_menu() {
    int choice;

    printf("\n ---------------------------------------------------- \n");
    printf(" |      APU Programming Cafe Management System      |\n");
    printf(" ---------------------------------------------------- \n");
    printf(" |                >>  ADMIN PAGE  <<                |\n");
    printf(" ---------------------------------------------------- \n");
    printf("1. Add User \n");
    printf("2. View all User \n");
    printf("3. Add New Programming Session \n");
    printf("4. Enroll Student to a Session\n");
    printf("5. Log Out \n");
    printf("Enter your choice: ");

    scanf("%d", &choice);

    return (choice);
}

void Admin(struct users session_user) {
    int flag = 1;
    int choice;

    while (flag) {
        choice = Admin_menu();
        switch (choice) {
            case 1:
                add_User(session_user);
                break;
            case 2:
                View_all_User(session_user);
                break;
            case 3:
                add_New_Session(session_user);
                break;
            case 4:
                Enroll_Student_to_Session(session_user);
                break;
            case 5:
                flag = 0;
                printf(" ~~ You are logged out. Thanks for using APU Programming Cafe Management System ~~ \n");
                break;
            default:
                printf(" !!! Invalid option, Please try again !!! \n");
                printf("--------------------------------------------\n");

        }
    }
}

//Admin Menu > 1. Add user
int add_User(struct users session_user) {
    struct users new_user;

    printf(" \n ---------------------------------------------------- \n");
    printf(" |                >>  ADMIN PAGE  <<                |\n");
    printf(" |                ***  ADD USER  ***                |\n");
    printf(" ---------------------------------------------------- \n");

    while (1) {
        printf(" Enter New User's Role (student or tutor): ");
        scanf("%s", new_user.user_role);
        //compare string
        if (strcmp(new_user.user_role, "student") == 0 || strcmp(new_user.user_role, "tutor") == 0) {
            break;
        } else {
            printf(" ** Role doesn't exist, Please try again with 'Student' or 'Tutor' only. \n\n");
            printf("-----------------------------------------------------------------------------\n");

        }
    }

    while (1) {
        printf(" Enter New User Code [Student (TP) or Tutor (TT)] : ");
        scanf("%s", new_user.user_code);
        int response = user_code_parser(new_user.user_code);

        if (response == 1) {
            break;
        } else {
            printf(" Please try again. (Exp: TP123456 or TT123123) \n\n");
            printf("---------------------------------------------------\n");

        }
    }

    printf(" Enter New User's Name: ");
    scanf("%s", new_user.user_name);

    printf(" Enter New User's Email: ");
    scanf("%s", new_user.email);

    while (1) {
        printf(" Enter New User's Password (1 Alphabet and 6 character): ");
        scanf("%s", new_user.password);
        if (strlen(new_user.password) >= 7) {
            break;
        } else {
            printf("\n Please try again. \n");
            printf("-----------------------\n");
        }
    }

    FILE *users_file = fopen("user_profile.txt", "a");

    if (strcmp(new_user.user_role, "student") == 0) {
        fprintf(users_file, "NULL;%s;%s;%s;%s;%s;\n", new_user.user_role,
                new_user.user_code, new_user.user_name, new_user.email, new_user.password);
    } else {
        fprintf(users_file, "%s;%s;%s;%s;%s;%s;\n", new_user.session_title, new_user.user_role,
                new_user.user_code, new_user.user_name, new_user.email, new_user.password);
    }

    fclose(users_file);

    printf(" ~~~ User successfully added ~~~\n");

    return 0;
}

//Print a header for the table
void Print_View_User_Row(char session_title[Max_String], char user_role[Max_String], char user_code[Max_User_Code],
                   char user_name[Max_String], char email[Max_String], char password[Max_String]) {
    printf("| %-20s | %-10s | %-10s | %-10s | %-20s | %-10s |\n", session_title, user_role, user_code, user_name, email, password);
}


//Admin Menu > 2. View all user (list)
int View_all_User(struct users session_user) {

    printf(" \n ---------------------------------------------------- \n");
    printf(" |                >>  ADMIN PAGE  <<                |\n");
    printf(" |             ***  VIEW ALL USER  ***              |\n");
    printf(" ---------------------------------------------------- \n");

    struct users u;

    FILE *users_file = fopen("user_profile.txt", "r");

    printf("|----------------------|------------|------------|------------|----------------------|------------|\n");
    printf("| %-20s | %-10s | %-10s | %-10s | %-20s | %-10s |\n", "Title", "User Role", "User Code", "User Name", "Email", "Password");
    printf("|----------------------|------------|------------|------------|----------------------|------------|\n");

    while (fscanf(users_file, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];\n", u.session_title,
                  u.user_role, u.user_code, u.user_name, u.email, u.password) != EOF) {
        Print_View_User_Row(u.session_title, u.user_role, u.user_code, u.user_name, u.email, u.password);
    }

    fclose(users_file);
    return 0;
}

//Admin Menu > 3. Add new session
int add_New_Session(struct users session_user) {

    printf(" \n ----------------------------------------------------\n");
    printf(" |                >>  ADMIN PAGE  <<                |\n");
    printf(" |            ***  ADD NEW SESSION  ***             |\n");
    printf(" ---------------------------------------------------- \n");

    FILE *session_file = fopen("default_session.txt", "a+");
    FILE *enrol_session_file = fopen("enrol_session.txt", "r");
    FILE *users_file = fopen("user_profile.txt", "r");

    struct sessions session;

    char user_code[Max_Session_Code];
    char session_code[Max_Session_Code];

    while (1) {
        printf("Enter New Session's Code (6 characters): ");
        scanf("%s", session_code);
        if (strlen(session_code) == 6) {
            break;
        } else {
            printf("\n Please try again. \n");
            printf("----------------------\n");
        }
    }

    // check if the session exists in the default_session.txt using session_code
    while (fscanf(session_file, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];\n", session.session_code, session.title,
                  session.day, session.start_time, session.location, session.tutor_code) != EOF) {
        if (strcmp(session.session_code, session_code) == 0) {
            printf("The session has already exists");
            return 0;
        }
    }

    printf("Enter New Session's Name: ");
    scanf("%s", session.title);

    printf("Enter New Session's Day: ");
    scanf("%s", session.day);

    printf("Enter New Session's start time: ");
    scanf("%s", session.start_time);

    printf("Enter New Session's Location: ");
    scanf("%s", session.location);

    printf("Tutor who handle this session (Please enter 6 digit Tutor Code start with 'TT'): ");
    scanf("%s", user_code);
    int response = user_code_parser(user_code);


    if (response) {
        //read file line by line
        struct users user;
        // check if the user exists in the user_profile.txt using user_code
        int flag = 0;
        while (fscanf(users_file, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];\n", user.session_title,
                      user.user_role, user.user_code, user.user_name, user.email, user.password) != EOF) {
            if (strcmp(user.user_code, user_code) == 0) {
                flag = 1;
                break;
            }
        }
        if (flag == 0) {
            printf("Tutor does not exists.\n");
            fclose(users_file);
            return 0;
        }

        struct enrol_sessions es;
        int flag2 = 0;
        // check if the enrolled session exists in the enrol_session.txt using session_code  & user_code
        while (fscanf(enrol_session_file, "%[^;];%[^;];%[^;];%[^;];\n",
                      es.user_code, es.user_name, es.user_role, es.session_code) != EOF) {

            if (strcmp(es.user_code, user_code) == 0 && strcmp(es.session_code, session_code) == 0) {
                flag2 = 1;
                break;
            }
        }
        if (flag2 == 0) {
            printf("This tutor has already enrolled in another session.\n");
            fclose(enrol_session_file);
            return 0;
        }
    }
    fprintf(session_file, "%s;%s;%s;%s;\n", session.title, session.day, session.start_time, session.location);
    fclose(session_file);
    return 0;
}

//Admin Menu > 4. enroll user to session (link to user)
int Enroll_Student_to_Session(struct users session_user) {

    printf(" \n ---------------------------------------------------- \n");
    printf(" |                >>  ADMIN PAGE  <<                |\n");
    printf(" |        ***  ENROLL STUDENT TO SESSION  ***       |\n");
    printf(" ---------------------------------------------------- \n");

    FILE *default_session_file = fopen("default_session.txt", "a+");
    FILE *users_file = fopen("user_profile.txt", "a+");

    // Sessions
    printf("Enter a session code to enroll student to that session: ");
    char session_code[Max_Session_Code];
    scanf("%s", session_code);

    struct sessions s;
    int FoundSession = 0;

    while (fscanf(default_session_file, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];\n",
                  s.session_code, s.title, s.day, s.start_time, s.location, s.tutor_code) != EOF) {
        if (strcmp(s.session_code, session_code) == 0) {
            FoundSession = 1;
            break;
        }
    }
    if (FoundSession == 0) {
        printf("This session does not exist. \n");
        return 0;
    }

    // Users
    printf("Enter the student code to enroll him/her in this session: ");
    char user_code[Max_User_Code];
    scanf("%s", user_code);

    struct users u;
    int FoundUser = 0;

    while (fscanf(users_file, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];\n",
                  u.session_title, u.user_role, u.user_code, u.user_name, u.email, u.password) != EOF) {
        if (strcmp(u.user_code, user_code) == 0) {
            FoundUser = 1;
            break;
        }
    }
    if (FoundUser == 0) {
        printf("This student does not exist.\n");
        return 0;
    }

    if (strcmp(u.user_role, "tutor") == 0) {
        printf("Tutor cannot be enrolled here.\n");
    }

    FILE *enrol_session_file = fopen("enrol_session.txt", "a+");
    struct enrol_sessions es;
    while (fscanf(enrol_session_file, "%[^;];%[^;];%[^;];%[^;];\n",
                  es.user_code, es.user_name, es.user_role, es.session_code) != EOF) {
        if (strcmp(es.user_code, user_code) == 0 && strcmp(es.session_code, session_code) == 0) {
            printf("This user already enrolled. \n");
            return 0;
        }
    }
    fprintf(enrol_session_file, "%s;%s;%s;%s;\n",
            u.user_code, u.user_name, u.user_role, s.session_code);
    printf("Student successfully added in this session ~~\n");

    fclose(default_session_file);
    fclose(enrol_session_file);
    fclose(users_file);
    return 0;
}


// 2. Tutor page (to view listing of sessions)//
int Tutor_menu() {
    int choice;

    printf("\n ---------------------------------------------------- \n");
    printf(" |      APU Programming Cafe Management System      |\n");
    printf(" ---------------------------------------------------- \n");
    printf(" |                >>  TUTOR PAGE  <<                |\n");
    printf(" ---------------------------------------------------- \n");
    printf("1. View my sessions\n");
    printf("2. View students in sessions\n");
    printf("3. Log Out \n");
    printf("Enter your choice: ");

    scanf("%d", &choice);

    return (choice);
}

void Tutor(struct users session_user) {
    int flag = 1;
    int choice;

    while (flag) {
        choice = Tutor_menu();
        switch (choice) {
            case 1:
                View_Tutor_Session(session_user);
                break;
            case 2:
                View_Students_in_Session(session_user);
                break;
            case 3:
                flag = 0;
                printf(" ~~~ You are logged out. Thanks for using APU Programming Cafe Management System ~~~ \n");
                break;
            default:
                printf(" !!! Invalid option, Please try again !!! \n");
                printf("--------------------------------------------\n");
        }
    }
}

//Tutor Menu > 1. View Tutor session
int View_Tutor_Session(struct users session_user) {

    printf(" \n ---------------------------------------------------- \n");
    printf(" |                >>  TUTOR PAGE  <<                |\n");
    printf(" |               ***  MY SESSION  ***               |\n");
    printf(" ---------------------------------------------------- \n");

    struct enrol_sessions s;

    FILE *enrol_session_file = fopen("enrol_session.txt", "r");

    while (fscanf(enrol_session_file, "%[^;];%[^;];%[^;];%[^;];\n",
                  s.user_code, s.user_name, s.user_role, s.session_code) != EOF) {

        if (strcmp(s.user_code, session_user.user_code) == 0) {
            printf("This is your sessions.\n");
            printf("%s %s %s %s \n", s.user_code, s.user_name, s.user_role, s.session_code);
            break;
        }
    }
    fclose(enrol_session_file);
    return 0;
}


//Tutor Menu > 2. View enrolled Student in session
int View_Students_in_Session(struct users session_user) {

    printf(" \n ---------------------------------------------------- \n");
    printf(" |                >>  TUTOR PAGE  <<                |\n");
    printf(" |     ***  STUDENT ENROLLED IN MY SESSION  ***     |\n");
    printf(" ---------------------------------------------------- \n");

    FILE *enrol_session_file = fopen("enrol_session.txt", "r");
    FILE *default_session_file = fopen("default_session.txt", "r");

    struct enrol_sessions es;
    struct sessions s;

    int foundSession = 0;
    while (fscanf(default_session_file, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];\n",
                  s.session_code, s.title, s.day, s.start_time, s.location, s.tutor_code) != EOF) {
        if (strcmp(session_user.user_code, s.tutor_code) == 0) {
            foundSession = 1;
            break;
        }
    }

    if (!foundSession) {
        printf("You have not enrolled in any session. \n");
        return 0;
    }

    while (fscanf(enrol_session_file, "%[^;];%[^;];%[^;];%[^;];\n",
                  es.user_code, es.user_name, es.user_role, es.session_code) != EOF) {
        if (strcmp(es.user_role, "student") == 0 && strcmp(es.session_code, s.session_code) == 0) {
            printf("%s %s %s ", es.user_code, es.user_name, es.session_code);

            //Def: pointer from below enrol_session_file move to the top of default session file.
            rewind(default_session_file);

            while (fscanf(default_session_file, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];\n",
                          s.session_code, s.title, s.day, s.start_time, s.location, s.tutor_code) != EOF) {
                if (strcmp(s.session_code, s.session_code) == 0) {
                    printf("%s \n", s.location);
                    break;
                }
            }
        }
    }
    fclose(enrol_session_file);
    fclose(default_session_file);
    return 0;
}


// 3. Student page (to view listing of sessions, can enrol themselves in available session)//
int Student_menu() {
    int choice;

    printf("\n ---------------------------------------------------- \n");
    printf(" |      APU Programming Cafe Management System      |\n");
    printf(" |--------------------------------------------------| \n");
    printf(" |               >>  STUDENT PAGE  <<               |\n");
    printf(" ---------------------------------------------------- \n");
    printf("1. View my sessions\n");
    printf("2. Enroll into session\n");
    printf("3.  Log Out\n");
    printf("Enter your choice: ");

    scanf("%d", &choice);

    return (choice);
}

void Student(struct users session_user) {
    int flag = 1;
    int choice;

    while (flag) {
        choice = Student_menu();
        switch (choice) {
            case 1:
                View_Student_Session(session_user);
                break;
            case 2:
                Enroll_into_Session(session_user);
                break;
            case 3:
                flag = 0;
                printf(" ~~~ You are logged out. Thanks for using APU Programming Cafe Management System ~~~ \n");
                break;
            default:
                printf(" !!! Invalid option, Please try again !!! \n");
                printf("--------------------------------------------\n");
        }
    }
}


//Student Menu > 1. View student session
int View_Student_Session(struct users session_user) {
    printf(" ---------------------------------------------------- \n");
    printf(" |               >>  STUDENT PAGE  <<               |\n");
    printf(" |             ***  VIEW MY SESSION  ***            |\n");
    printf(" ---------------------------------------------------- \n");

    struct enrol_sessions es;

    FILE *enrol_session_file = fopen("enrol_session.txt", "r");

    printf("This is your sessions.This is your sessions.\n");

    while (fscanf(enrol_session_file, "%[^;];%[^;];%[^;];%[^;];\n", es.user_code,
                  es.user_name, es.user_role, es.session_code) != EOF) {

        if (strcmp(es.user_code, session_user.user_code) == 0) {
            printf("%s %s %s %s \n", es.user_code, es.user_name, es.user_role, es.session_code);
        }
    }
    fclose(enrol_session_file);
    return 0;
}


//Student Menu > 2. Enroll themselves into session
int Enroll_into_Session(struct users session_user) {

    printf("\n ---------------------------------------------------- \n");
    printf(" |               >>  STUDENT PAGE  <<               |\n");
    printf(" |           ***  MY ENROLLED SESSION  ***          |\n");
    printf(" ---------------------------------------------------- \n");

    struct enrol_sessions new_es;

    FILE *enrol_session_file = fopen("enrol_session.txt", "a+");

    char session_code[Max_Session_Code];

    printf("Which session you want to enroll?\n ");
    printf("Please enter the session code: ");
    scanf("%s", session_code);

    while (fscanf(enrol_session_file, "%[^;];%[^;];%[^;];%[^;];\n",
                  new_es.user_code, new_es.user_name, new_es.user_role, new_es.session_code) != EOF) {
        if (strcmp(new_es.session_code, session_code) == 0 && strcmp(new_es.user_code, session_user.user_code) == 0) {
            printf("You are already in this session.");
            return 0;
        }
    }
    fprintf(enrol_session_file, "%s;%s;%s;%s;\n",
            session_user.user_code, session_user.user_name, session_user.user_role, session_code);
    fclose(enrol_session_file);
    printf("\nYou have enrolled in to a new session!!!\n\n");
    printf("This is you enrolled sessions.\n");
    printf("%s %s %s %s \n", session_user.user_code, session_user.user_name, session_user.user_role, session_code);
    return 0;
}


//Print a header for session menu table
void PrintTableRow(char session_code[Max_Session_Code], char title[Max_String], char day[Max_String],
                   char start_time[Max_String], char location[Max_String], char tutor_code[Max_User_Code]) {
    printf("| %-12s | %-25s | %-10s | %-10s | %-10s | %-11s |\n", session_code, title, day, start_time, location, tutor_code);
}


// 4. View session page//
int Session_menu() {

    printf("\n ---------------------------------------------------- \n");
    printf(" |      APU Programming Cafe Management System      |\n");
    printf(" |--------------------------------------------------| \n");
    printf(" |            >>  Available Sessions  <<            |\n");
    printf(" ---------------------------------------------------- \n\n");

    struct sessions s;

    FILE *default_session_file = fopen("default_session.txt", "r");

    printf("|--------------|---------------------------|------------|------------|------------|-------------|\n");
    printf("| %-12s | %-25s | %-10s | %-10s | %-10s | %-10s  |\n", "Session Code", "Title", "Day", "Time", "Location", "Tutor ID");
    printf("|--------------|---------------------------|------------|------------|------------|-------------|\n");

    while (fscanf(default_session_file, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];\n", s.session_code, s.title, s.day,
                  s.start_time, s.location, s.tutor_code) != EOF) {
        PrintTableRow(s.session_code, s.title, s.day, s.start_time, s.location, s.tutor_code);
    }

    fclose(default_session_file);
    return 0;
}


//create file//
int create_file() {
    FILE *default_session_file = fopen("default_session.txt", "w");
    FILE *enrol_session_file = fopen("enrol_session.txt", "w");
    FILE *users_file = fopen("user_profile.txt", "w");

    char default_session[5][6][100] = {     // [Row][column][characters]
            //{"s.code" ,       "s.name" ,             "day" ,        "time" ,   "location" ,     t.code   }//
            {"PYP101", "PYTHON PROGRAMMING",  "Saturday", "9.00am", "C-01-01", "TT123456"},
            {"JAV102", "JAVA PROGRAMMING",    "Sunday",   "9.00am", "C-01-02", "TT789123"},
            {"CPL103", "C PROGRAMMING",       "Saturday", "2.00pm", "C-01-03", "TT456789"},
            {"WEB104", "WEB DEVELOPMENT",     "Sunday",   "2.00pm", "C-01-04", "TT101112"},
            {"CSP105", "C SHARP PROGRAMMING", "Monday",   "7.00pm", "C-01-05", "TT131415"}
    };

    // sub x = row , y = column
    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 6; y++) {
            fprintf(default_session_file, "%s;", default_session[x][y]);
        }
        fprintf(default_session_file, "\n");
    };


    char enrol_session[8][4][100] = {     // [Row][column][characters]
            //{       "user code" ,    "t,name" ,     "role" ,       "s.code" }//
            {"TT123456", "Danny",     "tutor",   "PYP101"},
            {"TT789123", "YitWah",    "tutor",   "JAV102"},
            {"TT456789", "Joanne",    "tutor",   "CPL103"},
            {"TT101112", "Linda",     "tutor",   "WEB104"},
            {"TT131415", "William",   "tutor",   "CSP105"},
            {"TP987654", "Jia Xin",   "student", "CPL103"},
            {"TP321987", "Man Wei",   "student", "PYP101"},
            {"TP654321", "Wong Xuen", "student", "CSP105"}
    };

    // sub x = row , y = column
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 4; y++) {
            fprintf(enrol_session_file, "%s;", enrol_session[x][y]);
        }
        fprintf(enrol_session_file, "\n");
    };


    char users[12][6][100] = {     // [Row][column][characters]
            //{       "session_title",             "Role" ,       "User Code" ,     "Name" ,         "Email" ,                "Password"}//
            {"NULL",                "admin",   "AD111222", "Susan",     "susan1@apu.edu.my",   "Y111222"},
            {"NULL",                "admin",   "AD333444", "Anna",      "anna2@apu.edu.my",    "Y333444"},
            {"NULL",                "admin",   "AD555666", "Bryan",     "bryan3@apu.edu.my",   "Y555666"},

            {"PYTHON PROGRAMMING",  "tutor",   "TT123456", "Danny",     "danny1@apu.edu.my",   "X123456"},
            {"JAVA PROGRAMMING",    "tutor",   "TT789123", "YitWah",    "yitwah2@apu.edu.my",  "X789123"},
            {"C Programming",       "tutor",   "TT456789", "Joanne",    "joanne3@apu.edu.my",  "X456789"},
            {"Web Development",     "tutor",   "TT101112", "Linda",     "linda4@apu.edu.my",   "X101112"},
            {"C Sharp Programming", "tutor",   "TT131415", "William",   "william5@apu.edu.my", "X131415"},
            {"NULL"               , "tutor",   "TT121212", "Jennie",    "Jennie6@apu.edu.my",  "X121212"},


            {"NULL",                "student", "TP987654", "Jia xin",   "jiaxin1@apu.edu.my",  "Z987654"},
            {"NULL",                "student", "TP321987", "Man wei",   "manwei2@apu.edu.my",  "Z321987"},
            {"NULL",                "student", "TP654321", "Wong xuen", "wongxuen@apu.edu.my", "Z654321"},
    };

    // sub x = row , y = column
    for (int x = 0; x < 12; x++) {
        for (int y = 0; y < 6; y++) {
            fprintf(users_file, "%s;", users[x][y]);
        }
        fprintf(users_file, "\n");
    };

    fclose(default_session_file);
    fclose(enrol_session_file);
    fclose(users_file);

    return 0;
}


int user_code_parser(char *user_code) {

    int valid_user_id = 0;

    // For Admin
    if ((user_code[0] == 'A' && user_code[1] == 'D') ||
        (user_code[0] == 'a' && user_code[1] == 'd') ||
        (user_code[0] == 'a' && user_code[1] == 'D') ||
        (user_code[0] == 'A' && user_code[1] == 'd')) {

        user_code[0] = 'A';
        user_code[1] = 'D';

        valid_user_id = 1;
    }

        // For Tutor
    else if ((user_code[0] == 'T' && user_code[1] == 'T') ||
             (user_code[0] == 't' && user_code[1] == 't') ||
             (user_code[0] == 't' && user_code[1] == 'T') ||
             (user_code[0] == 'T' && user_code[1] == 't')) {

        user_code[0] = 'T';
        user_code[1] = 'T';

        valid_user_id = 1;
    }

        // For student
    else if ((user_code[0] == 'T' && user_code[1] == 'P') ||
             (user_code[0] == 't' && user_code[1] == 'p') ||
             (user_code[0] == 't' && user_code[1] == 'P') ||
             (user_code[0] == 'T' && user_code[1] == 'p')) {

        user_code[0] = 'T';
        user_code[1] = 'P';

        valid_user_id = 1;
    }

    return valid_user_id;
}
