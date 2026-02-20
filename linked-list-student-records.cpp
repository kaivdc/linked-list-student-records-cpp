#include <iostream>
#include <array>
#include <vector>
#include <iomanip>
#include <string>
#include <fstream>
#include <limits>
#include <cctype>
#include <algorithm>

using namespace::std;

string outputFileName;
ofstream ofs(outputFileName);

struct Student
{
    string firstName;
    string lastName;
    string id;
    string dateOfBirth;
    double overallGPA;
    Student* next;

    Student(string firstNameInp, string lastNameInp, string  idInp, string dateOfBirthInp)
    {
        firstName = firstNameInp;
        lastName = lastNameInp;
        id = idInp;
        dateOfBirth = dateOfBirthInp;
        overallGPA = 0.0;
        next = nullptr;
    }
};

struct Class
{
    string courseNumber;
    string courseName;
    int creditHours;
    Class* next;

    Class(string courseNumberInp, string courseNameInp, int creditHoursInp)
    {
        courseNumber = courseNumberInp;
        courseName = courseNameInp;
        creditHours = creditHoursInp;
        next = nullptr;
    }
};

struct Course
{
    string studentID;
    string courseNumber;
    string semester;
    char grade;
    int sortOrder;
    Course* next;

    Course(string studentIDInp, string courseNumberInp, string semesterInp, char gradeInp)
    {
        this->studentID = studentIDInp;
        this->courseNumber = courseNumberInp;
        this->semester = semesterInp;
        this->grade = gradeInp;
        this->next = nullptr;

        sortOrder = stoi(semesterInp.substr(1, 2)) * 10;
        char temp = semesterInp[0];
        switch (temp)
        {
        case 'W':
            sortOrder += 1;
            break;
        case 'S':
            sortOrder += 2;
            break;
        case 'F':
            sortOrder += 3;
            break;
        }  
    }
};

class StudentLinkedList
{
private:
    Student* head;

public:
    StudentLinkedList() : head(nullptr) {}
    StudentLinkedList(Student* headInput) : head(headInput) {}
    ~StudentLinkedList()
    {
        deleteList(head);
    }

    void deleteList(Student* head)
    {
        Student* current = head;
        Student* temp;
        while (current)
        {
            temp = current;
            current = current->next;
            delete temp;
        }
        head->next = nullptr;
    }

    Student* getHead() { return head; }

    Student* findNode(string idKey)
    {
        Student* current = head;
        while (current)
        {
            if (current->id == idKey)
                return current;
            current = current->next;
        }
        return nullptr;
    }

    void insertOrUpdate(string firstName, string lastName, string idKey, string dateOfBirth)
    {
        int tempID1;
        int tempID2;
        Student* node = findNode(idKey);
        if (node)
        {
            cout << "ERROR: Duplicate student ID" << endl;
            ofs << "ERROR: Duplicate student ID" << endl;
        }
        else
        {
            Student* newNode = new Student(firstName, lastName, idKey, dateOfBirth);

            if (!head)
            {
                head = newNode;
                return;
            }

            tempID1 = stoi(head->id.substr(2));
            tempID2 = stoi(idKey.substr(2));
            if (!head || tempID1 > tempID2)
            {
                newNode->next = head;
                head = newNode;
            }
            else
            {
                Student* current = head;
                while (current->next && stoi(current->next->id.substr(2)) < tempID2)
                    current = current->next;

                newNode->next = current->next;
                current->next = newNode;
            }
        }
    }

    void printList() 
    {
        if (!head || !head->next)
        {
            cout << "Student list is empty!" << endl;
            ofs << "Student list is empty!" << endl;
            return;
        }
        Student* temp = head;
        while (temp) {
            cout << left << setw(15) << temp->firstName
                << left << setw(15) << temp->lastName
                << left << setw(10) << temp->id
                << left << setw(15) << temp->dateOfBirth
                << fixed << setprecision(2) << left << setw(10) << temp->overallGPA << endl;
            ofs << left << setw(15) << temp->firstName
                << left << setw(15) << temp->lastName
                << left << setw(10) << temp->id
                << left << setw(15) << temp->dateOfBirth
                << fixed << setprecision(2) << left << setw(10) << temp->overallGPA << endl;
            temp = temp->next;
        }
        cout << endl;
        ofs << endl;
    }

