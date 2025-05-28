#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <thread>
#include <chrono>
#include <set>
#include "json.hpp"

extern int ONE_SECOND;
extern int TWO_SECOND;
extern int THREE_SECOND;

extern std::string UP_WINDOW_TILE;
extern std::string BOTTOM_WINDOW_TILE;
extern std::string INPUT_SENTENCE;
extern std::string INVALID_INPUT_ERROR;

// Main Page Manu Lists
extern std::vector<std::string> MAIN_PAGE_MENU; // MAIN_MENU_PAGE.cpp
extern std::vector<std::string> TEACHER_OR_STUDENT_DECIDE_PAGE_MENU; // MAIN_MENU_PAGE.cpp

// Login Page Menu List
extern std::vector<std::string> LOGIN_PAGE_MENU; // LOGIN_PAGE.cpp

// Signup Page Menu List
extern std::vector<std::string> SIGNUP_TEACHER_PAGE_MENU; // SIGNUP_PAGE.cpp
extern std::vector<std::string> SIGNUP_STUDENT_PAGE_MENU; // SIGNUP_PAGE.cpp

// Teacher's Page Menu List
extern std::vector<std::string> TEACHER_MAIN_PAGE_MENU; // TEACHER_MAIN_PAGE.cpp
extern std::vector<std::string> TEACHER_EXAM_CREATION_PAGE_MENU; // TEACHER_MAIN_PAGE.cpp
extern std::vector<std::string> TEACHER_SPECIFIC_EXAM_VIEW_PAGE_MENU; // TEACHER_EXAM_PAGE.cpp
extern std::vector<std::string> TEACHER_EXAM_EDIT_PAGE_MENU; // TEACHER_EXAM_PAGE.cpp
extern std::vector<std::string> TEACHER_ADD_EDIT_QUESTION_PAGE_MENU; // TEACHER_EXAM_PAGE.cpp
extern std::vector<std::string> TEACHER_SPECIFIC_EXAM_SCORE_BOARD_PAGE_MENU; // TEACHER_MAIN_PAGE.cpp
extern std::vector<std::string> TEACHER_SPECIFIC_STUDENT_GRADING_PAGE_MENU; // TEACHER_MAIN_PAGE.cpp

// Student's Page Menu List
extern std::vector<std::string> STUDENT_MAIN_PAGE_MENU; // STUDENT_MAIN_PAGE.cpp
extern std::vector<std::string> STUDENT_EXAM_PARTICIPATION_PAGE_MENU; // STUDENT_MAIN_PAGE.cpp
extern std::vector<std::string> STUDENT_EXAM_REPORT_PAGE_MENU; // STUDENT_EXAM_PAGE.cpp

std::string quote(const std::string& s);
std::string ReplaceWhitespaceWithDash(const std::string& input);
extern std::string (*RWD)(const std::string&);

void Clear();
std::string ADDRESS_CREATOR(std::vector<std::string> args);
void CreateDirectory(std::string DirectoryName);

void CatchException(const std::invalid_argument& error, const int Time);

template <class T>
void InputingSection(std::string InputSentence, T& Variable) {
    std::string FullSentence = "\n\t #--> " + InputSentence + ": ";
    std::cout << FullSentence;
    std::cin >> Variable;
    std::cin.ignore();
}

template <>
inline void InputingSection<std::string>(std::string InputSentence, std::string& Variable) {
    std::string FullSentence = "\n\t #--> " + InputSentence + ": ";
    std::cout << FullSentence;
    std::getline(std::cin, Variable);
}

void SuccessfulMessage(std::string Message, const int Time);
void UnSuccessfulMessage(std::string Message);