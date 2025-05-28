#pragma once

#include "GLOBAL_VARIABLES.hpp"
using json = nlohmann::json;
#include "Teacher.h"
#include "Student.h"
#include "Exam.hpp"

void StudentMainPage(bool& Logout_FLAG, std::string & USER_INPUT, json & Exam_File, Student * USER_Student, json & Student_File, json& Question_File);