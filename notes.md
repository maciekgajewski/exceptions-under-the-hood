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
- http://nibblestew.blogspot.com/2017/01/measuring-execution-performance-of-c.html - blog comparing C vs Exceptions
- http://www.stroustrup.com/except89.pdf - design rationale and history
- https://apps.dtic.mil/dtic/tr/fulltext/u2/a062363.pdf - The Revised Report on Scheme: A Dialect of LISP, Guy Lewis Steel Jr., Gerald Jay Sussman, Jan 1978
- http://www.softwarepreservation.org/projects/LISP/MIT/Moon-MACLISP_Reference_Manual-Apr_08_1974.pdf
- https://www.youtube.com/watch?v=XpRL7exdFL8 - interesting video with unwinding details
- https://www.airs.com/blog/archives/460 - eh_frame description
- https://www.usenix.org/legacy/events/osdi2000/wiess2000/full_papers/dinechin/dinechin_html/ - great overview
- http://www.stroustrup.com/except89.pdf

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
- how frame pointer works:
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
