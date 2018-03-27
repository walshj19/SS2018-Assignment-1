#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>
#include <signal.h>

extern int copy_file_updates();
extern int child();
extern int parent();
extern void sig_handler();
extern void backup();

char *dir = "/var/www/html";
char *lockName = "backupd.lock";

int main ()
{
	// int uid = geteuid();
	// printf("Effective user id: %d\n", uid);

	// create the child process
	int pid = fork();

	if (pid > 0)
	{
		exit(EXIT_SUCCESS);
	}
	else if (pid == 0)
	{
		child();
	}

	return 0;
}
/* Handles system signals, triggers a backup if the appropriate sigal is sent
 */
void sig_handler (int sigNum)
{
	syslog(LOG_NOTICE, "Signal rceived: %d", sigNum);

	// we have designated 10 to be the signal number used to trigger a backup
	if (sigNum == 10)
	{
		// trigger a backup
		backup();
	}
}

/* Performs the update and backup functionality
 */
void backup ()
{
	char lockPath[128];
	int fd;
	int fileCheck;

	// build the path to the lockfile
	strcpy(lockPath, dir);
	strcat(lockPath, "/dev/");
	strcat(lockPath, lockName);
	syslog(LOG_NOTICE, "%s\n",lockPath);

	// check for the lock
	if((fileCheck = access( lockPath, F_OK )) == 0 ) {
		// if it exists return as there is another backup underway
		syslog(LOG_NOTICE, "Failed to trigger a backup as there is a lock present on the folder: %d", fileCheck);
		return;
	}

	// lock the directory by creating the lock file
	fd = open(lockPath, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	close(fd);

	// perform the backup
	copy_file_updates("~/backup");

	// perform the update to prod
	copy_file_updates("/var/www/html/prod");

	// unlock the directory
	remove(lockPath);	
}

/* Copy all of the files in the source folder into the destination folder.
 */
int copy_file_updates (char *dest)
{
	int status;
	char command[256];

	// build the command string
	strcpy(command, "cp -u /var/www/html/dev/*.html ");
	strcat(command, dest);

	syslog(LOG_NOTICE, "Copying files");
	
	FILE *fp;
	// use popen to call cp with the -u flag to update files
	fp = popen(command, "r");
	status = pclose(fp);

	syslog(LOG_NOTICE, "Status of cp command: %d", status);
	return 0;
}

int child ()
{
	// open syslog
	openlog("backupd", LOG_CONS | LOG_PID, LOG_DAEMON);

	syslog(LOG_NOTICE, "Started daemon process");

	// Elevate the child to a new session leader
	if (setsid() < 0)
	{
		// if this fails exit the program
		syslog(LOG_NOTICE, "Failed to move process to a new session");
		exit(EXIT_FAILURE);
	}

	// set the file mode mask
	umask(0);

	// move to the root directory
	if (chdir("/") < 0)
	{
		// if this fails exit the program
		syslog(LOG_NOTICE, "Failed to move to the root directory");
		exit(EXIT_FAILURE);
	}

	// close open file descriptors
	for (int i = sysconf(_SC_OPEN_MAX); i >= 0 ; i--)
	{
		close(i);
	}

	// register the signal listener
	if (signal(SIGUSR1, sig_handler) == SIG_ERR)
	{
		syslog(LOG_NOTICE, "Failed to register the signal listener");
		exit(EXIT_FAILURE);
	}

	// daemon code starts here
	while(1)
	{
		sleep(1);
	}

	// exit the daemon
	syslog(LOG_NOTICE, "Exiting daemon");
	// close the connection to syslog
	closelog();
	exit(EXIT_SUCCESS);
	return 0;
}