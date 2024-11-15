
#include <pthread.h>		//Create POSIX threads.
#include <time.h>			//Wait for a random time.
#include <unistd.h>			//Thread calls sleep for specified number of seconds.
#include <semaphore.h>		//To create semaphores
#include <stdlib.h>			
#include <stdio.h>			//Input Output

pthread_t *Students;		//N threads running as Students.
pthread_t TA;				//Separate Thread for TA.

int ChairsCount = 0;
int CurrentIndex = 0;

/*TODO
 
//Declaration of Semaphores and Mutex Lock.
//Semaphores used:
//A semaphore to signal and wait TA's sleep.
//An array of 3 semaphores to signal and wait chair to wait for the TA.
//A semaphore to signal and wait for TA's next student.
 
 //Mutex Lock used:
//To lock and unlock variable ChairsCount to increment and decrement its value.
 
 //hint: use sem_t and pthread_mutex_t
 
 */

sem_t TA_sleep;				//TA sleeps if no students are at the desk
sem_t student_signal[3];	//Signals for when the student needs help
sem_t student_waiting;		//Controls the number of students that can wait
sem_t signal_done;			//Used to signal when a TA is finished

pthread_mutex_t mutex;


//Declared Functions
void *TA_Activity();
void *Student_Activity(void *threadID);

int main(int argc, char* argv[])
{
	int number_of_students;		//a variable taken from the user to create student threads.	Default is 5 student threads.
	int id;
	srand(time(NULL));

    /*TODO
	//Initializing Mutex Lock and Semaphores.
	
     //hint: use sem_init() and pthread_mutex_init()
     
     */
	sem_init(&TA_sleep, 0, 0);
	sem_init(&student_signal, 0, 3);
	sem_init(&student_waiting, 0, number_of_students);
	sem_init(&signal_done, 0, 0);
	//initialize mutex

	if(argc<2)
	{
		printf("Number of Students not specified. Using default (5) students.\n");
		number_of_students = 5;
	}
	else
	{
		printf("Number of Students specified. Creating %d threads.\n", number_of_students);
		number_of_students = atoi(argv[1]);
	}
		
	//Allocate memory for Students
	Students = (pthread_t*) malloc(sizeof(pthread_t)*number_of_students);

    /*TODO
	//Creating one TA thread and N Student threads.
     //hint: use pthread_create
	//Waiting for TA thread and N Student threads.
     //hint: use pthread_join
     */
	pthread_create(TA, NULL, TA_Activity, NULL);
	for(int i = 0; i < number_of_students; i++){
		pthread_create(&Students[i], NULL, Student_Activity, NULL);
	}

	for(int i = 0; i < number_of_students; i++){
		pthread_join(&Stidents[i], NULL);
	}
	//Free allocated memory
	free(Students); 
	return 0;
}

void *TA_Activity()
{
	/*
		Partially done
	*/
    /* TODO
	//TA is currently sleeping.

    // lock
    
    //if chairs are empty, break the loop.

	//TA gets next student on chair.

    //unlock

	//TA is currently helping the student
     
     //hint: use sem_wait(); sem_post(); pthread_mutex_lock(); pthread_mutex_unlock()

	 While(1) {
	 	//TA is doing nothing
	 	sem_wait(&TA_sleep);

		//TA is waiting for a student
		sem_wait(&student_signal);
		
		printf("The TA is helping the student.\n");
		//implement sleep time here to simulate the time spent helping the student
		sleep(X)

		//signals for next student
		sem_post(&done_signal);

		//releases chair for next student
		sem_post(&student_signal);

	 }

*/
}

void *Student_Activity(void *threadID) 
{
    /*TODO
     
	//Student  needs help from the TA
	//Student tried to sit on a chair.
	//wake up the TA.
    // lock
     
	// unlock
    //Student leaves his/her chair.
	//Student  is getting help from the TA
	//Student waits to go next.
	//Student left TA room
    
    //If student didn't find any chair to sit on.
    //Student will return at another time
     
     //hint: use sem_wait(); sem_post(); pthread_mutex_lock(); pthread_mutex_unlock()
			
	*/
}