#include "../include/json.hpp"
#include "../include/MAIN_MENU_PAGE.h"
#include "../include/LOGIN_PAGE.h"
#include "../include/SIGNUP_PAGE.h"

void MainMenuInterface(std::string& USER_INPUT, json& Teacher_File, json& Student_File, json& Exam_File, json& Question_File){
    while (USER_INPUT != "0"){
        try {
            Clear();

            std::cout << UP_WINDOW_TILE << std::endl;
            for (int Index = 0; Index < MAIN_PAGE_MENU.size(); Index++){
                std::cout << MAIN_PAGE_MENU[Index] << std::endl;
            }
            std::cout << BOTTOM_WINDOW_TILE << std::endl << INPUT_SENTENCE;

            // std::cin.ignore();
            std::getline(std::cin, USER_INPUT);

            if (USER_INPUT == "1") // Login
                DecideMenuInterface("Login", USER_INPUT, Teacher_File, Student_File, Exam_File, Question_File);
            else if (USER_INPUT == "2") // Signup
                DecideMenuInterface("Signup", USER_INPUT, Teacher_File, Student_File, Exam_File, Question_File);

            else if (USER_INPUT == "0"){ // Exit
                break;
            }
            else { // Wrong Input
                UnSuccessfulMessage(INVALID_INPUT_ERROR);
            }
            
        } catch (const std::invalid_argument& error){
            CatchException(error, TWO_SECOND);
        }
    }
}

void DecideMenuInterface(std::string INPUT_MODE, std::string& USER_INPUT, json& Teacher_File, json& Student_File, json& Exam_File, json& Question_File){
    bool Logout_FLAG = false;
    while (USER_INPUT != "0" && !Logout_FLAG) {
        Clear();
        try {
            // Printing  Options
            std::cout << UP_WINDOW_TILE << std::endl;
            if (INPUT_MODE == "Signup"){ // Signup Form

                std::cout << "\t#--------------- " << INPUT_MODE << " ---------------#\n" << std::endl;
                for (int Index = 0; Index < TEACHER_OR_STUDENT_DECIDE_PAGE_MENU.size(); Index++){
                    std::cout << TEACHER_OR_STUDENT_DECIDE_PAGE_MENU[Index] << std::endl;
                }
                std::cout << BOTTOM_WINDOW_TILE << std::endl << INPUT_SENTENCE ;

                std::getline(std::cin, USER_INPUT);
                // std::cin.ignore();

                // Checking the USER_INPUT
                if (USER_INPUT == "-1") break; // Return
                else if (USER_INPUT == "1") { // Teacher
                    SignUpFormPage(Logout_FLAG, "Teachers", USER_INPUT, Teacher_File, Student_File, Exam_File, Question_File);
                }
                else if (USER_INPUT == "2"){ // Student
                    SignUpFormPage(Logout_FLAG, "Students", USER_INPUT, Teacher_File, Student_File, Exam_File, Question_File);
                }
                else if (USER_INPUT == "0"){ // Exit
                    break;
                }
                else {
                    UnSuccessfulMessage(INVALID_INPUT_ERROR);
                }
            } else { // Login Form
                Teacher TMP_Teacher;
                Student TMP_Student;
                Teacher* USER_Teacher;
                Student* USER_Student;
                USER_Teacher = &TMP_Teacher;
                USER_Student = &TMP_Student;
                LoginFormPage(Logout_FLAG, USER_INPUT, Teacher_File, Student_File, USER_Teacher, USER_Student, Exam_File, Question_File);
                break;
            } 
        } catch (const std::invalid_argument& error){
                CatchException(error, TWO_SECOND);
            }
    }
}

