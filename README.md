# SCBL
Simple Command Bar Language

<img src="pic/logo.png"/>

A simple, small scripting language to be embedded with a C++ program.
It is constructed of functions and parameters:
```
<function> <parameters>;
```
the parameters can be constants, integers or strings.
#### Strings
Strings start and end with `"`. The parser turns the characters into separate integer parameters by their ascii codes and automatically puts a NULL Terminator after each string as an integer parameter, but if you put `~` after the string, it wont do so. Example using the SCBL Shell:
```
printr "A";
printr "A"~;
```
First `printr` would output `650` (As a combination of `A` (65) and the NULL Terminator (0)) and the second one would output `65`.

## Compiling and running the shell
#### Linux
Run `compile` as bash in the terminal:
```sh
bash compile
```
then run scbl:
```sh
scbl
```
#### Windows
First make sure you have g++ installed (Command Prompt will not recognize the `g++` command if you dont) and run the following commands:
```bat
mkdir bin
g++ -g shell/main.cc -o bin/scbl -O3 -s -Wall -std=c++17;
```
then run scbl:
```bat
scbl
```

## Embedding with C++
SCBL is really small and simple, so you only need to include the header file `scbl.hh` in your C++ program and then create an object of SCBL_Interpreter class which, as everything else SCBL-related, can be found inside the SCBL namespace. List of SCBL_Interpreter methods:
- `void AddFunction(function)`: Pushes the function to the functions stack
- `void SetFunction(function)`: Rewrites, or if not found, pushes the function to the functions stack
- `void AddConstant(constant)`: Pushes the constant to the constants stack
- `void SetConstant(constant)`: Rewrites, or if not found, pushes the constant to the constants stack
- `uint8_t Parse(text)`: Parses the text into tokens, returns `SCBL_PARSER_OK` (0) if no errors occured, else returns `SCBL_PARSER_ERROR` (1)
- `uint8_t Run()`: Interprets the parsed text, returns `SCBL_RUNTIME_OK` (0) if no errors occured, else returns `SCBL_RUNTIME_ERROR` (1)
- `std::string GetErrorMsg()`: Returns the description of the last error

`Function` and `Constant` are two other classes inside the SCBL namespace. `Function` holds the Name (std::string) and Callback (void (*callback)(std::vector <uint32_t>)) and `Constant` holds the Name (std::string) and Value (uint32_t). `Function` and `Constant` constructors:
- Function(name, callback)
- Constant(name, value)

`callback` is a function which takes two parameters, `std::vector <ui32>` which are the parameters passed to the SCBL function and `void*` which is the user data.

#### Example:
```cc
#include <iostream>
#include "scbl.hh"

int main() {
    SCBL::SCBL_Interpreter SCBLi;
    
    SCBLi.AddConstant(SCBL::Constant("newline", 10));
    SCBLi.AddFunction(SCBL::Function("print", [] (std::vector <uint32_t> Parameters, void* _) {
        for (uint32_t i = 0; i < (uint32_t)Parameters.size(); ++ i) 
            std::cout << (char)Parameters[i];
    }));
    
    if (SCBLi.Parse("print \"Hello, World!\"~ newline;") == SCBL_PARSER_ERROR) {
        std::cout << SCBLi.GetErrorMsg() << std::endl;
        
        return 0;
    };
    
    if (SCBLi.Run() == SCBL_RUNTIME_ERROR) {
        std::cout << SCBLi.GetErrorMsg() << std::endl;
        
        return 0;
    };
    
    return 0;
};
```
