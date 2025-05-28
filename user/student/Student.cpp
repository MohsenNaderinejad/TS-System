#include "../include/Student.h"
#include "../include/Exam.hpp"
#include <fstream>
#include <sstream>

Student::Student(): User(), Field_Of_Study(""), Exams() {
}

Student::Student(std::string name, std::string last_name, std::string email, std::string passwd, std::string ID, json & Student_File, std::string field_of_study, std::string gender, int age): User(name, last_name, email, passwd, ID, Student_File, gender, age),
    Field_Of_Study(field_of_study) {
        for (auto & person: Student_File) {
            if (person["ID"] == ID) {
                person["EXAM_LIST"] = json::array();
                person["FOS"] = field_of_study;
            }
        }
        SuccessfulMessage("SIGNED UP SUCCESSFULLY", TWO_SECOND);
    }


void Student::login(std::string ID, std::string passwd, json & Student_File) {
    if (ID.empty() || passwd.empty()) {
        UnSuccessfulMessage("ID AND PASSWORD CANNOT BE EMPTY");
    }
    this -> setID(ID);
    for (const auto & person: Student_File) {
        if (ID == person["ID"] && passwd == person["PASSWD"]) {
            this -> Name = person["NAME"];
            this -> Last_Name = person["LAST_NAME"];
            this -> ID = person["ID"];
            this -> Email = person["EMAIL"];
            this -> Gender = person["GENDER"];
            this -> Age = person["AGE"];
            this -> PassWD = person["PASSWD"];
            this -> Field_Of_Study = person["FOS"];

            this -> Exams.clear();
            for (const auto & exam: person["EXAM_LIST"]) {
                long long exam_guc = exam["EXAM_GUC"].get < long long > ();
                examHolder* TMP = new examHolder(exam["PARTICIPATION_STATUS"].get < studentExamMode > (), exam["SCORE"].get < float > ());
                Exams[exam_guc] = TMP;
                for (const auto & question: exam["MCQs"]) {
                    long long questionID = question["MCQID"].get < long long > ();
                    TMP -> SMCQs[questionID] = new multipleChoiceQuestion(questionID);
                    TMP -> SMCQs[questionID] -> setQuestionFace(question["FACE"]);
                    TMP -> SMCQs[questionID] -> setCorrectAnswer(question["CORRECT_ANSWER"]["CHOICE_NUMBER"].get < int > (), question["CORRECT_ANSWER"]["CHOICE_FACE"]);
                    TMP -> SMCQs[questionID] -> setStudentAnswer(question["STUDENT_ANSWER"]["CHOICE_NUMBER"].get < int > (), question["STUDENT_ANSWER"]["CHOICE_FACE"]);
                    TMP -> SMCQs[questionID] -> setStudentScore(question["SCORE"].get < float > ());
                    TMP -> SMCQs[questionID] -> setStudentQuestionStatus(question["ANSWER_STATUS"].get < QuestionStatus > ());
                }
                for (const auto & question: exam["SFQs"]) {
                    long long questionID = question["SFQID"].get < long long > ();
                    TMP -> SSFQs[questionID] = new ShortFormQuestion(questionID);
                    TMP -> SSFQs[questionID] -> setQuestionFace(question["FACE"]);
                    TMP -> SSFQs[questionID] -> setStudentAnswer(question["STUDENT_ANSWER"]);
                    TMP -> SSFQs[questionID] -> setStudentScore(question["SCORE"].get < float > ());
                    TMP->SSFQs[questionID] -> setAnswer(question["CORRECT_ANSWER"].get<std::string>());
                    TMP -> SSFQs[questionID] -> setStudentQuestionStatus(question["ANSWER_STATUS"].get < QuestionStatus > ());
                }
                for (const auto & question: exam["FFQs"]) {
                    long long questionID = question["FFQID"].get < long long > ();
                    TMP -> SFFQs[questionID] = new FullFormQuestion(questionID);
                    TMP -> SFFQs[questionID] -> setQuestionFace(question["FACE"]);
                    TMP -> SFFQs[questionID] -> setStudentAnswer(question["STUDENT_ANSWER"]);
                    TMP -> SFFQs[questionID] -> setStudentScore(question["TEACHER_SCORE"].get < float > ());
                    TMP -> SFFQs[questionID] -> setPreviousScore(question["TEACHER_SCORE"].get < float > ());
                    TMP -> SFFQs[questionID] -> setStudentQuestionStatus(question["ANSWER_STATUS"].get < QuestionStatus > ());
                    TMP -> SFFQs[questionID] -> setOriginalQuestionStatus(question["ORIGINAL_ANSWER_STATUS"].get < QuestionStatus > ());
                }
            }

            if (!teacherGradingMode) {
                std::string WELCOME = "WELCOME " + this -> Name + " " + this -> Last_Name;
                SuccessfulMessage("LOGGED IN SUCCESSFULLY", TWO_SECOND);
                SuccessfulMessage(WELCOME, TWO_SECOND);
            }
            return;
        }
    }
    throw std::invalid_argument("\n --> \tID NOT FOUND! / PASSWORD INCORRECT!\n");
}

