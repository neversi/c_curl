#include "my_curl.h"

char **url_to_parts(char *);
void clean_parts(char **);

int my_curl(int argc, char **argv)
{

        if (argc == 2)
        {
                char *IPbuffer; 
	        struct hostent *host_entry;
                char **url_parts = url_to_parts(argv[1]);

                int client_socket;

                if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
                {
                        perror("socket");
                        return -1;
                }

                struct sockaddr_in send_addr;
                struct sockaddr_in this_addr;

                if ((host_entry = gethostbyname(url_parts[1])) == NULL)
                {
                        perror("gethostbyname");
                }
                IPbuffer = inet_ntoa(*((struct in_addr*) 
						host_entry->h_addr_list[0]));
                if (IPbuffer == NULL) {
                        perror("inet_ntoa");
                }
                puts(IPbuffer);

                this_addr.sin_family = AF_INET;
                this_addr.sin_port = htons(0);
                this_addr.sin_addr.s_addr = INADDR_ANY;

                send_addr.sin_family = AF_INET;
                send_addr.sin_port = htons(80);
                send_addr.sin_addr.s_addr = inet_addr(IPbuffer);
                int bind_status = bind(client_socket, (struct sockaddr *) &this_addr, sizeof(this_addr));
                if (bind_status) {
                        perror("bind");
                }
                int connection_status;
                connection_status = connect(client_socket, (struct sockaddr *) &send_addr, sizeof(send_addr));
                if (connection_status == -1)
                {
                        perror("connect");
                }
                char request[4096];
                char relative_url[2048];
                strcpy(relative_url, "/");
                for (int i = 2; url_parts[i] != NULL; i++)
                {
                        if (i != 2) strcat(relative_url, "/");
                        strcat(relative_url, url_parts[i]);
                }
                // puts(relative_url);
                // puts(url_parts[1]);
                // sprintf(request, "GET /~fdc/sample.html HTTP/1.1\r\nHost: www.columbia.edu\r\n\r\n");
                sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", relative_url, url_parts[1]);
                
                write(client_socket,(void *) request, strlen(request) + 1);
                long n;
                char response[2049];
                int start = 0;
                while (( n = read(client_socket,(void *) response, 2047)) > 0) {
                        if (n == 2047) {
                                response[2048] = 0;
                        }
                        else {
                                response[n] = 0;
                        }
                        printf("%s",response);
                }
                
                
        
                
                close(client_socket);
        }
} 


char **url_to_parts(char *string) 
{
        int size = 1;
        char **result = malloc(size * sizeof(char *));
        char *temp;
        temp = strtok(string, "//");
        do
        {
                result[size - 1] = malloc(strlen(temp) + 1);
                strcpy(result[size - 1],temp);
                size += 1;
                result = realloc(result, size * sizeof(char *));
                temp = strtok(NULL, "/");
        } while(temp != NULL);
        result[size - 1] = NULL;
        return result;
}

void clean_parts(char **array)
{
        for (int i = 0; array[i] != NULL; i += 1)
        {      
                free(array[i]);
        }
        free(array);
}
