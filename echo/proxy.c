#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024
#define PROXY_PORT 8888
#define REMOTE_SERVER_IP "localhost"
#define REMOTE_SERVER_PORT 8080

void handle_client_request(int client_sockfd) {
    int remote_sockfd;
    struct sockaddr_in remote_addr;
    char buffer[MAX_BUFFER_SIZE];
    int bytes_read;

    // 원격 서버에 연결
    remote_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(REMOTE_SERVER_PORT);
    remote_addr.sin_addr.s_addr = inet_addr(REMOTE_SERVER_IP);

    if (connect(remote_sockfd, (struct sockaddr*)&remote_addr, sizeof(remote_addr)) < 0) {
        perror("연결 실패");
        close(client_sockfd);
        exit(1);
    }

    // 클라이언트로부터의 요청을 원격 서버로 전달
    while ((bytes_read = read(client_sockfd, buffer, sizeof(buffer))) > 0) {
        printf("client send: %d\n", bytes_read);
        write(remote_sockfd, buffer, bytes_read);
    }
    // 원격 서버로부터의 응답을 클라이언트에게 전달
    while ((bytes_read = read(remote_sockfd, buffer, sizeof(buffer))) > 0) {
        printf("server send: %d\n", bytes_read);
        write(client_sockfd, buffer, bytes_read);
    }

    // 소켓 종료
    close(remote_sockfd);
    close(client_sockfd);
}

int main() {
    int sockfd, client_sockfd;
    struct sockaddr_in server_addr, client_addr;
    unsigned int client_len;

    // 소켓 생성
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // 소켓 설정
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PROXY_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // 바인딩
    bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // 리스닝
    listen(sockfd, 5);

    printf("프록시 서버가 시작되었습니다. 포트: %d\n", PROXY_PORT);

    // 클라이언트 요청 처리
    while (1) {
        client_len = sizeof(client_addr);
        client_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
        printf("클라이언트가 연결되었습니다.\n");

        handle_client_request(client_sockfd);

        printf("클라이언트와의 연결이 종료되었습니다.\n");
    }

    // 소켓 종료
    close(sockfd);

    return 0;
}