void Student::studentFileUpdate(json & Student_File) {
    for (auto & person: Student_File) {
        if (person["ID"] == this -> ID) {
            person["AGE"] = this -> Age;
            person["EMAIL"] = this -> Email;
            person["FOS"] = this -> Field_Of_Study;
            person["GENDER"] = this -> Gender;
            person["LAST_NAME"] = this -> Last_Name;
            person["NAME"] = this -> Name;
            person["PASSWD"] = this -> PassWD;

            // Iterating through the exams and adding them to the JSON object
            json exams_list = json::array();

            for (auto exam: this -> Exams) {
                json specific_exam = json::object();

                specific_exam["EXAM_GUC"] = exam.first;
                specific_exam["PARTICIPATION_STATUS"] = exam.second -> examMode;
                specific_exam["SCORE"] = exam.second -> score;
                
                // MCQs
                json mcqs_list = json::array();
                for (const auto & question: exam.second -> SMCQs) {
                    json mcq;
                    mcq["MCQID"] = question.first;
                    mcq["FACE"] = question.second -> getQuestionFace();
                    
                    json correct_answer = {
                        {"CHOICE_NUMBER", question.second -> getCorrectAnswer().first},
                        {"CHOICE_FACE", question.second -> getCorrectAnswer().second}
                    };
                    mcq["CORRECT_ANSWER"] = correct_answer;
                    
                    json student_answer = {
                        {"CHOICE_NUMBER", question.second -> getStudentAnswer().first},
                        {"CHOICE_FACE", question.second -> getStudentAnswer().second}
                    };
                    mcq["STUDENT_ANSWER"] = student_answer;
                    
                    mcq["SCORE"] = question.second -> getStudentScore();
                    mcq["ANSWER_STATUS"] = question.second -> getStudentQuestionStatus();
                    
                    mcqs_list.push_back(mcq);
                }
                specific_exam["MCQs"] = mcqs_list;

                // SFQs
                json sfqs_list = json::array();
                for (const auto & question: exam.second -> SSFQs) {
                    json sfq;
                    sfq["SFQID"] = question.first;
                    sfq["FACE"] = question.second -> getQuestionFace();
                    sfq["CORRECT_ANSWER"] = question.second -> getAnswer();
                    sfq["STUDENT_ANSWER"] = question.second -> getStudentAnswer();
                    sfq["SCORE"] = question.second -> getStudentScore();
                    sfq["ANSWER_STATUS"] = question.second -> getStudentQuestionStatus();

                    sfqs_list.push_back(sfq);
                }
                specific_exam["SFQs"] = sfqs_list;

                // FFQs
                json ffqs_list = json::array();
                for (const auto & question: exam.second -> SFFQs) {
                    json ffq;
                    ffq["FFQID"] = question.first;
                    ffq["FACE"] = question.second -> getQuestionFace();
                    ffq["STUDENT_ANSWER"] = question.second -> getStudentAnswer();
                    ffq["TEACHER_SCORE"] = question.second -> getStudentScore();
                    ffq["ANSWER_STATUS"] = question.second -> getStudentQuestionStatus();
                    ffq["ORIGINAL_ANSWER_STATUS"] = question.second -> getOriginalQuestionStatus();

                    ffqs_list.push_back(ffq);
                }
                specific_exam["FFQs"] = ffqs_list;
                
                exams_list.push_back(specific_exam);
            }
            // Updating the exam list in the Student_File
            person["EXAM_LIST"].clear();
            person["EXAM_LIST"] = exams_list;
        }
    }
}

