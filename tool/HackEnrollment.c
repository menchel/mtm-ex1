#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "IsraeliQueue.h"
#include "node.h"
#include "Student.h"
#include "Course.h"
#include "hacker.h"
#include "HackEnrollment.h"
#include "israeliQueueElement.h"
#include "IsraeliQueueHelper.h"
#define ELEMENTS_FOR_STUDENT 7
#define ELEMENTS_FOR_COURSE 2
#define ID_LENGTH 9
#define ELEMENTS_FOR_BUFFER 2
#define BUFFER_SIZE 512
#define ERROR -1
#define SUCCESS 1
#define START_OF_ERROR_IN_HACK_ENROLLMENT_CONSTRAINT 32
#define START_INDEX 30
#define FRIENDS 20
#define ENEMIES -20
#define FRIENDSHIP_TH_FOR_QUEUE 20
#define RIVALRY_TH_FOR_QUEUE 0
struct EnrollmentSystem_t
{
    Node* m_students;
    Node* m_hackers;
    Node* m_courses;
    bool m_withBigCharacters;
}EnrollmentSystem_t;

//Creates a new course
static int createCourseHelper(EnrollmentSystem sys,Course*courseToAdd);

//The function returns if two string are equal
static bool areEqual(char*str1,char*str2);

//The function returns if two id are equal
static bool compareId(char*id1,char*d2);

//Put all the hackers in their respective queues
static int putHackersInQueues(EnrollmentSystem sys);

//Get a pointer to the student with the same id
static Student* getStudentFromId(EnrollmentSystem sys,char*id);

//Creates a new queue for the course detailed
//Returns ERROR or SUCCESS
static int createQueueFromFile(EnrollmentSystem sys,char** queueValues,int queueSize);

//Returns a pointer to the hacker with the same id
//Returns null if doesn't exists
static Hacker* getHackerFromId(EnrollmentSystem sys,char*id);

//Get the course with this course number
//Returns null if doesn't find one
static Course* getCourse(EnrollmentSystem sys,char* courseNumber);

//Gets the place of a person in the queue
static int getPlaceOfPerson(IsraeliQueue queue,char*id);

//Returns if the hacker is in enough courses in the place he needs
static bool isHackerInEnoughCourses(EnrollmentSystem sys,Hacker* hackerToCheck);

//Writes a single queue to the file
static void writeAQueueToFile(Course* course,FILE*out);

//Writes the new queues to out file
static void writeQueuesToFile(Node* courses,FILE*out);

//Writes the hackers without constraints to out file
static void writeHackersWithoutConstraint(Node*hackers,FILE* out);

//Destroies the list of courses
static void destroyCoursesList(EnrollmentSystem enrollementSys);

//Destroies the list of hackers
static void destroyHackersList(EnrollmentSystem enrollementSys);

//Destroies the list of students
static void destroyStudentList(EnrollmentSystem enrollementSys);

//Creates a courses list in the system
//Returns ERROR or SUCCESS

static int createCourseList(EnrollmentSystem sys,FILE* coursesFile);
//Creates a student list in the system
//Returns ERROR or SUCCESS

static int createStudentList(EnrollmentSystem sys,FILE* students);
//Creates a course from the buffer
//Returns NULL if fails
Course* createCourseFromBuffer(char buffer[BUFFER_SIZE]);

//Creates a list from the buffer context
static char** getListFromBuffer(char buffer[BUFFER_SIZE],int size);
//Creates a new pointer to a hacker from the buffer
//Return NULL if fails
Hacker* createHackerFromBuffer(char idBuffer[BUFFER_SIZE],char courseBuffer[BUFFER_SIZE],char friendsBuffer[BUFFER_SIZE],char enemiesBuffer[BUFFER_SIZE]);

//Gets the amount of different buffers
static int getNumberOfDifferentElementsInBuffer(char buffer[BUFFER_SIZE]);

//Claculate the numeric number of the string
static int calculateNumFromString(char* number);

//Creates an new instance of Student* from the information in the buffer
//Return NULL if fails
Student* createStudentFromBuffer(char buffer[BUFFER_SIZE]);

//Copies the values from one pointer to the other
static void memoryCopy(char* from,char* to,int fromStart,int toStart,int length);

//Returns an array with the buffer values
//The array size is num of values
//Returns NULL if there was an error
static char** getValuesFromBuffer(char buffer[BUFFER_SIZE],int numOfValues);

