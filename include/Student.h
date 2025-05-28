#pragma once

#include "User.h"
#include "Question.hpp"
#include <map>
#include <string>
using json = nlohmann::json;

// Forward declarations
class Exam;
enum Mode;

enum studentExamMode {
    ENROLLED = 0,
    PARTICIPATED = 1
};

// SMCQs --> Student Multiple Choice Questions
// SSFQs --> Student Short Form Questions
// SFFQs --> Student Full Form Questions
struct examHolder {
    studentExamMode examMode;
    float score;
    std::map<long long, multipleChoiceQuestion*> SMCQs;
    std::map<long long, ShortFormQuestion*> SSFQs;
    std::map<long long, FullFormQuestion*> SFFQs;

    examHolder() : examMode(ENROLLED), score(0) {}
    examHolder(studentExamMode mode, float score) : examMode(mode), score(score) {}

    inline std::map<long long, FullFormQuestion*>& getSFFQs() { return SFFQs; }
};

class Student : public User {
    friend class StudentReportCard; // Allow StudentReportCard to access private members
    private:
        std::string Field_Of_Study;
        std::map<long long, examHolder*> Exams;
        bool teacherGradingMode;

    public:
        // Constructor
        Student();
        Student(std::string name, std::string last_name, std::string email, std::string passwd, std::string ID, json& file, std::string field_of_study, std::string gender="", int age=0);

        // Destructor
        ~Student() {
            for (auto& exam: Exams) {
                for (auto& question: exam.second->SMCQs) {
                    delete question.second;
                }
                for (auto& question: exam.second->SSFQs) {
                    delete question.second;
                }
                for (auto& question: exam.second->SFFQs) {
                    delete question.second;
                }
                delete exam.second;
            }
        }

        // login
        void login(std::string ID, std::string passwd, json& file);

        // Update
        void studentFileUpdate(json& Student_File);

        // Exam Functions
        void showExams();
        void insertExam(long long GUC, json& Exam_File, json& Question_File, json& Student_File);
        void updateExamInStudentClass(long long exam_guc, Exam* Selected_Exam);
        bool checkExamForParticipation(long long exam_guc);
        bool checkExamForReportCard(long long exam_guc);

        // Setters & Getters
        inline void setFieldOfStudy(std::string new_field) {this->Field_Of_Study = new_field; }
        inline std::string getFieldOfStudy() { return Field_Of_Study; }
        inline std::map<long long, examHolder*>& getExams() { return Exams; }
        inline bool getTeacherGradingMode() { return teacherGradingMode; }
        inline void setTeacherGradingMode(bool mode) { teacherGradingMode = mode; }

};

class StudentReportCard {
    private:
        float Score;
        Exam* exam;
        Student* student;
        examHolder* examHolder;

    public:
        StudentReportCard(Student* student, Exam* Selected_Exam);

        void showReportCard(Mode QMode);
        void printReportCard(std::string ADDRESS);

        void writeBufferCSV_MCQ(std::string ADDRESS);
        void writeBufferCSV_SFQ(std::string ADDRESS);
        void writeBufferCSV_FFQ(std::string ADDRESS);
        void writeBufferCSV_GENERAL(std::string ADDRESS);
};

std::string quote(const std::string& s);
