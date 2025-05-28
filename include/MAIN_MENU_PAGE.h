#pragma once
#include "GLOBAL_VARIABLES.hpp"
#include "Teacher.h"
#include "Student.h"
#include "Exam.hpp"
using json = nlohmann::json;

void MainMenuInterface(std::string& USER_INPUT, json& Teacher_File, json& Student_File, json& Exam_File, json& Question_File);
void DecideMenuInterface(std::string INPUT_MODE, std::string& USER_INPUT, json& Teacher_File, json& Student_File, json& Exam_File, json& Question_File);