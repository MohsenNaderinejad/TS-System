#include "../include/STUDENT_EXAM_PAGE.hpp"
#include "../include/Student.h"
#include "../include/Exam.hpp"


void UpdatingExamRunningBoolean(std::chrono::steady_clock::time_point end_time, bool& exam_running) {
    if (std::chrono::steady_clock::now() >= end_time) exam_running = false; 
}

void PrintingTimeEndedExam(){
    SuccessfulMessage("EXAM TIME ENDED", TWO_SECOND);
}

bool isNumber(std::string const& toCheck) {
    for(auto c: toCheck) {
      if(!std::isdigit(c))
        return false;
    }
    return true;
}

void ExamParticipationPage(bool& Logout_FLAG, std::string & USER_INPUT, Exam * Selected_Exam, json & Exam_File, Student * USER_Student, json & Student_File, json& Question_File, Mode QMode)  {

    // Setting Up Default Values For Further Calculations
    bool exam_running = true;

    auto& MCQs = Selected_Exam -> getMCQ();
    auto& SFQs = Selected_Exam -> getSFQ();
    auto& FFQs = Selected_Exam -> getFFQ();

    auto MCQ_iterator = MCQs.begin();
    auto MCQ_BEGIN = MCQs.begin();
    auto MCQ_END = MCQs.end();

    auto SFQ_iterator = SFQs.begin();
    auto SFQ_BEGIN = SFQs.begin();
    auto SFQ_END = SFQs.end();

    auto FFQ_iterator = FFQs.begin();
    auto FFQ_BEGIN = FFQs.begin();
    auto FFQ_END = FFQs.end();
    
    // Setting Up Exam Time
    auto start_time = std::chrono::steady_clock::now();
    auto end_time = start_time + std::chrono::minutes(Selected_Exam->getTime());

    // Regex Patterns (Updated)
    std::regex pattern_movement_next("^n", std::regex_constants::icase);
    std::regex pattern_movement_prev("^p", std::regex_constants::icase);
    
    
    while (USER_INPUT != "0" && !Logout_FLAG && std::chrono::steady_clock::now() < end_time && exam_running) {
        try {
            // Updating Exam Running Boolean
            UpdatingExamRunningBoolean(end_time, exam_running);
            if (!exam_running) {
                PrintingTimeEndedExam();
                break;
            }
            
            // Getting Time
            auto remaning_time = std::chrono::duration_cast<std::chrono::seconds>(end_time - std::chrono::steady_clock::now());
            int minutes = remaning_time.count() / 60;
            int seconds = remaning_time.count() % 60;
            
            Clear();
            
            // Menu for Exam Participation
            std::cout << UP_WINDOW_TILE << std::endl;
            std::cout << "\t#--------- " << USER_Student -> getName() << " " << USER_Student -> getLast_Name() << " ---------#" << std::endl;
            std::cout << "\n\t#---------- EXAM NAME: " << Selected_Exam -> getExamName() << " - GUC: " << Selected_Exam -> getGUC() << " ----------#\n" << std::endl;
            for (int Index = 0; Index < STUDENT_EXAM_PARTICIPATION_PAGE_MENU.size(); Index++) {
                std::cout << STUDENT_EXAM_PARTICIPATION_PAGE_MENU[Index];
                if (Index == 0) {
                    std::cout << "\n\t $$$ TIME REMAINED: " << minutes << ":" << (seconds >= 10 ? "":"0") << seconds << std::endl;
                    std::cout << "\n\t#-------- TOTAL QUESTIONS: "<< Selected_Exam -> getMCQ().size() + Selected_Exam -> getSFQ().size() + Selected_Exam -> getFFQ().size() << " ---------#\n";   
                }
                if (Index == 3) {
                    if (QMode == MULTIPLE_CHOICE_QUESTION_MODE) {
                        if (MCQs.empty()) {
                            std::cout << "\n\t#-------- NO QUESTIONS ---------#\n";
                        } else if (MCQ_iterator != MCQs.end()) {
                            Selected_Exam->showSpecificQuestion(MCQ_iterator->second);
                        }
                    } 
                    else if (QMode == SHORT_FORM_QUESTION_MODE) {
                        if (SFQs.empty()) {
                            std::cout << "\n\t#-------- NO QUESTIONS ---------#\n";
                        } else if (SFQ_iterator != SFQs.end()) {
                            Selected_Exam->showSpecificQuestion(SFQ_iterator->second);
                        }
                    } else if (QMode == FULL_FORM_QUESTION_MODE) {
                        if (FFQs.empty()) {
                            std::cout << "\n\t#-------- NO QUESTIONS ---------#\n";
                        } else if (FFQ_iterator != FFQs.end()) {
                            Selected_Exam->showSpecificQuestion(FFQ_iterator->second);
                        }
                    }
                }
                std::cout << std::endl;
            }
            std::cout << BOTTOM_WINDOW_TILE << std::endl << INPUT_SENTENCE;
            
            // Getting User Input
            std::getline(std::cin, USER_INPUT);
            UpdatingExamRunningBoolean(end_time, exam_running);
            
            // Checking Exam Running Boolean
            UpdatingExamRunningBoolean(end_time, exam_running);
            if (!exam_running) {
                PrintingTimeEndedExam();
                break;
            }
            // Logout + Saving Exam
            if (USER_INPUT == "-1") { 
                std::string flag;
                std::cout << "\n\t !!!! ARE YOU SURE ???? (Y/n): ";
                std::cin >> flag;
                UpdatingExamRunningBoolean(end_time, exam_running);
                if (!exam_running) {
                    PrintingTimeEndedExam();
                    break;
                }
                if (flag == "Y") {
                    Logout_FLAG = true;
                    break;
                }
            }
            
            // Changing View
            else if (USER_INPUT == "MCQ" || USER_INPUT == "SFQ" || USER_INPUT == "FFQ") { 
                UpdatingExamRunningBoolean(end_time, exam_running);
                if (!exam_running) {
                    PrintingTimeEndedExam();
                    break;
                }
                if (USER_INPUT == "MCQ") {
                    QMode = MULTIPLE_CHOICE_QUESTION_MODE;
                } else if (USER_INPUT == "SFQ") {
                    QMode = SHORT_FORM_QUESTION_MODE;
                } else if (USER_INPUT == "FFQ") {
                    QMode = FULL_FORM_QUESTION_MODE;
                }
            }
            
            // Moving Questions
            else if (std::regex_match(USER_INPUT, pattern_movement_next) || std::regex_match(USER_INPUT, pattern_movement_prev)) { 
                UpdatingExamRunningBoolean(end_time, exam_running);
                if (!exam_running) {
                    PrintingTimeEndedExam();
                    break;
                }
                if (QMode == MULTIPLE_CHOICE_QUESTION_MODE && !Selected_Exam->getMCQ().empty()) {
                    Selected_Exam->moveThroughQuestions<multipleChoiceQuestion>(
                        USER_INPUT, MCQ_iterator, MCQ_BEGIN, MCQ_END
                    );
                } else if (QMode == SHORT_FORM_QUESTION_MODE && !Selected_Exam->getSFQ().empty()) {
                    Selected_Exam->moveThroughQuestions<ShortFormQuestion>(
                        USER_INPUT, SFQ_iterator, SFQ_BEGIN, SFQ_END
                    );
                } else if (QMode == FULL_FORM_QUESTION_MODE && !Selected_Exam->getFFQ().empty()) {
                    Selected_Exam->moveThroughQuestions<FullFormQuestion>(
                        USER_INPUT, FFQ_iterator, FFQ_BEGIN, FFQ_END
                    );
                } else {
                    UnSuccessfulMessage("NO QUESTIONS AVAILABLE");
                }
            }

            // Submitting Answer
            else if (USER_INPUT == "1") { 
                UpdatingExamRunningBoolean(end_time, exam_running);
                if (!exam_running) {
                    PrintingTimeEndedExam();
                    break;
                }
                std::string tmp;
                InputingSection("ENTER YOUR ANSWER: ", tmp);
                if (!isNumber(tmp) && QMode == MULTIPLE_CHOICE_QUESTION_MODE) {
                    UnSuccessfulMessage("INVALID INPUT");
                    continue;
                }
                UpdatingExamRunningBoolean(end_time, exam_running);
                if (!exam_running) {
                    PrintingTimeEndedExam();
                    break;
                }
                if (QMode == MULTIPLE_CHOICE_QUESTION_MODE && !Selected_Exam->getMCQ().empty()) {
                    MCQ_iterator->second->setStudentAnswer(stoi(tmp));
                } else if (QMode == SHORT_FORM_QUESTION_MODE && !Selected_Exam->getSFQ().empty()) {
                    SFQ_iterator->second->setStudentAnswer(tmp);
                } else if (QMode == FULL_FORM_QUESTION_MODE && !Selected_Exam->getFFQ().empty()) {
                    FFQ_iterator->second->setStudentAnswer(tmp);
                } else {
                    UnSuccessfulMessage("NO QUESTIONS AVAILABLE");
                }
            }

            // Deleting Answer
            else if (USER_INPUT == "2") { 
                UpdatingExamRunningBoolean(end_time, exam_running);
                if (!exam_running) {
                    PrintingTimeEndedExam();
                    break;
                }
                if (QMode == MULTIPLE_CHOICE_QUESTION_MODE) {
                    MCQ_iterator->second->clearAnswer();
                } else if (QMode == SHORT_FORM_QUESTION_MODE) {
                    SFQ_iterator -> second -> clearAnswer();
                } else if (QMode == FULL_FORM_QUESTION_MODE) {
                    FFQ_iterator -> second -> clearAnswer();
                }
                else {
                    UnSuccessfulMessage("NO QUESTIONS AVAILABLE");
                }
            }

            // Ending Exam
            else if (USER_INPUT == "3") { 
                UpdatingExamRunningBoolean(end_time, exam_running);
                if (!exam_running) {
                    PrintingTimeEndedExam();
                    break;
                }
                std::string flag;
                std::cout << "\n\t !!!! ARE YOU SURE ???? (Y/n): ";
                std::cin >> flag;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                UpdatingExamRunningBoolean(end_time, exam_running);
                if (!exam_running) {
                    PrintingTimeEndedExam();
                    break;
                }
                if (flag == "Y") {
                    exam_running = false;
                    break;
                } else {
                    continue;
                }

            } else {
                UnSuccessfulMessage(INVALID_INPUT_ERROR);
            }

        } catch (const std::invalid_argument& error) {
            CatchException(error, TWO_SECOND);
        }
    }

    // Object Updates
    Selected_Exam -> addStudentByScore(USER_Student);
    USER_Student -> updateExamInStudentClass(Selected_Exam -> getGUC(), Selected_Exam);
    
    // File Updates
    Selected_Exam -> examAndQuestionFileUpdate(Exam_File, Question_File);
    USER_Student -> studentFileUpdate(Student_File);
    
    SuccessfulMessage("EXAM ENDED SUCCESSFULLY", TWO_SECOND);

    // Exam Report Page
    if (!Logout_FLAG) ExamReportPage(Logout_FLAG, USER_INPUT, Selected_Exam, USER_Student);
}

