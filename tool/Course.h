#ifndef COURSE_H
#define COURSE_H
#include "IsraeliQueue.h"
#define FRIENDSHIP_TH 20
#define RIVALRY_TH -10

typedef struct Course
{
    char* m_courseName;
    IsraeliQueue m_studentsQueue;
    int m_hackerSize;

}Course;

//Create a new course
//Return NULL if mamory allocation fails
Course* createCourse(char*courseName,int hackerSize);


//destroies the given course
void destroyCourse(Course* courseToDestroy);

//Returns the length of the course name
int getCourseNameLength(Course* course);

#endif // COURSE_H
