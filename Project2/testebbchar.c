/**
 * @file   testebbchar.c
 * @author Derek Molloy
 * @date   7 April 2015
 * @version 0.1
 * @brief  A Linux user space program that communicates with the ebbchar.c LKM. It passes a
 * string to the LKM and reads the response from the LKM. For this example to work the device
 * must be called /dev/ebbchar.
 * @see http://www.derekmolloy.ie/ for a full description and follow-up descriptions.
*/
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_LENGTH 256               ///< The buffer length (crude but fine)
static char receive[BUFFER_LENGTH];     ///< The receive buffer from the LKM

void clear_buf(char* buf)
{
   int i = 0;
   for (; i < BUFFER_LENGTH; i++)
      buf[i] = 0;
}

int write_to_driver(int fd, char* str)
{
   int ret = write(fd, str, strlen(str));
   if (ret < 0){
      perror("Failed to write the message to the device.");
      return errno;
   }
   return 0;
}

int read_from_driver(int fd)
{
   char receive[256] = {0};
   int ret = read(fd, receive, 256);

   printf("The received message is: [%s]\n", receive);

   if (ret < 0){
      perror("Failed to read the message from the device.");
      return errno;
   }
   
   clear_buf(receive);
   return 0;
}

int main() {
   int ret, fd;
   char stringToSend[BUFFER_LENGTH];
   printf("Starting device test code example...\n");
   fd = open("/dev/ebbchar", O_RDWR);             // Open the device with read/write access
   if (fd < 0){
      perror("Failed to open the device...");
      return errno;
   }
   
   write_to_driver(fd, "Tyler");
   write_to_driver(fd, "Jacob");
   write_to_driver(fd, "dickbutt");

   read_from_driver(fd);

   write_to_driver(fd, "gerber");

   read_from_driver(fd);
   read_from_driver(fd);
   read_from_driver(fd);
   
   printf("End of the program\n");
   return 0;
}
