#include "../include/Question.hpp"
#include "../include/GLOBAL_VARIABLES.hpp"
#include "../include/Teacher.h"
#include "../include/Student.h"
#include "../include/Exam.hpp"
#include <random> // Added this include for std::shuffle
#include <regex> // Added this include for std::regex

// Question class implementations --> BASIC INTERFACE
Question::Question() 
    : Question_Face(""), correctScore(0), wrongScore(0), studentScore(0), studentQuestionStatus(NOT_ANSWERD) {
}

Question::Question(std::string question, float correctScore, float wrongScore) 
    : Question_Face(question), correctScore(correctScore), studentQuestionStatus(NOT_ANSWERD) {
        setWrongScore(wrongScore);
        studentScore = 0;
}

std::string Question::getQuestionFace() const {
    return Question_Face;
}

float Question::getCorrectScore() const {
    return correctScore;
}

float Question::getWrongScore() const {
    return wrongScore;
}

QuestionStatus Question::getStudentQuestionStatus() const {
    return studentQuestionStatus;
}

void Question::setQuestionFace(std::string question) {
    Question_Face = question;
}

void Question::setCorrectScore(float score) {
    correctScore = score;
}

void Question::setWrongScore(float score) {
    wrongScore = 0;
    if (score > 0) {
        wrongScore = -score;
    } else {
        wrongScore = score;
    }
}

void Question::setStudentQuestionStatus(QuestionStatus state) {
    studentQuestionStatus = state;
}

float Question::getStudentScore() const {
    return studentScore;
}

void Question::setStudentScore(float score) {
    this->studentScore = score;
}

// multipleChoiceQuestion implementations
multipleChoiceQuestion::multipleChoiceQuestion(long long MCQID) 
    : MCQID(MCQID), Question() {
    // Initialize correctAnswer and studentAnswerPair with default values
    correctAnswer = std::make_pair(-1, "");
    studentAnswerPair = std::make_pair(-1, "");
}

multipleChoiceQuestion::multipleChoiceQuestion(long long MCQID, std::string question, int correctScore, int wrongScore)
    : MCQID(MCQID), Question(question, correctScore, wrongScore) {
}

long long multipleChoiceQuestion::getMCQID() { 
    return MCQID; 
}

void multipleChoiceQuestion::getMCQID(long long MCQID) { 
    this->MCQID = MCQID; 
}

void multipleChoiceQuestion::setChoicesManually(int index, std::string choiceFace, bool correctness) {
    std::pair <std::string, bool> TMP_Choice;
    TMP_Choice.first = choiceFace;
    TMP_Choice.second = correctness;
    Choice[index] = TMP_Choice;
}

void multipleChoiceQuestion::setChoicesByMap(std::map<int, std::pair<std::string, bool>> Choices) { // CORRECT
    // Iterating Through Given Map and putting it in the Choices
    this->Choice.clear();
    for (const auto choice: Choices) {
        if (choice.first != -1) {
            std::pair<std::string, bool> TMP_Choice;
            TMP_Choice.first = choice.second.first;
            TMP_Choice.second = choice.second.second;
            this->Choice[choice.first] = TMP_Choice;
        }
    }
    setShuffledToOriginalMap(); // Shuffle the choices after setting them
}

void multipleChoiceQuestion::setShuffledToOriginalMap() { // CORRECT

    // Making a vector of key to shuffle in the future
    std::vector<int> keys;
    for (const auto& choice : Choice) {
        keys.push_back(choice.first);
    }
    if (keys.empty()) {
        UnSuccessfulMessage("NO CHOICES AVAILABLE");
        return;
    }

    // Making a random seed for Shuffle using MCQID and random_device
    std::random_device rd;
    std::seed_seq seed{static_cast<unsigned int>(MCQID), rd()};
    std::mt19937 g(seed);
    std::shuffle(keys.begin(), keys.end(), g);

    shuffledToOriginalMap.clear(); // Clear the mapping
    int ANSWER_INDEX = 1;
    for (const auto& key : keys) {
        // SHUFFELD INDEX <--> ORIGINAL INDEX
        shuffledToOriginalMap[ANSWER_INDEX] = key; // Map shuffled index to original index
        ANSWER_INDEX++;
    }
}

