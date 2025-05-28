#include "../include/STUDENT_MAIN_PAGE.h"
#include "../include/Student.h"
#include "../include/STUDENT_EXAM_PAGE.hpp"
#include <regex>

void StudentMainPage(bool& Logout_FLAG, std::string & USER_INPUT, json & Exam_File, Student * USER_Student, json & Student_File, json & Question_File) {
    // Setting Up Default Values For Further Calculations
    long long exam_GUC_enrollment = 0;
    long long exam_GUC_participation = 0;
    long long exam_GUC_report = 0;

    // Menu for Student Page
    while (USER_INPUT != "0" && !Logout_FLAG) {
        try {
            Clear();
            std::cout << UP_WINDOW_TILE << std::endl;
            std::cout << "\t#--------- " << USER_Student -> getName() << " " << USER_Student -> getLast_Name() << " ---------#\n" << std::endl;
            for (int Index = 0; Index < STUDENT_MAIN_PAGE_MENU.size(); Index++) {
                std::cout << STUDENT_MAIN_PAGE_MENU[Index];
                if (Index == 2) USER_Student -> showExams();
                if (Index == 3) std::cout << ": " << exam_GUC_participation;
                if (Index == 5) std::cout << ": " << exam_GUC_report;
                std::cout << std::endl;
            }
            std::cout << BOTTOM_WINDOW_TILE << std::endl << INPUT_SENTENCE;
            
            // std::cin.ignore();
            std::getline(std::cin, USER_INPUT);

            if (USER_INPUT == "-2"){ // Delete User
                std::string flag;
                std::cout << "\n\t !!!! ARE YOU SURE ???? (Y/n): ";
                std::cin >> flag;
                if (flag == "Y"){
                    if (USER_Student->DeleteUser(Student_File)) {
                        Logout_FLAG = true;
                        break;
                    } else {
                        continue;
                    }
                } else {
                    continue;
                }
            }

            if (USER_INPUT == "-1") { // Logout
                USER_Student -> studentFileUpdate(Student_File);
                Logout_FLAG = true;
            }

            else if (USER_INPUT == "1") { // Exam Generated Unique Code To Enroll
                std::string tmp;
                std::regex pattern("^([0-9]+)$");
                InputingSection("EXAM GUC TO ENROLL", tmp);
                if (!regex_match(tmp, pattern)){
                    UnSuccessfulMessage("EXAM GUC SHOULD BE A POSITIVE NUMBER");
                }
                exam_GUC_enrollment = stoll(tmp);
                USER_Student -> insertExam(exam_GUC_enrollment, Exam_File, Question_File, Student_File);
            } else if (USER_INPUT == "2") { // Exam Generated Unique Code To Participate
                std::string tmp;
                std::regex pattern("^([0-9]+)$");
                InputingSection("EXAM GUC TO PARTICIPATE", tmp);
                if (!regex_match(tmp, pattern)){
                    UnSuccessfulMessage("EXAM GUC SHOULD BE A POSITIVE NUMBER");
                }
                exam_GUC_participation = stoll(tmp);
            } else if (USER_INPUT == "3") { // Exam Participation
                USER_Student -> checkExamForParticipation(exam_GUC_participation);
                Exam * Selected_Exam;
                Exam TMP;
                TMP.examSetter(exam_GUC_participation, Exam_File, Question_File);
                Selected_Exam = & TMP;
                ExamParticipationPage(Logout_FLAG, USER_INPUT, Selected_Exam, Exam_File, USER_Student, Student_File, Question_File, MULTIPLE_CHOICE_QUESTION_MODE);
            } else if (USER_INPUT == "4") {
                std::string tmp;
                std::regex pattern("^([0-9]+)$");
                InputingSection("EXAM GUC TO ENROLL", tmp);
                if (!regex_match(tmp, pattern)){
                    UnSuccessfulMessage("EXAM GUC SHOULD BE A POSITIVE NUMBER");
                }
                exam_GUC_report = stoll(tmp);
            }
            else if (USER_INPUT == "5") {
                USER_Student -> checkExamForReportCard(exam_GUC_report);
                Exam * Selected_Exam;
                Exam TMP;
                TMP.examSetter(exam_GUC_report, Exam_File, Question_File);
                Selected_Exam = & TMP;
                ExamReportPage(Logout_FLAG, USER_INPUT, Selected_Exam, USER_Student);
            }
            
            else if (USER_INPUT == "0") { // Exit
                USER_Student -> studentFileUpdate(Student_File);
                break;
            } 
            
            else { // Wrong Input
                UnSuccessfulMessage(INVALID_INPUT_ERROR);
            }

        } catch (const std::invalid_argument & error) {
            CatchException(error, TWO_SECOND);
        }
    }

    // USER_Student->studentFileUpdate(Student_File);
}