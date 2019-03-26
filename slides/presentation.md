class: center, middle

# Exceptions

## Under the Hood

Maciek Gajewski

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
* Now: Freelance, Crypto-currency trading
* Active member of C++ community, previously in A'dam, now in W-w


<!-- =================== Introduction ==================== -->

<!-- TODO:
* exceptions are complex
* many implementations
* depends on both platform and compiler
* list docos:
 - itianium ABI
  - ARM aBI
  Windows SEH, 32, 64
   - ? gcc SJLS exceptions?
-->

---
class: center, middle

# Introduction

---

## Exceptions: it's complicated

.pull-left[
* Multiple actors
    - compiler
    - linker
    - runtime
]
.pull-right[
* Multiple standards
    - SJLJ Exceptions
    - DWARF-2 EH
    - Itanium ABI
    - ARM ABI
    - Windows SEH, 32, 64 bit
]
.center[
<img src="pics/itscomplicated.jpg" height="300"/>
]

???

* There is many actors than need to play together for exceptions to work
* Part of it compiler-specific, part is system specific
* Many standards that implement the exception in a very different way
* Itanium is important - unifies exception handling across compilers
* ARM follows Itanium is just smaller
* Win32 is terrible, with hardware exceptions, 64 is similar to Itanium
* Winbdows: gcc had issue wth exceptions going trough foreign stack frames

<!-- ================= Chapter I - C ==================== -->
---
class: center, middle

# Chapter I

## Error handling in C

???

It all started with C, after all

---

# Return value

```c
int open(const char *pathname, int flags);
```

```
RETURN VALUE
  open() return the new file descriptor, or -1 if an error occurred
  (in which case, errno is set appropriately).
```

???

* The classical way of signaling error is trough a return value
* 'errno' is needed to pass error details
* errno used to be a variable, but turned out the API is insufficient

--

### Errno is not a variable!

--

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
* Insert Rufus story here?

---

## Return value (3)

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
## Return value (4)

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
* Let caller decide if the error should be handled graciously
* Is this a correct code?

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
* Not all resoruces may be freed on process exit
* Processes were designed as 'microservices'
* How history repeats itself


---

## Releasing resources (2)

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
  return data;
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

### longjmp

.pull-left[
```c
jmp_buf buffer; // global variable

void fun_a() {

  int error_code = setjmp(buffer);
  if (r == 0) {
    // normal path
    // always taken!
    void fun_b();
  } else {
    // this is error handling code
    on_error(r);
  }
}
```
]
.pull-right[
```c
void fun_c() {
  if (do_something()) {
    // oops, error!
    longjmp(buffer);
    // the end
  }
  // other code
  // ...
}
```
]

???

* jmp_buf contains machine state, 150-260 bytes, depending on architecture
* setjmp/longjump allows for interrupting the code
* Similar to exceptions...
* One needs to adopt a convention for the entire project
* But doesn't handle resources!

---

## Non-local error handling (2)

### Hardware exceptions

* Can be caused by user code, or by CPU
* Normally, handled by the OS
* User-defined function is executed

???

* The goal of hardware exceptions is to notify user code if something bad happens
* User code can also raise an exceptions
* Exception can be asynchronous
* Execution can resume

---

background-image: url(pics/except89.jpg)


???

The picture is from 1989 exceptions proposal paper.
source: http://www.stroustrup.com/except89.pdf

* Sometimes you just want the program to die on error
* ... but sometimes not.
* Sometimes you want to check the error locally
* ... but sometimes you want central handling
* ... and then you want the program to remain consistent.
* It's good to know what went wrong, if only to print an error message

I think by now the design principles behind C++ exceptions are clear.


---
background-image: url(pics/maclisp-exceptions.jpg)

???

* Page from MacLisp manual, 1974: 
* Source: http://www.softwarepreservation.org/projects/LISP/MIT/Moon-MACLISP_Reference_Manual-Apr_08_1974.pdf
* Exceptions we used in earlier versions of Lips, but different keyword were used

