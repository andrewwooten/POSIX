#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>

int runChild(int id);
int * randomPids();
void handleSig1(int signo);
void handleSig2(int signo);


int flag;
int counter;

int main(int argc, char const *argv[]){

	pid_t pids[10];
	int i;
	char buffer[256];
	int bytesRead, bytesWritten, maxBytes;
	int flag = 0;
	int counter = 0;

	// reading/writing/opening/closing file
	
	// bytesWritten = write(file, (void*)buffer, maxBytes);
	// bytesRead = read(file, (void*)buffer, maxBytes);
	// close(file);

	// /* TODO create log file */
	int fd = open("log.txt", O_CREAT|O_RDWR);
	write(fd, "LOG FILE\n", 10);
	close(fd);

	for(i=0;i<10;i++){
		if ((pids[i]=fork())<0){
			printf("ERROR - could not fork\n");
			abort();
		} else if (pids[i]==0){
			//flags[i][0]=(long)getpid();
			/* running child i */
			int a = runChild(i);
			exit(a);
		}
	}

	/* parent execution */
	/*
		1. Go into loop for 9 iterations
			a) sleep for 1 second
			b) send SIGUSR1 signals to 4 children (randomly chosen)
			c) Reads and prints data from the shared log file
		2. send SIGUSR2 to all children and wait for them to end, print their PIDs and exit status
		3. Exit program
	*/



	int *chosenPids = (int*)malloc(sizeof(int)*5);
	for(i=0;i<9;i++){
		/* sleep for 1 second and send SIGUSR1 signals to 4 random children */
		sleep(1);
		chosenPids = randomPids();
		int a;
		printf("ROUND %d\n", i+1);
		for(a=0;a<4;a++){
			kill(pids[chosenPids[a]],10);

		}
		printf("###\n");
		char output[10] = "#####\n";
		int fd = open("log.txt", O_RDWR);

		// while(lockf(fd,F_TEST,0!=0)){}
		
		/* file is unlocked */
		lseek(fd,0,SEEK_END);
		lockf(fd, F_LOCK, 0);
		write(fd, output, strlen(output));
		lockf(fd, F_ULOCK, 0);
		
		/* TODO: read and print data to the shared file */


	}
	/* send SIGUSR2 signals to all children */
	for(i=0;i<10;i++){
		kill(pids[i],12);
	}

	int status;
	pid_t pid;
	for(;;){
		pid = wait(&status);
		if (pid==-1){
			break;
		}
		printf("Child with PID %ld exited with status %d\n", (long)pid, status);
	}

	return 0;

}

int * randomPids(){
	/* returns a list of 4 random ints between 0 and 9 */
	static int list[4];
	int i,a,repeated;
	int temp;
	for(i=0;i<4;i++){
		repeated = 0;
		temp = rand()%10;
		for(a=0;a<i;a++){
			if (temp == list[a]){
				repeated = 1;
			}
		}
		if(repeated==0){
			list[i]=temp;
		} else {
			i--;
		}
	}
	return list;

}

int runChild(int id){
	/*
		1. Go into infinite loop
		2. Upon receiving SIGUSR1, print to the log file:
			"I am process #XXX, writing for the Xth time"
		3. Upon receiving SIGUSR2, exit the loop (using flag)
	*/
	long pid = (long)getpid();
	printf("I AM BORN! MY PID IS %ld\n", (long)pid);
	int count = 0;
	int terminate = 0;
	signal(10, handleSig1);
	signal(12, handleSig2);
	while(!terminate){
		terminate = flag;
	}
	exit(0);
}

void handleSig1(int signo){
	long pid = (long)getpid();
	char output[256];
	counter++;
	printf("SIGNAL 1 received by process #%ld\n",pid);
	char suffix[2] = "st";
	if (counter == 2){
		suffix[0] = 'n';
		suffix[1] = 'd';
	} else if (counter ==3){
		suffix[0] = 'r';
		suffix[1] = 'd';
	} else if(counter > 3){
		suffix[0] = 't';
		suffix[1] = 'h';
	}
	sprintf(output,"I am process #%ld, writing for the %d%c%c time\n",pid, counter, suffix[0],suffix[1]);
	// /* TODO create log file */
	int fd = open("log.txt", O_RDWR);

	// while(lockf(fd,F_TEST,0!=0)){}
	
	/* file is unlocked */
	lseek(fd,0,SEEK_END);
	lockf(fd, F_LOCK, 0);
	write(fd, output, strlen(output));
	lockf(fd, F_ULOCK, 0);
	printf("PRINTED: %s\n", output);
	
}

void handleSig2(int signo){
	long pid = (long)getpid();
	printf("SIGNAL 2 received by #%ld\n",pid);
	flag = 1;
}















