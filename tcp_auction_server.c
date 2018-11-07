#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string.h>
#include<signal.h>
#include<errno.h>
#include<ctype.h>
#include<unistd.h>

#define BUF_LEN 250

/* Function prototypes */
/* Generic error handling */
void error(char *);
/* Manage connection between client and server using file descriptors */
void manage_connection(int, int);
/* Generic signal handler to manage zombie processes */
void handle_sigcld(int);




int main(int argc, char *argv[])
{
	int sockfd;	/* Sock descriptor for rendevouz socket */
	int newsockfd;	/* Sock descriptor for empheral socket */
	int portno;
	int pid;
	int uniq_id;	/* Clients connecting to server must have uniq id */
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	struct sigaction cldsig;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s portno\n", argv[0]);
		exit(1);
	}
	/* Signal Handling */
	cldsig.sa_handler = handle_sigcld;
	sigfillset(&cldsig.sa_mask);
	cldsig.sa_flags = SA_RESTART | SA_NOCLDSTOP;
	sigaction(SIGCHLD, &cldsig, NULL);

	/* Create stream oriented rendezous socket */
	printf("Double Auction Server Booting\n");
	portno = atoi(argv[1]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		error("Error calling socket()");
	}

	/* Setting server details */
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		error("Error calling bind()");
	}

	if (listen(sockfd, 5) < 0)
	{
		error("Error calling listen()");
	}
	printf("Waiting for client connection...\n");
	
	/* Main loop, hand waiting for clients to contact server and picked off
	   queue
	*/
	while (1)
	{
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,
				   &clilen);
		if (newsockfd < 0)
		{
			error("Error calling accept()");
		}

		/* Created process (pid == 0), close rendenzous socket and pass 
		   processing to manage_connection. Else, close descriptor from
		   accept() to reuse descriptors.
		*/
		if ((pid = fork()) == 0)
		{
			close(sockfd);
			printf("Created sessions for client with PID of %d\n",
			       getpid());
			manage_connection(newsockfd, newsockfd);
			exit(EXIT_SUCCESS);
		}
		else
		{
			close(newsockfd);
		}
	}
	close(sockfd);
	return 0;
}


void error(char *msg)
{
	perror(msg);
	exit(1);
}

void manage_connection(int in, int out)
{
	int rc;	/* Read count */
	char in_data[BUF_LEN]; /* Buffer to hold bid */
	
	while (1)
	{
		memset(&in_data, 0, sizeof(in_data));
		rc = read(in, in_data, BUF_LEN);
		
		if (in_data[0] == 'X')
		{
			break;
		}
		write(out, test, strlen(test));
	}
	fprintf(stderr, "\nClient has exited, close session.");
	close(in);
}

void handle_sigcld(int signo)
{
	pid_t cld;
	while (0 < waitpid(-1, NULL, WNOHANG));
}