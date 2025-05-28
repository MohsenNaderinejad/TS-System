#include "../include/Exam.hpp"
#include "../include/GLOBAL_VARIABLES.hpp"
#include "../include/Teacher.h"
#include "../include/Student.h"
#include <fstream>

Exam::Exam(): authorID(""), examName(""), subject(""), time(0), type(""), publishState(false) {
    MCQs  = std::map<long long, multipleChoiceQuestion *> ();
    SFQs  = std::map<long long, ShortFormQuestion *> ();
    FFQs  = std::map<long long, FullFormQuestion *> ();

    studentsByScore = std::set<StudentScore*, studentScoreComparator> ();
}

Exam::Exam(std::string authorID, std::string examName, std::string subject, std::string time, std::string type, std::string publishState, json & Exam_File, json& Question_File) {
    if (examName.empty() || subject.empty() || time.empty() || type.empty() || publishState.empty()) {
        UnSuccessfulMessage("ALL OF THE SLOTS SHOULD BE FILLED");
    }

    this -> authorID = authorID;
    setExamName(examName);
    setSubject(subject);
    setType(type);
    setTime(time);

    this -> GUC = Exam_File[0][0]["NEW_EXAM_GUC"].get < int > ();

    this -> newMCQID = 1;
    this -> newSFQID = 1;
    this -> newFFQID = 1;

    MCQs  = std::map<long long, multipleChoiceQuestion *> ();
    SFQs  = std::map<long long, ShortFormQuestion *> ();
    FFQs = std::map<long long, FullFormQuestion *> ();

    studentsByScore = std::set<StudentScore*, studentScoreComparator> ();

    this->maximumScore = 0;
    this->averageScore = 0;

    setPublishState(publishState);

    Exam_File[0][0]["NEW_EXAM_GUC"] = Exam_File[0][0]["NEW_EXAM_GUC"].get < int > () + 1;

    json exam = {
        { "AUTHOR_ID", this -> authorID },
        { "EXAM_GUC", this -> GUC },
        { "EXAM_NAME", this -> examName },
        { "SUBJECT", this -> subject },
        { "TIME", this -> time },
        { "TYPE", this -> type },
        { "NEW_MCQ_ID", this -> newMCQID },
        { "NEW_SFQ_ID", this -> newSFQID },
        { "NEW_FFQ_ID", this -> newFFQID },
        { "MAXIMUM_SCORE", this -> maximumScore },
        { "AVERAGE_SCORE", this -> averageScore },
        { "PUBLISH_STATE", this -> publishState }
    };
    exam["STUDENT_SCORES"] = json::array();

    json examQuestions = {
        { "EXAM_GUC", this->GUC }
    };
    examQuestions["MCQs"] = json::array();
    examQuestions["SFQs"] = json::array();
    examQuestions["FFQs"] = json::array();
    
    Exam_File[1].push_back(exam);
    Question_File.push_back(examQuestions);
}

Exam::~Exam() {
    // Clean up Multiple Choice Questions
    for (auto& pair : MCQs) {
        delete pair.second;
    }
    MCQs.clear();

    // Clean up Short Form Questions
    for (auto& pair : SFQs) {
        delete pair.second;
    }
    SFQs.clear();

    // Clean up Full Form Questions
    for (auto& pair : FFQs) {
        delete pair.second;
    }
    FFQs.clear();

    // Clean up Student Scores
    for (auto& pair : studentsByScore) {
        delete pair;
    }
    studentsByScore.clear();

}

