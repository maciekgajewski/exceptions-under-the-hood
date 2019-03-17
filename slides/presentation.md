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

```cpp
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

---

## The oring of exceptions (2)

TODO: if ladder

read all file

---

## The oring of exceptions (3)

TODO: the problem of resources

---

## Releading resources (1)

TODO: just die

---

## Releading resources (2)

TODO: goto ladder

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
