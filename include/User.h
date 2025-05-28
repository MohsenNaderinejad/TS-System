#pragma once

#include <string>
#include <iostream>
#include <regex>
#include <vector>
#include <map>
#include <set>
#include <thread>
#include <chrono>
#include "json.hpp"
#include "GLOBAL_VARIABLES.hpp"
#include "Question.hpp"
#include "Exam.hpp"

using json = nlohmann::json;

class User {
    protected:
        std::string Name;
        std::string Last_Name;
        std::string Email;
        std::string PassWD;
        std::string Gender;
        std::string ID;
        int Age;

    public:
        // Set Section
        void setName(std::string name);
        void setLast_Name(std::string last_name);
        void setEmail(std::string email, json& file);
        void setPassWD(std::string passwd);
        void setID(std::string ID);
        void setGender(std::string gender);
        void setAge(int age);

        // Get Section
        std::string getName();
        std::string getLast_Name();
        std::string getEmail();
        std::string getPassWD();
        std::string getID();
        std::string getGender();
        int getAge();

        // Login
        virtual void login(std::string ID, std::string passwd, json& file) = 0;
        
        // Delete User
        bool DeleteUser(json &file);

        // Constructor
        User();
        User(std::string name, std::string last_name, std::string email, std::string passwd, std::string ID, json& file, std::string gender, int age);
};