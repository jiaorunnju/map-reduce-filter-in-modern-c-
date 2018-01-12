This is a simple library which implement map-and-reduce like processing in C++. Within this project, I implement an "Option" class to support lazy evaluation. In fact, when you use map or where, It does not compute immediately, it just store your operations. when you use reduce, computing starts.

I also implement some function_traits to convert any callable object into std::function and a simple class to compose functions.