void Student::showExams() {
    std::cout << "\n\n\t#----------- EXAMS ENROLLED -----------#\n\n";
    int Index = 1;
    for (const auto exam: this -> Exams) {
        if (exam.second->examMode == ENROLLED) {
            std::cout << "\t # - " << Index << ". " << exam.first << "\n" << std::endl;
            Index++;
        }
    }
    std::cout << "\t#--------------------------------------#\n";
    std::cout << "\n\t#--------- EXAMS PARTICIPATED ---------#\n\n";
    Index = 1;
    for (const auto exam: this -> Exams) {
        if (exam.second->examMode == PARTICIPATED) {
            std::cout << "\t # - " << Index << ". " << exam.first << " --> Score: " << std::fixed << std::setprecision(2) << exam.second->score << "\n" << std::endl;
            Index++;
        }
    }
    std::cout << "\t#--------------------------------------#";
}

void Student::insertExam(long long exam_guc, json & Exam_File, json & Question_File, json & Student_File) {
    for (const auto & exam: Exam_File[1]) {
        if (exam["EXAM_GUC"] == exam_guc) {
            if (exam["PUBLISH_STATE"] == false) {
                UnSuccessfulMessage("EXAM IS NOT YET PUBLISHED BY THE AUTHOR");
                return;
            }
            if (this -> Exams.find(exam_guc) != this -> Exams.end()) {
                UnSuccessfulMessage("EXAM ALREADY ENROLLED");
                return;
            }
            examHolder* TMP = new examHolder();

            for (const auto & examQuestion: Question_File) {
                if (examQuestion["EXAM_GUC"] == exam_guc) {
                    // Iterating though the questions in the Exam_File in MCQs, SFQs, FFQs
                    //                                              |
                    //                                              |
                    //                                              |
                    //                                              |
                    //                                              |
                    //                                              V
                    for (const auto & question: examQuestion["MCQs"]) {
                        long long questionID = question["MCQID"].get < long long > ();
                        TMP -> SMCQs[questionID] = new multipleChoiceQuestion(questionID);
                        TMP -> SMCQs[questionID] -> setQuestionFace(question["FACE"]);
                        // Getting Correct Choice
                        for (const auto & choice: question["CHOICES"]) {
                            if (choice["CHOICE_BOOL"].get < bool > ()) TMP -> SMCQs[questionID] -> setCorrectAnswer(choice["CHOICE_NUMBER"].get < int > (), choice["CHOICE_FACE"]);
                        }
                    }
                    for (const auto & question: examQuestion["SFQs"]) {
                        long long questionID = question["SFQID"].get < long long > ();
                        TMP -> SSFQs[questionID] = new ShortFormQuestion(questionID);
                        TMP -> SSFQs[questionID] -> setQuestionFace(question["FACE"]);
                        TMP -> SSFQs[questionID] -> setAnswer(question["ANSWER"]);
                    }
                    for (const auto & question: examQuestion["FFQs"]) {
                        long long questionID = question["FFQID"].get < long long > ();
                        TMP -> SFFQs[questionID] = new FullFormQuestion(questionID);
                        TMP -> SFFQs[questionID] -> setQuestionFace(question["FACE"]);
                        TMP -> SFFQs[questionID] -> setOriginalQuestionStatus(NOT_ANSWERD);
                    }
                    this -> Exams[exam_guc] = TMP;
                    return;
                }
            }
            
        }
    }
    UnSuccessfulMessage("EXAM GUC NOT FOUND");
}

