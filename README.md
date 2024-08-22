# Simplang to Assembly Compiler

## Project Description
This project is a C++ compiler that translates Simplang, a simple programming language, into assembly code designed for an 8-bit computer. The compiler is organized into several key components:

- **Lexer**: Tokenizes Simplang source code into meaningful symbols.
- **Parser**: Analyzes the tokenized input to produce an Abstract Syntax Tree (AST).
- **Code Generator**: Converts the AST into assembly instructions tailored for an 8-bit computer.
- **Optimizer**: (Optional) Refines the assembly code to enhance performance and efficiency.

The compiler demonstrates how high-level language constructs can be systematically translated into low-level assembly instructions executable by an 8-bit computer.

## Table of Contents
1. [Features](#features)
2. [Project Structure](#project-structure)
3. [Installation](#installation)
4. [Usage](#usage)
5. [Example](#example)
6. [8-bit Instruction Set](#8-bit-instruction-set)
7. [Contributing](#contributing)
8. [License](#license)

## Features
- **Lexer**: Performs lexical analysis to tokenize Simplang code.
- **Parser**: Constructs an Abstract Syntax Tree (AST) from tokenized input.
- **Code Generator**: Converts the AST into assembly code.
- **8-bit Computer Instructions**: Supports a defined set of instructions for 8-bit architecture.
- **Optimization**: (Optional) Enhances assembly code for better performance.

## Project Structure
The project directory contains:

- `src/`: Source code for the compiler.
  - `lexer.cpp`, `lexer.h`: Lexer implementation.
  - `parser.cpp`, `parser.h`: Parser implementation.
  - `codegen.cpp`, `codegen.h`: Code generator implementation.
  - `optimizer.cpp`, `optimizer.h`: (Optional) Optimizer implementation.
- `tests/`: Unit tests and test cases.
- `examples/`: Sample Simplang code and expected assembly outputs.
- `README.md`: This file.
- `CMakeLists.txt`: Build configuration file for CMake.

## Installation
To build and run the compiler, follow these steps:

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/yourusername/simplang-compiler.git
   cd simplang-compiler