// Extracting Exam from Json File
void Exam::examSetter(long long ID, json Exam_File, json Question_File, std::string authorID) {
    bool examNotFound = true;
    for (const auto & exam: Exam_File[1]) {
        if (exam["EXAM_GUC"] == ID) {
            if (authorID.empty()){
                if(exam["PUBLISH_STATE"] == false)
                UnSuccessfulMessage("EXAM IS NOT YET PUBLISHED BY THE AUTHOR");
            } else {
                if (exam["AUTHOR_ID"] != authorID) {
                    UnSuccessfulMessage("YOU ARE NOT THE AUTHOR");
                }
            }
            this -> GUC = ID;
            this -> authorID = authorID;
            this -> examName = exam["EXAM_NAME"];
            this -> subject = exam["SUBJECT"];
            this -> time = exam["TIME"].get < int > ();
            this -> type = exam["TYPE"];

            this -> newMCQID = exam["NEW_MCQ_ID"];
            this -> newSFQID = exam["NEW_SFQ_ID"];
            this -> newFFQID = exam["NEW_FFQ_ID"];

            this -> maximumScore = exam["MAXIMUM_SCORE"];
            this -> averageScore = exam["AVERAGE_SCORE"];

            this -> publishState = exam["PUBLISH_STATE"].get<bool>();
            
            this -> studentsByScore = std::set<StudentScore*, studentScoreComparator> ();
            for (const auto& studentByScore: exam["STUDENT_SCORES"]) {
                StudentScore * TMP_studentScore = new StudentScore(studentByScore["STUDENT_ID"], studentByScore["STUDENT_NAME"], studentByScore["STUDENT_SCORE"].get < float > ());
                studentsByScore.emplace(TMP_studentScore);
            }

            examNotFound = false;
            break;
        }
    }
    if (examNotFound) UnSuccessfulMessage("EXAM GUC NOT FOUND");

    // Clear existing questions before loading new ones
    for (auto& pair : MCQs) {
        delete pair.second;
    }
    MCQs.clear();
    
    for (auto& pair : SFQs) {
        delete pair.second;
    }
    SFQs.clear();
    
    for (auto& pair : FFQs) {
        delete pair.second;
    }
    FFQs.clear();

    for (const auto & examQuestions: Question_File){
        if(examQuestions["EXAM_GUC"] == this->GUC) {
            
            // Multiple Answer Questions Extraction
            for (const auto & MCQuestion: examQuestions["MCQs"]) {
                long long MCQID = MCQuestion["MCQID"].get <long long> ();
                std::string Question_Face = MCQuestion["FACE"];
                float correctScore = MCQuestion["CORRECT_SCORE"].get <float> ();
                float wrongScore = MCQuestion["WRONG_SCORE"].get <float> ();
                int multipleAnswerCount = MCQuestion["CHOICES"].size();
                
                // Dynamically allocate memory for the question
                multipleChoiceQuestion* TMP = new multipleChoiceQuestion(MCQID, Question_Face, correctScore, wrongScore);
                
                // Set the correct answer and choices
                for (const auto& choice: MCQuestion["CHOICES"]) {
                    TMP->setChoicesManually(choice["CHOICE_NUMBER"].get<int>(), choice["CHOICE_FACE"], choice["CHOICE_BOOL"].get <bool> ());
                    if (choice["CHOICE_BOOL"].get <bool> ()) {
                        TMP->setCorrectAnswer(choice["CHOICE_NUMBER"].get<int>());
                    }
                }
                TMP->setShuffledToOriginalMap(); // Shuffle the choices after setting them
                
                MCQs[MCQID] = TMP;
            }

            // Short Form Questions Extraction
            for (const auto& SFQuestion: examQuestions["SFQs"]) {
                long long SFQID = SFQuestion["SFQID"].get <long long> ();
                std::string Question_Face = SFQuestion["FACE"];
                float correctScore = SFQuestion["CORRECT_SCORE"].get <float> ();
                float wrongScore = SFQuestion["WRONG_SCORE"].get <float> ();
                std::string Answer = SFQuestion["ANSWER"];

                // Dynamically allocate memory for the question
                ShortFormQuestion* TMP = new ShortFormQuestion(SFQID, Question_Face, Answer, correctScore, wrongScore);
                
                SFQs[SFQID] = TMP;
            }

            // Full Form Questions Extraction
            for (const auto& FFQuestion: examQuestions["FFQs"]) {
                long long FFQID = FFQuestion["FFQID"].get <long long> ();
                std::string Question_Face = FFQuestion["FACE"];

                // Dynamically allocate memory for the question
                FullFormQuestion* TMP = new FullFormQuestion(FFQID, Question_Face);
                
                FFQs[FFQID] = TMP;
            }
        }
    }
}