std::map<int, int> multipleChoiceQuestion::getShuffledToOriginalMap() const {
    return shuffledToOriginalMap;
}

void multipleChoiceQuestion::setStudentAnswer(int studentAnswer) {
    
    // Checking if the student Answer Exists
    //
    // Student Puts the shuffled index he/she sees but
    // in the background his chosen index changges to the original index that he/she does not see

    if (shuffledToOriginalMap.find(studentAnswer) == shuffledToOriginalMap.end()) {
        UnSuccessfulMessage("INVALID ANSWER INDEX");
        return;
    }

    // Chnaging Shuffled index to ORIGNAL
    int originalIndex = shuffledToOriginalMap[studentAnswer];
    // Setting Student answer
    studentAnswerPair.first = originalIndex;
    studentAnswerPair.second = Choice[originalIndex].first;
    
    // Change status to answerd and calculating the score
    setStudentQuestionStatus(ANSWERED);
    setQuesionBasedStudentScore();
}

int multipleChoiceQuestion::newChoiceNumber() const {
    auto last_choice = std::prev(this->Choice.end());
    return last_choice->first;
}

void multipleChoiceQuestion::setQuesionBasedStudentScore() {
    // Checking if the student answered --> I set it to answerd but its for precaution
    if (getStudentQuestionStatus() == ANSWERED) {
        if (getStudentAnswer().first == getCorrectAnswerIndex()) {
            //                               ^  
            //                               |       
            // Correct Index is an original Index
            setStudentScore(getCorrectScore());
            setStudentQuestionStatus(ANSWERED_CORRECTLY);
        } else {
            setStudentScore(getWrongScore());
            setStudentQuestionStatus(ANSWERED_INCORRECTLY);
        }
    }
}

std::map<int, std::pair<std::string, bool>> multipleChoiceQuestion::getChoices() {
    return Choice;
}

std::pair<int, std::pair<std::string, bool>> multipleChoiceQuestion::getChoicePair(int index) const {
    std::pair<int, std::pair<std::string, bool>> TMP;
    TMP.first = index;
    TMP.second = Choice.at(index);
    return TMP;
}

int multipleChoiceQuestion::getCorrectAnswerIndex() const {
    for (const auto& answer: Choice){
        if (answer.second.second == true) {
            return answer.first;
        }
    }
    return -1;
}

void multipleChoiceQuestion::setCorrectAnswer(int index) { // CORRECT
    // Its for changing the correct answer in the add question page
    // So it checks if the index exists and nothings wrong
    if (Choice.find(index) == Choice.end()) {
        UnSuccessfulMessage("NOT A VALID ANSWER");
    }
    std::pair<int, std::string> TMP_Choice;
    TMP_Choice.first = index;
    TMP_Choice.second = Choice[index].first;
    correctAnswer = TMP_Choice;
}

void multipleChoiceQuestion::setCorrectAnswer(int index, std::string choiceFace) {
    // Its for setting correct answer for inserting new exam in Student Class
    std::pair<int, std::string> TMP_Choice;
    TMP_Choice.first = index;
    TMP_Choice.second = choiceFace;
    correctAnswer = TMP_Choice;
}

std::pair<int, std::string> multipleChoiceQuestion::getCorrectAnswer() const {
    return correctAnswer;
}

void multipleChoiceQuestion::changeCorrectAnswer(int newChoiceIndex) { // CORRECT
    int previousCorrectAnswerIndex = getCorrectAnswerIndex();
    if (previousCorrectAnswerIndex != -1) {
        Choice[previousCorrectAnswerIndex].second = false;
    }
    Choice[newChoiceIndex].second = true;
    std::pair<int, std::string> TMP_Choice;
    TMP_Choice.first = newChoiceIndex;
    TMP_Choice.second = Choice[newChoiceIndex].first;
    correctAnswer = TMP_Choice;
}

