#include <windows.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <commdlg.h>
#include <ctype.h>  // added for isspace()

bool ProgramIsRunning = true;
void greet();
void Menu();
void ShowAllStudents();
void AddStudent();
void SearchStudent();
void ModifyStudent();
void RemoveStudent();
void RemoveAllStudents();
void LoadStudentRecords();
void SaveToFile();
void GoBackOrExit();
void GoBackOrExit();
int isValidEmail(const char *email) {
    // Find the '@' character in the email
    char *atSign = strchr(email, '@');
    // Check if '@' is not found or is the first character
    if (atSign == NULL || atSign == email) return 0;

    // Find the last '.' character after the '@' character
    char *dot = strrchr(atSign, '.');
    // Check if '.' is not found or is too close to the '@' character
    if (dot == NULL || dot < atSign + 2) return 0;

    // Check for any whitespace characters in the email
    for (int i = 0; email[i] != '\0'; i++) {
        if (isspace(email[i])) return 0;
    }

    // If all checks pass, the email is valid
    return 1;
}

// Dummy file to load student records
#define FILE_NAME "students.txt"

// Student Structure
typedef struct {
    int roll_number;
    char first_name[20];
    char last_name[20];
    char email[30];
    char course_code[10];
    float marks;
} Student;
Student *students = NULL;
int studentCount = 0;

// MAIN PROGRAM ---------------------------------------------------------------
int main() {

    greet();

    while (ProgramIsRunning)
    {
        Menu();
        int Option;
        scanf("%d", &Option);

        switch(Option)
        {
        case 0:
            ProgramIsRunning = false;
            break;
        case 1:
            system("cls");
            ShowAllStudents(students, studentCount);
            GoBackOrExit();
            break;
        case 2:
            system("cls");
            AddStudent();
            GoBackOrExit();
            break;
        case 3:
            system("cls");
            SearchStudent();
            GoBackOrExit();
            break;
        case 4:
            system("cls");
            ModifyStudent();
            GoBackOrExit();
            break;
        case 5:
            system("cls");
            RemoveStudent();
            GoBackOrExit();
            break;
        case 6:
            system("cls");
            RemoveAllStudents();
            GoBackOrExit();
            break;
        case 7:
            system("cls");
            LoadStudentRecords();
            GoBackOrExit();
            break;
        case 8:
            system("cls");
            SaveToFile();
            GoBackOrExit();
            break;
        default:
            system("cls");
            printf("Please enter a number between 0 and 8!");
            break;
        }

    } // end of main program loop
    return 0;
}
// Function to greet the user -------------------------------------------------
void greet()
{
    char name[51]; // extra space for null terminator
    printf("Please enter your name: ");

    if (fgets(name, sizeof(name), stdin) == NULL) {
        printf("Error reading input.\n");
        return;
    }

    // remove newline character if present
    name[strcspn(name, "\n")] = '\0';

    // check if input exceeded 50 characters
    if (strlen(name) >= 50 && name[49] != '\n') {
        printf("Error: Name too long! Please enter a name with 50 characters or less.\n\n");

        // clear input buffer
        while (getchar() != '\n');

        // call greet() recursively to retry
        greet();
        return;
    }

    system("cls");
    printf("Welcome to the Student's Database, %s!\n", name);
}