void Exam::setMaximumScore(){
    // Find the maximum score from the studentsByScore set (First Student)
    this -> maximumScore = (*this -> studentsByScore.begin())->Score;
}

void Exam::setAverageScore(){
    // Calculate the average score from the studentsByScore set
    double TMP_AverageScore = 0;
    for (const auto& student: studentsByScore) {
        TMP_AverageScore += student->Score;
    }
    this -> averageScore = TMP_AverageScore / this -> studentsByScore.size();
}

void Exam::setTime(std::string time) {
    std::regex pattern("^([0-9]+)$");
    if (!regex_match(time, pattern)) {
        UnSuccessfulMessage("TIME SHOULD BE A POSITIVE NUMBER FROM 1 TO ... ");
    }
    int tmp = stoi(time);
    if (tmp == 0) {
        UnSuccessfulMessage("TIME CANNOT BE ZERO");
    }
    this -> time = tmp;
}

void Exam::setPublishState(std::string publish){
    std::regex pattern_YES("^([yY]*[eE]*[sS]*)$");
    std::regex pattern_NO("^([nN]*[oO]*)$");
    if (regex_match(publish, pattern_YES)){
        this->publishState = true;
    } else if (regex_match(publish, pattern_NO)){
        this->publishState = false;
    }
    else {
        UnSuccessfulMessage("IT SHOULD BE EITHER YES OR NO");
    }
}

// ADD QESUTIONS
template<>
void Exam::addQuestion<multipleChoiceQuestion>(multipleChoiceQuestion* question) {
    this->MCQs[this->newMCQID++] = question;
}
template<>
void Exam::addQuestion<ShortFormQuestion>(ShortFormQuestion* question) {
    this->SFQs[this->newSFQID++] = question;
}
template<>
void Exam::addQuestion<FullFormQuestion>(FullFormQuestion* question) {
    this->FFQs[this->newFFQID++] = question;
}

// REMOVE QUESIONS
template<>
void Exam::removeQuestion<MULTIPLE_CHOICE_QUESTION_MODE>(long long questionID) { // CORRECT
    auto TMP_MCQ = MCQs.find(questionID);
    if (TMP_MCQ != MCQs.end()) {
        MCQs.erase(TMP_MCQ);
        return;
    }
    UnSuccessfulMessage("THE QUESTION NUMBER DOES NOT EXISTS");
}
template<>
void Exam::removeQuestion<SHORT_FORM_QUESTION_MODE>(long long questionID) { // CORRECT
    auto TMP_SFQ = SFQs.find(questionID);
    if (TMP_SFQ != SFQs.end()) {
        SFQs.erase(TMP_SFQ);
        return;
    }
    UnSuccessfulMessage("THE QUESTION ID DOES NOT EXISTS");
}
template<>
void Exam::removeQuestion<FULL_FORM_QUESTION_MODE>(long long questionID) { // CORRECT
    auto TMP_FFQ = FFQs.find(questionID);
    if (TMP_FFQ != FFQs.end()) {
        FFQs.erase(TMP_FFQ);
        return;
    }
    UnSuccessfulMessage("THE QUESTION ID DOES NOT EXISTS");
}

// GET QUESTION INFO FOR EDIT
template<>
multipleChoiceQuestion* Exam::getQuestionForEdit(long long questionID) { // NOT IN THE WORK
    auto TMP_MCQ = MCQs.find(questionID);
    if (TMP_MCQ == MCQs.end()) {
        UnSuccessfulMessage("THE QUESTION DOES NOT EXISTS");
    }
    return MCQs[questionID];
}
template<>
ShortFormQuestion* Exam::getQuestionForEdit(long long questionID) { // NOT IN THE WORK
    auto TMP_SFQ = SFQs.find(questionID);
    if (TMP_SFQ == SFQs.end()) {
        UnSuccessfulMessage("THE QUESTION DOES NOT EXISTS");
    }
    return SFQs[questionID];
}
template<>
FullFormQuestion* Exam::getQuestionForEdit(long long questionID) { // NOT IN THE WORK
    auto TMP_FFQ = FFQs.find(questionID);
    if (TMP_FFQ == FFQs.end()) {
        UnSuccessfulMessage("THE QUESTION DOES NOT EXISTS");
    }
    return FFQs[questionID];
}

