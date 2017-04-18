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
   int ret, fi, fo;
   char stringToSend[BUFFER_LENGTH];
   printf("Starting device test code example...\n");
   fi = open("/dev/copinput", O_RDWR);
   fo = open("/dev/copoutput", O_RDWR);
   if (fi < 0){
      perror("Failed to open the input device...");
      return errno;
   }
   if (fo < 0){
      perror("Failed to open the output device...");
      return errno;
   }
   
   write_to_driver(fi, "Tyler");
   write_to_driver(fi, "JACOB");

   read_from_driver(fo, 5);
   read_from_driver(fo, 5); 

   write_to_driver(fi, "gerber");

   read_from_driver(fo, 3);

   read_from_driver(fo, 6);

   printf("End of the program\n");
   return 0;
}