    void printStudent(string idKey)
    {
        Student* student = findNode(idKey);
        if(student)
        {
            cout << left << setw(15) << student->firstName
                << left << setw(15) << student->lastName
                << left << setw(10) << student->id
                << left << setw(15) << student->dateOfBirth
                << fixed << setprecision(2) << left << setw(10) << student->overallGPA << endl;
            ofs << left << setw(15) << student->firstName
                << left << setw(15) << student->lastName
                << left << setw(10) << student->id
                << left << setw(15) << student->dateOfBirth
                << fixed << setprecision(2) << left << setw(10) << student->overallGPA << endl;
        }
        else
        {
            cout << "A student associated with " << idKey << " does not exist in our system!" << endl;
            ofs << "A student associated with " << idKey << " does not exist in our system!" << endl;
        }
    }

    void updateGPAs(Course* head, Student* student)
    {
        if(!student)
        {
            return;
        }
        double gradeTotal = 0;
        int count = 0;
        Course* current = head;

        while (current) {
            if (current->studentID == student->id)
            {
                count++;
                switch (current->grade)
                {
                case 'A':
                    gradeTotal += 4;
                    break;
                case 'B':
                    gradeTotal += 3;
                    break;
                case 'C':
                    gradeTotal += 2;
                    break;
                case 'D':
                    gradeTotal += 1;
                    break;
                case 'F':
                    gradeTotal += 0;
                    break;
                }
            }
            current = current->next;
        }
        if(count > 0)
        {
            student->overallGPA = gradeTotal / count;
        }
        updateGPAs(head, student->next);
    }
};

class ClassLinkedList
{
private:
    Class* head;

public:
    ClassLinkedList() : head(nullptr) {}
    ClassLinkedList(Class* headInput) : head(headInput) {}

    ~ClassLinkedList()
    {
        deleteList(head);
    }

    void deleteList(Class* head)
    {
        Class* current = head;
        Class* temp;
        while (current)
        {
            temp = current;
            current = current->next;
            delete temp;
        }
        head->next = nullptr;
    }

    Class* getHead() { return head; }

    Class* findNode(string courseNumberKey)
    {
        Class* current = head;
        while (current)
        {
            if (current->courseNumber == courseNumberKey)
                return current;
            current = current->next;
        }
        return nullptr;
    }

    void insertOrUpdate(string courseNumber, string courseName, int creditHours)
    {
        if (findNode(courseNumber)) {
            cout << "ERROR: Duplicate Class Number : " << courseNumber << endl;
            ofs << "ERROR: Duplicate Class Number : " << courseNumber << endl;
            return;
        }

        Class* newNode = new Class(courseNumber, courseName, creditHours);

        if (!head) {
            head = newNode;
            return;
        }

        string newPrefix = courseNumber.substr(0, courseNumber.find('_'));
        int newNumber = stoi(courseNumber.substr(courseNumber.find('_') + 1));

        string headPrefix = head->courseNumber.substr(0, head->courseNumber.find('_'));
        int headNumber = stoi(head->courseNumber.substr(head->courseNumber.find('_') + 1));

        if (newPrefix < headPrefix || (newPrefix == headPrefix && newNumber < headNumber)) {
            newNode->next = head;
            head = newNode;
            return;
        }

        Class* current = head;
        while (current->next) {

            string currentPrefix = current->next->courseNumber.substr(0, current->next->courseNumber.find('_'));
            int currentNumber = stoi(current->next->courseNumber.substr(current->next->courseNumber.find('_') + 1));

            if (newPrefix < currentPrefix || (newPrefix == currentPrefix && newNumber < currentNumber)) {
                break;
            }
            current = current->next;
        }

        newNode->next = current->next;
        current->next = newNode;
    }


    void printList() {
        Class* temp = head;
        if (!head || !head->next)
        {
            cout << "Class list is empty!" << endl;
            ofs << "Class list is empty!" << endl;
            return;
        }
        while (temp) {
            cout << left << setw(10) << temp->courseNumber << left << setw(35) << temp->courseName << left << setw(10) << temp->creditHours << endl;
            ofs << left << setw(10) << temp->courseNumber << left << setw(35) << temp->courseName << left << setw(10) << temp->creditHours << endl;
            temp = temp->next;
        }
        cout << endl;
        ofs << endl;
    }

