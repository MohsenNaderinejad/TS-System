#pragma once

#include "GLOBAL_VARIABLES.hpp"
using json = nlohmann::json;

void SignUpFormPage(bool& Logout_FLAG, std::string INPUT_MODE, std::string& USER_INPUT, json& Teacher_File, json& Student_File, json& Exam_File, json& Question_File);