void multipleChoiceQuestion::questionPrint(std::string mode) { // IN DEBUGGING
    std::cout << "\n\t --> " << getQuestionFace() << std::endl << std::endl;

    // Using shuffled choices to print it for user
    for (const auto& shuffled: shuffledToOriginalMap) {
        int originalIndex = shuffled.second;
        std::cout << "\t\t" << shuffled.first << ") " << Choice[originalIndex].first 
                  << (Choice[originalIndex].second && std::regex_match(mode, std::regex("TEACHER", std::regex_constants::icase)) ? " --> ANSWER" : "") 
                  << std::endl;
    }
    std::cout << std::endl;

    // Student (while in exam) and Teacher should know the Scores
    if (mode == "TEACHER" || mode == "STUDENT") {
        std::cout << "\t\t" << "~~ Correct Score: " << this->getCorrectScore() << std::endl;
        std::cout << "\t\t" << "~~ Wrong Score: " << this->getWrongScore() << std::endl;
    }

    // If the student has answered the question, show their answer
    if (mode == "STUDENT") {
        std::cout << "\n\t\t" << "Your Answer: ";
        if (getStudentQuestionStatus() != NOT_ANSWERD) {
            int originalIndex = getStudentAnswer().first; // -> Getting original index of student answer
            int suffledIndex = -1;
            for (const auto& suffled: shuffledToOriginalMap){ // Getting the key that is the Suffle map that the value is Original index
                if (suffled.second == getStudentAnswer().first) {
                    suffledIndex = suffled.first;
                    break;
                }
            }
            if (Choice.find(originalIndex) != Choice.end()) {
                std::cout << suffledIndex << ") " << Choice[originalIndex].first << std::endl;
            } else {
                std::cout << "INVALID ANSWER INDEX" << std::endl;
            }
        } else {
            std::cout << "NOT ANSWERED" << std::endl;
        }
    }
}

void multipleChoiceQuestion::setStudentAnswer(int studentAnswer, std::string choiceFace) {
    // Its for the Student Class and extraction of the answers
    std::pair<int, std::string> TMP_Choice;
    TMP_Choice.first = studentAnswer;
    TMP_Choice.second = choiceFace;
    studentAnswerPair = TMP_Choice;
    setStudentQuestionStatus(ANSWERED);
}

void multipleChoiceQuestion::clearAnswer() {
    // Clearing Answer and put the score and QuestionStatus to default modes
    // In Multiple Choice
    studentAnswerPair.first = -1;
    studentAnswerPair.second = "";
    setStudentQuestionStatus(NOT_ANSWERD);
    setStudentScore(0);
}

std::pair<int, std::string> multipleChoiceQuestion::getStudentAnswer() const {
    return studentAnswerPair;
}

// StringBasedQuestion implementations
StringBasedQuestion::StringBasedQuestion()
    : studentAnswer(""), Question(),ORIGINAL_QStatus(NOT_ANSWERD) {
}

StringBasedQuestion::StringBasedQuestion(std::string question, int correctScore, int wrongScore) 
: Question(question, correctScore, wrongScore), ORIGINAL_QStatus(NOT_ANSWERD) {
}

std::string StringBasedQuestion::getStudentAnswer() const { 
    return studentAnswer; 
}

void StringBasedQuestion::clearAnswer() {
    // Clearing Answer and put the score and QuestionStatus to default modes
    // In String Based Question --> For FullFormQuestion
    this->studentAnswer.clear();
    setStudentQuestionStatus(NOT_ANSWERD);
    setOriginalQuestionStatus(NOT_ANSWERD);
    setStudentScore(0);
}

void StringBasedQuestion::setStudentAnswer(std::string answer) {
    // Its for the Student Class and extraction of the answers
    // In String Based Question --> For ShortFormQuestion 
    this->studentAnswer = answer;
    setStudentQuestionStatus(ANSWERED);
    setOriginalQuestionStatus(ANSWERED);
}

