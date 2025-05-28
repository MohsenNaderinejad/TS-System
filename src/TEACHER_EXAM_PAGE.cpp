#include "../include/TEACHER_EXAM_PAGE.hpp"


void SpecificExamViewPage(bool& Logout_FLAG, std::string & USER_INPUT, 
    Exam * Selected_Exam, json & Exam_File, 
    Teacher * USER_Teacher, json & Teacher_File, 
    json & Question_File, Mode QMode) {
        
    // Default Values
    long long exam_GUC = 0;
    bool examFileDeleted = false;
    
    while (USER_INPUT != "0" && !Logout_FLAG && !examFileDeleted) {
        try {

            Clear();
            std::cout << UP_WINDOW_TILE << std::endl;
            std::cout << "\t#--------- " << USER_Teacher -> getName() << " " << USER_Teacher -> getLast_Name() << " ---------#" << std::endl;
            std::cout << "\n\t#------------- EXAM VIEW -------------#\n" << std::endl;
            for (int Index = 0; Index < TEACHER_SPECIFIC_EXAM_VIEW_PAGE_MENU.size(); Index++) {
                std::cout << TEACHER_SPECIFIC_EXAM_VIEW_PAGE_MENU[Index];
                if (Index == 2) std::cout << ": " << Selected_Exam -> getGUC();
                if (Index == 3) std::cout << ": " << Selected_Exam -> getExamName();
                if (Index == 4) std::cout << ": " << Selected_Exam -> getSubject();
                if (Index == 5) std::cout << ": " << Selected_Exam -> getTime();
                if (Index == 6) std::cout << ": " << Selected_Exam -> getType();
                if (Index == 7) std::cout << ": " << Selected_Exam -> getPublishState();
                if (Index == 9) {
                    if (QMode == MULTIPLE_CHOICE_QUESTION_MODE) Selected_Exam -> viewAllQuestions<MULTIPLE_CHOICE_QUESTION_MODE>();
                    if (QMode == SHORT_FORM_QUESTION_MODE) Selected_Exam -> viewAllQuestions<SHORT_FORM_QUESTION_MODE>();
                    if (QMode == FULL_FORM_QUESTION_MODE) Selected_Exam -> viewAllQuestions<FULL_FORM_QUESTION_MODE>();
                }
                std::cout << std::endl;
            }
            std::cout << BOTTOM_WINDOW_TILE << std::endl << INPUT_SENTENCE;

            // std::cin.ignore();
            std::getline(std::cin, USER_INPUT);

            if (USER_INPUT == "-2") { // Logout
                Logout_FLAG = true;
                break;
            } 
            
            else if (USER_INPUT == "-1") { // Return
                break;
            } 
            
            else if (USER_INPUT == "1") { // Edit
                ExamEditPage(Logout_FLAG, USER_INPUT, 
                            Selected_Exam, Exam_File, 
                            USER_Teacher, Teacher_File, 
                            Question_File, examFileDeleted, QMode);
            } 
            
            else if (USER_INPUT == "0") { // Exit
                break;
            }

            else if (USER_INPUT == "MCQ") QMode = MULTIPLE_CHOICE_QUESTION_MODE;
            else if (USER_INPUT == "SFQ") QMode = SHORT_FORM_QUESTION_MODE;
            else if (USER_INPUT == "FFQ") QMode = FULL_FORM_QUESTION_MODE;
            
            else { // Wrong Input
                UnSuccessfulMessage(INVALID_INPUT_ERROR);
            }

        } catch (const std::invalid_argument & error) {
            CatchException(error, TWO_SECOND);
        }

    }
}

