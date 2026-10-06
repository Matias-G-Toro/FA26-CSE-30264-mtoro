#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT "54105" // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once 


void usage(int status);
void argparse(char * command, int argc, char * argv);
void *get_in_addr(struct sockaddr *sa);
size_t get_size(char * filename, int ipaddr, int port, char * token);

// takes in four parameters:
//  the name of the file
//  the hostname / IP address of the grab server
//  the port number for the grab server
//  the authorization token to use.

int main(int argc, char *argv[])
{

	/* PARSE ARGUMENTS (with error checking)*/
	if (argc != 5) usage(1);


	char * command;
	arg
	char * name = argv[1];
	char * hostname = argv[2]; // TODO: DNS resolution, atoi/to ip adrees convesion
	char * port = argv[3]; // TODO: atoi
	char * token = argv[4]; // TODO: atoi
	

	/* Beej Template Start */
	
    int sockfd, numbytes;  
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        inet_ntop(p->ai_family,
            get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
        printf("client: attempting connection to %s\n", s);

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            perror("client: connect");
            close(sockfd);
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family,
            get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
	// NOTE: Client connected here

	// TODO: do all of the "write" system calls here. Logic is hardcoded based on argparse.

    freeaddrinfo(servinfo); // all done with this structure

	// TODO : While true loop perhaps everything in a buffer as well.
    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';

    printf("client: received '%s'\n",buf);

    close(sockfd);

    return 0;


	/* TESTS
	FB001.dat 127.0.0.1 54000 BinaryFilePNG
	F001.dat 127.0.0.1 54000 AuthSimpleF001.dat 127.0.0.1 54000 AuthSimple
	F002.dat 127.0.0.1 54000 AFE4c3982a
	FB001.dat 127.0.0.1 54000 BinaryFilePNG
	F001.dat 127.0.0.1 54000 AuthSimple
	F002.dat 127.0.0.1 54000 AFE4c3982a
	F001.dat 127.0.0.1 54000 AuthSimple
	F002.dat 127.0.0.1 54000 AFE4c3982a
	F003.dat 127.0.0.1 54000 BooBadgers
	F004.dat 127.0.0.1 54000 BooSparty
	# Comments in the mix
	F001.dat 127.0.0.1 54000 AuthSimple
	F002.dat 127.0.0.1 54000 AFE4c3982a
	F003.dat 127.0.0.1 54000 BooBadgers
 	# Bad authorization
	F004.dat 127.0.0.1 54000 BooSparty!
	# Unknown file
	F100.dat 127.0.0.1 54000 BooOwls
	F001.dat 127.0.0.1 54000 AuthSimple
	F002.dat 127.0.0.1 54000 AFE4c3982a
	FB001.dat 127.0.0.1 54000 BinaryFilePNG
	FB002.dat 127.0.0.1 54000 BinaryFileTwo
	FB003.dat 127.0.0.1 54000 BinaryOwls

	*/

}


void usage(int status) {
    fprintf(stderr, "Usage: cgrab FILE [HOST|IP] PORT TOKEN\n\n");
    exit(status);
}


void argparse(char * command, int argc, char * argv) {
	if (argc != 5) usage(1);
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
