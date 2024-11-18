#include <pthread.h>        //Create POSIX threads.
#include <time.h>           //Wait for a random time.
#include <unistd.h>         //Thread calls sleep for specified number of seconds.
#include <semaphore.h>      //To create semaphores
#include <stdlib.h>         
#include <stdio.h>          //Input Output

pthread_t *Students;        //N threads running as Students.
pthread_t TA;               //Separate Thread for TA.

int ChairsCount = 0;
int CurrentIndex = 0;

//Semaphores and Mutex Lock
sem_t TA_sleep;             //TA sleeps if no students are at the desk
sem_t student_signal[3];    //Signals for when the student needs help
sem_t student_waiting;      //Controls the number of students that can wait
sem_t signal_done;          //Used to signal when a TA is finished

pthread_mutex_t mutex;

//Declared Functions
void *TA_Activity();
void *Student_Activity(void *threadID);

int main(int argc, char* argv[])
{
    int number_of_students;     //a variable taken from the user to create student threads. Default is 5 student threads.
    int id;
    srand(time(NULL));

    //Initializing Mutex Lock and Semaphores
    pthread_mutex_init(&mutex, NULL);
    sem_init(&TA_sleep, 0, 0);
    sem_init(&student_waiting, 0, 3);
    sem_init(&signal_done, 0, 0);

    for (int i = 0; i < 3; i++) {
        sem_init(&student_signal[i], 0, 0);
    }

    if (argc < 2)
    {
        printf("Number of Students not specified. Using default (5) students.\n");
        number_of_students = 5;
    }
    else
    {
		number_of_students = atoi(argv[1]);

		if (number_of_students <= 0) {
			printf("Invalid number of students specified. Using default (5) students.\n");
		} else {
			printf("Number of Students specified. Creating %d threads.\n", number_of_students);
		}

    }
        
    //Allocate memory for Students
    Students = (pthread_t*) malloc(sizeof(pthread_t) * number_of_students);

    //Creating one TA thread and N Student threads
    pthread_create(&TA, NULL, TA_Activity, (void*)(long)number_of_students);

    for (int i = 0; i < number_of_students; i++) {
        pthread_create(&Students[i], NULL, Student_Activity, (void*)(long)i);
    }

    //Waiting for TA thread and N student threads
    pthread_join(TA, NULL);

    for (int i = 0; i < number_of_students; i++) {
        pthread_join(Students[i], NULL);
    }

    //Output when all students are helped
    printf("All students have been helped for the day.\n");

    //Free allocated memory
    free(Students);

    //Destroy mutex and semaphores
    pthread_mutex_destroy(&mutex);
    sem_destroy(&TA_sleep);
    sem_destroy(&signal_done);
    sem_destroy(&student_waiting);

    for (int i = 0; i < 3; i++) {
        sem_destroy(&student_signal[i]);
    }

    //Final output message
    printf("Program finished successfully.\n");

    return 0;
}

void *TA_Activity(void *number_of_students)
{
    int total_students = (int)(long)number_of_students;
    int students_helped = 0;

    while (students_helped < total_students) {
        sem_wait(&TA_sleep);

        //Lock for ChairsCount
        pthread_mutex_lock(&mutex);

        if (ChairsCount == 0) {
            pthread_mutex_unlock(&mutex);
            continue;
        }

        //TA helps the next student
        sem_post(&student_signal[CurrentIndex]);
        ChairsCount--;
        CurrentIndex = (CurrentIndex + 1) % 3;

        printf("The TA is helping a student.\n");

        //Simulate helping time
        sleep(rand() % 3 + 1);

        //Signal when help is done
        sem_post(&signal_done);

        //Count the student as helped
        students_helped++;

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void *Student_Activity(void *threadID)
{
    int id = (int)(long)threadID;

    while (1) {
        //Student arrives
        sleep(rand() % 10 + 1);

        printf("Student %d needs help from the TA.\n", id);

        //Attempt to wait for an available chair
        if (sem_trywait(&student_waiting) == 0) {
            pthread_mutex_lock(&mutex);

            if (ChairsCount < 3) {
                int index = (CurrentIndex + ChairsCount) % 3;
                ChairsCount++;

                printf("Student %d is waiting on chair %d\n", id, 3 - ChairsCount);

                pthread_mutex_unlock(&mutex);

                //Wake TA if they are asleep
                sem_post(&TA_sleep);

                //Wait for TA to help
                sem_wait(&student_signal[index]);

                printf("Student %d is getting help from the TA.\n", id);

                //Wait for TA to finish
                sem_wait(&signal_done);

                printf("Student %d leaves after getting help.\n", id);

                //Release waiting slot
                sem_post(&student_waiting);
                break;
            } else {
                pthread_mutex_unlock(&mutex);
            }

            //Release the waiting slot if no chair was found
            sem_post(&student_waiting);
        } else {
            printf("Student %d will return later as no chairs are available.\n", id);
        }
    }

    return NULL;
}