<!-- ===================== Chapter II - C++ exceptions =============== -->
---
class: center, middle

# Chapter II

## C++ exceptions

???

Introduced in 1990

---

## The basics

.pull-left[
Throwing

```c++
int bar(int p) {
  if (p == 666)
    throw std::logic_error("Evil!");
  return p+7;
}
```

"Innocent bystander"

```c++
int bystander(int p) {
  Widget w;
  return w.baz(bar(p) + foo(p));
}
```

]

.pull-right[
Catching

```c++
  int error_handler(int p) {
    try {
      auto x = foo(p);
      return fun(x);
    } catch(const std::exception& e) {
      log_err(e.what());
    }
  }
```

]

???
There are 3 main scenarios:

* Throwing exception
* Handling exception
* "Innocent bystander"

---

## Catching

```c++
  int error_handler(int p) {
    try {
      return fun(x);
    }
    catch(const std::logic_error& e) {
      on_logic_error(e.what());
    }
    catch(const std::exception& e) {
      log_err(e.what());
    }
    catch(int error_code) {
      log_err(strerror(error_code));
    }
    catch(...) {
      log_err("Unknown error");
    }
  }
```

???

* Catch ladder looks a bit like function overloading
* But in fact is more akin to a switch statement
* Catch block are evaluated at runtime in order of appearance
* Type is the key, const/volatile/reference is ignored

---

## Quiz

Which handler will be called?

```c++
try {
  throw 7;
}
catch (const int e) {
  // A
}
catch (int e) {
  // B
}
catch(...) {
  // C
}
```

???

This code won't compile, or it will generate a warning

---

## Quiz (2)

Which handler will be called?

```c++
try {
  throw 7;
}
catch (unsigned int e) {
  // A
}
catch (int e) {
  // B
}
catch(...) {
  // C
}
```

???

'unsigned int' is different type than 'int'

---

## Quiz (3)

Which handler will be called?

```c++
try {
  throw 7u;
}
catch (unsigned int e) {
  // A
}
catch (int e) {
  // B
}
catch(...) {
  // C
}
```

???

'unsigned int' is different type than 'int'

---

## noexcept specifier

"The noexcept-specification is a part of the function type and may appear as part of any function declarator"
```c++
void fun() noexcept => void fun() noexcept(constexpr-true)
```

```c++
void fun() => void fun() noexcept(constexpr-false)
```

By default, the below are `noexcept(true)`:

* destructors
* default constructors, copy constructors, move constructors
* default copy-assignment operators, move-assignment operators
* deallocation functions

Replaces `throw()`

???

* noexcept is also an operator, but this is outside of the scope
* since C++11, replaces obsolete throw() specification

---

## std::terminate

* `std::set_terminate(std::terminate_handler h)`
* `std::get_terminate(std::terminate_handler h)`

Obsolete, used to work with `throw()`
* `std::unexpected()`

???

* Called when no suitable handler for exception can be thrown
* Can be set by the user: `std::set_terminate`, `std::get_terminate`
* Type: `std::terminate_handler`

* There used to be `std::unexpected`, working with `throw()`

* std::terminate is also a part of exception handling mechanism
* Allows the user to customize the handling of uncaught exceptions

---

background-image: url(pics/Explosion_of_first_Ariane_5_flight_June_4_1996.jpg)

## std::terminate in practice

???

* This is how uncaught exception looks ike on 700M EUR rocket (Ariane 5)
* In this case, it was uncaught integer overflow exception in Ada
* Of course, for some projects, terminating is the best choice

---

## std::exception_ptr

Get current exception:
```c++
std::exception_ptr std::current_exception() noexcept;
```

Rethrow the exception:
```c++
[[noreturn]] void std::rethrow_exception(std::exception_ptr p);
```

Check if there are exceptions in-flight
```c++
int std::uncaught_exceptions() noexcept;
```

Make your own:
```c++
template<class E> std::exception_ptr std::make_exception_ptr(E e) noexcept;
```

???

* Very opaque smart pointer
* Allows for storing current exception
* Allows for transferring the exception, ie across threads

