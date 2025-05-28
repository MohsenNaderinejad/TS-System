#include "../include/TEACHER_MAIN_PAGE.h"
#include "../include/TEACHER_EXAM_PAGE.hpp"
#include <regex>

void TeacherMainPage(bool& Logout_FLAG, std::string & USER_INPUT, json & Exam_File, Teacher * USER_Teacher, json & Teacher_File, json & Question_File, json & Student_File) {
    long long exam_GUC = 0;
    while (USER_INPUT != "0" && !Logout_FLAG) {
        try {
            Clear();
            std::cout << UP_WINDOW_TILE << std::endl;
            std::cout << "\t#--------- " << USER_Teacher -> getName() << " " << USER_Teacher -> getLast_Name() << " ---------#" << std::endl;
            for (int Index = 0; Index < TEACHER_MAIN_PAGE_MENU.size(); Index++) {
                std::cout << TEACHER_MAIN_PAGE_MENU[Index];
                if (Index == 3) std::cout << ": " << exam_GUC;
                if (Index == 5) USER_Teacher -> viewExams();
                std::cout << std::endl;
            }
            std::cout << BOTTOM_WINDOW_TILE << std::endl << INPUT_SENTENCE;
            
            // std::cin.ignore();
            std::getline(std::cin, USER_INPUT);

            if (USER_INPUT == "-2"){ // Delete User
                std::string flag;
                std::cout << "\n\t !!!! ARE YOU SURE ???? (Y/n): ";
                std::cin >> flag;
                std::cin.ignore();
                if (flag == "Y"){
                    if(USER_Teacher->DeleteUser(Teacher_File)){
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
                Logout_FLAG = true;
                break;
            } 

            else if (USER_INPUT == "1") { // Exam Creation section
                ExamCreationPage(Logout_FLAG, USER_INPUT, USER_Teacher, Exam_File, Teacher_File, Question_File);
            } 

            else if (USER_INPUT == "2") { // Exam GUC Selection
                std::string tmp;
                InputingSection("EXAM GUC", tmp);

                std::regex pattern("^([0-9]+)$");
                if (!regex_match(tmp, pattern)) {
                    UnSuccessfulMessage("EXAM GENERATED UNIQUE CODE IS NOT VALID");
                }

                exam_GUC = stoi(tmp);
            } 
            
            else if (USER_INPUT == "3") { // Exam Viewing
                Exam TMP;
                TMP.examSetter(exam_GUC, Exam_File, Question_File, USER_Teacher -> getID());
                Exam * Selected_Exam;
                Selected_Exam = & TMP;
                SpecificExamViewPage(Logout_FLAG, USER_INPUT, Selected_Exam, Exam_File, USER_Teacher, Teacher_File, Question_File);
            } 

            else if (USER_INPUT == "4") { // Exam Score Board
                Exam TMP;
                TMP.examSetter(exam_GUC, Exam_File, Question_File, USER_Teacher -> getID());
                Exam * Selected_Exam;
                Selected_Exam = &TMP;
                ExamScoreBoardPage(Logout_FLAG, USER_INPUT, Selected_Exam, Exam_File, USER_Teacher, Teacher_File, Question_File, Student_File);
            }
            
            else if (USER_INPUT == "5") { // Edit Teacher Informations

            }

            else if (USER_INPUT == "0") { // Exit
                break;
            }

            else { // Wrong Input
                UnSuccessfulMessage(INVALID_INPUT_ERROR);
            }

        } catch (const std::invalid_argument & error) {
            CatchException(error, TWO_SECOND);
        }
    }
}

void ExamCreationPage(bool& Logout_FLAG, std::string & USER_INPUT, Teacher * USER_Teacher, json & Exam_File, json & Teacher_File, json & Question_File) {

    // Default Datas
    std::string ExamName;
    std::string Subject;
    std::string Time;
    std::string Type;
    std::string PublishState;

    while (USER_INPUT != "0" && !Logout_FLAG) {
        try {
            Clear();
            std::cout << UP_WINDOW_TILE << std::endl;
            std::cout << "\t#--------- " << USER_Teacher -> getName() << " " << USER_Teacher -> getLast_Name() << " ---------#" << std::endl;
            std::cout << "\n\t#--------- EXAM CREATE SECTION ---------#\n" << std::endl;
            for (int Index = 0; Index < TEACHER_EXAM_CREATION_PAGE_MENU.size(); Index++) {
                std::cout << TEACHER_EXAM_CREATION_PAGE_MENU[Index];
                if (Index == 2) std::cout << Exam_File[0][0]["NEW_EXAM_GUC"];
                if (Index == 3) std::cout << ": " << ExamName;
                if (Index == 4) std::cout << ": " << Subject;
                if (Index == 5) std::cout << ": " << Time;
                if (Index == 6) std::cout << ": " << Type;
                if (Index == 7) std::cout << ": " << PublishState;
                std::cout << std::endl;
            }
            std::cout << BOTTOM_WINDOW_TILE << std::endl << INPUT_SENTENCE;

            std::getline(std::cin, USER_INPUT);
            // std::cin.ignore();

            if (USER_INPUT == "-2") { // Logout + Saving Exam
                Logout_FLAG = true;
            }

            else if (USER_INPUT == "-1") { // Return
                break;
            }

            else if (USER_INPUT == "1") { // Exam Name
                InputingSection("EXAM NAME", ExamName);
            } else if (USER_INPUT == "2") { // Exam Subject
                InputingSection("SUBJECT", Subject);
            } else if (USER_INPUT == "3") { // Exam Time
                InputingSection("TIME", Time);
            } else if (USER_INPUT == "4") { // Exam Type
                InputingSection("TYPE", Type);
            } else if (USER_INPUT == "5") { // Exam Publish State
                InputingSection("BE PUBLISHED (Y/N)", PublishState);
                std::regex pattern_YES("^([yY]*[eE]*[sS]*)$");
                std::regex pattern_NO("^([nN]*[oO]*)$");
                if (regex_match(PublishState, pattern_YES)){
                    PublishState = "YES";
                } else if (regex_match(PublishState, pattern_NO)){
                    PublishState = "NO";
                } else {
                    PublishState = "";
                    UnSuccessfulMessage("IT SHOULD BE EITHER YES OR NO");
                }
            } 
            
            else if (USER_INPUT == "6") { // Creating Exam
                Exam* Created_Exam = new Exam(USER_Teacher->getID(), ExamName, Subject, Time, Type, PublishState, Exam_File, Question_File);
                USER_Teacher->insertExamID(Created_Exam->getGUC());
                USER_Teacher->teacherFileUpdate(Teacher_File);

                SuccessfulMessage("EXAM CREATED SUCCESSFULLY", TWO_SECOND);

                SpecificExamViewPage(Logout_FLAG, USER_INPUT, Created_Exam, Exam_File, USER_Teacher, Teacher_File, Question_File);
                break;
            }
             
            else if (USER_INPUT == "0") { // Exit
                break;
            }
            
            else { // Wrong Input
                UnSuccessfulMessage(INVALID_INPUT_ERROR);
            }

        } catch (const std::invalid_argument & error) {
            CatchException(error, TWO_SECOND);
        }
    }
}

void ExamScoreBoardPage(bool& Logout_FLAG, std::string & USER_INPUT, Exam * Selected_Exam, json & Exam_File, Teacher * USER_Teacher, json & Teacher_File, json & Question_File ,json & Student_File) {
    
    // Variables
    std::string studentID;
    ExamScoreBoard * SELECTED_ExamScoreBoard = new ExamScoreBoard(Selected_Exam, USER_Teacher);
    
    while (USER_INPUT != "0" && !Logout_FLAG) {
        try {
            Clear();
            std::cout << UP_WINDOW_TILE << std::endl;
            std::cout << "\t#--------- " << USER_Teacher -> getName() << " " << USER_Teacher -> getLast_Name() << " ---------#" << std::endl;
            std::cout << "\n\t#--------- EXAM SCORE BOARD ---------#\n" << std::endl;
            for (int Index = 0; Index < TEACHER_SPECIFIC_EXAM_SCORE_BOARD_PAGE_MENU.size(); Index++) {
                std::cout << TEACHER_SPECIFIC_EXAM_SCORE_BOARD_PAGE_MENU[Index];
                if (Index == 2) std::cout << ": " << Selected_Exam->getGUC();
                if (Index == 3) std::cout << ": " << Selected_Exam->getExamName();
                if (Index == 4) std::cout << ": " << Selected_Exam->getSubject();
                if (Index == 5) std::cout << ": " << Selected_Exam->getTime();
                if (Index == 6) std::cout << ": " << Selected_Exam->getType();
                if (Index == 7) std::cout << ": " << Selected_Exam->getPublishState();
                if (Index == 8) std::cout << ": " << std::fixed << std::setprecision(2) << Selected_Exam->getMaximumScore();
                if (Index == 9) std::cout << ": " << std::fixed << std::setprecision(2) << Selected_Exam->getAverageScore();
                if (Index == 10) std:: cout << ": " << studentID;
                if (Index == 11) SELECTED_ExamScoreBoard -> showScoreBoard();
                std::cout << std::endl;
            }
            std::cout << BOTTOM_WINDOW_TILE << std::endl << INPUT_SENTENCE;

            std::getline(std::cin, USER_INPUT);

            if (USER_INPUT == "-2") { // Logout
                Logout_FLAG = true;
                break;
            }
            else if (USER_INPUT == "-1") { // Return
                break;
            }
            else if (USER_INPUT == "1") { // Getting Student ID
                InputingSection("STUDENT ID", studentID);
            }

            else if (USER_INPUT == "2") { // Teacher Grading Section
                GradingExamPage(Logout_FLAG, USER_INPUT, SELECTED_ExamScoreBoard, Exam_File, Student_File, studentID, Question_File);
            }

            else if (USER_INPUT == "3") { // Printing Score Board
                std::string ADDRESS;
                std::string DEFAULT_ADDRESS = ADDRESS_CREATOR(std::vector<std::string>{".", "reports", "TEACHER_FILES"});
                std::cout << "\n\t #--> DEFAULT ADDRESS: " << DEFAULT_ADDRESS << std::endl;
                InputingSection("ADDRESS TO PRINT EXAM SCORE BOARD (DEF -> DEFAULT ADDRESS)", ADDRESS);
                if (ADDRESS == "DEF") ADDRESS = DEFAULT_ADDRESS;
                SELECTED_ExamScoreBoard -> printScoreBoard(ADDRESS);
            }

            else if (USER_INPUT == "0") { // Exit
                break;
            }
                
            else { // Wrong Input
                UnSuccessfulMessage(INVALID_INPUT_ERROR);
            }
        } catch (const std::invalid_argument & error) {
            CatchException(error, TWO_SECOND);
        }
    }

    // Freeing Space
    delete SELECTED_ExamScoreBoard;
}


void GradingExamPage(bool& Logout_FLAG, std::string & USER_INPUT, ExamScoreBoard * SELECTED_ExamScoreBoard, json& Exam_File, json& Student_File, std::string studentID, json& Question_File){
    auto SELECTED_Student = SELECTED_ExamScoreBoard -> studentFinder(Student_File, studentID);
    auto SELECTED_Exam = SELECTED_ExamScoreBoard -> getExam();
    
    auto originalExamData = SELECTED_Student->getExams()[SELECTED_Exam->getGUC()];
    
    auto SELECTED_StudentFFQ = originalExamData -> getSFFQs();
    auto FFQ_iterator = SELECTED_StudentFFQ.begin();
    auto FFQ_BEGIN = SELECTED_StudentFFQ.begin();
    auto FFQ_END = SELECTED_StudentFFQ.end();
    
    // Regex Patterns (Updated)
    std::regex pattern_movement_next("^n", std::regex_constants::icase);
    std::regex pattern_movement_prev("^p", std::regex_constants::icase);
    
    while (USER_INPUT != "0" && !Logout_FLAG) {
        try {
            Clear();
            std::cout << UP_WINDOW_TILE << std::endl;
            std::cout << "\n\t#--------- EXAM GRADING SECTION ---------#\n" << std::endl;
            for (int Index = 0; Index < TEACHER_SPECIFIC_STUDENT_GRADING_PAGE_MENU.size(); Index++) {
                std::cout << TEACHER_SPECIFIC_STUDENT_GRADING_PAGE_MENU[Index];
                if (Index == 2) std::cout << ": " << SELECTED_Exam->getGUC();
                if (Index == 3) std::cout << ": " << SELECTED_Exam->getExamName();
                
                if (Index == 4) std::cout << ": " << SELECTED_Student->getID();
                if (Index == 5) std::cout << ": " << SELECTED_Student->getName() << " " << SELECTED_Student->getLast_Name();
                
                if (Index == 6) std::cout << ": " << SELECTED_Student->getExams()[SELECTED_Exam->getGUC()]->score;
                if (Index == 8) {
                    if (SELECTED_StudentFFQ.empty()) {
                        std::cout << "\n\t#-------- NO QUESTIONS ---------#\n";
                    } else {
                        FFQ_iterator -> second -> gradingQuestionPrint();
                    }
                }
                std::cout << std::endl;
            }
            std::cout << BOTTOM_WINDOW_TILE << std::endl << INPUT_SENTENCE;
            
            std::getline(std::cin, USER_INPUT);
            
            if (USER_INPUT == "-2") { // Logout
                Logout_FLAG = true;
                break;
            }
            else if (USER_INPUT == "-1") { // Return
                break;
            }
            
            // Movements
            else if (std::regex_match(USER_INPUT, pattern_movement_next) || std::regex_match(USER_INPUT, pattern_movement_prev)) {
                SELECTED_Exam->moveThroughQuestions<FullFormQuestion>(
                    USER_INPUT, FFQ_iterator, FFQ_BEGIN, FFQ_END
                );
            }
            
            else if (USER_INPUT == "1") { // SETTING SCORE
                FFQ_iterator->second->setQuesionBasedStudentScore();
            }
            else if (USER_INPUT == "2") { // DELETEING SCORE
                FFQ_iterator->second->deleteStudentScore();
            }
            else if (USER_INPUT == "3") { // SAVING THE PROCCES
                break;
            }
            
            else { // Wrong Input
                UnSuccessfulMessage(INVALID_INPUT_ERROR);
            }
            
            
        } catch (const std::invalid_argument & error) {
            CatchException(error, THREE_SECOND);
        }
    }
    
    if (USER_INPUT != "-1") {
        // Object Updates
        SELECTED_ExamScoreBoard->SFFQsGrading(SELECTED_Student);
        SELECTED_Exam->addStudentByScore(SELECTED_Student);
        
        // File Updates
        SELECTED_Student->studentFileUpdate(Student_File);
        SELECTED_Exam->examAndQuestionFileUpdate(Exam_File, Question_File);
    }
    delete SELECTED_Student;
}