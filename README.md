<p align="center">
  <img src="extras/assets/pinum_logo.svg" alt="PiNum Logo" width="200" />
</p>

# PiNum-Lang

PiNum (represented by the `.pn` extension) is a lightweight, C-inspired programming language currently under development. It features a custom lexer, parser, and code generation pipeline designed for simplicity and potential transpilation or embedding.

## ✨ Features

- **C-Style Syntax:** Familiar data types (`int`, `float`, `char`, `string`, `bool`) and control structures (`if`, `else`, `return`).
- **Standard Library Support:** Includes a modular system using the `@import` directive (e.g., `@import stdlib`, `@import math`).
- **Specialized Modes:** Supports an `ENGINE_MODE` triggered by the `@for engine` directive, allowing for specialized execution contexts.
- **Rich Operator Set:** Includes standard arithmetic, logical, and bitwise operators, as well as pointer-like syntax (`&`).
- **Editor Support:** Built-in syntax highlighting for:
  - **VS Code:** Extension available in `extras/vscode/`.
  - **Neovim:** Lua/Vim syntax files in `extras/nvim/`. To enable it, run the setup script:
    ```bash
    chmod +x activate_syntax.sh
    ./activate_syntax.sh
    ```

## 🚀 Getting Started

### Prerequisites

- A C compiler (e.g., `gcc`)
- `make`

### Building from Source

To build the `pinum` binary:

```bash
make
```

The compiled binary will be located in the `bin/` directory.

### Running a Program

To execute a `.pn` file:

```bash
./bin/pinum example/helloworld.pn
```

## 📝 Syntax

Even though its under development, the syntax (to some extent) is defined.

# Comments
```pinum
# this is a comment!
# there is only single line comments.
```

# Libreries
```pinum
# includes standard librery in transpiled code
@import stdlib
# includes math librery in transpiled librery
@import math

# this flag enables this language to be transpiled into bare C
# if you include this flag you can't import any libreries
@for engine
```

# Veriable declaration
```pinum
# it has keywords like long, short, unsigned and signed

# declares an intager veriable
int number
# declares floating point veriable
float num
# declares a duble type verible
double numero
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
# there is 2 loops while and for

# for loop
for (veriable; condition; increment or decrement) {
        # task
}

# while loop
while (condition) {
        # task
}
```

## 🛠 Project Structure

- `src/`: Core implementation (Lexer, Parser, Codegen, Helper functions).
- `include/`: Header files defining the language structures and interfaces.
- `bin/`: Compiled binaries.
- `example/`: Sample programs demonstrating language features.
- `extras/`: Editor extensions and syntax highlighting.

## 🚧 Development Status

PiNum is currently in its early stages:
- [x] Lexer / Tokenizer
- [ ] Parser (In Progress)
- [ ] Code Generation (In Progress)
- [ ] Runtime Library

## 📄 License

This project is licensed under the [MIT License](LICENSE).

I use Neovim BTW.