---

## Party trick

```c++
class ExceptionDetector {

  int exception_count;

public:

  ExceptionDetector() {
    exception_count_ = std::uncaught_exceptions();
  }

  ~ExceptionDetector() {
    if (std::uncaught_exceptions() == exception_count)
      std::cout << "Going out of scope" << std::endl;
    else
      std::cout << "Stack unwinding!" << std::endl;
  }
};
```

???

I don't know oif this is useful, but it's cool!

<!-- ============================ RTTI ============================ -->
---
class: center, middle

# Chapter III

## RTTI

(Run-time type identification)

???

* RunTime Type Identification
* RTTI is a key component of exception handling implementation
* The closest C++ (currently) has to reflection

---

# The 'typeid' operator

```c++
const std::type_info& ti = typeid(value);
```

```c++
const std::type_info& ti = typeid(Type);
```

```sh
$ objdump -T libstdc++.so|c++filt|grep 'typeinfo for'|wc -l
260
```

???

* Compiler generates global variable for every type
* typeid may be invoked on type or value
* for static types it's the same
* for virtual types, the typeinfo is loaded from vtable
* GCC 7' C++ lib contains 260 typeinfos

---

# The 'std::type_info' class

```c++
class type_info {
  type_info() = delete;
  type_info(const type_info&) = delete;
  type_info& operator=(const type_info) = delete;

  bool operator==(const type_info& rhs) const;
  bool operator!=(const type_info& rhs) const;

  const char* name() const;

  size_t hash_code() const;
  bool before(const type_info& rhs) const;
};
```

???

* This is the tip of the iceberg that we're allowed to see
* The 'name' may be mangled

---

# The 'std::type_info' class (2)

As defined by the Itanium ABI

For classes with single base:

```c++
class __si_class_type_info : public __class_type_info {
	public:
	  const __class_type_info *__base_type;
};
```

---

# The 'std::type_info' class (3)

For classes with multiple bases:

```c++
class __vmi_class_type_info : public __class_type_info {
	public:
	  unsigned int __flags;
	  unsigned int __base_count;
	  __base_class_type_info __base_info[1];

	  enum __flags_masks {
	    __non_diamond_repeat_mask = 0x1,
	    __diamond_shaped_mask = 0x2
	  };
};

struct abi::__base_class_type_info {
    const __class_type_info *__base_type;
    long __offset_flags;

    enum __offset_flags_masks {
      __virtual_mask = 0x1,
      __public_mask = 0x2,
      __offset_shift = 8
    };
};
```
<!-- ============================ Exceptions ============================ -->
---
class: center, middle

# Chapter IV

### Exceptions: How are they implemented

.center[
<img src="pics/finally.jpg" height="250"/>
]

???

* Finally, we arrive art this point of the presentation, after as many slides

---

## Disclaimer

* Everything wil be simplified
* Pseudo - code
* Old (SJLJ) vs NEW ("zero-cost")

???

* Everything in this chapter will be a simplification and caricature
* There is too many standards, details, micro-optimizations etc
* I will boil down everything toi the lowest common denominator

---

## Throw

1. Allocate exception object
2. Scan stack for suitable 'catch' block
3. If found: 
  2. Unwind stack
  1. Transfer control
4. If not found:
  1. Call `std::terminate`

---

## Throw (Itanium)

```c++
void fun() {
  throw Error(p0, p1);
}
```

--

```c++
thread_local __cxa_exception *global_exception = nullptr;

struct __cxa_exception { 
	std::type_info  *exceptionType;
	void (*exceptionDestructor) (void *); 
	__cxa_exception *nextException;
};

void fun() {

  // allocate sizeof(__cxa_exception) + sizeof(Error)
  __cxa_exception *temp = __cxa_allocate_exception(sizeof(Error));

  new(temp+sizeof(__cxa_exception)) Error(p0, p1);

  temp.exceptionType = &typeid(Error);
  temp.exceptionDestructor = &Error::~Error;
  temp.nextException = global_exception;
  global_exception = temp;

  __cxa_throw(temp); // Never returns
}
```
???