void ExamReportPage(bool& Logout_FLAG, std::string& USER_INPUT, Exam* Selected_Exam, Student* USER_Student) {
    Mode QMode = MULTIPLE_CHOICE_QUESTION_MODE; 
    // Printing Report Card
    StudentReportCard reportCard(USER_Student, Selected_Exam);
    while (USER_INPUT != "0" && !Logout_FLAG) {
        try {
            Clear();
            std::cout << UP_WINDOW_TILE << std::endl;
            std::cout << "\t#--------- " << USER_Student -> getName() << " " << USER_Student -> getLast_Name() << " ---------#" << std::endl;
            std::cout << "\n\t#---------- EXAM NAME: " << Selected_Exam -> getExamName() << " - GUC: " << Selected_Exam -> getGUC() << " ----------#\n" << std::endl;
            for (int Index = 0; Index < STUDENT_EXAM_REPORT_PAGE_MENU.size(); Index++) {
                std::cout << STUDENT_EXAM_REPORT_PAGE_MENU[Index];
                if (Index == 2) reportCard.showReportCard(QMode);
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
            else if (USER_INPUT == "MCQ" || USER_INPUT == "SFQ" || USER_INPUT == "FFQ") { // Change View
                if (USER_INPUT == "MCQ") QMode = MULTIPLE_CHOICE_QUESTION_MODE;
                else if (USER_INPUT == "SFQ") QMode = SHORT_FORM_QUESTION_MODE;
                else if (USER_INPUT == "FFQ") QMode = FULL_FORM_QUESTION_MODE;
            }
            else if (USER_INPUT == "1") { // Print Report Card
                std::string ADDRESS;
                std::string DEFAULT_ADDRESS = ADDRESS_CREATOR(std::vector<std::string>{".", "reports", "STUDENT_FILES"});
                std::cout << "\n\t #--> DEFAULT ADDRESS: " << DEFAULT_ADDRESS << std::endl;
                InputingSection("ADDRESS TO PRINT REPORT CARD (DEF -> DEFAULT ADDRESS)", ADDRESS);
                if (ADDRESS == "DEF") ADDRESS = DEFAULT_ADDRESS;
                reportCard.printReportCard(ADDRESS);
            } 
            else if (USER_INPUT == "0") { // Exit
                break;
            }
            else { // Wrong Input
                UnSuccessfulMessage(INVALID_INPUT_ERROR);
            }

        } catch (const std::invalid_argument& error) {
            CatchException(error, TWO_SECOND);
        }
    }
}