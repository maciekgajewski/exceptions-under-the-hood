class: center, middle

# Exceptions
## Under the Hood

Maciej Gajewski

---

# About me

.center[
<img src="pics/Maciek.jpg" height="300"/>

Maciek Gajewski

[maciej.gajewski0@gmail.com](mailto:maciej.gajewski0@gmail.com)

]


???

* 30 years of programming, 20 years of C++
* Previously: Embedded, backend, (Wrocław) 
* Previously: HFT, Teaching (London, Amsterdam)
* Now: Freelance, Crypto-currnecy trading
* Active member of C++ community, previously in A'dam, now in W-w

---
class: center, middle

## Error handling in C

---
# Return value

```c
int open(const char *pathname, int flags);
```

```
RETURN VALUE
  open() return the new file descriptor, or -1 if an error occurred
  (in which case, errnois set appropriately).
```

???

* The classical way of signaling error is trough a return value
* 'errno' is needed to pass error details
* errno used to be a variable, but turned out the API is insufficient

--

### Errno is not a variable!

```c
extern int *__errno_location (void) __THROW __attribute_const__;
# define errno (*__errno_location ())
```

???
This one is from GNU libc

--

```c
extern int *__geterrno(void);
#define errno (*__geterrno())
```
???
This one is from Android
---

## Return value (2)

```c
int fd, pos;
char* data;
struct stat sb;

fd = open(path, O_RDONLY);
fstat(fd, &sb);
data = (char *)malloc(sb.st_size);
pos = 0;
while (pos < sb.st_size) {
    pos += read(fd, data + pos, sb.st_size - pos);
}
```

???
* Simple C code reading file content into a buffer
* Easy to read, but no error handling

---

## Return value (2)

```c
int fd, r, pos;
char* data;
struct stat sb;

fd = open(path, O_RDONLY);
if (fd < 0) {
  perror("Error opening file"); exit(1);
}
fstat(fd, &sb);
data = (char *)malloc(sb.st_size);
if (data == 0) {
  perror("Error allocating buffer"); exit(2);
}
pos = 0;
while (pos < sb.st_size) {
    r = read(fd, data + pos, sb.st_size - pos);
    if (r < 0) {
      perror("Error reading data"); exit(3);
    }
    pos += r;
}
```
???

* Basic error handling interrupts the program
* Code is getting more difficult to read
* What if we want to turn it into a library?

---
## Return value (2)

```c
char* readall(const char* pathname) {
  int fd, r, pos;
  char* data;
  struct stat sb;

  fd = open(path, O_RDONLY);
  if (fd < 0)
    return NULL;
  fstat(fd, &sb);
  data = (char *)malloc(sb.st_size);
  if (data == 0)
    return NULL;
  pos = 0;
  while (pos < sb.st_size) {
      r = read(fd, data + pos, sb.st_size - pos);
      if (r < 0)
        return NULL;
      pos += r;
  }
  return data;
}
```

???

* The goal: propagate error to the caller
* Let caller decide if the error should be habdled gracioulsy
* Is it a correct code?

---
## Releasing resources

```c
char* readall(const char* pathname) {
  int fd, r, pos;
  char* data;
  struct stat sb;

  fd = open(path, O_RDONLY);
  if (fd < 0)
    return NULL;
  fstat(fd, &sb);
  data = (char *)malloc(sb.st_size);
  if (data == 0) {
    close(fd);
    return NULL;
  }
  pos = 0;
  while (pos < sb.st_size) {
      r = read(fd, data + pos, sb.st_size - pos);
      if (r < 0) {
        free(data);
        close(fd);
        return NULL;
      }
      pos += r;
  }
  return data;
}
```

???

* Note how complex the code have become
* The releasing of resources was not needed previously
* Not all reseoruices may be freed on process exit
* Processes were designed as 'microservices'
* How history repeats itself


---

## Releading resources (2)

.pull-left[
```c
char* readall(const char* pathname) {
  int fd, r, pos;
  char* data;
  struct stat sb;

  fd = open(path, O_RDONLY);
  if (fd < 0)
    goto open_failed:;
  fstat(fd, &sb);
  data = (char *)malloc(sb.st_size);
  if (data == 0)
    goto malloc_failed;
  pos = 0;
  while (pos < sb.st_size) {
      r = read(fd, data + pos,
            sb.st_size - pos);
      if (r < 0)
        goto read_failed;
      pos += r;
  }
  // to be continued...
```
]

.pull-right[
```c
 // ...continued
read_failed:
  free(data);
malloc_failed:
  close(fd);
open_failed:
  return NULL;
}
```
]

???

* This pattern is commonly found in C code
* Remember this pattern!
* Advantage: the error handling code is out-of-line

---

## Non-local error handling

TODO: longjmp

---

## Non-local error handling (2)

TODO: structured exceptions


---
# The Last Slide

* Maciej Gajewski <maciej.gajewski0@gmail.com>
* This presentation: https://maciekgajewski.github.io/exceptions-under-the-hood/
* Compiler Explorer: https://gcc.godbolt.org
* CppCon 2017, _“What Has My Compiler Done for Me Lately?”_, Matt Godbolt
* CppCon 2017, _"When a Microsecond Is an Eternity"_, Carl Cook