1. Exception is allocated _somwhere_
2. Exception is preceded by a header, that contains type-erasure info
3. Exception info is added to a global linked-list of exceptions
4. Control is passed to a stack-unwinding routine

---
class: asmslide
## Throw (Itanium, asm)


.pull-left[
```c++
struct Error {
    Error(int a, int b) noexcept;
    ~Error();
private:
    char data[77];
};

void throw_error() {
    throw Error(44, 55);
}
```
]
.pull-right[
```asm
throw_error(): # @throw_error()
  push rbx
  mov edi, 77
  call __cxa_allocate_exception
  mov rbx, rax
  mov rdi, rax
  mov esi, 44
  mov edx, 55
  call Error::Error(int, int)
  mov esi, offset typeinfo for Error
  mov edx, offset Error::~Error()
  mov rdi, rbx
  call __cxa_throw
typeinfo name for Error:
  .asciz "5Error"

typeinfo for Error:
  .quad vtable for 
    __cxxabiv1::__class_type_info+16
  .quad typeinfo name for Error
```
]

---

# Throw (MSVC)

```c++
void fun() {
  throw Error(p0, p1);
}
```

--

```c++
struct _ThrowInfo {
  std::type_info  *exceptionType;
	void (*exceptionDestructor) (void *);
};

_ThrowInfo _TI1Error {
  &typeid(Error),
  &Error::~Error
};

void fun() {
  Error e(p0, p1);
  _CxxThrowException(&e, &_TI1Error); // never returns
}
```

???

* an instance of _ThrowInfo is created for every exception thrown, just like std::type_info
* The actual exception is created on the stack, which will make things more interesting later on

---
class: asmslide
# Throw (MSVC, asm)

.pull-left[
```c++
struct Error {
    Error(int a, int b) noexcept;
    ~Error();
private:
    char data[77];
};

void throw_error() {
    throw Error(44, 55);
}
```
]
.pull-right[
```asm
xdata$x SEGMENT
_TI1 ?? AUError DD 00H
  DD imagerel Error::~Error(void)
  DD 00H
  DD imagerel _CTA1 ?? AUError
xdata$x ENDS

void throw_error(void) PROC
$LN4:
  sub rsp, 136 ; 00000088H
  mov rax, QWORD PTR __security_cookie
  xor rax, rsp
  mov QWORD PTR __$ArrayPad$[rsp], rax
  mov edx, 44 ; 0000002cH
  lea rcx, QWORD PTR $T1[rsp]
  lea r8d, QWORD PTR [rdx+11]
  call Error::Error(int,int)
  lea rdx, OFFSET FLAT:_TI1??AUError
  lea rcx, QWORD PTR $T1[rsp]
  call _CxxThrowException
  int 3
```
]

???

* The int3 at the end is a breakpoint
* The int3 is there so that the unwinder does not treatthe preceeding instructions as an 'epilogue'

---
## Bystander

.pull-left[
```C++
void fun() {
  Widget w1;
  foo();
  Widget w2;
  foo();
  Widget w2;
  foo();
}
```
]

???

Let's consider this simple function

---

## Bystander (Old SJLJ)

.pull-left[
```C++
void fun() {
  Widget w1;
  foo();
  Widget w2;
  foo();
  Widget w2;
  foo();
}
```
```C++
SjLj_Function_Context* _sjlf_fc;

struct SjLj_Function_Context {
  SjLj_Function_Context *prev;
  int call_site;
  void* exception_table;
  void *cleanup;
  jmp_buf jbuf;
};

```
]
.pull-right[
```C++
void fun() {
  SjLj_Function_Context _ctx;
  Widget w1;
  _ctx.prev = _sjlf_fc;
  _ctx.cleanup = &&cleanup_block;
  _sjlf_fc = &_ctx;
  _ctx.call_side = 1;
  foo();
  Widget w2;
  _ctx.call_side = 2;
  foo();
  Widget w3;
  _ctx.call_side = 3;
  foo();
  ~Widget::Widget(&w3);
  ~Widget::Widget(&w2);
  ~Widget::Widget(&w1);
  _sjlf_fc = _ctx.prev;
  return;
cleanup_block:
  switch(_ctx.call_side) {
    3: ~Widget::Widget(&w3);
    2: ~Widget::Widget(&w2):
    1: ~Widget::Widget(&w1);
  }
  _Unwind_SjLj_Resume();
}
```
]

