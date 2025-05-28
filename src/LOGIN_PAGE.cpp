#include "../include/STUDENT_MAIN_PAGE.h"
#include "../include/TEACHER_MAIN_PAGE.h"
#include "../include/GLOBAL_VARIABLES.hpp"
#include <regex>
#include "../include/LOGIN_PAGE.h"
#include "../include/Teacher.h"
#include "../include/Student.h"
#include "../include/Exam.hpp"

void LoginFormPage(bool& Logout_FLAG, std::string& USER_INPUT, json& Teacher_File, json& Student_File, Teacher* USER_Teacher, Student* USER_Student, json& Exam_File, json& Question_File){
    
    // Defining Variables
    std::string ID;
    std::string Pass;
    std::string Role;
    
    // Show Options
    while (USER_INPUT != "0" && !Logout_FLAG) {
        try {
            Clear();
            std::cout << UP_WINDOW_TILE << std::endl;
            std::cout << "\t#--------------- Login ---------------#\n" << std::endl;
            for (int Index = 0; Index < LOGIN_PAGE_MENU.size(); Index++){
                std::cout << LOGIN_PAGE_MENU[Index];
                if (Index == 1) std::cout << ": " << ID;
                if (Index == 2) std::cout << ": " << Pass;
                if (Index == 3) std::cout << ": " << Role;
                std::cout << std::endl;
            }
            std::cout << BOTTOM_WINDOW_TILE << std::endl<< INPUT_SENTENCE ;

            // std::cin.ignore();
            std::getline(std::cin, USER_INPUT);

            if (USER_INPUT == "-1") { // Return
                break;
            }
            else if (USER_INPUT == "1") { // ID
                InputingSection("ID", ID);
            }
            else if (USER_INPUT == "2") { // Password
                InputingSection("PASSWORD", Pass);
            }
            else if (USER_INPUT == "3"){ // Role
                InputingSection("ROLE", Role);
                std::regex pattern_teacher("^([tT]*[eE]*[aA]*[cC]*[hH]*[eE]*[rR]*)$");
                std::regex pattern_student("^([sS]*[tT]*[uU]*[dD]*[eE]*[nN]*[tT]*)$");
                if (std::regex_match(Role, pattern_teacher)){
                    Role = "Teacher";
                } else if (std::regex_match(Role, pattern_student)){
                    Role = "Student";
                } else {
                    Role = "";
                    UnSuccessfulMessage("CHOOSE A VALID ROLE (TEACHER / STUDENT)!");
                }
            }
            else if (USER_INPUT == "4"){ // Enter
                if (Role.empty()) UnSuccessfulMessage("YOU NEED TO CHOOSE A ROLE (TEACHER / STUDENT)");
                if (Role == "Teacher") {
                    USER_Teacher->login(ID, Pass, Teacher_File);
                    TeacherMainPage(Logout_FLAG, USER_INPUT, Exam_File, USER_Teacher, Teacher_File, Question_File, Student_File);
                    }
                if (Role == "Student") {
                    USER_Student->login(ID, Pass, Student_File);
                    StudentMainPage(Logout_FLAG, USER_INPUT, Exam_File, USER_Student, Student_File, Question_File);    
                    }
            }

            else if (USER_INPUT == "0"){ // Exit
                break;
            } 
            
            else { // Invalid Options
                UnSuccessfulMessage(INVALID_INPUT_ERROR);
            }

        } catch (const std::invalid_argument& error){
            CatchException(error, TWO_SECOND);
        }
    }
}