    void printClass(string courseNumber) {
        Class* current = findNode(courseNumber);
        if(current)
        {
            cout << left << setw(10) << current->courseNumber << left << setw(35) << current->courseName << left << setw(10) << current->creditHours << endl;
            ofs << left << setw(10) << current->courseNumber << left << setw(35) << current->courseName << left << setw(10) << current->creditHours << endl;
        }
        else
        {
            cout << "A class associated with " << courseNumber << " does not exist in our system!" << endl;
            ofs << "A class associated with " << courseNumber << " does not exist in our system!" << endl;
        }
    }
};

class CourseLinkedList
{
private:
    Course* head;

public:
    CourseLinkedList() : head(nullptr) {}
    CourseLinkedList(Course* headInput) : head(headInput) {}

    ~CourseLinkedList()
    {
        deleteList(head);
    }

    void deleteList(Course* head)
    {
        Course* current = head;
        Course* temp;
        while (current)
        {
            temp = current;
            current = current->next;
            delete temp;
        }
    }

    Course* getHead() { return head; }

    Course* findNodeByCourseNumber(string courseNumberKey)
    {
        Course* current = head;
        while (current)
        {
            if (current->courseNumber == courseNumberKey)
                return current;
            current = current->next;
        }
        return nullptr;
    }
    Course* findNodeBySemester(int sortOrderKey)
    {
        Course* current = head;
        while (current)
        {
            if (current->sortOrder == sortOrderKey)
                return current;
            current = current->next;
        }
        return nullptr;
    }
    Course* findNodeByStudentID(string studentIDKey)
    {
        Course* current = head;
        while (current)
        {
            if (current->studentID == studentIDKey)
                return current;
            current = current->next;
        }
        return nullptr;
    }

    void insertOrUpdate(string semester, string courseNumber, string studentID, char grade, StudentLinkedList& studentList, ClassLinkedList& classList)
    {
        Course* node = findNodeByCourseNumber(courseNumber);
        if (node && node->studentID == studentID)
        {
            cout << "ERROR: Duplicate entry." << endl;
            ofs << "ERROR: Duplicate entry." << endl;
            return;
        }
        if (!studentList.findNode(studentID))
        {
            cout << "ERROR: Student with ID : " << studentID << " does not exist!" << endl;
            ofs << "ERROR: Student with ID : " << studentID << " does not exist!" << endl;
            return;
        }
        if (!classList.findNode(courseNumber))
        {
            cout << "Error: Class with Course Number : " << courseNumber << " does not exist!" << endl;
            ofs << "Error: Class with Course Number : " << courseNumber << " does not exist!" << endl;
        }
        else
        {
            Course* newNode = new Course(studentID, courseNumber, semester, grade);

            if (!head)
            {
                head = newNode;
                return;
            }

            if (!head)
            {
                newNode->next = head;
                head = newNode;
            }
            else
            {
                Course* current = head;

                if (newNode->sortOrder < current->sortOrder)
                {
                    newNode->next = head;
                    head = newNode;
                    return;
                }

                while (current->next && current->next->sortOrder < newNode->sortOrder)
                {
                    current = current->next;
                }

                newNode->next = current->next;
                current->next = newNode;
            }
        }
    }

    void printList() {
        Course* temp = head;
        if (!head || !head->next)
        {
            cout << "Grade list is empty!" << endl;
            ofs << "Grade list is empty!" << endl;
            return;
        }
        while (temp) {
            cout << left << setw(10) << temp->studentID << left << setw(10) << temp->courseNumber << left << setw(10) << temp->semester << left << setw(10) << temp->grade << endl;
            ofs << left << setw(10) << temp->studentID << left << setw(10) << temp->courseNumber << left << setw(10) << temp->semester << left << setw(10) << temp->grade << endl;

            temp = temp->next;
        }
        cout << endl;
        ofs << endl;
    }
};

bool hasExactlyOneUnderscore(string str);
void loadStudentFile(ifstream &studentFile, StudentLinkedList& list);
void loadCourseFile(ifstream &courseFile, ClassLinkedList& list);
void loadGradeFile(ifstream &gradeFile, CourseLinkedList& list, StudentLinkedList& studentList, ClassLinkedList& classList);
void generateTranscript(StudentLinkedList& list, ClassLinkedList& list2, CourseLinkedList& list3, string ID);