???

* Compiler generates function state block on the stack
* The blocks are linked as a linked list
* Block contains, among other things, the information about unwind-cleanup

---

## Bystander (MSVC 32)

.pull-left[
```C++
void fun() {
  Widget w1;
  foo();
  Widget w2;
  foo();
  Widget w2;
  foo();
}
```

.smalltable[
call_site | next | handler
----------|------------------------
0         | -1   | __unwindfunclet0
1         | 0    | __unwindfunclet1
2         | 1    | __unwindfunclet2

]
]
.pull-right[
```C++
void fun() {
  EXCEPTION_REGISTRATION _ctx;
  _ctx.prev = _exc_regp;
  _ctx.cleanup_table = <...>;
  _sjlf_fc = &_ctx;
  Widget w1;
  _ctx.call_side = 0;
  foo();
  Widget w2;
  _ctx.call_side = 1;
  foo();
  Widget w3;
  _ctx.call_side = 2;
  foo();
  ~Widget::Widget(&w3);
  ~Widget::Widget(&w2);
  ~Widget::Widget(&w1);
  _sjlf_fc = _ctx.prev;
  return;
}
__unwindfunclet0() {~Widget::Widget(&w1); }
__unwindfunclet1() {~Widget::Widget(&w2); }
__unwindfunclet2() {~Widget::Widget(&w3); }
```
]

???

* The cleanup code is not part of the function, because the exception is on the stack!
* Funclets have very special calling convention, among others - they can't have local variables
* The order is determined by a cleanup table


---

## Bystander (modern)

.pull-left[
Itanium
```C++
void fun() {
  Widget w1;
  foo();
  Widget w2;
  foo();
  Widget w3;
  foo();
  ~Widget::Widget(&w3);
  ~Widget::Widget(&w2);
  ~Widget::Widget(&w1);
  return;
cleanup_3:
  ~Widget::Widget(&w3);
cleanup_2:
  ~Widget::Widget(&w2);
cleanup_1:
  ~Widget::Widget(&w1);
  _Unwind_Resume
}
```
]
.pull-right[
  MSVC x64
```C++
void fun() {
  Widget w1;
  foo();
  Widget w2;
  foo();
  Widget w3;
  foo();
  ~Widget::Widget(&w3);
  ~Widget::Widget(&w2);
  ~Widget::Widget(&w1);
  return;
}
__unwindfunclet0() {~Widget::Widget(&w1); }
__unwindfunclet1() {~Widget::Widget(&w2); }
__unwindfunclet2() {~Widget::Widget(&w3); }
```
]

???

* Around 1999 DWARF appeared, which allows for non-intrusive stack unwinding (.eh_info)
* Compiler generates table for both cleanup and catch blocks
* Linker builds index for quick lookup (.eh_info_hdr)

---
class: asmslide

## Bystander (Itanium, asm)

.pull-left[
```asm
fun():
        push    rbx
        sub     rsp, 16
        lea     rdi, [rsp+13]
        call    Widget::Widget()
        call    foo()
        lea     rdi, [rsp+14]
        call    Widget::Widget()
        call    foo()
        lea     rdi, [rsp+15]
        call    Widget::Widget()
        call    foo()
        lea     rdi, [rsp+15]
        call    Widget::~Widget()
        lea     rdi, [rsp+14]
        call    Widget::~Widget()
        lea     rdi, [rsp+13]
        call    Widget::~Widget()
        add     rsp, 16
        pop     rbx
        ret 
        ; continued ...

```
]
.pull-right[
```asm
fun() [clone .cold.0]:
.L2:
        lea     rdi, [rsp+15]
        call    Widget::~Widget()
.L3:
        lea     rdi, [rsp+14]
        call    Widget::~Widget()
.L4:
        lea     rdi, [rsp+13]
        call    Widget::~Widget()
        mov     rdi, rbx
        call    _Unwind_Resume
```
]

