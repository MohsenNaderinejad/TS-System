#pragma once

#include "GLOBAL_VARIABLES.hpp"
using json = nlohmann::json;
#include "Teacher.h"
#include "Student.h"
#include "Exam.hpp"

void UpdatingExamRunningBoolean(std::chrono::steady_clock::time_point end_time, bool& exam_running);
void PrintingTimeEndedExam();
bool isNumber(std::string const& toCheck);
void ExamParticipationPage(bool& Logout_FLAG, std::string & USER_INPUT, Exam * Selected_Exam, json & Exam_File, Student * USER_Student, json & Student_File, json& Question_File, Mode QMode);
void ExamReportPage(bool& Logout_FLAG, std::string& USER_INPUT, Exam* Selected_Exam, Student* USER_Student);