void Student::updateExamInStudentClass(long long exam_guc, Exam* Selected_Exam) {
    auto exam = this->Exams.find(exam_guc);
    exam->second->examMode = PARTICIPATED; // Update participation status
    exam->second->score = Selected_Exam->examScoreCalculator();

    for (const auto& question : Selected_Exam->getMCQ()) {
        exam->second->SMCQs[question.first]->setQuestionFace(question.second->getQuestionFace());
        exam->second->SMCQs[question.first]->setCorrectAnswer(question.second->getCorrectAnswer().first, question.second->getCorrectAnswer().second);
        if (question.second->getStudentQuestionStatus() != NOT_ANSWERD) {
            exam->second->SMCQs[question.first]->setStudentAnswer(question.second->getStudentAnswer().first, question.second->getStudentAnswer().second);
        }
        exam->second->SMCQs[question.first]->setStudentScore(question.second->getStudentScore());
        exam->second->SMCQs[question.first]->setStudentQuestionStatus(question.second->getStudentQuestionStatus());
    }
    for (const auto& question : Selected_Exam->getSFQ()) {
        exam->second->SSFQs[question.first]->setQuestionFace(question.second->getQuestionFace());
        exam->second->SSFQs[question.first]->setAnswer(question.second->getAnswer());
        if (question.second->getStudentQuestionStatus() != NOT_ANSWERD) {
            exam->second->SSFQs[question.first]->setStudentAnswer(question.second->getStudentAnswer());
        }
        exam->second->SSFQs[question.first]->setStudentScore(question.second->getStudentScore());
        exam->second->SSFQs[question.first]->setStudentQuestionStatus(question.second->getStudentQuestionStatus());
    }
    for (const auto& question : Selected_Exam->getFFQ()) {
        exam->second->SFFQs[question.first]->setQuestionFace(question.second->getQuestionFace());
        if (question.second->getStudentQuestionStatus() != NOT_ANSWERD) {
            exam->second->SFFQs[question.first]->setStudentAnswer(question.second->getStudentAnswer());
        }
        exam->second->SFFQs[question.first]->setStudentScore(question.second->getStudentScore());
        exam->second->SFFQs[question.first]->setStudentQuestionStatus(question.second->getStudentQuestionStatus());
        exam->second->SFFQs[question.first]->setOriginalQuestionStatus(question.second->getOriginalQuestionStatus());
    }
}

bool Student::checkExamForParticipation(long long exam_guc) {
    auto exam = Exams.find(exam_guc);
    if (exam == Exams.end()) {
        UnSuccessfulMessage("EXAM GUC NOT FOUND");
        return false;
    }
    if (exam -> second->examMode == PARTICIPATED) {
        UnSuccessfulMessage("EXAM ALREADY PARTICIPATED");
        return false;
    }
    return true;
}

bool Student::checkExamForReportCard(long long exam_guc) {
    auto exam = Exams.find(exam_guc);
    if (exam == Exams.end()) {
        UnSuccessfulMessage("EXAM GUC NOT FOUND");
        return false;
    }
    if (exam -> second->examMode == ENROLLED) {
        UnSuccessfulMessage("EXAM NOT PARTICIPATED YET");
        return false;
    }
    return true;
}

StudentReportCard::StudentReportCard(Student* student, Exam* Selected_Exam) {
    this -> exam = Selected_Exam;
    this -> student = student;
    this -> examHolder = student -> Exams[exam -> getGUC()];
    auto examInExamHolder = student -> Exams.find(exam -> getGUC());
    if (examInExamHolder == student -> Exams.end()) {
        UnSuccessfulMessage("EXAM GUC NOT FOUND");
        return;
    }
    if (examInExamHolder -> second->examMode == ENROLLED) {
        UnSuccessfulMessage("EXAM NOT PARTICIPATED YET");
        return;
    }
    this -> Score = this -> student -> Exams[exam -> getGUC()] -> score;
}

void StudentReportCard::showReportCard(Mode QMode) {
    std::cout << "\n\t#--------------------- REPORT CARD ---------------------#\n" << std::endl;
    std::cout << " &&& - Exam GUC: " << this -> exam -> getGUC() << std::endl;
    std::cout << " &&& - Exam Name: " << this -> exam -> getExamName() << "\n" << std::endl;
    std::cout << " @@@ - Maximum Score: " << std::fixed << std::setprecision(2) << this -> exam -> getMaximumScore() << std::endl;
    std::cout << " @@@ - Average Score: " << std::fixed << std::setprecision(2) << this -> exam -> getAverageScore() << "\n" << std::endl;
    std::cout << " $$$ - Score: " << std::fixed << std::setprecision(2) << this -> Score << std::endl;
    std::cout << " $$$ - Rank: " << this -> exam -> studentRankCalculator(this -> student -> getID()) << "\n" << std::endl;
    if (QMode == MULTIPLE_CHOICE_QUESTION_MODE) {
        for (const auto & mcq : this -> examHolder -> SMCQs) {
            mcq.second -> reportQuestionPrint();
        }
    }
    else if (QMode == SHORT_FORM_QUESTION_MODE) {
        for (const auto & sfq : this -> examHolder -> SSFQs) {
            sfq.second -> reportQuestionPrint();
        }
    } else if (QMode == FULL_FORM_QUESTION_MODE) {
        for (const auto & ffq : this -> examHolder -> SFFQs) {
            ffq.second -> reportQuestionPrint();
        }
    }
    std::cout << "\t#-------------------------------------------------------#" << std::endl;
}

