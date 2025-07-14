#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

void write_all(int desc, const char *buf)
{
  size_t total_written = 0;
  while(total_written < strlen(buf)) {

    ssize_t ch_written = write(desc, buf + total_written, strlen(buf));
    if(ch_written < 0)
    {
      perror("Unable to access file!\n");
      exit(EXIT_FAILURE);
    }
    total_written += ch_written;

  }
}

// TODO add error handling for read_reverse
void read_reverse(int desc, char *buf, size_t buf_size)
{
  off_t pos = lseek(desc, 0, SEEK_END);
  pos = lseek(desc, -1, SEEK_CUR);
  size_t n = 0;

  while(n < buf_size - 1 && pos >= 0)
  {
    read(desc, buf + n, 1);
    pos = lseek(desc, -2, SEEK_CUR);
    n++;
  }

  buf[n] = '\0';

}
int main()
{
  int desc = open("input.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  if(desc < 0)
  {
    perror("Unable to open file!\n");
    exit(EXIT_FAILURE);
  }

  char buf_write[] = "String from file";

  // Writing data in a loop, to ensure everything is written

  char buf_read[100];

  write_all(desc, buf_write);

  read_reverse(desc, buf_read, 100);

  printf("%s\n", buf_read);


  close(desc);

}
