# Myths about exceptions

- Exception object is always copied
- noexcept and throw simply dictate that a call to std::terminate is made in the case where an unmentioned exception is thrown.

# Links

- https://monoinfinito.wordpress.com/series/exception-handling-in-c/ - some info of the "under the hood" action

# TODO

- explain "personality routine"
- explain stack unwinding
- explain looking for catch
- explain matching types
- explain the landing pads
- compare to C-style, goto init/deinit pattern
- becnhmark throwing
- benchmark exceptions vs error codes

# Other

* From Chrome C++ standard:
    "The overhead is in the range of 5% to 10% of the binary size, depending of the average size of your functions, so we disable it by default."