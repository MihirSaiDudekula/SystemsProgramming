#include "send_error_message.h"

int sendErrorMessage(int socket, int status_code){

    char str[1024];
    char currentTime[50];
    time_t now = time(0);

    struct tm data = *gmtime(&now);
    strftime(currentTime, sizeof(currentTime), "%a, %d %b %Y %H:%M:%S %Z", &data);

    switch (status_code) {
        case 400:
                sprintf(str, sizeof(str),
                "HTTP/1.1 400 Bad Request\r\n\
                Content-Length: 95\r\n\
                Connection: keep-alive\r\n\
                Content-Type: text/html\r\n\
                Date: %s\r\n\
                Server: Server\r\n\r\n\
                <html>\
                    <head>\
                        <title>400 Bad Request</title>\
                    </head>\n\
                    <body>\
                        <h1>400 Bad Request</h1>\n\
                    </body>\
                </html>", currentTime);
                perror("400 Bad Request\n");
                send(socket, str, strlen(str), 0);
                break;
        case 403:
                sprintf(str, sizeof(str),
                "HTTP/1.1 403 Forbidden\r\n\
                Content-Length: 112\r\n\
                Connection: keep-alive\r\n\
                Content-Type: text/html\r\n\
                Date: %s\r\n\
                Server: Serves\r\n\r\n\
                <html>\
                    <head>\
                        <title>403 Forbidden</title>\
                    </head>\n\
                    <body>\
                        <h1>403 Forbidden</h1>\n\
                    </body>\
                </html>", currentTime);
                perror("403 Forbidden\n");
                send(socket, str, strlen(str), 0);
                break;
        case 404:
                sprintf(str, sizeof(str),
                "HTTP/1.1 404 Not Found\r\n\
                Content-Length: 91\r\n\
                Connection: keep-alive\r\n\
                Content-Type: text/html\r\n\
                Date: %s\r\n\
                Server: Serves\r\n\r\n\
                <html>\
                    <head>\
                        <title>404 Not Found</title>\
                    </head>\n\
                    <body>\
                        <h1>404 Not Found</h1>\n\
                    </body>\
                </html>", currentTime);
                perror("404 Not Found\n");
                send(socket, str, strlen(str), 0);
                break;
        case 500:
                sprintf(str, sizeof(str),
                "HTTP/1.1 500 Internal Server Error\r\n\
                Content-Length: 115\r\n\
                Connection: keep-alive\r\n\
                Content-Type: text/html\r\n\
                Date: %s\r\n\
                Server: Serves\r\n\r\n\
                <html>\
                    <head>\
                        <title>500 Internal Server Error</title>\
                    </head>\n\
                    <body>\
                        <h1>500 Internal Server Error</h1>\n\
                    </body>\
                </html>", currentTime);
                perror("500 Internal Server Error\n");
                send(socket, str, strlen(str), 0);
                break;
        case 501:
                sprintf(str, sizeof(str),
                "HTTP/1.1 501 Not Implemented\r\n\
                Content-Length: 103\r\n\
                Connection: keep-alive\r\n\
                Content-Type: text/html\r\n\
                Date: %s\r\n\
                Server: Serves\r\n\r\n\
                <html>\
                    <head>\
                        <title>501 Not Implemented</title>\
                    </head>\n\
                    <body>\
                        <h1>501 Not Implemented</h1>\n\
                    </body>\
                </html>", currentTime);
                perror("501 Not Implemented\n");
                send(socket, str, strlen(str), 0);
                break;
        case 505: 
                sprintf(str, sizeof(str),
                "HTTP/1.1 505 HTTP Version Not Supported\r\n\
                Content-Length: 125\r\n\
                Connection: keep-alive\r\n\
                Content-Type: text/html\r\n\
                Date: %s\r\n\
                Server: Serves\r\n\r\n\
                <html>\
                    <head>\
                        <title>505 HTTP version Not Supported</title>\
                    </head>\n\
                    <body>\
                        <h1>505 HTTP Version Not Supported</h1>\n\
                    </body>\
                </html>", currentTime);
                perror("505 HTTP Version Not Supported\n");
                send(socket, str, strlen(str), 0);
                break;
        default: return -1;
    }
    return 0;
}