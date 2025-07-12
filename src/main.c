#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include "cache.h"
#include "utils.h"

int main() {
    printf("Zerostore Started.\n");
    
    // Creates socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket initialisation failed!");
        exit(EXIT_FAILURE);
    }

    // Setting socket options
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Setting socket options failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Binding socket to port 12345
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(12345); 
    
    
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Socket bind failed!");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen to the server
    if (listen(server_fd, 1) < 0) {
        perror("Listening to server failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Listening on port 12345....\n");

    // Accept connectiong from a single client
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_len);
    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd < 0) {
        perror("Client connection failed!");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Printing client IP
    printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));

    // Setting a receive timeout
    struct timeval timeout;
    timeout.tv_sec = 100;
    timeout.tv_usec = 0;
    setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    // Read one message from client and print it
    char buffer[1024];
    while (1) {
        ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                fprintf(stderr, "Receive timed out\n");
                break;
            } else {
                perror("Receive failed\n");
                break;
            }
        } else if (bytes_received == 0) {
            printf("Client closed the connection.\n");
            break;
        } else {
            buffer[bytes_received] = '\0'; // null terminate
            buffer[strcspn(buffer, "\r\n")] = '\0';
            printf("Client says: %s\n", buffer);

            // Parse command
            char *cmd = NULL, *key = NULL, *value = NULL;
            int tokens = split_command(buffer, &cmd, &key, &value);
            char response[1024] = "";
            if (tokens == 0 || !cmd) {
                snprintf(response, sizeof(response), "ERR Invalid command\n");
            } else if (strcasecmp(cmd, "SET") == 0 && tokens == 3 && key && value) {
                set_key(key, value);
                snprintf(response, sizeof(response), "OK\n");
            } else if (strcasecmp(cmd, "SET") == 0) {
                snprintf(response, sizeof(response), "ERR Invalid SET command\n");
            } else if (strcasecmp(cmd, "GET") == 0 && tokens >= 2) {
                const char *result = get_key(key);
                if (result) {
                    snprintf(response, sizeof(response), "%s\n", result);
                } else {
                    snprintf(response, sizeof(response), "NOT FOUND\n");
                }
            } else if (strcasecmp(cmd, "DEL") == 0 && tokens >= 2) {
                const char *result = get_key(key);
                if (result) {
                    del_key(key);
                    snprintf(response, sizeof(response), "OK\n");
                } else {
                    snprintf(response, sizeof(response), "NOT FOUND\n");
                }
            } else if (strcasecmp(cmd, "EXIT") == 0) {
                snprintf(response, sizeof(response), "BYE\n");
                send(client_fd, response, strlen(response), 0);
                printf("Client wants to close the connection. Closing connection..\n");
                break;
            } else {
                snprintf(response, sizeof(response), "ERR Unknown command\n");
            }
            send(client_fd, response, strlen(response), 0);
        }
    }

    // Closing the connections
    close(client_fd);
    close(server_fd);

    // Clean up
    free_cache();

    return 0;
}