//The function gets the distance of names ignoring capital letters
int getDistanceOfNameWithoutCapital(void* student1,void* student2);

//The function gets the distance of names regularly
int getDistanceOfNameDefault(void* student1,void* student2);

//The function calculate the distance of names
static int getNameDistanceHelper(char* name1,char*name2);

//The function calculates the distance of ids
int getDistanceOfId(void*student1,void*student2);

//The function gets the friends mesure from the hacker file
int getFriendsShipFromHacker(void* hackerStudent,void* student);

//The function gets the distance of names ignoring capital letters
static int getNameDistanceHelperWithoutCapital(char* name1,char*name2);

//The function compares two students
bool compare(void* student1,void* student2);

static bool areEqual(char*str1,char*str2)
{
    int index=0;
    while(str1[index]!='\0' && str2[index]!='\0')
    {
        if(str1[index]!=str2[index])
        {
            return false;
        }
        index++;
    }
    if(str1[index]<='9' && str1[index]>='0')
    {
        return false;
    }
    if(str2[index]<='9' && str2[index]>='0')
    {
        return false;
    }
    return true;
}
bool compare(void* student1,void* student2)
{
    if(student1==NULL || student2==NULL)
    {
        return false;
    }
    char* id1=((Student*)student1)->m_studentId;
    char* id2=((Student*)student2)->m_studentId;
    return strcmp(id1,id2)==0;
}
static int getNameDistanceHelperWithoutCapital(char* name1,char*name2)
{
    int index=0;
    int dist=0;
    char c1;
    char c2;
    while(name1[index]!='\0' && name2[index]!='\0')
    {
        c1=name1[index];
        c2=name2[index];
        if(c1>='A' && c1<='Z')
        {
            c1=c1-'A'+'a';
        }
        if(c2>='A' && c2<='Z')
        {
            c2=c2-'A'+'a';
        }
        if(c1>c2)
        {
            dist=c1-c2;
        }
        else
        {
            dist=c2-c1;
        }
        index++;
    }
    while(name1[index]!='\0')
    {
        c1=name1[index];
        if(c1>='A' && c1<='Z')
        {
            c1=c1-'A'+'a';
        }
        dist+=c1;
        index++;
    }
    while(name2[index]!='\0')
    {
        c2=name2[index];
        if(c2>='A' && c2<='Z')
        {
            c2=c2-'A'+'a';
        }
        dist+=c2;
        index++;
    }
    return dist;
}
static int getNameDistanceHelper(char* name1,char*name2)
{
    int index=0;
    int dist=0;
    while(name1[index]!='\0' && name2[index]!='\0')
    {
        if(name1[index]>name2[index])
        {
            dist+=name1[index]-name2[index];
        }
        else
        {
            dist+=name2[index]-name1[index];
        }
        index++;
    }
    while(name1[index]!='\0')
    {
        dist+=name1[index];
        index++;
    }
    while(name2[index]!='\0')
    {
        dist+=name2[index];
        index++;
    }
    return dist;
}

int getDistanceOfNameWithoutCapital(void* student1,void* student2)
{
    if(student1==NULL || student2==NULL)
    {
        return 0;
    }
    char* name1=((Student*)student1)->m_name;
    char* sureName1=((Student*)student1)->m_surename;
    char* name2=((Student*)student1)->m_name;
    char* sureName2=((Student*)student1)->m_surename;
    int dist=getNameDistanceHelperWithoutCapital(name1,name2)+getNameDistanceHelperWithoutCapital(sureName1,sureName2);
    if(dist<0)
    {
        return -1*dist;
    }
    return dist;
}