// SHOWCASE FOR ALL QUESTIONS
template<>
void Exam::viewAllQuestions<MULTIPLE_CHOICE_QUESTION_MODE>(){ // CORRECT
    std::cout << "\n\t----------------------------------------------" << std::endl;
    std::cout << "\n\t #-> MULTIPLE CHOICE QUESTION LIST: " << std::endl;
    if (MCQs.empty()) {
        std::cout << "\n\t --> No Questions Found" << std::endl;
    }
    else {
        std::cout << "\n\t &&& Total Multiple Choice Questions: " << MCQs.size() << std::endl;
    }
    for (const auto& TMP_MCQ: MCQs) {
        std::cout << "\n\t ###-> Question No. " << TMP_MCQ.first << ": ";
        TMP_MCQ.second->questionPrint("TEACHER");
        std::cout << "\n\t----------------------------------------------" << std::endl;
    }
}
template<>
void Exam::viewAllQuestions<SHORT_FORM_QUESTION_MODE>(){ // CORRECT
    std::cout << "\t\n----------------------------------------------" << std::endl;
    std::cout << "\n\t #-> SHORT FORM QUESTION LIST: " << std::endl;
    if (SFQs.empty()) {
        std::cout << "\n\t --> No Questions Found" << std::endl;
    }
    else {
        std::cout << "\n\t &&& Total Short Form Questions: " << SFQs.size() << std::endl;
    }
    for (const auto& TMP_SFQ: SFQs) {
        std::cout << "\n\t ###-> Question No. " << TMP_SFQ.first << ": ";
        TMP_SFQ.second->questionPrint("TEACHER");
        std::cout << "\n\t----------------------------------------------" << std::endl;
    }
}
template<>
void Exam::viewAllQuestions<FULL_FORM_QUESTION_MODE>(){ // CORRECT
    std::cout << "\t\n----------------------------------------------" << std::endl;
    std::cout << "\n\t #-> FULL FORM QUESTION LIST: " << std::endl;
    if (FFQs.empty()) {
        std::cout << "\n\t --> No Questions Found" << std::endl;
    }
    else {
        std::cout << "\n\t &&& Total Full Form Questions: " << FFQs.size() << std::endl;
    }
    for (const auto& TMP_FFQ: FFQs) {
        std::cout << "\n\t ###-> Question No. " << TMP_FFQ.first << ": ";
        TMP_FFQ.second->questionPrint("TEACHER");
        std::cout << "\n\t----------------------------------------------" << std::endl;
    }
}

// SHOW SPECIFIC QUESTION (FOR STUDENT)
template<>
void Exam::showSpecificQuestion(multipleChoiceQuestion* question) { // CORRECT
    std::cout << "\n\tP -> PREVIOUS QUESTION /// N -> NEXT QUESTION\n";
    std::cout << "\n\t#---------------------------------------#\n";
    question->questionPrint("STUDENT");
    std::cout << "\n\t#---------------------------------------#\n";
}

template<>
void Exam::showSpecificQuestion(ShortFormQuestion* question) { // CORRECT
    std::cout << "\n\tP -> PREVIOUS QUESTION /// N -> NEXT QUESTION\n";
    std::cout << "\n\t#---------------------------------------#\n";
    question->questionPrint("STUDENT");
    std::cout << "\n\t#---------------------------------------#\n";
}

template<>
void Exam::showSpecificQuestion(FullFormQuestion* question) { // CORRECT
    std::cout << "\n\tP -> PREVIOUS QUESTION /// N -> NEXT QUESTION\n";
    std::cout << "\n\t#---------------------------------------#\n";
    question->questionPrint("STUDENT");
    std::cout << "\n\t#---------------------------------------#\n";
}

