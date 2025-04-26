// Practical 8: fork() + exec()

#include <stdio.h>
#include <unistd.h>  // for fork() and exec()
#include <sys/types.h> // for pid_t
#include <sys/wait.h>  // for wait()

int main() {
    pid_t pid;

    // Create a child process
    pid = fork();

    if (pid < 0) {
        // Fork failed
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // This is the child process
        printf("Child process (PID: %d) executing /bin/ls...\n", getpid());
        
        // Replace the child process with /bin/ls
        execl("/bin/ls", "ls", "-l", NULL);

        // If execl returns, it must have failed
        perror("execl failed");
        return 1;
    } else {
        // This is the parent process
        printf("Parent process (PID: %d) waiting for child (PID: %d)...\n", getpid(), pid);

        // Wait for the child process to finish
        wait(NULL);

        printf("Child process completed.\n");
    }

    return 0;
}