int getDistanceOfNameDefault(void* student1,void* student2)
{
    if(student1==NULL || student2==NULL)
    {
        return 0;
    }
    char* name1=((Student*)student1)->m_name;
    char* sureName1=((Student*)student1)->m_surename;
    char* name2=((Student*)student1)->m_name;
    char* sureName2=((Student*)student1)->m_surename;
    int dist=getNameDistanceHelper(name1,name2)+getNameDistanceHelper(sureName1,sureName2);
    if(dist<0)
    {
        return -1*dist;
    }
    return dist;
}
int getDistanceOfId(void*student1,void*student2)
{
    if(student1==NULL || student2==NULL)
    {
        return 0;
    }
    char*id1=((Student*)student1)->m_studentId;
    char*id2=((Student*)student2)->m_studentId;
    int id1Num=calculateNumFromString(id1);
    int id2Num=calculateNumFromString(id2);
    if(id1Num>id2Num)
    {
        return id1Num-id2Num;
    }
    return id2Num-id1Num;
}
int getFriendsShipFromHacker(void* hackerStudent,void* student)
{
    if(hackerStudent==NULL || student==NULL)
    {
        return 0;
    }
    Hacker* hackerToCheck=((Student*)hackerStudent)->m_hacker;
    if(hackerToCheck==NULL)
    {
        return 0;
    }
    char*id=((Student*)student)->m_studentId;
    for(int i=0;i<hackerToCheck->m_friendlyStudentsIdSize;i++)
    {
        if(strcmp(hackerToCheck->m_friendlyStudentsId[i],id))
        {
            return FRIENDS;
        }
    }
    for(int i=0;i<hackerToCheck->m_rivalStudentsIdSize;i++)
    {
        if(strcmp(hackerToCheck->m_rivalStudentsId[i],id))
        {
            return ENEMIES;
        }
    }
    return 0;
}
Course* createCourseFromBuffer(char buffer[BUFFER_SIZE])
{
    char** courseValues=getValuesFromBuffer(buffer,ELEMENTS_FOR_COURSE);
    if(courseValues==NULL)
    {
        return NULL;
    }
    int size=calculateNumFromString(courseValues[1]);
    Course* newCourse=createCourse(courseValues[0],size);
    if(newCourse==NULL)
    {
        return NULL;
    }
    free(courseValues[1]);
    free(courseValues);
    return newCourse;
}
static char** getListFromBuffer(char buffer[BUFFER_SIZE],int numOfElements)
{
    char** bufferValues=getValuesFromBuffer(buffer,numOfElements);
    if(bufferValues==NULL)
    {
        return NULL;
    }
    return bufferValues;
}

Hacker* createHackerFromBuffer(char idBuffer[BUFFER_SIZE],char courseBuffer[BUFFER_SIZE],char friendsBuffer[BUFFER_SIZE],char enemiesBuffer[BUFFER_SIZE])
{
    if(idBuffer==NULL || courseBuffer==NULL || friendsBuffer==NULL || enemiesBuffer==NULL)
    {
        return NULL;
    }
    //name setup
    char* id=malloc(ID_LENGTH);
    if(id==NULL)
    {
        return NULL;
    }
    for(int i=0;i<ID_LENGTH;i++)
    {
        id[i]=idBuffer[i];
    }
    int coursesNumberElements=getNumberOfDifferentElementsInBuffer(courseBuffer);
    char** courses=NULL;
    if(coursesNumberElements>0)
    {
        courses=getListFromBuffer(courseBuffer,coursesNumberElements);
        if(courses==NULL)
        {
            return NULL;
        }
    }
    int friendsNumberElements=getNumberOfDifferentElementsInBuffer(friendsBuffer);
    char** friends=NULL;
    if(friendsNumberElements>0)
    {
        friends=getListFromBuffer(friendsBuffer,friendsNumberElements);
        if(friends==NULL)
        {
            return NULL;
        }
    }
    int enemiesNumberElements=getNumberOfDifferentElementsInBuffer(enemiesBuffer);
    char** enemies=NULL;
    if(enemiesNumberElements>0)
    {
        enemies=getListFromBuffer(enemiesBuffer,enemiesNumberElements);
        if(enemies==NULL)
        {
            return NULL;
        }
    }
    Hacker* newHacker=createHacker(id,courses,coursesNumberElements,friends,friendsNumberElements,enemies,enemiesNumberElements);
    if(newHacker==NULL)
    {
        return NULL;
    }
    return newHacker;
}
static int getNumberOfDifferentElementsInBuffer(char buffer[BUFFER_SIZE])
{
    bool elementExists=false;
    bool spaceBefore=false;
    int count=0;
    int index=0;
    while(index<BUFFER_SIZE && buffer[index]!='\n' && buffer[index]!='\0')
    {
        elementExists=true;
        if(buffer[index]==' ' && !spaceBefore)
        {
            count++;
            spaceBefore=true;
        }
        if(buffer[index]!=' ')
        {
            spaceBefore=false;
        }
        index++;
    }
    if(elementExists)
    {
        count++;
    }
    return count;
}
static int calculateNumFromString(char* number)
{
    if(number==NULL)
    {
        return 0;
    }
    int numberToReturn=0;
    int index=0;
    while(number[index]!='\0' && number[index]>='0' && number[index]<='9')
    {
        numberToReturn=numberToReturn*10+number[index]-'0';
        index++;
    }
    return numberToReturn;
}

