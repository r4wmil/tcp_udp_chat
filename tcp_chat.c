#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>

#define MAX_BUF 4096

int main() {
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serv_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(7777)
	};
	inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

	int c = -1;
	for (;;) {
				c = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
				if (c >= 0) break;
		perror("connect");
		sleep(2);
	}

	printf("connected.\n");

	char buf[MAX_BUF];

	struct pollfd fds[2] = {
		{ STDIN_FILENO, POLLIN, 0 },
		{ sock,		 POLLIN, 0 }
	};

	for (;;) {
		if (poll(fds, 2, -1) < 0) {
			perror("poll");
			break;
		}

		// stdin -> server
		if (fds[0].revents & POLLIN) {
			int n = read(STDIN_FILENO, buf, sizeof(buf));
			if (n <= 0) break;

			send(sock, buf, n, 0);
		}

		// server -> stdout
		if (fds[1].revents & POLLIN) {
			int n = recv(sock, buf, sizeof(buf) - 1, 0);

			if (n == 0) {
				printf("\ndisconnected by server.\n");
				break;
			}

			if (n < 0) {
				perror("recv");
				break;
			}

			buf[n] = '\0';
			printf("%s", buf);
			fflush(stdout);
		}
	}

	close(sock);
	return 0;
}
