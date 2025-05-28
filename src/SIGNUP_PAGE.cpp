#include "../include/STUDENT_MAIN_PAGE.h"
#include "../include/TEACHER_MAIN_PAGE.h"
#include "../include/GLOBAL_VARIABLES.hpp"
#include "../include/SIGNUP_PAGE.h"
#include "../include/Teacher.h"
#include "../include/Student.h"
#include "../include/Exam.hpp"

void SignUpFormPage(bool& Logout_FLAG, std::string INPUT_MODE, std::string& USER_INPUT, json& Teacher_File, json& Student_File, json& Exam_File, json& Question_File){
    
    // Defining Values
    std::string Name;
    std::string Last_Name;
    std::set<std::string> Courses;
    std::string Email;
    std::string Gender;
    std::string ID;
    std::string Pass;
    int Age = 0;
    std::string field_of_study;


    // Inputing
    while (USER_INPUT != "0" && !Logout_FLAG) {
        try {

            // Showing Menu
            Clear();
            std::cout << UP_WINDOW_TILE << std::endl;
            std::cout << "\t#--------------- Signup ---------------#" << std::endl;
            std::cout << "\t#-------------- "<< INPUT_MODE <<" --------------#\n" << std::endl;
            if (INPUT_MODE == "Teachers") {
                for (int Index = 0; Index < SIGNUP_TEACHER_PAGE_MENU.size(); Index++){
                    std::cout << SIGNUP_TEACHER_PAGE_MENU[Index];
                    if (Index == 1) std::cout << ": " << Name;
                    if (Index == 2) std::cout << ": " << Last_Name;
                    if (Index == 4) std::cout << ": " << ID;
                    if (Index == 5) std::cout << ": " << Pass;
                    if (Index == 3) {
                        std::cout << ": ";
                        for (auto item: Courses){
                            std::cout << item << ", ";
                        }
                    }
                    if (Index == 6) std::cout << ": " << Email;
                    if (Index == 7) std::cout << ": " << Gender;
                    if (Index == 8) std::cout << ": " << Age;
                    if (Index == 9) std::cout << ": " << field_of_study;
                    std::cout << std::endl;
                }
            } else { // Student
                for (int Index = 0; Index < SIGNUP_STUDENT_PAGE_MENU.size(); Index++){
                    std::cout << SIGNUP_STUDENT_PAGE_MENU[Index];
                    if (Index == 1) std::cout << ": " << Name;
                    if (Index == 2) std::cout << ": " << Last_Name;
                    if (Index == 3) std::cout << ": " << field_of_study;
                    if (Index == 4) std::cout << ": " << ID;
                    if (Index == 5) std::cout << ": " << Pass;
                    if (Index == 6) std::cout << ": " << Email;
                    if (Index == 7) std::cout << ": " << Gender;
                    if (Index == 8) std::cout << ": " << Age;
                    std::cout << std::endl;
                }
            }
            std::cout << BOTTOM_WINDOW_TILE << std::endl<< INPUT_SENTENCE ;
            
            // std::cin.ignore();
            std::getline(std::cin, USER_INPUT);

            // Getting Inputs
            if (USER_INPUT == "-1") { // Return
                break;
            }

            // Getting DATA
            else if (USER_INPUT == "1") { // Name
                InputingSection<std::string>("NAME", Name);
            }
            else if (USER_INPUT == "2") { // Last Name
                InputingSection<std::string>("LAST NAME", Last_Name);
            }
            else if (USER_INPUT == "3") { // Teacher -> Course /// Student -> Field Of Study
                if (INPUT_MODE == "Teachers") {
                    std::string tmp;
                    InputingSection<std::string>("ADD COURSE", tmp);
                    Courses.emplace(tmp);
                } else { // Student
                    InputingSection<std::string>("FIELD OF STUDY", field_of_study);
                }
            }
            else if (USER_INPUT == "4") { // ID
                InputingSection<std::string>("ID", ID);
            }
            else if (USER_INPUT == "5") { // Password
                InputingSection<std::string>("PASSWORD (8 CHARACHTERS MINIMUM)", Pass);
            }
            else if (USER_INPUT == "6") { // Email
                InputingSection<std::string>("EMAIL", Email);
            }
            else if (USER_INPUT == "7") { // Gender
                InputingSection("GENDER", Gender);
                std::regex pattern_male("^([mM]*[aA]*[lL]*[eE]*)$");
                std::regex pattern_female("^([fF]*[eE]*[mM]*[aA]*[lL]*[eE]*)$");
                if (regex_match(Gender, pattern_male)){
                    Gender = "Male";
                } else if (regex_match(Gender, pattern_female)){
                    Gender = "Female";
                } else {
                    Gender = "";
                    UnSuccessfulMessage("IT SHOULD BE EITHER FEMALE OR MALE");
                }
            }
            else if (USER_INPUT == "8") { // Age
                std::string tmp;
                std::regex pattern("^([0-9]+)$");
                InputingSection("AGE", tmp);
                if (!regex_match(tmp, pattern)){
                    UnSuccessfulMessage("AGE SHOULD BE A POSITIVE NUMBER");
                }
                Age = stoi(tmp);
            }
            else if (USER_INPUT == "9") { // Teacher -> Field Of Study /// Student -> SignUp
                if (INPUT_MODE == "Teachers") {
                    InputingSection("FIELD OF STUDY", field_of_study);
                } else { // Student
                    Student *USER_Student = new Student(Name, Last_Name, Email, Pass, ID, Student_File, field_of_study, Gender, Age);
                    delete USER_Student;
                    break;
                }
            }
            else if (USER_INPUT == "10"){ // Teacher -> SignUp /// Student -> Invalid_Input
                if (INPUT_MODE == "Teachers") { // Teacher
                    Teacher *USER_Teacher = new Teacher(Name, Last_Name, Email, Pass, ID, Teacher_File, Courses, field_of_study, Gender, Age);
                    delete USER_Teacher;
                    break;
                } else { // Student
                    UnSuccessfulMessage(INVALID_INPUT_ERROR);
                }  
            } 
            
            else if (USER_INPUT == "0"){ // Exit
                break;
            }

            else { // Wrong Input
                UnSuccessfulMessage(INVALID_INPUT_ERROR);
            }

        } catch (const std::invalid_argument& error){
            CatchException(error, THREE_SECOND);
        }
    }
}