#pragma once

#include "GLOBAL_VARIABLES.hpp"
#include "Teacher.h"
#include "Student.h"
#include "Exam.hpp"
using json = nlohmann::json;

void LoginFormPage(bool& Logout_FLAG, std::string& USER_INPUT, json& Teacher_File, json& Student_File, Teacher* USER_Teacher, Student* USER_Student, json& Exam_File, json& Question_File);
