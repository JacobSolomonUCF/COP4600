/**
 * File: testchar.c
 * Authors: Tyler Petresky, Jacob Solomon, David Akridge
 *
 * NOTE: This test file was based on a file from http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/
 */

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_LENGTH 1024
static char receive[BUFFER_LENGTH];

void clear_buf(char* buf)
{
   int i = 0;
   for (; i < BUFFER_LENGTH; i++)
      buf[i] = 0;
}

int write_to_driver(int fd, char* str)
{
   printf("Writing to driver: [%s]\n", str);
   int ret = write(fd, str, strlen(str));
   if (ret < 0){
      perror("Failed to write the message to the device.");
      return errno;
   }
   return 0;
}

int read_from_driver(int fd, int n)
{
   char* receive = malloc(sizeof(char) * 256);
   int ret = read(fd, receive, n);

   printf("The received message is: [%s]\n", receive);

   if (ret < 0){
      perror("Failed to read the message from the device.");
      return errno;
   }
   
   free(receive);
   receive = NULL;
   return 0;
}

int main() {
   int ret, fd;
   char stringToSend[BUFFER_LENGTH];
   printf("Starting device test code example...\n");
   fd = open("/dev/copchar", O_RDWR);
   if (fd < 0){
      perror("Failed to open the device...");
      return errno;
   }
   
   write_to_driver(fd, "Tyler");
   write_to_driver(fd, "Jacob");

   read_from_driver(fd, 2); //ty

   write_to_driver(fd, "gerber");

   read_from_driver(fd, 3); //ler

   read_from_driver(fd, 6); // jacobg
   read_from_driver(fd, 8); // erberd
   read_from_driver(fd, 3);
   
   printf("End of the program\n");
   return 0;
}