???

* Output from the latest GCC
* One can see that the 'good path' is as optimal as possible
* Th cleanup part is marked 'cold', and will be put in a separate block of the object file


---
class: asmslide

## Bystander (Itanium, asm)

.pull-left[
```asm
fun():
LFB1030:
        push    rbx
        sub     rsp, 16
        lea     rdi, [rsp+13]
        call    Widget::Widget()
LEHB0:
        call    foo()
LEHE0:
        lea     rdi, [rsp+14]
        call    Widget::Widget()
.LEHB1:
        call    foo()
.LEHE1:
        lea     rdi, [rsp+15]
        call    Widget::Widget()
.LEHB2:
        call    foo()
.LEHE2:
        lea     rdi, [rsp+15]
        call    Widget::~Widget()
        lea     rdi, [rsp+14]
        call    Widget::~Widget()
        lea     rdi, [rsp+13]
        call    Widget::~Widget()
        add     rsp, 16
        pop     rbx
        ret 
        ; continued ...

```
]
.pull-right[
```asm
        .section        .gcc_except_table,"a",@progbits
.LLSDA1030: ; header
        .byte   0xff
        .byte   0xff
        .byte   0x1 ; 1 = no catch blocks
        .uleb128 .LLSDACSE1030-.LLSDACSB1030
.LLSDACSB1030: ; content
        .uleb128 .LEHB0-.LFB1030
        .uleb128 .LEHE0-.LEHB0
        .uleb128 .L4-.LFB1030
        .uleb128 0 ; 0 = cleaup only
        .uleb128 .LEHB1-.LFB1030
        .uleb128 .LEHE1-.LEHB1
        .uleb128 .L3-.LFB1030
        .uleb128 0 ; 0 = cleaup only
        .uleb128 .LEHB2-.LFB1030
        .uleb128 .LEHE2-.LEHB2
        .uleb128 .L2-.LFB1030
        .uleb128 0 ; 0 = cleaup only
.LLSDACSE1030:
```
]

???

* Example of Language Specific Data Area

---
background-image: url(pics/itanium-eh-tables.png)

???

* Image from Itanium ABI: http://itanium-cxx-abi.github.io/cxx-abi/exceptions.pdf
* Describes the flow trough the EH tables

---

## Catch

```C++
void fun() {
    Widget w1;
    foo();
    try {
        Widget w2;
        foo();
    } catch(const std::exception& e) {
        on_error();
    }
}
```

---
class: asmslide
## Catch (Itanium, asm)

.pull-left[
```asm
fun():
.LFB1030:
        push    rbp
        push    rbx
        sub     rsp, 24
        lea     rdi, [rsp+14]
        call    Widget::Widget()
.LEHB0:
        call    foo()
.LEHE0:
        lea     rdi, [rsp+15]
        call    Widget::Widget()
.LEHB1:
        call    foo()
.LEHE1:
        lea     rdi, [rsp+15]
        call    Widget::~Widget() 
.L5:
        lea     rdi, [rsp+14]
        call    Widget::~Widget()
        add     rsp, 24
        pop     rbx
        pop     rbp
        ret
```
]
.pull-right[
```asm
fun() [clone .cold.0]:
.L2:
        lea     rdi, [rsp+15]
        call    Widget::~Widget(); w2
        sub     rbp, 1
        je      .L11
.L4:
        lea     rdi, [rsp+14]
        call    Widget::~Widget() ; w1
        mov     rdi, rbx
        call    _Unwind_Resume
.L11:
        mov     rdi, rbx
        call    __cxa_begin_catch
        call    on_error()
        call    __cxa_end_catch
        jmp     .L5
```
]

???