void ExamEditPage(bool& Logout_FLAG, std::string & USER_INPUT, 
    Exam * Selected_Exam, json & Exam_File, 
    Teacher * USER_Teacher, json & Teacher_File, 
    json & Question_File, bool& examDeleteFlag, Mode QMode) {
    

    while (USER_INPUT != "0" && !Logout_FLAG) {
        try {
            Clear();
            std::cout << UP_WINDOW_TILE << std::endl;
            std::cout << "\t#--------- " << USER_Teacher -> getName() << " " << USER_Teacher -> getLast_Name() << " ---------#" << std::endl;
            std::cout << "\n\t#--------- EXAM EDIT SECTION ---------#\n" << std::endl;
            for (int Index = 0; Index < TEACHER_EXAM_EDIT_PAGE_MENU.size(); Index++) {
                std::cout << TEACHER_EXAM_EDIT_PAGE_MENU[Index];
                if (Index == 2) std::cout << Selected_Exam -> getGUC();
                if (Index == 3) std::cout << ": " << Selected_Exam -> getExamName();
                if (Index == 4) std::cout << ": " << Selected_Exam -> getSubject();
                if (Index == 5) std::cout << ": " << Selected_Exam -> getTime();
                if (Index == 6) std::cout << ": " << Selected_Exam -> getType();
                if (Index == 7) std::cout << ": " << Selected_Exam -> getPublishState();
                if (Index == 11) {
                    if (QMode == MULTIPLE_CHOICE_QUESTION_MODE) Selected_Exam -> viewAllQuestions<MULTIPLE_CHOICE_QUESTION_MODE>();
                    if (QMode == SHORT_FORM_QUESTION_MODE) Selected_Exam -> viewAllQuestions<SHORT_FORM_QUESTION_MODE>();
                    if (QMode == FULL_FORM_QUESTION_MODE) Selected_Exam -> viewAllQuestions<FULL_FORM_QUESTION_MODE>();
                }
                std::cout << std::endl;
            }
            std::cout << BOTTOM_WINDOW_TILE << std::endl << INPUT_SENTENCE;

            std::getline(std::cin, USER_INPUT);
            // std::cin.ignore();

            if (USER_INPUT == "-2") { // Logout + Saving Exam
                Logout_FLAG = true;
                Selected_Exam -> examAndQuestionFileUpdate(Exam_File, Question_File);
                SuccessfulMessage("EXAM SAVED SUCCESSFULLY", ONE_SECOND);
            } 
            
            else if (USER_INPUT == "-1") { // Return Without Saving
                std::cout << "\n\t !!!! ARE YOU SURE ???? (Y/n): ";
                std::string answer;
                std::cin >> answer;
                std::cin.ignore();
                if (answer == "Y")
                    break;
                else continue;
            } 
            
            else if (USER_INPUT == "1") { // New Exam Name
                std::string tmp;
                InputingSection("NEW EXAM NAME", tmp);
                Selected_Exam -> setExamName(tmp);
            } else if (USER_INPUT == "2") { // New Exam Subject
                std::string tmp;
                InputingSection("NEW SUBJECT", tmp);
                Selected_Exam -> setSubject(tmp);
            } else if (USER_INPUT == "3") { // New Exam Time
                std::string tmp;
                InputingSection("NEW TIME", tmp);
                Selected_Exam -> setTime(tmp);
            } else if (USER_INPUT == "4") { // New Exam Type
                std::string tmp;
                InputingSection("NEW TYPE", tmp);
                Selected_Exam -> setType(tmp);
            } 
            else if (USER_INPUT == "5") { // Publish State
                std::string tmp;
                InputingSection("BE PUBLISHED (Y/N)", tmp);
                Selected_Exam -> setPublishState(tmp);
            }
            
            else if (USER_INPUT == "6") { // Add Question
                AddOrEditQuestionPage(Logout_FLAG, USER_INPUT, QMode, Selected_Exam);
            } 
            else if (USER_INPUT == "7") { // Editing Question
                std::string QuestionIndex;
                InputingSection("QUESTION NUMBER", QuestionIndex);
                std::regex pattern("^([0-9]+)$");
                if (!regex_match(QuestionIndex, pattern)) {
                    UnSuccessfulMessage("QUESTION NUMBER SHOULD BE A POSITIVE INTEGER");
                }
                long long QuestionIndex_Integer = stoll(QuestionIndex);
                AddOrEditQuestionPage(Logout_FLAG, USER_INPUT, QMode, Selected_Exam, QuestionIndex_Integer);
            }    
            else if (USER_INPUT == "8") { // Delete Question
                std::string QuestionIndex;
                InputingSection("QUESTION NUMBER", QuestionIndex);
                std::regex pattern("^([0-9]+)$");
                if (!regex_match(QuestionIndex, pattern)) {
                    UnSuccessfulMessage("QUESTION NUMBER SHOULD BE A POSITIVE INTEGER");
                }
                long long QuestionIndex_Integer = stoll(QuestionIndex);

                if (QMode == MULTIPLE_CHOICE_QUESTION_MODE) {
                    Selected_Exam->removeQuestion<MULTIPLE_CHOICE_QUESTION_MODE>(QuestionIndex_Integer);
                }
                else if (QMode == SHORT_FORM_QUESTION_MODE) {
                    Selected_Exam->removeQuestion<SHORT_FORM_QUESTION_MODE>(QuestionIndex_Integer); 
                }
                else if (QMode == FULL_FORM_QUESTION_MODE) {
                    Selected_Exam->removeQuestion<FULL_FORM_QUESTION_MODE>(QuestionIndex_Integer);
                }
                
                SuccessfulMessage("QUESTION REMOVED SUCCESSFULLY", TWO_SECOND);
            }
            
            else if (USER_INPUT == "9") { // Delete Exam
                std::cout << "\n\t !!!! ARE YOU SURE ???? (Y/n): ";
                std::string answer;
                std::cin >> answer;
                std::cin.ignore();
                if (answer == "Y") {
                    examDeleteFlag = Selected_Exam -> deleteExam(Exam_File, Question_File);
                    USER_Teacher -> deleteExamID(Selected_Exam -> getGUC());
                    break;   
                }
                else {
                    continue;
                }
            }

            else if (USER_INPUT == "10") { // Save And Return
                Selected_Exam -> examAndQuestionFileUpdate(Exam_File, Question_File);
                SuccessfulMessage("EXAM SAVED SUCCESSFULLY", TWO_SECOND);
                break;
            }

            else if (USER_INPUT == "MCQ") QMode = MULTIPLE_CHOICE_QUESTION_MODE;
            else if (USER_INPUT == "SFQ") QMode = SHORT_FORM_QUESTION_MODE;
            else if (USER_INPUT == "FFQ") QMode = FULL_FORM_QUESTION_MODE;
            
            else { // Wrong Input
                UnSuccessfulMessage(INVALID_INPUT_ERROR);
            }

        } catch (const std::invalid_argument & error) {
            CatchException(error, TWO_SECOND);
        }
    }
}

