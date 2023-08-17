#ifndef HACK_ENROLLMENT_H
#define HACK_ENROLLMENT_H
#include <stdbool.h>
typedef struct EnrollmentSystem_t* EnrollmentSystem;

EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers);

EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues);

void hackEnrollment(EnrollmentSystem sys, FILE* out);

void deleteEnrollment(EnrollmentSystem sysToDestroy);

void updateFlag(EnrollmentSystem sys,bool withBigCharacters);

#endif // HACK_ENROLLMENT_H