Student* createStudentFromBuffer(char buffer[BUFFER_SIZE])
{
    char** studentValues=getValuesFromBuffer(buffer,ELEMENTS_FOR_STUDENT);
    if(studentValues==NULL)
    {
        return NULL;
    }
    int totalCredits=calculateNumFromString(studentValues[1]);
    int gpa=calculateNumFromString(studentValues[2]);
    Student* newStudent= createStudent(gpa,totalCredits,studentValues[0],studentValues[3],studentValues[4],studentValues[5],studentValues[6]);
    free(studentValues[1]);
    free(studentValues[2]);
    free(studentValues);
    return newStudent;
}
static char** getValuesFromBuffer(char buffer[BUFFER_SIZE],int numOfValues)
{
    if(buffer==NULL)
    {
        return NULL;
    }
    char** bufferValues=malloc(sizeof(char*)*numOfValues);
    if(bufferValues==NULL)
    {
        return NULL;
    }
    int bufferValuesIndex=0;
    int startIndex=0;
    int endIndex=0;
    while(endIndex<BUFFER_SIZE && buffer[endIndex]!='\0')
    {
        if(buffer[endIndex]==' ' || buffer[endIndex]==10)
        {
            if(endIndex-startIndex>0)
            {
                bufferValues[bufferValuesIndex]=malloc(endIndex-startIndex+1);
                if(bufferValues[bufferValuesIndex]==NULL)
                {
                    for(int i=0;i<bufferValuesIndex;i++)
                    {
                        free(bufferValues[i]);
                    }
                    free(bufferValues);
                    return NULL;
                }
                memoryCopy(buffer,bufferValues[bufferValuesIndex],startIndex,0,endIndex-startIndex);
                (bufferValues[bufferValuesIndex])[endIndex-startIndex]='\0';
                endIndex++;
                startIndex=endIndex;
                bufferValuesIndex++;
            }
        }
        else
            endIndex++;
    }
    if(endIndex-startIndex>0)
    {
        bufferValues[bufferValuesIndex]=malloc(endIndex-startIndex+1);
        if(bufferValues[bufferValuesIndex]==NULL)
        {
            for(int i=0;i<bufferValuesIndex;i++)
            {
                free(bufferValues[i]);
            }
            free(bufferValues);
            return NULL;
        }
        memoryCopy(buffer,bufferValues[bufferValuesIndex],startIndex,0,endIndex-startIndex);
        (bufferValues[bufferValuesIndex])[endIndex-startIndex]='\0';
        bufferValuesIndex++;
    }
    return bufferValues;
}

static void memoryCopy(char* from,char* to,int fromStart,int toStart,int length)
{
    for(int i=0;i<length;i++)
    {
        to[toStart+i]=from[fromStart+i];
    }
}
static int createCourseList(EnrollmentSystem sys,FILE* coursesFile)
{
    char buffer[BUFFER_SIZE];
    while(fgets(buffer,BUFFER_SIZE,coursesFile)!=NULL)
    {
        Course* newCourse=createCourseFromBuffer(buffer);
        if(newCourse==NULL)
        {
            return ERROR;
        }
        if(createCourseHelper(sys,newCourse)==ERROR)
        {
            destroyCourse(newCourse);
            return ERROR;
        }
        Node* temp=createNode(newCourse);
        if(temp==NULL)
        {
            destroyCourse(newCourse);
            return ERROR;
        }
        sys->m_courses=addToEndOfList(sys->m_courses,temp);
    }
    return SUCCESS;
}
static int createCourseHelper(EnrollmentSystem sys,Course*courseToAdd)
{
    FriendshipFunction arrayOfFunctions[4];
    arrayOfFunctions[0]=getDistanceOfId;
    arrayOfFunctions[1]=getFriendsShipFromHacker;
    if(sys->m_withBigCharacters)
    {
        arrayOfFunctions[2]=getDistanceOfNameDefault;
    }
    else
    {
        arrayOfFunctions[2]=getDistanceOfNameWithoutCapital;
    }
    arrayOfFunctions[3]=NULL;
    ComparisonFunction compareFunc=(ComparisonFunction)compare;
    courseToAdd->m_studentsQueue=IsraeliQueueCreate(arrayOfFunctions,compareFunc,FRIENDSHIP_TH_FOR_QUEUE,RIVALRY_TH_FOR_QUEUE);
    if(courseToAdd->m_studentsQueue==NULL)
    {
        return ERROR;
    }
    return SUCCESS;
}
static Hacker* getHackerFromId(EnrollmentSystem sys,char*id)
{
    Node* current=sys->m_hackers;
    while(current!=NULL)
    {
        Hacker* currentHacker=((Hacker*)current->m_element);
        if(compareId(currentHacker->m_studentId,id)==0)
        {
            return currentHacker;
        }
        current=current->m_next;
    }
    return NULL;
}