void AddOrEditQuestionPage(bool& Logout_FLAG, std::string& USER_INPUT, Mode QMode, Exam* Selected_Exam, long long Question_Index) {
    multipleChoiceQuestion *TMP_MCQ;
    ShortFormQuestion *TMP_SFQ;
    FullFormQuestion *TMP_FFQ;
    bool editMode = false;
    std::string questionFace;

    if (Question_Index != -1) {
        editMode = true;
        if (QMode == MULTIPLE_CHOICE_QUESTION_MODE) {
            TMP_MCQ = Selected_Exam->getQuestionForEdit<multipleChoiceQuestion>(Question_Index);
            questionFace = TMP_MCQ->getQuestionFace();
        }
        else if (QMode == SHORT_FORM_QUESTION_MODE) {
            TMP_SFQ = Selected_Exam->getQuestionForEdit<ShortFormQuestion>(Question_Index);
            questionFace = TMP_SFQ->getQuestionFace();
        }
        else if (QMode == FULL_FORM_QUESTION_MODE) {
            TMP_FFQ = Selected_Exam->getQuestionForEdit<FullFormQuestion>(Question_Index);
            questionFace = TMP_FFQ->getQuestionFace();
        }
    }


    if (QMode == MULTIPLE_CHOICE_QUESTION_MODE || QMode == SHORT_FORM_QUESTION_MODE){
        float correctScore = 0, wrongScore = 0; // Initialize both scores to 0
        if (QMode == MULTIPLE_CHOICE_QUESTION_MODE) {
            int newChoiceNumber = 1;
            std::map<int, std::pair<std::string, bool>> MCQChoices;
            bool answerSelected = false;
            if (editMode) {
                newChoiceNumber += TMP_MCQ->newChoiceNumber();
                answerSelected = (TMP_MCQ->getCorrectAnswerIndex() == -1) ? false:true;
                MCQChoices = TMP_MCQ->getChoices();
                correctScore = TMP_MCQ->getCorrectScore();
                wrongScore = TMP_MCQ->getWrongScore();
            }
            while (!Logout_FLAG) {
                try {

                    Clear();
                    std::cout << UP_WINDOW_TILE << std::endl;
                    if (editMode) std::cout << "\n\t\e[41m\e[1;37m#--------- QUESTION EDIT SECTION ---------#\e[0m\n" << std::endl;
                    else std::cout << "\n\t\e[41m\e[1;37m#--------- QUESTION ADD SECTION ---------#\e[0m\n" << std::endl;
                    for (int menuIndex = 0; menuIndex < TEACHER_ADD_EDIT_QUESTION_PAGE_MENU.size(); menuIndex++) {
                        std::cout << TEACHER_ADD_EDIT_QUESTION_PAGE_MENU[menuIndex];
                        if (menuIndex == 2) std::cout << ((editMode) ? TMP_MCQ->getMCQID():Selected_Exam->getNewMCQID());
                        if (menuIndex == 3) std::cout << ": " << questionFace;
                        if (menuIndex == 4) {
                            std::cout << "\n";
                            for (const auto choice: MCQChoices) {
                                std::cout << "\n\t" << choice.first << ") " << choice.second.first 
                                          << ((choice.second.second) ? " --> ANSWER" : "") << std::endl;
                            }
                        }
                        if (menuIndex == 5) std::cout << ": " << std::fixed << std::setprecision(2) << correctScore;
                        if (menuIndex == 6) std::cout << ": " << std::fixed << std::setprecision(2) << wrongScore;
                        std::cout << std::endl;
                    }

                    std::cout << BOTTOM_WINDOW_TILE << std::endl << INPUT_SENTENCE;
                    std::getline(std::cin, USER_INPUT);
                    
                    if (USER_INPUT == "-2") { // Logout (Without Saving)
                        std::string YES_NO_TMP;
                        InputingSection("ARE YOU SURE ??? (Y/n)", YES_NO_TMP);
                        if (YES_NO_TMP == "Y") {
                            Logout_FLAG = true;
                            break;
                        }
                    }
                    else if (USER_INPUT == "-1") { // Return (without Saving)
                        std::string YES_NO_TMP;
                        InputingSection("ARE YOU SURE ??? (Y/n)", YES_NO_TMP);
                        if (YES_NO_TMP == "Y") {
                            break;
                        }
                    }

                    else if (USER_INPUT == "1") { // QUESTION FACE
                        InputingSection("QUESTION FACE", questionFace);
                    }
                    else if (USER_INPUT == "2") { // QUESTION ADD/EDIT/REMOVE
                        std::string Mode;
                        InputingSection("ADD CHOICE (A) / EDIT CHOICE (E) / REMOVE CHOICE (R)", Mode);
                        if (Mode == "A") {
                            std::string choiceFace;
                            std::string choiceBool;
                            InputingSection("CHOICE DESCRIPTION", choiceFace);
                            InputingSection("IS IT THE ASNWER (Y/n)", choiceBool);
                            bool CBool;
                            if (choiceBool == "Y") {
                                if (answerSelected == true){
                                    for (auto& choice : MCQChoices) {
                                        if (choice.second.second == true) {
                                            choice.second.second = false;
                                            break;
                                        }
                                    }
                                }
                                CBool = true;
                                answerSelected = true;
                            }
                            else {
                                CBool = false;
                            }
                            std::pair<std::string, bool> TMP_Choice;
                            TMP_Choice.first = choiceFace;
                            TMP_Choice.second = CBool;
                            MCQChoices[newChoiceNumber++] = TMP_Choice; 
                        }
                        else if (Mode == "E") {
                            std::string indexStr;
                            int index;
                            bool found = false;
                            InputingSection("CHOICE INDEX", indexStr);
                            try {
                                index = std::stoi(indexStr);
                            } catch (const std::invalid_argument&) {
                                UnSuccessfulMessage("INVALID CHOICE INDEX");
                                continue;
                            }
                            for (auto& choice : MCQChoices) {
                                if (choice.first == index) {
                                    found = true;
                                    std::string faceOrBool;
                                    InputingSection("CHANGE FACE (F) / CHANGE ANSWER (A)", faceOrBool);
                                    if (faceOrBool == "F") {
                                        InputingSection("CHOICE DESCRIPTION", choice.second.first);
                                    } else if (faceOrBool == "A") {
                                        std::string answer;
                                        InputingSection("IS IT THE ANSWER (Y/n)", answer);
                                        if (answer == "Y") {
                                            for (auto& c : MCQChoices) {
                                                c.second.second = false;
                                            }
                                            choice.second.second = true;
                                            answerSelected = true;
                                        }
                                        else {
                                            if (choice.second.second == true) {
                                                answerSelected = false;
                                            }
                                            choice.second.second = false;
                                        }
                                    }
                                    break;
                                }
                            }
                            if (!found) {
                                UnSuccessfulMessage("CHOICE NOT FOUND");
                            }
                        }
                        else if (Mode == "R") {
                            std::string indexStr;
                            int index;
                            bool found = false;
                            InputingSection("CHOICE INDEX", indexStr);
                            try {
                                index = std::stoi(indexStr);
                            } catch (const std::invalid_argument&) {
                                UnSuccessfulMessage("INVALID CHOICE INDEX");
                                continue;
                            }
                            for (auto& choice : MCQChoices) {
                                if (choice.first == index) {
                                    found = true;
                                    if (choice.second.second == true) answerSelected = false;
                                    MCQChoices.erase(index); // Remove the choice from the map
                                    break;
                                }
                            }
                            if (!found) {
                                UnSuccessfulMessage("CHOICE NOT FOUND");
                            }
                        }
                    }
                    else if (USER_INPUT == "3") { // CORRECT SCORE
                        InputingSection("CORRECT SCORE", correctScore);
                    }
                    else if (USER_INPUT == "4") { // WRONG SCORE
                        InputingSection("WRONG SCORE", wrongScore);
                    }
                    else if (USER_INPUT == "5") { // Entering Data
                        if (questionFace.empty()) {
                            UnSuccessfulMessage("QUESTION FACE CANNOT BE EMPTY");
                            continue;
                        }
                        if (MCQChoices.empty()) {
                            UnSuccessfulMessage("CHOICES CANNOT BE EMPTY");
                            continue;
                        }
                        if (correctScore == 0) {
                            UnSuccessfulMessage("SCORE CANNOT BE ZERO");
                            continue;
                        }
                        if (answerSelected == false) {
                            UnSuccessfulMessage("AT LEAST ONE CHOICE SHOULD BE THE ANSWER");
                            continue;
                        }
                        // Create a new multipleChoiceQuestion object and add it to the exam
                        if (editMode) {
                            TMP_MCQ->setQuestionFace(questionFace);
                            TMP_MCQ->setCorrectScore(correctScore);
                            TMP_MCQ->setWrongScore(wrongScore);
                            TMP_MCQ->setChoicesByMap(MCQChoices);
                        }
                        else {
                            TMP_MCQ = new multipleChoiceQuestion(Selected_Exam->getNewMCQID(), questionFace, correctScore, wrongScore);
                            TMP_MCQ->setChoicesByMap(MCQChoices);
                            Selected_Exam->addQuestion<multipleChoiceQuestion>(TMP_MCQ);
                        }
                        SuccessfulMessage("QUESTION ADDED/EDITED SUCCESSFULLY", TWO_SECOND);
                        break;
                    }
                    else { // Wrong Input
                        UnSuccessfulMessage(INVALID_INPUT_ERROR);
                    }
                } catch (const std::invalid_argument error) {
                    CatchException(error, TWO_SECOND);
                }
            }
        } else {
            std::string SFQAnswer = "";
            if (editMode) {
                SFQAnswer = TMP_SFQ->getAnswer();
                correctScore = TMP_SFQ->getCorrectScore();
                wrongScore = TMP_SFQ->getWrongScore();
            }
            while (!Logout_FLAG) {
                try {
                    Clear();
                    std::cout << UP_WINDOW_TILE << std::endl;
                    std::cout << "\n\t\e[41m\e[1;37m#--------- QUESTION ADD SECTION ---------#\e[0m\n" << std::endl;
                    for (int menuIndex = 0; menuIndex < TEACHER_ADD_EDIT_QUESTION_PAGE_MENU.size(); menuIndex++) {
                        std::cout << TEACHER_ADD_EDIT_QUESTION_PAGE_MENU[menuIndex];
                        if (menuIndex == 2) std::cout << ((editMode) ? TMP_SFQ->getSFQID():Selected_Exam->getNewSFQID());
                        if (menuIndex == 3) std::cout << ": " << questionFace;
                        if (menuIndex == 4) std::cout << ": " << SFQAnswer;
                        if (menuIndex == 5) std::cout << ": " << std::fixed << std::setprecision(2) << correctScore;
                        if (menuIndex == 6) std::cout << ": " << std::fixed << std::setprecision(2) << wrongScore;
                        std::cout << std::endl;
                    }

                    std::cout << BOTTOM_WINDOW_TILE << std::endl << INPUT_SENTENCE;
                    std::getline(std::cin, USER_INPUT);
                    
                    if (USER_INPUT == "-2") { // Logout (Without Saving)
                        std::string YES_NO_TMP;
                        InputingSection("ARE YOU SURE ??? (Y/n)", YES_NO_TMP);
                        if (YES_NO_TMP == "Y") {
                            Logout_FLAG = true;
                            break;
                        }
                    }
                    else if (USER_INPUT == "-1") { // Return (without Saving)
                        std::string YES_NO_TMP;
                        InputingSection("ARE YOU SURE ??? (Y/n)", YES_NO_TMP);
                        if (YES_NO_TMP == "Y") {
                            break;
                        }
                    }

                    else if (USER_INPUT == "1") { // QUESTION FACE
                        InputingSection("QUESTION FACE", questionFace);
                    }

                    else if (USER_INPUT == "2") { // ANSWER
                        InputingSection("ANSWER", SFQAnswer);
                    }
                    else if (USER_INPUT == "3") { // CORRECT SCORE
                        InputingSection("CORRECT SCORE", correctScore);
                    }
                    else if (USER_INPUT == "4") { // WRONG SCORE
                        InputingSection("WRONG SCORE", wrongScore);
                    }
                    else if (USER_INPUT == "5") { // Entering Data
                        if (questionFace.empty()) {
                            UnSuccessfulMessage("QUESTION FACE CANNOT BE EMPTY");
                            continue;
                        }
                        if (SFQAnswer.empty()) {
                            UnSuccessfulMessage("ANSWER CANNOT BE EMPTY");
                            continue;
                        }
                        if (correctScore == 0) {
                            UnSuccessfulMessage("SCORE CANNOT BE ZERO");
                            continue;
                        }
                        // Create a new ShortFormQuestion object and add it to the exam or edit the existing one
                        if (editMode) {
                            TMP_SFQ->setQuestionFace(questionFace);
                            TMP_SFQ->setAnswer(SFQAnswer);
                            TMP_SFQ->setCorrectScore(correctScore);
                            TMP_SFQ->setWrongScore(wrongScore);
                        }
                        else  {
                            TMP_SFQ = new ShortFormQuestion(Selected_Exam->getNewSFQID(), 
                                                            questionFace,
                                                            SFQAnswer,
                                                            correctScore,
                                                            wrongScore);
                            Selected_Exam->addQuestion<ShortFormQuestion>(TMP_SFQ);
                        }
                        SuccessfulMessage("QUESTION ADDED/EDITED SUCCESSFULLY", TWO_SECOND);
                        break;
                    }
                    else { // Wrong Input
                        UnSuccessfulMessage(INVALID_INPUT_ERROR);
                    }
            } catch (const std::invalid_argument error) {
                    CatchException(error, TWO_SECOND);
                }
            }
        }
    } else {
        while (!Logout_FLAG) {
            try {
                Clear();
                std::cout << UP_WINDOW_TILE << std::endl;
                std::cout << "\n\t\e[41m\e[1;37m#--------- QUESTION ADD SECTION ---------#\e[0m\n" << std::endl;
                for (int menuIndex = 0; menuIndex < TEACHER_ADD_EDIT_QUESTION_PAGE_MENU.size(); menuIndex++) {
                    std::cout << TEACHER_ADD_EDIT_QUESTION_PAGE_MENU[menuIndex];
                    if (menuIndex == 2) std::cout << ((editMode) ? TMP_FFQ->getFFQID() : Selected_Exam->getNewFFQID());
                    if (menuIndex == 3) std::cout << ": " << questionFace;
                    std::cout << std::endl;
                }

                std::cout << BOTTOM_WINDOW_TILE << std::endl << INPUT_SENTENCE;
                std::getline(std::cin, USER_INPUT);
                
                if (USER_INPUT == "-2") { // Logout (Without Saving)
                    std::string YES_NO_TMP;
                    InputingSection("ARE YOU SURE ??? (Y/n)", YES_NO_TMP);
                    if (YES_NO_TMP == "Y") {
                        Logout_FLAG = true;
                        break;
                    }
                }
                else if (USER_INPUT == "-1") { // Return (without Saving)
                    std::string YES_NO_TMP;
                    InputingSection("ARE YOU SURE ??? (Y/n)", YES_NO_TMP);
                    if (YES_NO_TMP == "Y") {
                        break;
                    }
                }

                else if (USER_INPUT == "1") { // QUESTION FACE
                    InputingSection("QUESTION FACE", questionFace);
                }
                else if (USER_INPUT == "5") { // Entering Data
                    if (questionFace.empty()) {
                        UnSuccessfulMessage("QUESTION FACE CANNOT BE EMPTY");
                        continue;
                    }
                    // Create a new FullFormQuestion object and add it to the exam
                    if (editMode) {
                        TMP_FFQ->setQuestionFace(questionFace);
                    }
                    else {
                        TMP_FFQ = new FullFormQuestion(Selected_Exam->getNewFFQID(), questionFace);
                        Selected_Exam->addQuestion<FullFormQuestion>(TMP_FFQ);
                    }
                    SuccessfulMessage("QUESTION ADDED/EDITED SUCCESSFULLY", TWO_SECOND);
                    break;
                }
                else { // Wrong Input
                    UnSuccessfulMessage(INVALID_INPUT_ERROR);
                }
            } catch (const std::invalid_argument error) {
                CatchException(error, TWO_SECOND);
            }
        }
    }
}