* Catch block code is in rthe cold section as well
* catch block is surrounded by calls cxa_begin_catch/cxa_end_catch
* According to Itanium ABI:

__cxa_begin_catch:
*Increment's the exception's handler count.
*Places the exception on the stack of currently-caught exceptions if it is not already there, linking the exception to the previous top of the stack.
*Decrements the uncaught_exception count.
*Returns the adjusted pointer to the exception object.

__cxa_end_catch:
* Locates the most recently caught exception and decrements its handler count.
* Removes the exception from the caught exception stack, if the handler count goes to zero.
* Destroys the exception if the handler count goes to zero, and the exception was not re-thrown by throw.

* There is also: __cxa_rethrow

---
class: asmslide
## Catch (Itanium, asm)

.pull-left[
```asm
fun():
.LFB1030:
        push    rbp
        push    rbx
        sub     rsp, 24
        lea     rdi, [rsp+14]
        call    Widget::Widget()
.LEHB0:
        call    foo()
.LEHE0:
        lea     rdi, [rsp+15]
        call    Widget::Widget()
.LEHB1:
        call    foo()
.LEHE1:
        lea     rdi, [rsp+15]
        call    Widget::~Widget() 
.L5:
        lea     rdi, [rsp+14]
        call    Widget::~Widget()
        add     rsp, 24
        pop     rbx
        pop     rbp
        ret
```
]
.pull-right[
```asm
.LLSDA1030:
        .byte   0xff
        .byte   0x3
        .uleb128 .LLSDATT1030-.LLSDATTD1030
.LLSDATTD1030:
        .byte   0x1
        .uleb128 .LLSDACSE1030-.LLSDACSB1030
.LLSDACSB1030:
        .uleb128 .LEHB0-.LFB1030
        .uleb128 .LEHE0-.LEHB0
        .uleb128 .L4-.LFB1030
        .uleb128 0
        .uleb128 .LEHB1-.LFB1030
        .uleb128 .LEHE1-.LEHB1
        .uleb128 .L2-.LFB1030
        .uleb128 0x3 ; positive => catch
.LLSDACSE1030:
        .byte   0 ; type index, T0
        .byte   0 ; next record
        .byte   0x1
        .byte   0x7d
        .align 4
        .long   _ZTISt9exception ; T0
.LLSDATT1030:
```
]

???

* Language Specific Data Area for this function

---
background-image: url(pics/iknowexceptions.jpg)

???

* Break. The worst is behind us

---
class: center, middle

# Chapter V

## noexcept

???

This chapter will explore the influence of noexcept on the generated code

---
class: asmslide 
# noexcept

.pull-left[
```c++
struct Widget {
    Widget();
    ~Widget();
};
void foo();
void fun() {
    Widget w;
    foo();
}
```
]
--
.pull-right[
```asm
fun():
        push    rbx
        sub     rsp, 16
        lea     rdi, [rsp+15]
.LEHB0:
        call    Widget::Widget() [complete object constructor]
.LEHE0:
.LEHB1:
        call    foo()
.LEHE1:
        lea     rdi, [rsp+15]
        call    Widget::~Widget() [complete object destructor]
        add     rsp, 16
        pop     rbx
        ret
        mov     rbx, rax
        jmp     .L2
fun() [clone .cold.0]:
.L2:
        lea     rdi, [rsp+15]
        call    Widget::~Widget() [complete object destructor]
        mov     rdi, rbx
        call    _Unwind_Resume
```
]

???

* The landing pad is generated, and the call to foo() is treaded as a potential source of exception

---
class: asmslide 
# noexcept (2)

.pull-left[
```c++
struct Widget {
    Widget();
    ~Widget()=default;
};
void foo();
void fun() {
    Widget w;
    foo();
}
```
]

--

.pull-right[
```asm
fun():
        sub     rsp, 24
        lea     rdi, [rsp+15]
        call    Widget::Widget()
        call    foo()
        add     rsp, 24
        ret
```
]

???

