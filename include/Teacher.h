#pragma once

#include <set>
#include <string>
#include "User.h"
#include "Question.hpp"

using json = nlohmann::json;

// Forward declaration
class User;

class Teacher: public User {
    private:
        std::string Field_Of_Study;
        std::set<long long> Exam_IDs;
        std::set<std::string> Courses;

    public:
        // Constructor
        Teacher();
        Teacher(std::string name, std::string last_name, std::string email, std::string passwd, std::string ID, json& file, std::set<std::string> Courses, std::string fos ,std::string gender, int age = 0);

        // login
        void login(std::string ID, std::string passwd, json& file);

        // Update
        void teacherFileUpdate(json& file);

        // Exam Functions
        void insertExamID(long long GUC);
        void deleteExamID(long long GUC);
        void viewExams();

        // Setters & Getters
        inline void setFieldOfStudy(std::string new_field) {this->Field_Of_Study = new_field; }
        inline std::string getFieldOfStudy() { return Field_Of_Study; }

};