static Student* getStudentFromId(EnrollmentSystem sys,char*id)
{
    Node* current=sys->m_students;
    while(current!=NULL)
    {
        Student* currentStudent=((Student*)current->m_element);
        if(compareId(currentStudent->m_studentId,id))
        {
            return currentStudent;
        }
        current=current->m_next;
    }
    return NULL;
}
static bool compareId(char*id1,char*id2)
{
    if(id1==NULL || id2==NULL)
    {
        return false;
    }
    for(int i=0;i<ID_LENGTH;i++)
    {
        if(id1[i]!=id2[i])
        {
            return false;
        }
    }
    return true;
}
static int createStudentList(EnrollmentSystem sys,FILE* students)
{
    char buffer[BUFFER_SIZE];
    while(fgets(buffer,BUFFER_SIZE,students)!=NULL)
    {
        Student* newStudent=createStudentFromBuffer(buffer);
        if(newStudent==NULL)
        {
            return ERROR;
        }
        Hacker* newHacker=getHackerFromId(sys,newStudent->m_studentId);
        newStudent->m_hacker=newHacker;
        Node* temp=createNode(newStudent);
        if(temp==NULL)
        {
            destroyStudent(newStudent);
            return ERROR;
        }
        sys->m_students=addToList(sys->m_students,temp);
    }
    return SUCCESS;
}
int static createHackersList(EnrollmentSystem sys,FILE* hackers)
{
    char bufferId[BUFFER_SIZE];
    char bufferCourses[BUFFER_SIZE];
    char bufferFriends[BUFFER_SIZE];
    char bufferRivals[BUFFER_SIZE];
    while(true)
    {
        if(fgets(bufferId,BUFFER_SIZE,hackers)==NULL)
        {
            break;
        }
        if(fgets(bufferCourses,BUFFER_SIZE,hackers)==NULL)
        {
            break;
        }
        if(fgets(bufferFriends,BUFFER_SIZE,hackers)==NULL)
        {
            break;
        }
        if(fgets(bufferRivals,BUFFER_SIZE,hackers)==NULL)
        {
            break;
        }
        Hacker* newHacker=createHackerFromBuffer(bufferId,bufferCourses,bufferFriends,bufferRivals);
        if(newHacker==NULL)
        {
            return ERROR;
        }
        Node* temp=createNode(newHacker);
        if(temp==NULL)
        {
            destroyHacker(newHacker);
            return ERROR;
        }
        sys->m_hackers=addToEndOfList(sys->m_hackers,temp);
    }
    return SUCCESS;
}
static void destroyCoursesList(EnrollmentSystem enrollementSys)
{
    Node*temp=enrollementSys->m_courses;
    while(temp!=NULL)
    {
        Course* courseToDestroy=((Course*)temp->m_element);
        destroyCourse(courseToDestroy);
        temp=temp->m_next;
    }
    destroyNodeList(enrollementSys->m_courses);
}
static void destroyHackersList(EnrollmentSystem enrollementSys)
{
    Node*temp=enrollementSys->m_hackers;
    while(temp!=NULL)
    {
        Hacker* hackerToDestroy=((Hacker*)temp->m_element);
        destroyHacker(hackerToDestroy);
        temp=temp->m_next;
    }
    destroyNodeList(enrollementSys->m_hackers);
}
static void destroyStudentList(EnrollmentSystem enrollementSys)
{
    Node*temp=enrollementSys->m_students;
    while(temp!=NULL)
    {
        Student* studentToDestroy=((Student*)temp->m_element);
        destroyStudent(studentToDestroy);
        temp=temp->m_next;
    }
    destroyNodeList(enrollementSys->m_students);
}
EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers)
{
    EnrollmentSystem enrollementSys=(EnrollmentSystem)malloc(sizeof(struct EnrollmentSystem_t));
    enrollementSys->m_students=NULL;
    enrollementSys->m_courses=NULL;
    enrollementSys->m_hackers=NULL;
    enrollementSys->m_withBigCharacters=true;
    if(enrollementSys==NULL)
    {
        return NULL;
    }
    if(createHackersList(enrollementSys,hackers)<0)
    {
        destroyHackersList(enrollementSys);
        free(enrollementSys);
        return NULL;
    }
    if(createStudentList(enrollementSys,students)<0)
    {
        destroyStudentList(enrollementSys);
        destroyHackersList(enrollementSys);
        free(enrollementSys);
        return NULL;
    }
    if(createCourseList(enrollementSys,courses)<0)
    {
        destroyHackersList(enrollementSys);
        destroyStudentList(enrollementSys);
        destroyCoursesList(enrollementSys);
        free(enrollementSys);
        return NULL;
    }
    return enrollementSys;
}
EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues)
{
    char buffer[BUFFER_SIZE];
    while(fgets(buffer,BUFFER_SIZE,queues))
    {
        int elementsSize=getNumberOfDifferentElementsInBuffer(buffer);
        char** queueValues=getListFromBuffer(buffer,elementsSize);
        if(createQueueFromFile(sys,queueValues,elementsSize)<0)
        {
            for(int i=0;i<elementsSize;i++)
            {
                free(queueValues[i]);
            }
            free(queueValues);
            deleteEnrollment(sys);
            return NULL;
        }
        for(int i=0;i<elementsSize;i++)
        {
            free(queueValues[i]);
        }
        free(queueValues);
    }
    return sys;
}
static int createQueueFromFile(EnrollmentSystem sys,char** queueValues,int queueSize)
{
    Course* courseToAdd=getCourse(sys,queueValues[0]);
    if(courseToAdd==NULL)
    {
        return ERROR;
    }
    for(int i=1;i<queueSize;i++)
    {
        Student* newStudent=getStudentFromId(sys,queueValues[i]);
        if(newStudent==NULL)
        {
            return ERROR;
        }
        if(enqueueRegular(courseToAdd->m_studentsQueue,newStudent)!=ISRAELIQUEUE_SUCCESS)
        {
            return ERROR;
        }
    }
    return SUCCESS;
}