* The destructor is inlined, and it tuens out there it's empty, i has no observable side effects
* No need to generate landing pad or excaption table for the function
* The constructor in this exampledoesn't have to be empty; it simply should have no observable side effects

---
class: asmslide 
# noexcept (3)

.pull-left[
```c++
struct Widget {
    Widget();
    ~Widget();
};
void foo() noexcept;
void fun() {
    Widget w;
    foo();
}
```
]

--

.pull-right[
```asm
fun():
        sub     rsp, 24
        lea     rdi, [rsp+15]
        call    Widget::Widget()
        call    foo()
        lea     rdi, [rsp+15]
        call    Widget::~Widget()
        add     rsp, 24
        ret```
]

???

* In this case, call to 'foo' is not considered to be a throwing call.
---
class: asmslide 
# noexcept (4)

.pull-left[
```c++
void foo();
void bar();
void fun(){
    foo();
    bar();
}
```
]

--

.pull-right[
```asm
fun():
        sub     rsp, 8
        call    foo()
        add     rsp, 8
        jmp     bar()
```
]

???

* No landing pad and no EH table is generated for this function
---
class: asmslide 
# noexcept (5)

.pull-left[
```c++
void foo();
void bar() noexcept;
void fun(){
    foo();
    bar();
}```
]

--

.pull-right[
```asm
fun():
        sub     rsp, 8
        call    foo()
        add     rsp, 8
        jmp     bar()
```
]

???

* Then 'noexceptness' of 'bar' has no impact

---

class: asmslide 
# noexcept (6)

.pull-left[
```c++
struct Widget {
    Widget();
    ~Widget();
};
void foo();
void bar() noexcept;
void fun() {
    Widget w1;
    foo();
    Widget w2;
    bar();
}
```
]

--

.pull-right[
```asm
fun():
        push    rbx
        sub     rsp, 16
        lea     rdi, [rsp+14]
.LEHB0:
        call    Widget::Widget()
.LEHE0:
.LEHB1:
        call    foo()
.LEHE1:
        lea     rdi, [rsp+15]
        call    Widget::Widget()
        call    bar()
        lea     rdi, [rsp+15]
        call    Widget::~Widget()
        lea     rdi, [rsp+14]
        call    Widget::~Widget()
        add     rsp, 16
        pop     rbx
        ret
        mov     rbx, rax
        jmp     .L2
fun() [clone .cold.0]:
.L2:
        lea     rdi, [rsp+14]
        call    Widget::~Widget()
        mov     rdi, rbx
        call    _Unwind_Resume
```
]

???

* The table is generated for the function, but the landing pad only deatroys w1
* If exception is thrown from 'bar', then the unwinding routine will find no matching call-site, and call std::terminate

---

class: asmslide 
# noexcept (7)

.pull-left[
```c++
void foo();

void bar() noexcept {
    foo();
}
```
]

---

.pull-right[
```asm
bar():
        jmp     foo()
.LLSDA0:
        .byte   0xff
        .byte   0xff
        .byte   0x1
        .uleb128 .LLSDACSE0-.LLSDACSB0
.LLSDACSB0:
.LLSDACSE0:
```
]

???

* This is the last assembly slide
* If 'noexcept' function contains a potnetially throwing call, an empoty EH table is generated for it,
* so that std::terminate will be called in the event of stack unwinding


<!-- ========================== the End ============================ -->

---
# The Last Slide

* Maciej Gajewski <maciej.gajewski0@gmail.com>
* This presentation: https://maciekgajewski.github.io/exceptions-under-the-hood/
* Compiler Explorer: https://gcc.godbolt.org
* Itanium ABI: https://itanium-cxx-abi.github.io/cxx-abi/
* Exception Handling ABI for the ARM® Architecture: http://infocenter.arm.com/help/topic/com.arm.doc.ihi0038b/IHI0038B_ehabi.pdf
* WIESS Paper: https://www.usenix.org/legacy/events/osdi2000/wiess2000/full_papers/dinechin/dinechin_html/#31449
* CppCon 2017, _“What Has My Compiler Done for Me Lately?”_, Matt Godbolt