// Calculating Exam Score after finishing the exam
float Exam::examScoreCalculator() { // SUSPECTED TO BE INCORRECT

    // Calculating The Total Score
    // Each question that have been Answered

    for (auto& mcq: MCQs){
        mcq.second->setQuesionBasedStudentScore();
    }
    for (auto& sfq: SFQs){
        sfq.second->setQuesionBasedStudentScore();
    }

    float totalPoints = 0;
    float questionAnswered = 0;
    for (const auto MCQ: MCQs) {
        if (MCQ.second->getStudentQuestionStatus() == ANSWERED_INCORRECTLY 
        || MCQ.second->getStudentQuestionStatus() == ANSWERED_CORRECTLY) {
            totalPoints += MCQ.second->getStudentScore();
            questionAnswered++;
        } 
    }

    for (const auto SFQ: SFQs) {
        if (SFQ.second->getStudentQuestionStatus() == ANSWERED_CORRECTLY
        || SFQ.second->getStudentQuestionStatus() == ANSWERED_INCORRECTLY) {
            totalPoints += SFQ.second->getStudentScore();
            questionAnswered++;
        }
    }
    return totalPoints;
    // return  (totalPoints / questionAnswered) * 20; // Convert to 20 points scale
}


// Calculating Student Rank
int Exam::studentRankCalculator(std::string studentID) { // CORRECT
    
    // Using a rank calculator to extract sepeecific student Rank

    int rank = 1;
    for (const auto& student: studentsByScore) {
        if (student->SDK->ID == studentID) {
            return rank;
        }
        rank++;
    }
    UnSuccessfulMessage("STUDENT NOT FOUND IN THE EXAM");
    return -1;
}

StudentScore* Exam::studentScoreFinderByID(std::string studentID) { // CORRECT
    for (const auto& student: studentsByScore) {
        if (student->SDK->ID == studentID) {
            return student;
        }
    }
    return nullptr;
}

// Updating Exam_File
void Exam::examAndQuestionFileUpdate(json& Exam_File, json& Question_File) { // CORRECT TO THIS MOMENT
    for (auto & exam: Exam_File[1]) {
        if (exam["EXAM_GUC"] == this -> GUC) {

            exam["EXAM_NAME"] = this -> examName;
            exam["SUBJECT"] = this -> subject;
            exam["TIME"] = this -> time;
            exam["TYPE"] = this -> type;

            exam["NEW_MCQ_ID"] = this -> newMCQID;
            exam["NEW_SFQ_ID"] = this -> newSFQID;
            exam["NEW_FFQ_ID"] = this -> newFFQID;

            exam["MAXIMUM_SCORE"] = this -> maximumScore;
            exam["AVERAGE_SCORE"] = this -> averageScore;

            exam["PUBLISH_STATE"] = this -> publishState;

            json studentScoresJson = json::array();
            for (const auto& score: studentsByScore) {
                json TMP_Score;
                TMP_Score["STUDENT_ID"] = score->SDK->ID;
                TMP_Score["STUDENT_NAME"] = score->SDK->Name;
                TMP_Score["STUDENT_SCORE"] = score->Score;
                studentScoresJson.push_back(TMP_Score);
            }
            exam["STUDENT_SCORES"] = studentScoresJson;
        }
    }

    for (auto & examQuestions: Question_File){
        if (examQuestions["EXAM_GUC"] == this -> GUC) {
            // Multiple Choice Question Update
            json TMP_MCQsArray = json::array();
            for (const auto & MCQuestion: MCQs) {
                json TMP_MCQ;
                multipleChoiceQuestion* MCQ = MCQuestion.second;

                TMP_MCQ["MCQID"] = MCQ->getMCQID();
                TMP_MCQ["FACE"] = MCQ->getQuestionFace();
                TMP_MCQ["CORRECT_SCORE"] = MCQ->getCorrectScore();
                TMP_MCQ["WRONG_SCORE"] = MCQ->getWrongScore();

                int FIRST_CHOICE = 1;
                TMP_MCQ["CHOICES"] = json::array();
                for (const auto& choice: MCQ->getChoices()) {
                    json TMP_Choice;
                    TMP_Choice["CHOICE_NUMBER"] = FIRST_CHOICE++;
                    TMP_Choice["CHOICE_FACE"] = choice.second.first;
                    TMP_Choice["CHOICE_BOOL"] = choice.second.second;
                    TMP_MCQ["CHOICES"].push_back(TMP_Choice);
                }

                TMP_MCQsArray.push_back(TMP_MCQ);
            }
            examQuestions["MCQs"] = TMP_MCQsArray;

            // Short Form Question Update
            json TMP_SFQsArray = json::array();
            for (const auto & SFQuestion: SFQs) {
                json TMP_SFQ;
                ShortFormQuestion* SFQ = SFQuestion.second;

                TMP_SFQ["SFQID"] = SFQ->getSFQID();
                TMP_SFQ["FACE"] = SFQ->getQuestionFace();
                TMP_SFQ["CORRECT_SCORE"] = SFQ->getCorrectScore();
                TMP_SFQ["WRONG_SCORE"] = SFQ->getWrongScore();
                TMP_SFQ["ANSWER"] = SFQ->getAnswer();
                
                TMP_SFQsArray.push_back(TMP_SFQ);
            }
            examQuestions["SFQs"] = TMP_SFQsArray;

            // Full Form Question Update
            json TMP_FFQsArray = json::array();
            for (const auto & FFQuestion: FFQs) {
                json TMP_FFQ;
                FullFormQuestion* FFQ = FFQuestion.second;
                TMP_FFQ["FFQID"] = FFQ->getFFQID();
                TMP_FFQ["FACE"] = FFQ->getQuestionFace();

                TMP_FFQsArray.push_back(TMP_FFQ);
            }
            examQuestions["FFQs"] = TMP_FFQsArray;
        }
    }
}

