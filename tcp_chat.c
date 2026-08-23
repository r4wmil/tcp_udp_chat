#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serv_addr = {AF_INET, htons(7777)};
	inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

	connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	char* msg = "hello\n";
	write(sock, msg, strlen(msg));

	char buf[1024] = {0};
	read(sock, buf, 1024);

	printf("%s\n", buf);

	return 0;
}
