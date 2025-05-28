#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <set>
#include <unordered_map>
#include "GLOBAL_VARIABLES.hpp"

enum QuestionStatus {
    NOT_ANSWERD = -2,
    ANSWERED = -1,
    ANSWERED_INCORRECTLY = 0,
    ANSWERED_CORRECTLY = 1,
    TEACHER_CALCULATED_SCORE = 2
}; // End of Question class

class Question {
    private:
    std::string Question_Face;
    float correctScore;
    float wrongScore;
    float studentScore;
    QuestionStatus studentQuestionStatus;

    public:
    Question();
    Question(std::string question, float correctScore, float wrongScore);

    // Getters and Setters
    std::string getQuestionFace() const;
    float getCorrectScore() const;
    float getWrongScore() const;
    QuestionStatus getStudentQuestionStatus() const;
    void setQuestionFace(std::string question);
    void setCorrectScore(float score);
    void setWrongScore(float score);
    void setStudentQuestionStatus(QuestionStatus state);
    float getStudentScore() const;
    void setStudentScore(float score);

    // Virtuals
    virtual void setQuesionBasedStudentScore() = 0;
    virtual void questionPrint(std::string mode) = 0;
    virtual void clearAnswer() = 0;
};

class multipleChoiceQuestion : public Question {
    private:
    long long MCQID;
    std::map<int, std::pair<std::string, bool>> Choice;
    std::pair<int, std::string> correctAnswer;
    std::pair<int, std::string> studentAnswerPair;

    // Add this member to track shuffled indices
    std::map<int, int> shuffledToOriginalMap;

    public:
    multipleChoiceQuestion(long long MCQID);
    multipleChoiceQuestion(long long MCQID, std::string question, int correctScore, int wrongScore);
    
    long long getMCQID();
    void getMCQID(long long MCQID);
    
    void setChoicesManually(int index, std::string choiceFace, bool correctness);
    void setChoicesByMap(std::map<int, std::pair<std::string, bool>> Choices);
    void setShuffledToOriginalMap();
    std::map<int, int> getShuffledToOriginalMap() const;
    
    int newChoiceNumber() const;
    
    void setStudentAnswer(int studentAnswer);
    void setStudentAnswer(int studentAnswer, std::string choiceFace);
    std::pair<int, std::string> getStudentAnswer() const;
    
    void clearAnswer() override;
    
    void setQuesionBasedStudentScore() override;
    
    std::map<int, std::pair<std::string, bool>> getChoices();
    std::pair<int, std::pair<std::string, bool>> getChoicePair(int index) const;
    
    int getCorrectAnswerIndex() const;
    void setCorrectAnswer(int index);
    void setCorrectAnswer(int index, std::string choiceFace);
    std::pair<int, std::string> getCorrectAnswer() const;
    void changeCorrectAnswer(int choiceIndex);
    
    void questionPrint(std::string mode) override;
    void reportQuestionPrint();
};

class StringBasedQuestion : public Question {
    private:
    std::string studentAnswer;
    QuestionStatus ORIGINAL_QStatus; // --> For Keeping The Orginal Status
                                     // For when teacher deletes the score
    
    public:
    StringBasedQuestion();
    StringBasedQuestion(std::string question, int correctScore, int wrongScore);
    
    std::string getStudentAnswer() const;
    void clearAnswer() override;
    void setStudentAnswer(std::string answer);
    void questionPrint(std::string mode) override;

    inline QuestionStatus getOriginalQuestionStatus() const { return ORIGINAL_QStatus; }
    inline void setOriginalQuestionStatus(QuestionStatus status) { ORIGINAL_QStatus = status; }
};

class ShortFormQuestion : public StringBasedQuestion {
    private:
    long long SFQID;
    std::string Answer;
    
    public:
    ShortFormQuestion(long long SFQID);
    ShortFormQuestion(long long SFQID, std::string question, std::string answer, int correctScore, int wrongScore);
    
    bool getResultChoice() const;
    void setQuesionBasedStudentScore() override;
    long long getSFQID();
    void setSFQID(long long SFQID);
    const std::string& getAnswer() const;
    void questionPrint(std::string mode) override;
    void setAnswer(std::string answer);
    void reportQuestionPrint();
};

class FullFormQuestion : public StringBasedQuestion {
    private:
    float previousScore; // --> For Teacher to keep the previous score
    long long FFQID;
    
    public:
    FullFormQuestion(long long FFQID);
    FullFormQuestion(long long FFQID, std::string question);
    
    void setQuesionBasedStudentScore() override;
    void deleteStudentScore(); // --> For Teacher to clean the given score
    long long getFFQID();
    void setFFQID(long long FFQID);
    void reportQuestionPrint();
    void gradingQuestionPrint();
    inline void setPreviousScore(float score) { previousScore = score; }
    inline float getPreviousScore() const { return previousScore; }
};