<p align="center">
  <img src="extras/assets/pinum_logo.svg" alt="PiNum Logo" width="200" />
</p>

# PiNum-Lang 1.0.0-beta

https://tanvir-techbro.github.io/pinum-site - official site for pinum. check for documentation and other info.
PiNum (represented by the `.pn` extension) is a lightweight, C-inspired programming language currently under development.
## ✨ Features

- **C-Style Syntax:** Familiar data types (`int`, `float`, `char`, `string`, `bool`) and control structures (`if`, `else`, `return`).
- **Standard Library Support:** Includes a modular system using the `@import` directive (e.g., `@import stdlib`, `@import math`).
- **Editor Support:** Built-in syntax highlighting for:
  - **VS Code:** Extension available in `extras/vscode/`.
  - **Neovim:** Lua/Vim syntax files in `extras/nvim/`. To enable it, run the setup script:
    ```bash
    chmod +x activate_syntax.sh
    ./activate_syntax.sh
    ```

## 🚀 Getting Started

### One-line Installation

The fastest way to install PiNum-Lang on your system:

#### Linux/macOS (bash/zsh)
```bash
curl -sSL https://raw.githubusercontent.com/tanvir-techbro/PiNum-Lang/main/install.sh | bash
```
*This will build the language and ask if you want to activate Neovim syntax highlighting automatically.*

> **Windows users:** PiNum-Lang supports Linux/macOS only. If you're on Windows, use [WSL (Windows Subsystem for Linux)](https://learn.microsoft.com/en-us/windows/wsl/install) and follow the Linux instructions above.

### Prerequisites

- A C compiler (e.g., `gcc`) and `make`.

### Building from Source

To build the `pinum` binary:

```bash
make
```

The compiled binary will be located in the `bin/` directory.

### Installation

To install `pinum` to your system path:

#### Linux/macOS
```bash
sudo make install
```
This moves the binary to `/usr/local/bin/`.

### Running a Program

To transpile and compile a `.pn` file to a binary (default output is `a.out`):

```bash
./bin/pinum example/demonstration.pn
```

PiNum transpiles your code to C, then compiles it with an available C compiler (it searches for `cc`, `gcc`, `clang`, or `tcc`, or uses your `$CC`). The compiled binary lands in your current working directory.

### Output Flags

Control what gets produced with the output flags:

```bash
# compile to a binary named `program` (temporary C file is deleted)
./bin/pinum -o program example/demonstration.pn

# output only the C source file
./bin/pinum -o program.c example/demonstration.pn

# output both the C source file and a compiled binary
./bin/pinum -oc program example/demonstration.pn
```

Long forms `--output` and `--output-c` are also accepted. Use `--help` to see all flags.

## 📝 Syntax

Even though its under development, the syntax (to some extent) is defined.

**Semicolons are optional.** A newline ends a statement, but semicolons let you write multiple statements on one line:

```pinum
# both of these work
int a = 5;
int b = 5

# semicolons are needed to pack statements on one line
int c = 1; print(c, "\n"); c = c + 1; print(c, "\n");
```

# Comments
```pinum
# this is a comment!
# there is only single line comments.
```

# Libreries
```pinum
# stdlib is imported automatically, no need for @import
# this flag enables this language to be transpiled into bare C
@for engine
```

# Veriable declaration
```pinum
# it has keywords like long, short, unsigned and signed

# declares an intager veriable
int number = 10
# declares floating point veriable
float num = 3.14
# declares a duble type verible
double numero = 2.718
# declares a string veriable
string name = "pinum"
# declares a boolean veriable
bool ready = true
```

# Output & input
```pinum
# print accepts any number of arguments
print("hello ", name, "\n")

# read stores user input into an existing veriable
int data
read(data)
print(data, "\n")

# multiple statements on one line
int n = 2; n = n + 3; print(n, "\n")
```

# Conditions
```pinum
if (condition) {
        # task 1
} else if (condition) {
        # task 2
} else {
        # default task
}
```

# Loops
```pinum
# while loop
while (condition) {
        # task
        if (something) {
                break;      # exit the loop
        }
        if (something_else) {
                continue;   # skip to the next iteration
        }
}
```

# Return
```pinum
# exits the program with the given value
return 0
```

## 🛠 Project Structure

- `src/`: Core implementation (Lexer, Parser, Codegen, Helper functions).
- `include/`: Header files defining the language structures and interfaces.
- `bin/`: Compiled binaries.
- `example/`: Sample programs demonstrating language features.
- `extras/`: Editor extensions and syntax highlighting.
- `test/`: Automated tests for the lexer, parser, AST, and codegen.

## 🧪 Testing

Run the test suite interactively (choose which category to run, optionally with Valgrind):

```bash
bash test/run_tests.sh
```

Tests cover the lexer, parser, AST, and codegen stages. Codegen tests transpile each program to a C file, compile it, and run the resulting binary to confirm it behaves correctly.

## 🚧 Development Status

PiNum is currently in its early stages:
- [x] Lexer / Tokenizer
- [x] ast
- [x] Parser
- [x] Code Generation (print, read, variables, if/else, while, return, break, continue)
- [ ] For loops, functions, data structures (in progress)
- [ ] Runtime Library (later)

## 🤝 Contributing

Contributions are what make the open-source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

Please see our [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines on branching, coding standards, and testing.

## 📄 License

This project is licensed under the [MIT License](LICENSE).

I use Neovim BTW.
