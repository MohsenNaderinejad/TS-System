#pragma once

#include "GLOBAL_VARIABLES.hpp"
#include "Teacher.h"
#include "Student.h"
#include "Exam.hpp"

using json = nlohmann::json;

void SpecificExamViewPage(bool& Logout_FLAG, std::string & USER_INPUT, Exam * Selected_Exam, json & Exam_File, Teacher * USER_Teacher, json & Teacher_File, json & Question_File, Mode QMode = MULTIPLE_CHOICE_QUESTION_MODE);
void ExamEditPage(bool& Logout_FLAG, std::string & USER_INPUT, Exam * Selected_Exam, json & Exam_File, Teacher * USER_Teacher, json & Teacher_File, json & Question_File, bool& examDeleteFlag, Mode QMode = MULTIPLE_CHOICE_QUESTION_MODE);
void AddOrEditQuestionPage(bool& Logout_FLAG, std::string& USER_INPUT, Mode QMode, Exam* Selected_Exam, long long Question_Index = -1);

