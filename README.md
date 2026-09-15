# TS-System

A terminal exam platform written in C++17. Teachers build exams out of three question types, publish them, grade the written answers and export scoreboards; students enrol, sit the exam under a time limit, and get a report card with their score, their rank and the class average.

About 4,800 lines across 28 files, split into headers and translation units and built with a Makefile. Persistence is JSON on disk, reports are CSV. The only third-party code is `nlohmann/json`, vendored in `include/`.

---

## Contents

- [Building](#building)
- [Running](#running)
- [Data model](#data-model)
- [Question types and scoring](#question-types-and-scoring)
- [Exam lifecycle](#exam-lifecycle)
- [Ranking](#ranking)
- [Persistence and reports](#persistence-and-reports)
- [Project layout](#project-layout)
- [Known limitations](#known-limitations)

---

## Building

Requires a C++17 compiler.

```bash
make
```

Or directly:

```bash
g++ -std=c++17 -Iinclude \
    src/*.cpp exam/*.cpp user/*.cpp user/teacher/*.cpp user/student/*.cpp \
    -o TS-APP
```

Run from the project root, because the JSON store and the report directories are resolved relative to the working directory.

```bash
./TS-APP
```

---

## Running

The program opens on a menu offering login and signup. From there the interface branches by role.

A teacher can create exams, add and edit questions, publish an exam so students can see it, grade the written answers, and print a scoreboard. A student can enrol in published exams, sit them inside the time window, and print a report card once grading is done.

Email format is validated with a regular expression at signup, and identifiers are checked for collisions against the existing store before an account is created.

---

## Data model

```
User  (abstract)
 |-- Teacher
 |-- Student

Question  (abstract)
 |-- multipleChoiceQuestion
 |-- ShortFormQuestion
 |-- FullFormQuestion

Exam
 |-- ExamScoreBoard   (friend of Exam)
```

`User` holds the shared fields and declares `login` pure virtual, so each role authenticates against its own JSON store. `Question` declares `setQuesionBasedStudentScore`, `questionPrint` and `clearAnswer` pure virtual; every question type decides for itself how a score is derived and how it is rendered in teacher mode versus student mode.

---

## Question types and scoring

Each question carries a correct score and a wrong score, so wrong answers can carry a penalty rather than just zero, and the penalty can differ per question.

Answer state is an explicit enum rather than a sentinel value:

```cpp
enum QuestionStatus {
    NOT_ANSWERD              = -2,
    ANSWERED                 = -1,
    ANSWERED_INCORRECTLY     =  0,
    ANSWERED_CORRECTLY       =  1,
    TEACHER_CALCULATED_SCORE =  2
};
```

The separation matters: a multiple-choice question moves itself to `ANSWERED_CORRECTLY` or `ANSWERED_INCORRECTLY` without a teacher, while a written answer sits at `ANSWERED` until a teacher grades it and it becomes `TEACHER_CALCULATED_SCORE`. Nothing can be reported until every question in the exam has left the pending states.

Multiple-choice questions have no limit on the number of options, and they are shuffled per student. The shuffle keeps a `shuffledToOriginalMap` so the answer a student picked can be mapped back to the original option when the question is graded or reprinted. Shuffling without that map would make the stored answer meaningless.

---

## Exam lifecycle

```
created (teacher)  ->  published  ->  student enrols  ->  student sits it (timed)
      ->  teacher grades written answers  ->  report cards released
```

An exam has a time limit, and a student's attempt is closed when the limit expires. Only published exams appear to students. A teacher can delete an exam or individual questions, and can edit question text and scores while the exam is still being built.

---

## Ranking

Scoreboards are kept in an ordered set of `StudentScore*` with a custom comparator. The ordering is total rather than partial: higher score first, then student ID, then name. Two students on the same score get a deterministic order instead of an arbitrary one, which means a scoreboard printed twice is identical both times.

```cpp
struct studentScoreComparator {
    bool operator()(const StudentScore* a, const StudentScore* b) const {
        if (a->Score != b->Score) return a->Score > b->Score;
        if (a->SDK->ID != b->SDK->ID) return a->SDK->ID < b->SDK->ID;
        return a->SDK->Name < b->SDK->Name;
    }
};
```

`ExamScoreBoard` is declared a friend of `Exam` so it can read the score map directly without exposing it publicly.

---

## Persistence and reports

State lives in `data/`:

```
data/MAIN_DATAS/teachers.json      teacher accounts
data/MAIN_DATAS/students.json      student accounts
data/MAIN_DATAS/exams.json         exams, enrolments, scores
data/EXAM_QUESTIONS/questions.json question bank
```

Reports are written under `reports/` as CSV. A student gets a general report plus one file per question type; a teacher gets a per-exam scoreboard. The output directory can be chosen at print time and is created if it does not exist.

---

## Project layout

```
include/        headers, plus vendored json.hpp
src/            entry point, global helpers, and one file per interface page
                (main menu, login, signup, teacher main, teacher exam,
                 student main, student exam)
exam/           Exam.cpp, Question.cpp
user/           User.cpp
user/teacher/   Teacher.cpp
user/student/   Student.cpp
data/           JSON store
reports/        generated CSV output
Makefile
```

Interface code sits in `src/` and never manipulates JSON directly; the domain classes own their own serialization. Adding a question type means adding a subclass of `Question` and touching the page that renders it, not rewriting the exam logic.

---

## Known limitations

- **Passwords are stored in plain text** in the JSON files. This was a first-year exercise in class design, not in security, and it should not hold real accounts.
- **Single process, single user at a time.** There is no locking around the JSON files, so two copies of the program running against the same `data/` directory will lose writes.
- **`StudentScore` manages raw `new` and `delete`.** It works, but the ownership would be clearer with a value type or a smart pointer.
- **Path handling differs between Windows and Unix.** `backup_main/main_unix.cpp` exists because file paths were hard-coded differently for each platform. Using `std::filesystem` would remove the need for two mains.
- The repository currently contains built `.exe` files and a zipped copy of itself. Those are build artefacts and should not be tracked.
