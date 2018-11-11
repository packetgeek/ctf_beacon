/* ctf_beacon.c for CTFs

Version 0.01 
Tim Kramer - joatblog@gmail.com 20181110

Purpose: This program periodically sends out a UDP broadcast beacon, containing a
         given string.  Main idea is to check if contestants are paying attention
         to the network. Syntax is:

         ./ctf_beacon <IP address> <port> "<string>" <seconds between beacons>

         Example:

         ./ctf_beacon 192.168.2.255 9999 "flag ABCD-1234-xy2z" 5

Compile this program via:

        gcc -o ctf_beacon ctf_beacon.c

Notes:

  - Doesn't require root permissions to run (unless port < 1024).
  - This was written at 2 a.m. so please excuse the errors.

Ideas for future versions:

  - instead of a set sleep span, how about sleep span plus a random number?
  - embedd a timestamp

Adapted from/inspired by: 

  - Noah Davids - http://noahdavids.org/self_published/beacon.html
  - Michael J. Donahoo - http://cs.ecs.baylor.edu/~donahoo/practical/CSockets/code/BroadcastSender.c

*/

#include <stdio.h>	// provides socket() and bind()
#include <stdlib.h>	// provides printf() and fprintf()
#include <string.h>	// provides sockaddr_in structure
#include <unistd.h> 	// provides sleep()
#include <sys/socket.h>	// provides atoi() and exit()
#include <arpa/inet.h>	// provides memset()

int main(int argc, char *argv[]) {

	// declare the variables

	int sock;
	struct sockaddr_in broadcastAddr;
	char *broadcastIP;
	int broadcastPort;
	int sleepSeconds;
	char *message;
	int broadcastPermission;
	int messageLen;

	// test for the correct number of arguments

	if(argc != 5) {
		fprintf(stderr,"Usage: %s <IP address> <port> \"<string>\" <secs to sleep>\n", argv[0]);
		exit(1);
	}

	// parse the arguments

	broadcastIP = argv[1];
	broadcastPort = atoi(argv[2]);
	message = argv[3];
	sleepSeconds = atoi(argv[4]);

	// check the broadcast IP argument

	if((strlen(broadcastIP) <7) || (strlen(broadcastIP)>15)) {
		fprintf(stderr,"IP address has issues\n");
		exit(1);
	}

	// check the port number argument

	if((broadcastPort < 1) || (broadcastPort > 65535)) {
		fprintf(stderr,"port is outside of acceptable range\n");
		exit(1);
	}

	// check the sleep seconds argument

	if(sleepSeconds <= 0) {
		fprintf(stderr,"sleep seconds needs to be greater than 0\n");
		exit(1);
	}

	// check the message argument

	if(strlen(message) > 32) {
		fprintf(stderr,"cannot send message > 32 characters in length\n");
		exit(1);
	}

	// create the socket for sending datagrams

	if((sock=socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		fprintf(stderr,"socket() failed\n");
		exit(1);
	}

	// allow the socket to broadcast

	broadcastPermission = 1;

	// check that the above worked

	if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, sizeof(broadcastPermission)) <0) {
		fprintf(stderr,"setsockopt() failed\n");
		exit(1);
	}

	// construct the broadcast address structure

	memset(&broadcastAddr, 0, sizeof(broadcastAddr));
	broadcastAddr.sin_family = AF_INET;
	broadcastAddr.sin_addr.s_addr = inet_addr(broadcastIP);
	broadcastAddr.sin_port = htons(broadcastPort);

	// determine the length of the message

	messageLen = strlen(message);

	// periodically send the message until someone hits Ctrl-C

	for (;;) {
		if(sendto(sock, message, messageLen, 0, (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr)) != messageLen) {
			fprintf(stderr,"sendto() sent a different number of bytes than expected\n");
			exit(1);
		}	
		sleep(sleepSeconds);
	}
}