void StringBasedQuestion::questionPrint(std::string mode) { // IN DEBUGGING
    int i = 1;
    std::cout << "\n\t --> " << getQuestionFace() << std::endl;
    if (getStudentQuestionStatus() != NOT_ANSWERD){
        std::cout << "\n\t $$$ Your Answer: " << getStudentAnswer() << std::endl;
    }
}

// ShortFormQuestion implementations
ShortFormQuestion::ShortFormQuestion(long long SFQID) 
    : SFQID(SFQID), StringBasedQuestion() {
}

ShortFormQuestion::ShortFormQuestion(long long SFQID, std::string question, std::string answer, int correctScore, int wrongScore)
    : SFQID(SFQID), StringBasedQuestion(question, correctScore, wrongScore), Answer(answer) {
}

bool ShortFormQuestion::getResultChoice() const {
    return Answer == getStudentAnswer();
}

void ShortFormQuestion::setQuesionBasedStudentScore() {
    // Checking if the student answered --> I set it to answerd but its for precaution
    // In Short Form Question
    if (getStudentQuestionStatus() == ANSWERED) {
        if (getResultChoice()) {
            setStudentScore(getCorrectScore());
            setStudentQuestionStatus(ANSWERED_CORRECTLY);
        } else {
            setStudentScore(getWrongScore());
            setStudentQuestionStatus(ANSWERED_INCORRECTLY);
        }
    }
}

long long ShortFormQuestion::getSFQID() { 
    return SFQID; 
}

void ShortFormQuestion::setSFQID(long long SFQID) { 
    this->SFQID = SFQID; 
}

const std::string& ShortFormQuestion::getAnswer() const {
     return Answer; 
} 

void ShortFormQuestion::setAnswer(std::string answer) { 
    this->Answer = answer; 
}

void ShortFormQuestion::questionPrint(std::string mode) { // CORRECT
    std::cout << "\n\t --> " << getQuestionFace() << ((mode == "TEACHER") ? "\n\t --> ANSWER: " + this -> Answer:"") << std::endl << std::endl;
    if (mode == "TEACHER" || mode == "STUDENT") {
        std::cout << "\t\t" << "~~ Correct Score: " << this -> getCorrectScore() << std::endl;
        std::cout << "\t\t" << "~~ Wrong Score: " << this -> getWrongScore() << std::endl;
    }
    if (getStudentQuestionStatus() != NOT_ANSWERD){
        std::cout << "\n\t $$$ Your Answer: " << getStudentAnswer() << std::endl;
    }
}

// FullFormQuestion implementations
FullFormQuestion::FullFormQuestion(long long FFQID) 
    : FFQID(FFQID), StringBasedQuestion() {
}

FullFormQuestion::FullFormQuestion(long long FFQID, std::string question)
    : FFQID(FFQID), StringBasedQuestion(question, 0, 0) {
}

void FullFormQuestion::setQuesionBasedStudentScore() {
    // This part is for teacher to set Score for the student
    // In Full Form Question
    this -> previousScore = this -> getStudentScore();
    int TMP_Score;
    InputingSection<int>("Your Score", TMP_Score);
    setStudentScore(TMP_Score);
    setStudentQuestionStatus(TEACHER_CALCULATED_SCORE);
}

void FullFormQuestion::deleteStudentScore() {
    if (getStudentQuestionStatus() == TEACHER_CALCULATED_SCORE) {
        std::string Flag;
        InputingSection("!!!! ARE YOU SURE !!!! (Y/n)", Flag);
        if (Flag == "Y") {
            setStudentScore(0);
            if (getOriginalQuestionStatus() == ANSWERED) {
                setStudentQuestionStatus(ANSWERED);
            } else {
                setStudentQuestionStatus(getOriginalQuestionStatus());
            }
        }
    } else {
        UnSuccessfulMessage("NOT GRADED YET");
    }
}

