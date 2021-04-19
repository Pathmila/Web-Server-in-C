#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

#define PORT 9080

char imageheader[]="HTTP/1.1 200 OK\r\n"
"Content-Type: image/png\r\n\r\n";
char imageheader2[]="HTTP/1.1 200 OK\r\n"
"Content-Type: image/jpg\r\n\r\n";
char imageheader3[]="HTTP/1.1 200 OK\r\n"
"Content-Type: video/webm\r\n\r\n";
char imageheader4[]="HTTP/1.1 200 OK\r\n"
"Content-Type: application/pdf\r\n\r\n";
char imageheader5[]="HTTP/1.1 200 OK\r\n"
"Content-Type: audio/ogg\r\n\r\n";
char imageheader6[]="HTTP/1.1 200 OK\r\n"
"Content-Type: image/gif\r\n\r\n";
char imageheader7[]="HTTP/1.1 200 OK\r\n"
"Content-Type: image/jpeg\r\n\r\n";
char imageheader8[]="HTTP/1.1 200 OK\r\n"
"Content-Type: image/tar\r\n\r\n";
char imageheader9[]="HTTP/1.1 200 OK\r\n"
"Content-Type: text/html\r\n\r\n";

char html[]= 
"<!DOCTYPE html>"
"<html><head><title>18020798-Web Server</title><link rel=\"shortcut icon\" type=\"image/jpg\" href=\"doctext.jpg\"/>"
"<body bgcolor= #fffdd0><center><h2><font color=dodgerblue size=18px>IS2111 Computer Networks - Assignment</font></h2>"
"<center><h2><font color=dodgerblue size=18px>E.A.P.Silva - 18020798</font></h2></center>"
"<center><h4><font color=dodgerblue size=13px>Web server using C language</font></h4></center>"
"<h3>Following file types are sent from the server.</h3><br>"
"This is JPG image<br /><br />"
"<img width=\"200\" src=\"bird.jpg\"><br /><br />"
"This is PNG image<br /><br />"
"<img width=\"200\" src=\"berry.png\"><br /><br />"
"This is JPEG image<br /><br />"
"<img width=\"200\" src=\"bee.jpeg\"><br /><br />"
"This is GIF image<br /><br />"
"<img width=\"300\" src=\"minions.gif\"><br /><br />"
"You can view or download PDF document by clicking below link<br />"
"<a href=\"PDFdoc.pdf\">View PDF document</a><br /><br />"
"You can view or download Tar fie by clicking below link<br />"
"<a href=\"flower.tar\">Get the flower.tar</a><br /><br />"
"You can go index.html webpage by clicking below link<br />"
"<a href=\"index.html\">Go to index.html</a><br /><br />"
"This is a webm file type video<br /><br />"
"<video  controls><source src=\"count.webm\" type=\"video/webm\"></video><br /><br />"
"This is a ogg file type audio<br /><br />"
"<audio  controls><source src=\"bell.ogg\" type=\"audio/ogg\"></audio><br /><br />"
"</body></html>";

char html2[]= 
"<!DOCTYPE html>"
"<html><head><title>18020798-Web Server</title><link rel=\"shortcut icon\" type=\"image/jpg\" href=\"doctext.jpg\"/>"
"<body><center><h2>404 Page Not Found</h2>"
"<img width=\"500\" src=\"404.jpg\"></center><br /><br />"
"</body></html>";

char mybrowser[40000] = {0};
char *Type;

int myserver, client_socket;
struct sockaddr_in sock_address;
int addrlength = sizeof(sock_address);

