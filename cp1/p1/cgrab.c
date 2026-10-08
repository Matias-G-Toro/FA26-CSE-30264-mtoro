#include <fcntl.h>
#include <assert.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <openssl/evp.h>
#include <openssl/sha.h>


#define MAXDATASIZE 100 // max number of bytes we can get at once 


void usage(int status);
void *get_in_addr(struct sockaddr *sa);
size_t get_size(char * filename, int ipaddr, int port, char * token);
bool sha1sum_file(const char* path, char* cksum);


// takes in four parameters:
//  the name of the file
//  the hostname / IP address of the grab server
//  the port number for the grab server
//  the authorization token to use.

int main(int argc, char *argv[])
{

	/* Check arg count */
	if (argc != 5) usage(1);

	char * name = argv[1];  // NOTE: Validity handled by the server.
	char * hostname = argv[2]; // NOTE: DNS handled by Beej template
	char * port = argv[3]; // NOTE: DNS handled by Beej template
	char * token = argv[4]; // NOTE: Validity handled by the server

	/* Beej Template Start */
	
    int sockfd, numbytes;  
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(hostname, port, &hints, &servinfo)) != 0) {
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

    freeaddrinfo(servinfo); // all done with this structure


	// NOTE: Client connected here & addrinfo freed

	// https://cis.temple.edu/~giorgio/old/cis307s96/readings/docs/sockets.html#Connect
	// int bcount; /* counts bytes read */
	// int br;     /* bytes read this pass */


    /* Open file stream from socket file descriptor */

	// Ask for the info, checksum of everything:
	char send_buf[MAXDATASIZE];
	sprintf(send_buf, "INFO %s %s", name, token);
	printf("%s\n",send_buf);
	send(sockfd, send_buf, strlen(send_buf), NULL);

	/* Read HTTP Response */
    char read_buf[BUFSIZ];
	recv(sockfd, read_buf, BUFSIZ, NULL);
	printf("%s\n", read_buf);

	// close(socketfd); // first, perhaps?
    close(sockfd);

    return 0;


	/*
	int fail_count = EXIT_SUCCESS; // idk, i say EXIT_SUCCESS Being used before.
	char cksum[BUFSIZ];
	for (int i = 1; i < argc; i++) { 
		if (!sha1sum_file(argv[i], cksum)) {
			fail_count += 1;
		} else {
			printf("%s %s\n", cksum, argv[i]);
		}
	}
	*/

	/* TESTS
	FB001.dat 127.0.0.1 54000 BinaryFilePNG
	F001.dat 127.0.0.1 54000 AuthSimple
	F001.dat 127.0.0.1 54000 AuthSimple
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



// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// NOTE: Bui reading 1 code
bool sha1sum_file(const char* path, char* cksum) {
	bool success_flag = false;
	EVP_MD_CTX *mdctx = NULL;

	int fd = open(path, O_RDONLY); // Open file for reading
	if (fd < 0) {
		goto failure;
	}

	mdctx = EVP_MD_CTX_new(); // Create & initialize context	

	if (!mdctx || !EVP_DigestInit_ex(mdctx, EVP_sha1(), NULL)) {
		goto failure;
	}

	char buffer[BUFSIZ];
	ssize_t nread = 0;

	while ((nread = read(fd, buffer, BUFSIZ)) > 0) {
		// read file chunk by chunk and do a little math on each chunk to update and digest
		if (!EVP_DigestUpdate(mdctx, buffer, nread)) {
			goto failure;
		}
	}

	if (nread < 0) {
		goto failure;
	}

	/* Computer SHA1 */	
	uint8_t digest[SHA_DIGEST_LENGTH];
	if (!EVP_DigestFinal_ex(mdctx, digest, NULL)) {
		goto failure;
	}


	/* Convert digest to hexadecimal digest */
	for (int b = 0; b < SHA_DIGEST_LENGTH; b++) {
		snprintf(cksum + 2*b, 3, "%02x", digest[b]);
	}

	success_flag = true;

	failure:
		/* Clean up */
		if (fd >= 0) close (fd); // Every time I fall down to this label, I close down the file.
		if (mdctx) EVP_MD_CTX_free(mdctx);
		return success_flag;
}
