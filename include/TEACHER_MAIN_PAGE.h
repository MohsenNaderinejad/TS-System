#pragma once

#include "GLOBAL_VARIABLES.hpp"
#include "Teacher.h"
#include "Student.h"
#include "Exam.hpp"

using json = nlohmann::json;

void TeacherMainPage(bool& Logout_FLAG, std::string & USER_INPUT, json & Exam_File, Teacher * USER_Teacher, json & Teacher_File, json & Question_File, json & Student_File);
void ExamCreationPage(bool& Logout_FLAG, std::string & USER_INPUT, Teacher * USER_Teacher, json & Exam_File, json & Teacher_File, json & Question_File);
void ExamScoreBoardPage(bool& Logout_FLAG, std::string & USER_INPUT, Exam * Selected_Exam, json & Exam_File, Teacher * USER_Teacher, json & Teacher_File, json & Question_File ,json & Student_File);
void GradingExamPage(bool &Logout_FLAG, std::string &USER_INPUT, ExamScoreBoard *SELECTED_ExamScoreBoard, json &Exam_File, json &Student_File, std::string studentID, json &Question_File);