void Exam::addStudentByID(std::string ID, StudentScore* studentScore) { // CORRECT
    this -> studentsByID[ID] = studentScore;
}

// Adding Student Score to the Exam to Store
void Exam::addStudentByScore(Student* Selected_Student) {
    bool studentFound = false;
    auto studentSearch = studentScoreFinderByID(Selected_Student->getID());
    if (studentSearch != nullptr) {
        // Student already exists, remove the old score
        delete studentSearch;
        studentsByScore.erase(studentSearch);
        studentFound = true;
    }
    // Create a new StudentScore object and add it to the set
    float examScore;
    if (studentFound) {
        examScore = Selected_Student->getExams()[this->GUC]->score;
    }
    else {
        examScore = examScoreCalculator();
    }

    StudentScore* newStudentScore = new StudentScore(Selected_Student->getID(), Selected_Student->getLast_Name() + " " + Selected_Student->getName(), examScore);
    studentsByScore.insert(newStudentScore);

    this->addStudentByID(Selected_Student->getID(), newStudentScore);

    // Calculating Maximum Score And Average Score after the Exam to refresh
    // The Exam File with new data
    this -> setMaximumScore();
    this -> setAverageScore();
}

std::string Exam::getPublishState(){ // CORRECT
    if (this->publishState == true) return "YES";
    return "NO";
}

bool Exam::deleteExam(json& Exam_File, json& Question_File) { // NOT GOING BACK YET --> CORRECTED
    std::string flag;
    std::cout << "\n\t !!!! ARE YOU SURE THAT ARE YOU SURE ???? (Y/n): ";
    std::getline(std::cin, flag);
    if (flag == "Y") {
        for (auto exam = Exam_File[1].begin(); exam != Exam_File[1].end(); exam++){
            if ((*exam)["EXAM_GUC"] == this->getGUC()) {
                exam = Exam_File[1].erase(exam);
                break;
            }
        }
        for (auto examQuestions = Question_File.begin(); examQuestions != Question_File.end(); examQuestions++){
            if ((*examQuestions)["EXAM_GUC"] == this->getGUC()) {
                examQuestions = Question_File.erase(examQuestions);   
                SuccessfulMessage("EXAM DELETED SUCCESSFULLY", TWO_SECOND);
                return true;
            }
        }
    }
    return false;
}