int main()
{
    StudentLinkedList list;
    ClassLinkedList list2;
    CourseLinkedList list3;

    string studentFileName;
    ifstream studentFile;
    
    string classFileName;
    ifstream classFile;
    
    string gradeFileName;
    ifstream gradeFile;
    
    cout << "Please enter the name of the output file : ";
    cin >> outputFileName;
    ofs.open(outputFileName);
    if (!ofs.is_open())
    {
        cout << "Output file failed to open!" << endl;
        ofs << "Output file failed to open!" << endl;
        return 0;
    }
    cout << "Please enter the file name for student data : ";
    cin >> studentFileName;
    studentFile.open(studentFileName);
    if (!studentFile.is_open())
    {
        cout << "Student file failed to open!" << endl;
        ofs << "Student file failed to open!" << endl;
        return 0;
    }
    cout << "Please enter the file name for class data : ";
    cin >> classFileName;
    classFile.open(classFileName);
    if (!classFile.is_open())
    {
        cout << "Class file failed to open!" << endl;
        ofs << "Class file failed to open!" << endl;
        return 0;
    }
    cout << "Please enter the file name for grade data : ";
    cin >> gradeFileName;
    gradeFile.open(gradeFileName);
    if (!gradeFile.is_open())
    {
        cout << "Grade file failed to open!" << endl;
        ofs << "Grade file failed to open!" << endl;
        return 0;
    }

    loadStudentFile(studentFile, list);
    loadCourseFile(classFile, list2);
    loadGradeFile(gradeFile, list3, list, list2);

    char userInputChar;
    int userInputInt;
    string userInputStr;
    string userInputStr2;
    string userInputStr3;
    string userInputStr4;
    Class* course = nullptr;
    Student* student = nullptr;
    do
    {
        cout << left << setw(35) << "1. Display all Student Records" << left << setw(35) << "6. Enter a student into the system" << endl;
        cout << left << setw(35) << "2. Display all course records " << left << setw(35) << "7. Enter a grade into the system" << endl;
        cout << left << setw(35) << "3. Search for a student record" << left << setw(35) << "8. Create a transcript for student" << endl;
        cout << left << setw(35) << "4. Search for a course record" << left << setw(35) << "9. Update overall GPAs" << endl;
        cout << left << setw(35) << "5. Enter a course into the system" << left << setw(35) << "10. Empty entire system" << endl;
        cout << "Plase enter an option from the above menu: ";
        cin >> userInputInt;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        while (userInputInt < 1 || userInputInt > 10)
        {
            cout << "Invalid option, please enter a option from the menu above:";
            ofs << "User entered invalid option in main menu." << endl;
            cin >> userInputInt;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
            switch (userInputInt)
            {
            case 1:
                list.printList();
                break;
            case 2:
                list2.printList();
                break;
            case 3:
                cout << "please enter the student ID to search for : ";
                cin >> userInputStr;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                student = list.findNode(userInputStr);
                while (userInputStr.substr(0, 2) != "ID" || !student)
                {
                    cout << "Student either does not exist or ID is incorrectly formatted, please enter a valid student ID in the format (ex. ID101) : ";
                    ofs << "User entered invalid option for searching a student by ID" << endl;
                    cin >> userInputStr;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    student = list.findNode(userInputStr);
                }
                list.printStudent(userInputStr);
                break;
            case 4:
                cout << "Please enter the course number to search for : ";
                cin >> userInputStr;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                course = list2.findNode(userInputStr);
                while (!course)
                {
                    cout << "Course does not exist or Course Number is incorrectly formatted, please enter a valid Course Number (ex. CIS_150) : ";
                    ofs << "User entered invalid option for Course Number in Grade Data Input." << endl;
                    cin >> userInputStr;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    course = list2.findNode(userInputStr);
                }
                list2.printClass(userInputStr);
                break;
            case 5:
                cout << "Please enter the data in the format (Course Number, Course Name, Credit Hours) : ";
                cin >> userInputStr >> userInputStr2 >> userInputInt;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                course = list2.findNode(userInputStr);
                while (!hasExactlyOneUnderscore(userInputStr) || course)
                {
                    cout << "Please enter a course number in the format (CIS_210) : ";
                    ofs << "User entered a Coruse Number with incorrect formatting" << endl;
                    cin >> userInputStr;
                    course = list2.findNode(userInputStr);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                list2.insertOrUpdate(userInputStr, userInputStr2, userInputInt);
                break;
            case 6:
                cout << "Please enter the student's data in the format (First Name, Last Name, ID, Date of Birth : ";
                cin >> userInputStr >> userInputStr2 >> userInputStr3 >> userInputStr4;
                student = list.findNode(userInputStr3);
                while (userInputStr3.substr(0, 2) != "ID" || student)
                {
                    cout << "Student either alerady exists or ID is incorrectly formatted, please enter a valid student ID in the format (ex. ID101) : ";
                    ofs << "User entered invalid ID when attempting to add Student Data." << endl;
                    cin >> userInputStr3;
                    student = list.findNode(userInputStr3);
                }
                list.insertOrUpdate(userInputStr, userInputStr2, userInputStr3, userInputStr4);
                break;
            case 7:
                cout << "Please enter the grade data in the format (Student ID, Course Number, Semester, Grade) : ";
                cin >> userInputStr >> userInputStr2 >> userInputStr3 >> userInputChar;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                student = list.findNode(userInputStr);
                while (userInputStr.substr(0, 2) != "ID" || !student)
                {
                    cout << "Student either does not exist or ID is incorrectly formatted, please enter a valid student ID in the format (ex. ID101) : ";
                    cin >> userInputStr;
                    ofs << "User entered invalid option for inputting a student's ID in Grade Data input." << endl;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    student = list.findNode(userInputStr);
                }
                course = list2.findNode(userInputStr2);
                while (!course)
                {
                    cout << "Course does not exist or Course Number is incorrectly formatted, please enter a valid Course Number (ex. CIS_150) : ";
                    ofs << "User entered invalid option for Course Number in Grade Data Input." << endl;
                    cin >> userInputStr2;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    course = list2.findNode(userInputStr2);
                }
                while (userInputStr3.length() != 3 || (userInputStr3[0] != 'F' && userInputStr3[0] != 'W' && userInputStr3[0] != 'S'))
                {
                    cout << "Incorrect format for semester data, please enter a valid semester (ex. W25) : ";
                    ofs << "User entered incorrectly formatted Semester in Grade Data Input." << endl;
                    cin >> userInputStr3;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                while (userInputChar != 'A' && userInputChar != 'B' && userInputChar != 'C' && userInputChar != 'D' && userInputChar != 'F')
                {
                    cout << "Incorrect grade data, please enter a valid option (A, B, C, D, F) : ";
                    ofs << "User entered invalid option for Grade in Grade Data Input." << endl;
                    cin >> userInputChar;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                list3.insertOrUpdate(userInputStr3, userInputStr2, userInputStr, userInputChar, list, list2);
                break;
            case 8:
                cout << "Please enter the student ID you would like to generate a transcript for : ";
                cin >> userInputStr;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                student = list.findNode(userInputStr);
                while (userInputStr.substr(0, 2) != "ID" || !student)
                {
                    cout << "Student either does not exist or ID is incorrectly formatted, please enter a valid student ID in the format (ex. ID101) : ";
                    ofs << "User entered invalid ID when attempting to generate a transcript." << endl;
                    cin >> userInputStr;
                    student = list.findNode(userInputStr);
                }
                generateTranscript(list, list2, list3, userInputStr);
                break;
            case 9:
                list.updateGPAs(list3.getHead(), list.getHead());
                cout << "Succesfully updated student's GPAs!" << endl;
                break;
            case 10:
                list.deleteList(list.getHead());
                list2.deleteList(list2.getHead());
                list3.deleteList(list3.getHead());
                cout << "Cleared lists!" << endl;
                break;
            }

            cout << "Would you like to continue (y/n) : ";
            cin >> userInputChar;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            while (userInputChar != 'y' && userInputChar != 'Y' && userInputChar != 'n' && userInputChar != 'N')
            {
                cout << "Enter a valid option (y/n) : ";
                cin >> userInputChar;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
    } while (userInputChar == 'y' || userInputChar == 'Y');

    cout << "Exiting program!" << endl;
    ofs << "Exiting program!" << endl;
    studentFile.close();
    classFile.close();
    gradeFile.close();
    ofs.close();
    delete(student);
    delete(course);
    system("pause");
    return 0;
}

bool hasExactlyOneUnderscore(string str) {
    int count = 0;
    for (char c : str) {
        if (c == '_') count++;
    }
    return (count == 1);
}

void loadStudentFile(ifstream &studentFile, StudentLinkedList& list)
{
    string studentFirstName;
    string studentLastName;
    string studentID;
    string studentBirthDay;

    while (studentFile >> studentFirstName >> studentLastName >> studentID >> studentBirthDay)
    {
        list.insertOrUpdate(studentFirstName, studentLastName, studentID, studentBirthDay);
    }
}

void loadCourseFile(ifstream &courseFile, ClassLinkedList& list)
{
    string courseNumber;
    string courseName;
    int creditHours;

    while (courseFile >> courseNumber >> courseName >> creditHours)
    {
        list.insertOrUpdate(courseNumber, courseName, creditHours);
    }
}

void loadGradeFile(ifstream &gradeFile, CourseLinkedList& list, StudentLinkedList& studentList, ClassLinkedList& classList)
{
    string studentID;
    string courseNumber;
    string semester;
    char grade;

    while (gradeFile >> studentID >> courseNumber >> semester >> grade) {
        list.insertOrUpdate(semester, courseNumber, studentID, grade, studentList, classList);
    }
}

void generateTranscript(StudentLinkedList& list, ClassLinkedList& list2, CourseLinkedList& list3, string ID)
{
    Student* student = list.findNode(ID);
    vector<Course*> studentCourses;
    cout << student->firstName << " " << student->lastName << endl << ID << endl << endl;
    ofs << student->firstName << " " << student->lastName << endl << ID << endl << endl;
    list.updateGPAs(list3.getHead(), list.getHead());

    Course* current = list3.getHead();
    while (current)
    {
        if (current->studentID == ID)
        {
            studentCourses.push_back(current);
        }
        current = current->next;
    }

    sort(studentCourses.begin(), studentCourses.end(), [](Course* a, Course* b) 
        { return a->sortOrder < b->sortOrder; });
    
    string currentSemester = studentCourses[0]->semester;
    string semesterText;
    int count = 0;
    double totalGradePoints = 0;
    double semesterGPA = 0;

    for (Course* course : studentCourses) {
        Class* classInfo = list2.findNode(course->courseNumber);
        if (!classInfo) continue;
        count++;
        switch (course->grade)
        {
        case 'A':
            totalGradePoints += 4;
            break;
        case 'B':
            totalGradePoints += 3;
            break;
        case 'C':
            totalGradePoints += 2;
            break;
        case 'D':
            totalGradePoints += 1;
            break;
        case 'F':
            totalGradePoints += 0;
            break;
        }
        if (course->semester != currentSemester) {
            currentSemester = course->semester;
            switch (course->semester[0])
            {
            case 'W':
                semesterText = "Winter ";
                break;
            case 'S':
                semesterText = "Spring ";
                break;
            case 'F':
                semesterText = "Fall ";
                break;
            }

            if (stoi(course->semester.substr(1, 2)) > 25)
            {
                semesterText += "19" + course->semester.substr(1, 2);
            }
            else
            {
                semesterText += "20" + course->semester.substr(1, 2);
            }
            semesterGPA = totalGradePoints / count;
            cout << "Semester GPA : " << fixed << setprecision(2) << semesterGPA << endl << endl;
            ofs << "Semester GPA : " << fixed << setprecision(2) << semesterGPA << endl << endl;
            cout << "Semester: " << semesterText << endl;
            ofs << "Semester: " << semesterText << endl;
            semesterGPA = 0;
            totalGradePoints = 0;
            count = 0;
        }
            cout << left << setw(10) << course->courseNumber
                << setw(35) << classInfo->courseName
                << setw(15) << classInfo->creditHours
                << setw(5) << course->grade << endl;
            ofs << left << setw(10) << course->courseNumber
                << setw(35) << classInfo->courseName
                << setw(15) << classInfo->creditHours
                << setw(5) << course->grade << endl;
    }

    cout << endl << "Overall GPA : " << fixed << setprecision(2)
        << student->overallGPA << endl;
    ofs << endl << "Overall GPA : " << fixed << setprecision(2)
        << student->overallGPA << endl;
}