#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "IsraeliQueue.h"
#include "Course.h"
Course* createCourse(char*courseName,int hackerSize)
{
    Course* newCourse=(Course*)malloc(sizeof(Course));
    if(newCourse==NULL)
    {
        return NULL;
    }
    newCourse->m_courseName=courseName;
    newCourse->m_hackerSize=hackerSize;
    newCourse->m_studentsQueue=NULL;
    return newCourse;
}

void destroyCourse(Course* courseToDestroy)
{
    free(courseToDestroy->m_courseName);
    IsraeliQueueDestroy(courseToDestroy->m_studentsQueue);
    free(courseToDestroy);
}

int getCourseNameLength(Course* course)
{
    int count=0;
    while(course->m_courseName[count]!='\0')
    {
        count++;
    }
    return count;
}