static void writeAQueueToFile(Course* course,FILE*out)
{
    int queueSize=IsraeliQueueSize(course->m_studentsQueue);
    if(queueSize<1)
    {
        return;
    }
    int courseNameSize=getCourseNameLength(course);
    char* stringToWrite=malloc(courseNameSize+5+queueSize*(ID_LENGTH+1));
    int index=courseNameSize;
    memoryCopy(course->m_courseName,stringToWrite,0,0,courseNameSize);
    for(int i=0;i<queueSize;i++)
    {
        stringToWrite[index]=' ';
        index++;
        Student* temp=((Student*)IsraeliQueueDequeue(course->m_studentsQueue));
        memoryCopy(temp->m_studentId,stringToWrite,0,index,ID_LENGTH);
        index+=ID_LENGTH;
    }
    stringToWrite[index]='\n';
    stringToWrite[index+1]='\0';
    fputs(stringToWrite,out);
    free(stringToWrite);
}
static void writeQueuesToFile(Node* courses,FILE*out)
{
    Node*current=courses;
    while(current!=NULL)
    {
        writeAQueueToFile(((Course*)current->m_element),out);
        current=current->m_next;
    }
}
static void writeHackersWithoutConstraint(Node*hackers,FILE* out)
{
    Node*current=hackers;
    current=hackers;
    int listLength=getListLength(current);
    char* stringToWrite=malloc(START_OF_ERROR_IN_HACK_ENROLLMENT_CONSTRAINT+(ID_LENGTH+1)*listLength);
    char* stringStart="Cannot satisfy constraints for";
    memoryCopy(stringStart,stringToWrite,0,0,31);
    int index=START_INDEX;
    while(current!=NULL)
    {
        stringToWrite[index]=' ';
        index++;
        char*id=((char*)current->m_element);
        memoryCopy(id,stringToWrite,0,index,ID_LENGTH);
        index+=ID_LENGTH;
        stringToWrite[index]='\0';
        current=current->m_next;
    }
    stringToWrite[index]='\n';
    stringToWrite[index+1]='\0';
    fputs(stringToWrite,out);
    free(stringToWrite);
}
static Course* getCourse(EnrollmentSystem sys,char* courseNumber)
{
    Node*current=sys->m_courses;
    while(current!=NULL)
    {
        Course* courseToCheck=((Course*)current->m_element);
        if(areEqual(courseToCheck->m_courseName,courseNumber))
        {
            return courseToCheck;
        }
        current=current->m_next;
    }
    return false;
}
static int getPlaceOfPerson(IsraeliQueue queue,char*id)
{
    IsraeliQueue queueToCheck=IsraeliQueueClone(queue);
    int size=IsraeliQueueSize(queueToCheck);
    int count=0;
    for(int i=0;i<size;i++)
    {
        Student*student=((Student*)IsraeliQueueDequeue(queueToCheck));
        if(student==NULL)
        {
            break;
        }
        if(compareId(student->m_studentId,id))
        {
            IsraeliQueueDestroy(queueToCheck);
            return count;
        }
        count++;
    }
    IsraeliQueueDestroy(queueToCheck);
    return ERROR;
}
static bool isHackerInEnoughCourses(EnrollmentSystem sys,Hacker* hackerToCheck)
{
    int counter=0;
    for(int i=0;i<hackerToCheck->m_courseNumbersSize;i++)
    {
        Course* courseToCheck=getCourse(sys,hackerToCheck->m_courseNumbers[i]);
        if(courseToCheck==NULL)
        {
            continue;
        }
        int place=getPlaceOfPerson(courseToCheck->m_studentsQueue,hackerToCheck->m_studentId);
        if(place<0 || place>=courseToCheck->m_hackerSize)
        {
            continue;
        }
        else
        {
            counter++;
        }
    }
    if(counter>=2 || counter==hackerToCheck->m_courseNumbersSize)
    {
        return true;
    }
    return false;
}
static int putHackersInQueues(EnrollmentSystem sys)
{
    Node*current=sys->m_hackers;
    Student* studentToAdd=NULL;
    while(current!=NULL)
    {
        Hacker* currentHacker=((Hacker*)current->m_element);
        if(currentHacker==NULL)
        {
            return ERROR;
        }
        studentToAdd=getStudentFromId(sys,currentHacker->m_studentId);
        if(studentToAdd==NULL)
        {
            return ERROR;
        }
        for(int i=0;i<currentHacker->m_courseNumbersSize;i++)
        {
            Course* courseToAdd=getCourse(sys,currentHacker->m_courseNumbers[i]);
            if(courseToAdd==NULL)
            {
                return ERROR;
            }
            if(courseToAdd->m_studentsQueue==NULL)
            {
                continue;
            }
            if(IsraeliQueueEnqueue(courseToAdd->m_studentsQueue,studentToAdd)!=ISRAELIQUEUE_SUCCESS)
            {
                return ERROR;
            }
        }
        current=current->m_next;
    }
    return SUCCESS;
}
void hackEnrollment(EnrollmentSystem sys, FILE* out)
{
    if(sys==NULL)
    {
        return;
    }
    Node* hackersWithoutCoursesNeeded=NULL;
    Node*current=sys->m_hackers;
    //first put every hacker in their queues
    if(putHackersInQueues(sys)==ERROR)
    {
        return;
    }
    //passes over all the hackers
    while(current!=NULL)
    {
        bool hackerOK=isHackerInEnoughCourses(sys,((Hacker*)current->m_element));//returns if he is in enough courses
        if(!hackerOK)
        {
            char* id=((Hacker*)current->m_element)->m_studentId;
            Node*temp=createNode(id);
            hackersWithoutCoursesNeeded=addToEndOfList(hackersWithoutCoursesNeeded,temp);
        }
        current=current->m_next;
    }
    if(hackersWithoutCoursesNeeded==NULL)//write queues to out
    {
         writeQueuesToFile(sys->m_courses,out);
    }
    else//write id of hackers without constraint
    {
        writeHackersWithoutConstraint(hackersWithoutCoursesNeeded,out);
        destroyNodeList(hackersWithoutCoursesNeeded);
    }
}
void deleteEnrollment(EnrollmentSystem sysToDestroy)
{
    destroyHackersList(sysToDestroy);
    destroyStudentList(sysToDestroy);
    destroyCoursesList(sysToDestroy);
    free(sysToDestroy);
}

void updateFlag(EnrollmentSystem sys,bool withBigCharacters)
{
    sys->m_withBigCharacters=withBigCharacters;
}

