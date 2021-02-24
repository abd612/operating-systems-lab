int
main(int argc, char *argv[])
{
  int* ptr = 0;
  printf("hello world! Using null pointer's value, it will cause the segmentation fault\n");
  printf(1, "%d\n", *ptr);
}