// MAIN MENU Items -------------------------------------------------------------
void Menu()
{
    printf("\n\n\t\t\tMAIN MENU\n");
    printf("\t\t=======================\n");
    printf("\t\t[1] Show All Students\n");
    printf("\t\t[2] Add A New Student\n");
    printf("\t\t[3] Search For A Student\n");
    printf("\t\t[4] Modify Student Details\n");
    printf("\t\t[5] Remove A Student\n");
    printf("\t\t[6] Remove All Students\n");
    printf("\t\t[7] Load Student Records\n");
    printf("\t\t[8] Save To File\n");
    printf("\t\t[0] Exit The Program\n");
    printf("\t\t=======================\n");
    printf("\t\tEnter Your Choice: ");
}
//------------------------------------------------------------------------------
void ShowAllStudents(Student *students, int count)
{
    char choice;
    int ascending;
    printf("\n\t--------- Show All Students ---------\n\n");

    printf("\tDo you want to sort students before displaying? (Y/N): ");
    scanf(" %c", &choice);

    if (choice == 'Y' || choice == 'y') {
        printf("\tSort marks in ascending (1) or descending (0) order?: ");
        scanf("%d", &ascending);

        // Sorting logic (Bubble Sort)
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if ((ascending && students[j].marks > students[j + 1].marks) ||
                    (!ascending && students[j].marks < students[j + 1].marks)) {
                    Student temp = students[j];
                    students[j] = students[j + 1];
                    students[j + 1] = temp;
                }
            }
        }
        printf("\tStudents sorted successfully!\n\n");
    } else {
        printf("\tDisplaying students without sorting.\n\n");
    }

    // Display header
    printf("\t|===================|==========================|==========================|==========================|======================|==============|\n");
    printf("\t|    Roll Number    |        First Name        |         Last Name        |         Email            |     Course Code      |     Marks    |\n");
    printf("\t|===================|==========================|==========================|==========================|======================|==============|\n");

    // Loop through students and print each record
    for (int i = 0; i < count; i++) {
        // Determine if the student has passed or failed
        char *status = (students[i].marks >= 40) ? "(P)" : "(F)";

        printf("\t| %-17d | %-24s | %-24s | %-24s | %-20s | %-6.2f %-5s |\n",
               students[i].roll_number, students[i].first_name, students[i].last_name,
               students[i].email, students[i].course_code, students[i].marks, status);
    }

    printf("\t|===================|==========================|==========================|==========================|======================|==============|\n");

    // Calculate and display average marks
    if (count == 0) {
        printf("\tNo students to calculate average marks.\n\n");
        return;
    }

    float total = 0;
    for (int i = 0; i < count; i++) {
        total += students[i].marks;
    }

    printf("\n\t-------------------------------------------------\n");
    printf("\tAverage student marks: %.2f\n\n", total / count);
}
//------------------------------------------------------------------------------
void AddStudent()
{
    char choice;
    do {
        studentCount++;
        students = realloc(students, studentCount * sizeof(Student));

        if (students == NULL) {
            printf("Memory allocation failed.\n");
            return;
        }

        printf("\n\t--------- Add A New Student ---------\n\n");

        int roll_num;
        char input[100];
        int valid_input;

        do { // Loop until a valid roll number is entered and it is unique
            valid_input = 1;

            printf("\tEnter Roll Number: ");
            scanf("%s", input);

            char *endptr;
            roll_num = strtol(input, &endptr, 10);
            if (*endptr != '\0') {
                printf("\tInvalid input. Please enter a valid roll number(integer)\n\n");
                valid_input = 0;
                continue;
            }

            for (int i = 0; i < studentCount - 1; i++) {
                if (students[i].roll_number == roll_num) {
                    printf("\tRoll number already exists. Please enter a unique roll number.\n\n");
                    valid_input = 0;
                    break;
                }

            }
            students[studentCount - 1].roll_number = roll_num;
        } while (valid_input == 0);

        printf("\tEnter First Name: ");
        scanf("%s", students[studentCount - 1].first_name);

        printf("\tEnter Last Name: ");
        scanf("%s", students[studentCount - 1].last_name);

        do { // Loop until a valid email is entered
            printf("\tEnter Email: ");
            scanf("%s", students[studentCount - 1].email);
            if (!isValidEmail(students[studentCount - 1].email)) {
                printf("\tInvalid email format! Please enter a valid email (e.g., user@example.com)\n\n");
            }
        } while (!isValidEmail(students[studentCount - 1].email));

        printf("\tEnter Course code: ");
        scanf("%s", students[studentCount - 1].course_code);

        float marks;
        valid_input = 0;

        do { // Loop until a valid mark is entered
            printf("\tEnter Mark: ");
            if (scanf("%f", &marks) != 1) {
                while (getchar() != '\n');
                printf("\tInvalid mark. Please enter a number\n\n");
            } else if (marks < 0 || marks > 100) {
                printf("\tInvalid mark. Please enter a value between 0 and 100 \n\n");
            } else {
                valid_input = 1;
                students[studentCount - 1].marks = marks;
            }
        } while (!valid_input);

        printf("\n\tStudent added successfully!\n\n");

        while (getchar() != '\n');

        printf("\tDo you want to add another student? (Y/N): ");
        scanf(" %c", &choice);

        while (getchar() != '\n');

    } while (choice == 'Y' || choice == 'y');

    printf("\t-------------------------------------\n");
    printf("\tPress Enter to continue\n\n");
}
//------------------------------------------------------------------------------
void SearchStudent()
{
    if (studentCount == 0) {
        printf("\n\tNo students available to search.\n\n");
        return;
    }

    printf("\n\t--------- Search For A Student ---------\n\n");

    int rollNumber;
    printf("\tRoll Number of the student to search for: ");
    scanf("%d", &rollNumber);

    bool found = false;
    for (int i = 0; i < studentCount; i++) {
        if (students[i].roll_number == rollNumber) {
            found = true;
            printf("\n\n\tOne student found!\n\n\n");

            printf("\tSTUDENT DETAILS\n");
            printf("\t----------------\n");
            printf("\tRoll Number: %d\n", students[i].roll_number);
            printf("\tFirst Name: %s\n", students[i].first_name);
            printf("\tLast Name: %s\n", students[i].last_name);
            printf("\tEmail: %s\n", students[i].email);
            printf("\tCourse Code: %s\n", students[i].course_code);
            printf("\tMarks: %.2f\n", students[i].marks);
            printf("\t----------------------------------------\n\n");
            break;
        }
    }

    if (!found) {
        printf("\tStudent with Roll Number %d not found.\n", rollNumber);
        printf("\t----------------------------------------\n\n");
    }
}
//------------------------------------------------------------------------------
void ModifyStudent()
{
    if (studentCount == 0) {
        printf("\n\tNo students available to modify.\n\n");
        return;
    }

    printf("\n\t--------- Modify Student Details ---------\n\n");

    int rollNumber;
    int index = -1;

    // Keep prompting until a valid roll number is entered
    do {
        printf("\tEnter the Roll Number of the student to modify: ");
        if (scanf("%d", &rollNumber) != 1) {
            while (getchar() != '\n');  // Clear input buffer
            printf("\tInvalid input. Please enter a valid roll number.\n\n");
            continue;
        }

        // Search for the student by roll number
        for (int i = 0; i < studentCount; i++) {
            if (students[i].roll_number == rollNumber) {
                index = i;  // Store index of found student
                break;
            }
        }

        if (index == -1) {
            printf("\tStudent with Roll Number %d not found. Please enter a valid roll number.\n\n", rollNumber);
        }
    } while (index == -1);  // Repeat until a valid roll number is entered

    // Modify the found student
    printf("\n\tEnter new First Name: ");
    scanf("%s", students[index].first_name);

    printf("\tEnter new Last Name: ");
    scanf("%s", students[index].last_name);

    do { // Loop until a valid email is entered
            printf("\tEnter new Email: ");
            scanf("%s", students[index].email);
            if (!isValidEmail(students[index].email)) {
                printf("\tInvalid email format! Please enter a valid email (e.g., user@example.com)\n\n");
            }
        } while (!isValidEmail(students[index].email));

    printf("\tEnter new Course Code: ");
    scanf("%s", students[index].course_code);

    float marks;
    int valid_input = 0;

    do { // Loop until a valid mark is entered
        printf("\tEnter new Mark: ");
        if (scanf("%f", &marks) != 1) {
            while (getchar() != '\n');  // Clear buffer
            printf("\tInvalid mark. Please enter a number\n\n");
        } else if (marks < 0 || marks > 100) {
            printf("\tInvalid mark. Please enter a value between 0 and 100\n\n");
        } else {
            valid_input = 1;
            students[index].marks = marks;
        }
    } while (!valid_input);

    printf("\n\n\tStudent details updated successfully!\n");
    printf("\t------------------------------------------\n");
    printf("\tPress Enter to continue\n\n");

    while (getchar() != '\n');  // Wait for Enter key
}
//------------------------------------------------------------------------------
void RemoveStudent()
{
    if (studentCount == 0) {
        printf("\n\tNo student data to remove!\n\n");
        return;
    }

    printf("\n\t--------- Remove A Student ---------\n\n");

    int rollNumber, studentIndex = -1; // found is equal to -1 so that when searching, it won't miss out the data at index 0
    printf("\tEnter roll number of student to remove: ");
    scanf("%d", &rollNumber);

    // Find the student in the array
    for (int i = 0; i < studentCount; i++) {
        if (students[i].roll_number == rollNumber) {
            studentIndex = i; // storing the index of the student to remove
            break;
        }
    }

    // To check if the student was found
    if (studentIndex == -1) {
        printf("\n\tStudent not found!\n");
        printf("\t------------------------------------\n\n");

        return;
    }

    // Shift the students left to remove the student at studentIndex
    for (int i = studentIndex; i < studentCount -1; i++) {
        students[i] = students[i + 1]; // Shift the data of the students to the left
    }

    studentCount--; // Reduce the total number of students

    // Resize memory to avoid wasting space
    Student *temp = realloc(students, studentCount * sizeof(Student));
    if (temp != NULL) {
        students = temp; // Update the student array only if realloc is successful
    }

    printf("\n\tStudent removed successfully!\n");
    printf("\t------------------------------------\n\n");
}
//------------------------------------------------------------------------------
void RemoveAllStudents()
{
    if (studentCount == 0) {
        printf("\n\tNo student data to remove!\n\n");
        return;
    }

    printf("\n\t--------- Remove All Students ---------\n\n");

    char choice;

    // Prompt the user to confirm the removal of all students
    printf("\n\tAre you sure you want to remove all students?\n");
    printf("\tThis action cannot be undone!\n\n");
    printf("\tYes (Y) or No (N): ");
    scanf(" %c", &choice);

    if (choice == 'Y' || choice == 'y') {
        free(students);
        students = NULL;
        studentCount = 0;
        printf("\n\tAll students have been removed successfully!\n");
    } else {
        printf("\n\tNo students have been removed\n");
    }

    printf("\t---------------------------------------\n\n");
}
//------------------------------------------------------------------------------
void LoadStudentRecords() {
    char filePath[256];

    printf("\n\t--------- Load Student Records ---------\n\n");
    printf("\tEnter the path to your student records file: ");

    // Clear input buffer
    while (getchar() != '\n');

    // Get file path from user
    if (fgets(filePath, sizeof(filePath), stdin) == NULL) {
        printf("\n\tError reading file path.\n");
        printf("\t----------------------------------------\n\n");
        return;
    }
    filePath[strcspn(filePath, "\n")] = 0; // Remove newline

    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        printf("\n\tError: Could not open file %s\n", filePath);
        printf("\t----------------------------------------\n\n");
        return;
    }

    // Free existing student data, if any
    if (students != NULL) {
        free(students);
        students = NULL;
        studentCount = 0;
    }

    // Count number of students in file
    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strlen(line) > 1) { // Skip empty lines
            count++;
        }
    }

    // Reset file pointer to beginning
    rewind(file);

    // Allocate memory for all students
    students = malloc(count * sizeof(Student));
    if (students == NULL) {
        printf("\n\tError: Memory allocation failed\n");
        fclose(file);
        return;
    }

    // Read student data
    int i = 0;
    while (i < count && fscanf(file, "%d %s %s %s %s %f",
           &students[i].roll_number,
           students[i].first_name,
           students[i].last_name,
           students[i].email,
           students[i].course_code,
           &students[i].marks) == 6) {
        i++;
    }

    studentCount = i;

    printf("\n\tSuccessfully loaded %d student records from: %s !\n", studentCount, filePath);
    printf("\t----------------------------------------\n");
    printf("\tPress Enter to continue\n\n"); // added

    fclose(file);
}
//------------------------------------------------------------------------------
void SaveToFile() {
    FILE *file = fopen("studentrecords.txt", "w");
    if (!file) {
        printf("\n\tError: Unable to save file.\n\n");
        return;
    }

    fprintf(file, "All Students:\n\n");

    float totalMarks = 0;

    // Loop through and write each student record to the file
    for (int i = 0; i < studentCount; i++) {
        char *status = (students[i].marks >= 40) ? "(P)" : "(F)";

        fprintf(file, "%d, %s, %s, %s, %s, %.2f %s\n",
                students[i].roll_number, students[i].first_name, students[i].last_name,
                students[i].email, students[i].course_code, students[i].marks, status);

        totalMarks += students[i].marks;
    }

    // Calculate and write average marks to the file
    float averageMarks = (studentCount > 0) ? totalMarks / studentCount : 0;

    fprintf(file, "\n------------------------------------\n");
    fprintf(file, "\nAverage Student Marks: %.2f\n", averageMarks);

    fclose(file);
    printf("\n\tStudent records successfully saved to studentrecords.txt!\n\n");
}
//------------------------------------------------------------------------------
void GoBackOrExit()
{
    while (getchar() != '\n'); // clear the input buffer

    char Option;
    printf("\tGo back (b)? or Exit Program (0)?: ");
    scanf("%c", &Option);

    while (getchar() != '\n'); // clear any remaining input

    if (Option == '0')
    {
        printf("Leaving program now!\nBye!\n");
        ProgramIsRunning = false;
    }
    else if (Option == 'b')
    {
        system("cls");
    }
    else
    {
        printf("Invalid option. Returning to menu.\n");
        system("cls");
    }
}
//------------------------------------------------------------------------------
// END OF PROGRAM
//------------------------------------------------------------------------------