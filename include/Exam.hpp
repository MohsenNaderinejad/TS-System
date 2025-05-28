#ifndef EXAM_HPP
#define EXAM_HPP
#pragma once
#include <map>
#include <string>
#include <set>
#include "json.hpp"
#include "Question.hpp"

// Forward declarations
class Student;
class Teacher;
class multipleChoiceQuestion;
class ShortFormQuestion;
class FullFormQuestion;


using json = nlohmann::json;

// Define the Mode enum here
enum Mode {
    MULTIPLE_CHOICE_QUESTION_MODE = 1,
    SHORT_FORM_QUESTION_MODE = 2,
    FULL_FORM_QUESTION_MODE = 3,
};

struct StudentKey {
    std::string ID;
    std::string Name;

    StudentKey(std::string ID, std::string Name) : ID(ID), Name(Name) {};
};

struct StudentScore
{
    StudentKey *SDK;
    float Score;

    StudentScore(std::string ID, std::string Name, float Score) : Score(Score) {
        SDK = new StudentKey(ID, Name);
    };

    ~StudentScore() {
        delete SDK;
    };
};



struct studentScoreComparator {
    bool operator()(const StudentScore* SDC1,
                    const StudentScore* SDC2) const {
        // Compare based on the score first, then by student ID and name as a tie-breaker
        if (SDC1->Score != SDC2->Score) {
            return SDC1->Score > SDC2->Score; // Higher scores come first
        }
        if (SDC1->SDK->ID != SDC2->SDK->ID) {
            return SDC1->SDK->ID < SDC2->SDK->ID; // Compare by student ID
        }
        return SDC1->SDK->Name < SDC2->SDK->Name; // Compare by student name
    }
};

// MCQs -> Multiple Choice Questions
// SFQs -> Short From Question
// FFQs -> Full Form Question
class Exam{
    friend class ExamScoreBoard;
    private:
        std::string authorID;
        std::string examName;
        std::string subject;
        std::string type;
        int time;

        long long GUC; // Generated Unique Code

        long long newMCQID; // MCQID -> Multiple Choice Question ID
        long long newSFQID; // SFQID -> Short Form Question ID
        long long newFFQID; // FFQID -> Full Form Question ID

        std::map<long long, multipleChoiceQuestion*> MCQs;
        std::map<long long, ShortFormQuestion*> SFQs;
        std::map<long long, FullFormQuestion*> FFQs;

        std::set<StudentScore*, studentScoreComparator> studentsByScore;
        std::unordered_map<std::string, StudentScore*> studentsByID;
        
        float maximumScore;
        double averageScore;

        bool publishState; // State of Publishing Exam
        
    public:
        // Constructors
        Exam();
        Exam(std::string authorID, std::string examName, std::string subject, std::string time, std::string type, std::string publishState, json& Exam_File, json& Question_File);
        
        // Destructor
        ~Exam();
        
        // Exam Setter
        void examSetter(long long ID, json Exam_File, json Question_File, std::string authorID = "");

        // Setters
        inline void setExamName(std::string examName) { this->examName = examName; } ;
        inline void setSubject(std::string subject) { this->subject = subject; }
        void setTime(std::string time);
        inline void setType(std::string type) { this->type = type; }
        
        void setMaximumScore();
        void setAverageScore();
        
        void setPublishState(std::string publish);
        
        // Question Based Functions
        template<typename T> void addQuestion(T* question); 
        template<Mode M> void removeQuestion(long long questionID);
        template<typename T> T* getQuestionForEdit(long long questionID);
        template<Mode M> void viewAllQuestions();
        template<typename questionMode> void showSpecificQuestion(questionMode* question);
        
        template<typename questionMode>
        void moveThroughQuestions(std::string movement, typename std::map<long long, questionMode*>::iterator& iter, typename std::map<long long, questionMode*>::iterator& StartOfMap, typename std::map<long long, questionMode*>::iterator& EndOfMap) {
            std::regex pattern_movement_next("^n$", std::regex_constants::icase);
            std::regex pattern_movement_prev("^p$", std::regex_constants::icase);

            if (StartOfMap == EndOfMap) {
                UnSuccessfulMessage("NO QUESTIONS AVAILABLE");
            }
            
            if (std::regex_match(movement, pattern_movement_next)) {
                if (std::next(iter) != EndOfMap && iter != EndOfMap) {
                    iter++;
                } else {
                    UnSuccessfulMessage("NO NEXT QUESTION");
                }
            } else if (std::regex_match(movement, pattern_movement_prev)) {
                if (iter != StartOfMap) {
                    --iter;
                } else {
                    UnSuccessfulMessage("NO PREVIOUS QUESTION");
                }
            }
        }
        
        // Exam Scores Calculators
        float examScoreCalculator();
        int studentRankCalculator(std::string studentID);
        StudentScore* studentScoreFinderByID(std::string studentID);


        // Updator
        void examAndQuestionFileUpdate(json& Exam_File, json& Question_File);
        void addStudentByScore(Student* Selected_Student);
        void addStudentByID(std::string ID, StudentScore* studentScore);

        // Getters
        inline std::string getExamName() { return examName; }
        inline std::string getSubject() { return subject; }
        inline int getTime() { return time; }
        inline std::string getType() { return type; }

        inline long long getGUC() { return GUC; }

        inline long long getNewMCQID() { return newMCQID; }
        inline long long getNewSFQID() { return newSFQID; }
        inline long long getNewFFQID() { return newFFQID; }

        inline std::map<long long, multipleChoiceQuestion*>& getMCQ() { return this->MCQs; }
        inline std::map<long long, ShortFormQuestion*>& getSFQ() { return this->SFQs; }
        inline std::map<long long, FullFormQuestion*>& getFFQ() { return this->FFQs; }
        
        inline std::set<StudentScore*, studentScoreComparator>& getStudentsScores() { return this->studentsByScore; }
        inline std::unordered_map<std::string, StudentScore*>& getStudentsByID() { return studentsByID; }

        inline float getMaximumScore() { return maximumScore; }
        inline double getAverageScore() { return averageScore; }
        
        std::string getPublishState();

        bool deleteExam(json& Exam_File, json& Question_File);
};

class ExamScoreBoard {
    private:
        Exam* exam;
        Teacher* teacher;

    public:
        ExamScoreBoard(Exam* exam, Teacher* teacher) : exam(exam), teacher(teacher) {}

        void showScoreBoard();
        void printScoreBoard(std::string ADDRESS);
        void writeBufferESC(std::string ADDERSS);

        Student* studentFinder(json& Student_File, std::string studentID);
        inline Exam* getExam() const { return exam; }
        inline Teacher* getTeacher() const { return teacher; }

        void SFFQsGrading(Student* student);
};

#endif // EXAM_HPP