// Shoeing Score Board Of The Exam
void ExamScoreBoard::showScoreBoard() {
    // Printing the Score Board of the Exam
    std::cout << "\n\n\t#--------------------- SCORE BOARD ----------------------#\n" << std::endl;
    int Rank = 1;
    for (const auto& student: exam -> studentsByScore){
        if (Rank == 1) {
            std::cout << "\t      RANK  |  STUDENT ID  |  STUDENT NAME  |  SCORE\n" << std::endl;
        }
        std::cout << "\t---------------------------------------------------------" << std::endl;
        std::cout << "\t    " << Rank++ << "  |  "
        << student->SDK->ID << "  |  "
        << student->SDK->Name << "  |  "
        << std::fixed << std::setprecision(2) << student->Score << std::endl;
    }
    std::cout << "\t#--------------------------------------------------------#" << std::endl;
}

void ExamScoreBoard::printScoreBoard(std::string ADDRESS) {
    std::string ADDRESS_ID = ADDRESS_CREATOR(std::vector<std::string>{ADDRESS, RWD(teacher -> getID()) + "-" + RWD(teacher -> getName()) + "-" + RWD(teacher -> getLast_Name())});
    CreateDirectory(ADDRESS_ID);

    std::string EXAM_SCORE_BOARD = "EXAM-SCOREBOARD-" 
                                    + std::to_string(exam -> getGUC()) 
                                    + "-" + RWD(exam -> getExamName()) 
                                    + "-" + RWD(teacher -> getID())
                                    + "-" + RWD(teacher -> getName()) 
                                    + "-" + RWD(teacher -> getLast_Name())
                                    + ".csv";
    
    std::string EXAM_SCORE_BOARD_ADDRESS = ADDRESS_CREATOR(std::vector<std::string>{ADDRESS_ID, EXAM_SCORE_BOARD});
    writeBufferESC(EXAM_SCORE_BOARD_ADDRESS);

}

void ExamScoreBoard::writeBufferESC(std::string ADDRESS) {
    std::ostringstream buffer;
    // Header
    buffer << "\"RANK\",\"Student ID\",\"Student Name\",\"Student Score\",\"\",\"\",\"MAXIMUM SCORE\",\"AVERAGE SCORE\"\n";
    int RANK = 1;
    for (const auto& student: exam -> studentsByScore) {
        buffer << RANK++ << ","
               << quote(student->SDK->ID) << ","
               << quote(student->SDK->Name) << ","
               << std::fixed << std::setprecision(2) << student->Score;

        if (RANK == 2) {
            buffer << ",,,"
                   << std::fixed << std::setprecision(2) << exam -> getMaximumScore() << ","
                   << std::fixed << std::setprecision(2) << exam -> getAverageScore();
        }

        buffer << "\n";
    }

    std::ofstream examScoreBoard(ADDRESS);
    examScoreBoard << buffer.str();
    examScoreBoard.close();
    SuccessfulMessage("EXAM SCORE BOARD SVAED IN:\n\n\t " + ADDRESS, THREE_SECOND + ONE_SECOND);
}

Student* ExamScoreBoard::studentFinder(json& Student_File, std::string studentID) {
    for (auto& student : Student_File) {
        if (student["ID"] == studentID) {
            Student* SELECTED = new Student();
            SELECTED->setTeacherGradingMode(true);
            SELECTED->login(studentID, student["PASSWD"], Student_File);
            return SELECTED;
        }
    }
    UnSuccessfulMessage("STUDENT ID DOES NOT EXIST");
    return nullptr;
}

void ExamScoreBoard::SFFQsGrading(Student* student) {
    float TMP_Score = 0;
    auto originalExamData = student->getExams()[exam->getGUC()];
    for (auto& question : originalExamData -> getSFFQs()) {
        question.second->setStudentScore(question.second->getStudentScore());
        TMP_Score += question.second->getStudentScore() - question.second->getPreviousScore();
    }
    originalExamData->score += TMP_Score;
}