void StudentReportCard::printReportCard(std::string ADDRESS) {
    std::string ADDRESS_ID = ADDRESS_CREATOR(std::vector<std::string>{ADDRESS, RWD(student -> getID()) + "-" + RWD(student -> getName()) + "-" + RWD(student -> getLast_Name())});
    CreateDirectory(ADDRESS_ID);

    std::string ADDRESS_ID_GUC = ADDRESS_CREATOR(std::vector<std::string>{ADDRESS_ID, std::to_string(exam -> getGUC()) + "-" + RWD(exam -> getExamName())});
    CreateDirectory(ADDRESS_ID_GUC);

    std::string MCQ_REPORT_UNIQUE = "MCQ-REPORT-" 
                                    + RWD(student -> getID())
                                    + "-" + RWD(student -> getName()) 
                                    + "-" + RWD(student -> getLast_Name()) 
                                    + "-" + std::to_string(exam -> getGUC()) 
                                    + "-" + RWD(exam -> getExamName()) 
                                    + ".csv";

    std::string SFQ_REPORT_UNIQUE = "SFQ-REPORT-" 
                                    + RWD(student -> getID()) 
                                    + "-" + RWD(student -> getName())
                                    + "-" + RWD(student -> getLast_Name()) 
                                    + "-" + std::to_string(exam -> getGUC()) 
                                    + "-" + RWD(exam -> getExamName()) 
                                    + ".csv";

    std::string FFQ_REPORT_UNIQUE = "FFQ-REPORT-"
                                    + RWD(student -> getID()) 
                                    + "-" + RWD(student -> getName()) 
                                    + "-" + RWD(student -> getLast_Name()) 
                                    + "-" + std::to_string(exam -> getGUC()) 
                                    + "-" + RWD(exam -> getExamName()) 
                                    + ".csv";
    
    std::string GENERAL_REPORT_UNIQUE = "GENERAL-REPORT-"
                                        + RWD(student -> getID()) 
                                        + "-" + RWD(student -> getName()) 
                                        + "-" + RWD(student -> getLast_Name()) 
                                        + "-" + std::to_string(exam -> getGUC()) 
                                        + "-" + RWD(exam -> getExamName()) 
                                        + ".csv";

    std::string ADDRESS_ID_GUC_MCQ = ADDRESS_CREATOR(std::vector<std::string>{ADDRESS_ID_GUC, MCQ_REPORT_UNIQUE});
    std::string ADDRESS_ID_GUC_SFQ = ADDRESS_CREATOR(std::vector<std::string>{ADDRESS_ID_GUC, SFQ_REPORT_UNIQUE});
    std::string ADDRESS_ID_GUC_FFQ = ADDRESS_CREATOR(std::vector<std::string>{ADDRESS_ID_GUC, FFQ_REPORT_UNIQUE});
    std::string ADDRESS_ID_GUC_GENERAL = ADDRESS_CREATOR(std::vector<std::string>{ADDRESS_ID_GUC, GENERAL_REPORT_UNIQUE});
    writeBufferCSV_MCQ(ADDRESS_ID_GUC_MCQ);
    writeBufferCSV_SFQ(ADDRESS_ID_GUC_SFQ);
    writeBufferCSV_FFQ(ADDRESS_ID_GUC_FFQ);
    writeBufferCSV_GENERAL(ADDRESS_ID_GUC_GENERAL);

}

