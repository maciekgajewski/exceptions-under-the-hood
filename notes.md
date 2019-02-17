# Myths about exceptions

- Exception object is always copied
- noexcept and throw simply dictate that a call to std::terminate is made in the case where an unmentioned exception is thrown.

# Links

- https://monoinfinito.wordpress.com/series/exception-handling-in-c/ - some info of the "under the hood" action
- https://itanium-cxx-abi.github.io/cxx-abi/abi-eh.html - Itanium ABI
- https://llvm.org/docs/ExceptionHandling.html - Exception Handling in LLVM
- http://itanium-cxx-abi.github.io/cxx-abi/exceptions.pdf - z rysunkami!
- http://www.ucw.cz/~hubicka/papers/amd64/node2.html - on exception in gcc
- http://infocenter.arm.com/help/topic/com.arm.doc.ihi0038b/IHI0038B_ehabi.pdf
- https://mortoray.com/2013/09/12/the-true-cost-of-zero-cost-exceptions/ - kolejny fajny blogpost

# TODO

- explain "personality routine"
- explain stack unwinding
- explain looking for catch
- explain matching types
- explain the landing pads
- compare to C-style, goto init/deinit pattern
- becnhmark throwing
- benchmark exceptions vs error codes
- mixing exception-aware and -unaware code
- itianium ABI - what is this, what are the alternatives?
- resumptive exception handling 
- how frame pointer worksd:
- show std::uncaught_exceptions() 
- show std::exception_ptr and others
- show 2 exceptions in flight
- mention the emergency buffer, 4-64kB
- unwind table
- Setjmp/Longjmp Exception Handling
- Windows Runtime exception handling
- ARM?

# Other

* From Chrome C++ standard:
    "The overhead is in the range of 5% to 10% of the binary size, depending of the average size of your functions, so we disable it by default."
* from article on exception in gcc:
    "We found the use of frame pointer and push/pop instructions to be common bottleneck for the function call performance. The AMD Opteron CPU can execute stores at the rate of two per cycle, while it requires 2 cycles to compute new %rsp value in push and pop operations so the sequence of push and pop operations executes 4 times slower."