long long FullFormQuestion::getFFQID() { 
    return FFQID; 
}

void FullFormQuestion::setFFQID(long long FFQID) { 
    this->FFQID = FFQID; 
}


// Printing the report of the question
// This is the function that will be used to print the report of the questions For THE STUDENT
void multipleChoiceQuestion::reportQuestionPrint() {
    std::cout << "\n\t<><><><><><><><><><><><><><><><><><><><><><><><>" << std::endl;
    std::cout << "\n\t --> Question No." << getMCQID() << std::endl;
    std::cout << "\t --> " << getQuestionFace() << std::endl << std::endl;
    std::cout << "\t\t#-> Correct Answer: " << getCorrectAnswer().second << std::endl;
    if (getStudentQuestionStatus() != NOT_ANSWERD) {
        std::cout << "\t\t#-> Student Answer: " << getStudentAnswer().second << std::endl;
    } else {
        std::cout << "\t\t#-> Student Answer: NOT ANSWERED" << std::endl;
    }
    if (getStudentQuestionStatus() == ANSWERED_CORRECTLY) {
        std::cout << "\n\t\t#-> Student Answer Is CORRECT!"<< std::endl;
    } else if (getStudentQuestionStatus() == ANSWERED_INCORRECTLY) {
        std::cout << "\n\t\t#-> Student Answer Is INCORRECT!"<< std::endl;
    }
    std::cout << "\n\t\t#-> Student Score: " << getStudentScore() << std::endl;
    std::cout << "\n\t<><><><><><><><><><><><><><><><><><><><><><><><>" << std::endl;
}

void ShortFormQuestion::reportQuestionPrint() {
    std::cout << "\n\t<><><><><><><><><><><><><><><><><><><><><><><><>" << std::endl;
    std::cout << "\n\t --> Question No." << getSFQID() << std::endl;
    std::cout << "\t --> " << getQuestionFace() << std::endl << std::endl;
    std::cout << "\t\t#-> Correct Answer: " << getAnswer() << std::endl;
    if (getStudentQuestionStatus() != NOT_ANSWERD) {
        std::cout << "\t\t#-> Student Answer: " << getStudentAnswer() << std::endl;
    } else {
        std::cout << "\t\t#-> Student Answer: NOT ANSWERED" << std::endl;
    }
    if (getStudentQuestionStatus() == ANSWERED_CORRECTLY) {
        std::cout << "\n\t\t#-> Student Answer Is CORRECT!" << std::endl;
    } else if (getStudentQuestionStatus() == ANSWERED_INCORRECTLY) {
        std::cout << "\n\t\t#-> Student Answer Is INCORRECT!" << std::endl;
    }
    std::cout << "\n\t\t#-> Student Score: " << getStudentScore() << std::endl;
    std::cout << "\n\t<><><><><><><><><><><><><><><><><><><><><><><><>" << std::endl;
}

void FullFormQuestion::reportQuestionPrint() {
    std::cout << "\n\t<><><><><><><><><><><><><><><><><><><><><><><><>" << std::endl;
    std::cout << "\n\t --> Question No." << getFFQID() << std::endl;
    std::cout << "\t --> " << getQuestionFace() << std::endl << std::endl;
    if (getOriginalQuestionStatus() != NOT_ANSWERD) {
        std::cout << "\t\t#-> Student Answer: " << getStudentAnswer() << std::endl;
    } else {
        std::cout << "\t\t#-> Student Answer: NOT ANSWERED" << std::endl;
    }
    if (getStudentQuestionStatus() == TEACHER_CALCULATED_SCORE) {
        std::cout << "\n\t\t#-> Student Score: " << getStudentScore() << std::endl;
    }
    std::cout << "\n\t<><><><><><><><><><><><><><><><><><><><><><><><>" << std::endl;
}

void FullFormQuestion::gradingQuestionPrint() {
    std::cout << "\n\n\tP -> PREVIOUS QUESTION /// N -> NEXT QUESTION";
    this -> reportQuestionPrint();
}