void StudentReportCard::writeBufferCSV_MCQ(std::string ADDRESS) {
    std::ostringstream buffer;
    // Header
    buffer << "\"Question Number\",\"Question Face\",\"Correct Answer\",\"Student Answer\",\"Student Asnwer Status\",\"Question Score\"\n";
    for (const auto & mcq : this -> examHolder -> SMCQs) {
        std::string SAS; // Student Answer Status

        if (mcq.second -> getStudentQuestionStatus() == ANSWERED_CORRECTLY) {
            SAS = "CORRECT";
        } else if (mcq.second -> getStudentQuestionStatus() == ANSWERED_INCORRECTLY) {
            SAS = "INCORRECT";
        }

        buffer << mcq.first << "," 
        << quote(mcq.second -> getQuestionFace()) << "," 
        << quote(mcq.second -> getCorrectAnswer().second) << "," 
        << quote(mcq.second -> getStudentAnswer().second) << ","
        << SAS << ","
        << std::fixed << std::setprecision(2) << mcq.second -> getStudentScore() << "\n";
    }

    std::ofstream studentMCQFile(ADDRESS);
    studentMCQFile << buffer.str();
    studentMCQFile.close();
    SuccessfulMessage("MCQ REPORT GENERATED SUCCESSFULLY IN:\n\n\t\t " + ADDRESS, TWO_SECOND + TWO_SECOND);
}
void StudentReportCard::writeBufferCSV_SFQ(std::string ADDRESS) {
    std::ostringstream buffer;
    // Header
    buffer << "\"Question Number\",\"Question Face\",\"Correct Answer\",\"Student Answer\",\"Student Asnwer Status\",\"Question Score\"\n";
    for (const auto & sfq : this -> examHolder -> SSFQs) {
        std::string SAS; // Student Answer Status
        if (sfq.second -> getStudentQuestionStatus() == ANSWERED_CORRECTLY) {
            SAS = "CORRECT";
        } else if (sfq.second -> getStudentQuestionStatus() == ANSWERED_INCORRECTLY) {
            SAS = "INCORRECT";
        }
        buffer << sfq.first << "," 
        << quote(sfq.second -> getQuestionFace()) << "," 
        << quote(sfq.second -> getAnswer()) << "," 
        << quote(sfq.second -> getStudentAnswer()) << "," 
        << SAS << ","
        << std::fixed << std::setprecision(2) << sfq.second -> getStudentScore() << "\n";
    }

    std::ofstream studentSFQFile(ADDRESS);
    studentSFQFile << buffer.str();
    studentSFQFile.close();
    SuccessfulMessage("SFQ REPORT GENERATED SUCCESSFULLY IN:\n\n\t\t " + ADDRESS, TWO_SECOND + TWO_SECOND);
}
void StudentReportCard::writeBufferCSV_FFQ(std::string ADDRESS) {
    std::ostringstream buffer;
    
    buffer << "\"Question Number\",\"Question Face\",\"Student Answer\",\"Question Score\"\n";
    for (const auto & ffq : this -> examHolder -> SFFQs) {
        buffer << ffq.first << "," 
        << quote(ffq.second -> getQuestionFace()) << "," 
        << quote(ffq.second -> getStudentAnswer()) << "," 
        << std::fixed << std::setprecision(2) << ffq.second -> getStudentScore() << "\n";
    }

    std::ofstream studentFFQFile(ADDRESS);
    studentFFQFile << buffer.str();
    studentFFQFile.close();
    SuccessfulMessage("FFQ REPORT GENERATED SUCCESSFULLY IN:\n\n\t\t " + ADDRESS, TWO_SECOND + TWO_SECOND);
}
void StudentReportCard::writeBufferCSV_GENERAL(std::string ADDRESS) {
    std::ostringstream buffer;
    
    buffer << "\"Exam Generated Unique Code\",\"Exam Name\",\"Maximum Score\",\"Average Score\",\"Total Score\",\"Rank\"\n";
    buffer << this -> exam -> getGUC() << "," 
    << quote(this -> exam -> getExamName()) << "," 
    << std::fixed << std::setprecision(2) << this -> exam -> getMaximumScore() << "," 
    << std::fixed << std::setprecision(2) << this -> exam -> getAverageScore() << "," 
    << std::fixed << std::setprecision(2) << this -> Score << "," 
    << this -> exam -> studentRankCalculator(this -> student -> getID()) << "\n";
    
    std::ofstream studentGeneralFile(ADDRESS);
    studentGeneralFile << buffer.str();
    studentGeneralFile.close();
    SuccessfulMessage("GENERAL REPORT GENERATED SUCCESSFULLY IN:\n\n\t\t " + ADDRESS, TWO_SECOND + TWO_SECOND);
}