int main(){
	printf("Port Number : 9080\n");
    	char requestFileType[4];
	char requestFilepath[1024];

	//creating socket
	if ((myserver = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        	perror("There is an error in your socket"); //Making the socket
        	exit(EXIT_FAILURE);
	}
	sock_address.sin_family = AF_INET;
	sock_address.sin_addr.s_addr = INADDR_ANY;
	sock_address.sin_port = htons(PORT);
	
	memset(sock_address.sin_zero, '\0', sizeof sock_address.sin_zero); //memset() is used to fill a block of memory with a particular value.
	
	//binding the socket
	if (bind(myserver, (struct sockaddr *)&sock_address, sizeof(sock_address)) == -1){
        	perror("There is an error in bind");
        	exit(EXIT_FAILURE);
    	}

	//listning 
	if (listen(myserver, 10) < 0){
	        perror("There is an error in listening");
        	exit(EXIT_FAILURE);
	}

   	while (1){
        	printf("Conection is establishing....\n");
        	if ((client_socket = accept(myserver, (struct sockaddr *)&sock_address, (socklen_t *)&addrlength)) < 0){
	        	perror("There is an error in accept");
            		exit(EXIT_FAILURE);
	        }

        	read(client_socket, mybrowser, 1024); //reads data previously written to a file
        	sscanf(mybrowser, "%s %s", requestFileType, requestFilepath);

        	char *filemime = strrchr(requestFilepath, '.') + 1; //return last occurence in string
        	char *name = strtok(requestFilepath, "/");      // split string to token

        	if (filemime){
        		Type = filemime;
	        }else{
		        Type = NULL;
		}
		
	   	int image;
	  	char mybrowser[2048];		

        	if (!strcmp(requestFileType, "GET") && !strcmp(requestFilepath, "/")){ // compare two string character by character

			if(!fork()){
				memset(mybrowser,0,2048);
				read(client_socket,mybrowser,2047);

			
				if(strncmp(mybrowser, "GET /doctext.jpg", 16)){
					image = open("doctext.jpg",O_RDONLY);
					write(client_socket,imageheader2,sizeof(imageheader2)-1);
					sendfile(client_socket,image,NULL,56000);
					printf("doctext.jpg sent to the server\n");
					close(image);	
				}else if(strncmp(mybrowser,"GET /bird.jpg",80) >= 0){
					image = open("bird.jpg",O_RDONLY);
					write(client_socket,imageheader2,sizeof(imageheader2)-1);
					sendfile(client_socket,image,NULL,36000);
					close(image);
				}else if(strncmp(mybrowser,"GET /berry.png",80) >= 0){
					image = open("berry.png",O_RDONLY);
					write(client_socket,imageheader,sizeof(imageheader)-1);
					sendfile(client_socket,image,NULL,12000);
					printf("berry.png sent to the server\n");
					close(image);
				}else if(strncmp(mybrowser,"GET /bee.jpeg",80) >= 0){								
					image = open("bee.jpeg",O_RDONLY);
					write(client_socket,imageheader7,sizeof(imageheader7)-1);
					sendfile(client_socket,image,NULL,32000);
					printf("images.jpeg sent to the server\n");
					close(image);					
				}else if(strncmp(mybrowser,"GET /minions.gif",80) >= 0){
					image = open("minions.gif",O_RDONLY);
					write(client_socket, "HTTP/1.1 200 OK\r\n", 17);
					write(client_socket,imageheader6,sizeof(imageheader6)-1);
					sendfile(client_socket,image,NULL,260000);
					printf("minions.gif sent to the server\n");
					close(image);
				}
				else if(strncmp(mybrowser,"GET /PDFdoc.pdf",80) >= 0){
					image = open("PDFdoc.pdf",O_RDONLY);
					write(client_socket, "HTTP/1.1 200 OK\r\n", 17);
					write(client_socket,imageheader4,sizeof(imageheader4)-1);
					sendfile(client_socket,image,NULL,14000);
					printf("PDFdoc.pdf sent to the server\n");
					close(image);	
				}else if(strncmp(mybrowser,"GET /flower.tar",80) >= 0){
					image = open("flower.tar",O_RDONLY);
					write(client_socket,imageheader8,sizeof(imageheader8)-1);
					sendfile(client_socket,image,NULL,37000);
					printf("flower.tar sent to the server\n");
					close(image);
				}else if(strncmp(mybrowser,"GET /bell.ogg",80) >= 0){
					image = open("bell.ogg",O_RDONLY);
					write(client_socket, "HTTP/1.1 200 OK\r\n", 17);
					write(client_socket,imageheader5,sizeof(imageheader5)-1);
					sendfile(client_socket,image,NULL,190000);
					close(image);
				}else if(strncmp(mybrowser,"GET /count.webm",80) >= 0){
					image = open("count.webm",O_RDONLY);					
					write(client_socket, "HTTP/1.1 200 OK\r\n", 17);
					write(client_socket,imageheader3,sizeof(imageheader3)-1);
					sendfile(client_socket,image,NULL,47000);
					close(image);
				}else{
					write(client_socket,html,sizeof(html)-1);
				}

			}

			char *data = html;
           		Type = "text/html";
			int fd_client_scok= client_socket;
			char *header="HTTP/1.1 200 OK";
			void *body=data;
			int ConLength=strlen(data);
      

			char response[ConLength + 100];
    			int response_length = sprintf(response, //sprintf similar to prontf but return the string
                       		"%s\n"
                                "Connection: close\n"
                                "Content-Length: %d\n"
                                "Content-Type: %s\n"
                                "\n",

                                header,
                                ConLength,
                                Type);
    			memcpy(response + response_length, body, ConLength); //copy block of memory from a location to another

	    		send(fd_client_scok, response, response_length + ConLength, 0);  //transmit message to another socket
		}else{
			int fd_client_scok= client_socket;
			char *FileName=name;

			char *entry;
			FILE *file = fopen(FileName, "r"); //fopen - open file
    			size_t bufsize;
			
			if (file != NULL){
        			if (fseek(file, 0L, SEEK_END) == 0){ //fseek - sets the file position of the sream to given offset
	            			bufsize = ftell(file); //return current position of file
            				entry = malloc(sizeof(char) * (bufsize + 1));
            				fseek(file, 0L, SEEK_SET);
            				fread(entry, sizeof(char), bufsize, file);

					char *header="HTTP/1.1 200 OK";
					void *body=entry;
					int ConLength=bufsize;

					char response[ConLength + 100];
	   				int response_length = sprintf(response, //sprintf similar to prontf but return the string
			                        "%s\n"
			                        "Connection: close\n"
		                        	"Content-Length: %d\n"
			                        "Content-Type: %s\n"
			                        "\n",
			                        header,
			                        ConLength,
			                        Type);
					memcpy(response + response_length, body, ConLength); //copy block of memory from a location to another
					send(fd_client_scok, response, response_length + ConLength, 0);  //transmit message to another socket

        			}
	    		}else{
				if(!fork()){
					memset(mybrowser,0,2048);
					read(client_socket,mybrowser,2047);
					if(strncmp(mybrowser, "GET /doctext.jpg", 16)){
						image = open("doctext.jpg",O_RDONLY);
						write(client_socket,imageheader2,sizeof(imageheader2)-1);
						sendfile(client_socket,image,NULL,56000);
						printf("doctext.jpg sent to the server\n");
						close(image);	
					}else if(strncmp(mybrowser,"GET /404.jpg",80) >= 0){
						image = open("404.jpg",O_RDONLY);
						write(client_socket,imageheader2,sizeof(imageheader2)-1);
						sendfile(client_socket,image,NULL,111000);
						close(image);
					}else{
						write(client_socket,html2,sizeof(html2)-1);
					}
				}

        			//char *error = "file not found";
				char *error = html2;
        			Type = "text/html";
				
				char *header="HTTP/1.1 404 NOT FOUND";
				void *body=error;
				int ConLength=strlen(error);

				char response[ConLength + 100];
   				int response_length = sprintf(response, //sprintf similar to prontf but return the string
	                                "%s\n"
	                                "Connection: close\n"
                                	"Content-Length: %d\n"
	                                "Content-Type: %s\n"
	                                "\n",
	                                header,
        	                        ConLength,
        	                        Type);
				memcpy(response + response_length, body, ConLength); //copy block of memory from a location to another
				send(fd_client_scok, response, response_length + ConLength, 0);  //transmit message to another socket
    			}
       		}
        	printf("Server sent the response.\n");
	}
	return 0;
}
