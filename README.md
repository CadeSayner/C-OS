# C-OS
C-OS is a simple operating system built from scratch, written in C and x86. 
## Features
- **Kernel**: A lightweight kernel written in C and assembly with basic hardware abstraction.
- **System Calls**: Includes system calls for interacting with hardware, such as vga and keyboard drivers. Also includes system calls for process management (such as `brk` and `exit`).
- **Memory Management**: Implements memory allocation and demand paging as well as an OS managed per process heap.
- **Text Mode VGA Support**: Provides direct manipulation of the VGA text buffer for terminal output.
- **libc Implementation**: Provides wrappers for system calls and including implementations of `malloc`, `free` and `print`.

## System Calls Implemented
- **`write()`**: Writes data to stdout
- **`read()`**: Reads data from stdin
- **`get()`**: Reads a char from stdin 
- **`brk()`**: Adjusts the program's dynamic data segment, used for dynamic memory allocation.

## Drivers Implemented
- **VGA Driver**: Allows writing text to screen coordinates
- **Keyboard Driver**: Captures and Buffers user input. 

## Getting Started

### Prerequisites
- A Linux-based development environment.
- GCC (GNU Compiler Collection) or Clang for compiling the kernel.
- QEMU for emulation.
- NASM (Netwide Assembler) for bootloader assembly.

### Building C-OS
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/C-OS.git
   cd C-OS
   ```
2. Build the kernel:
   ```bash
   make
   ```
3. Run the operating system in QEMU:
   ```bash
   make run
   ```

### Using the System
- On boot, the system initializes hardware and provides a basic shell interface.
- You will be greeted by a shell-like interface prompting you to run a user-mode program that was loaded by grub.
- Enter the pid of the process you wish to start and press enter

### Example Modules

C-OS includes example userland modules to demonstrate the functionality of the operating system, system calls, and the libc implementation:

### Hello World: 
A simple program that prints "Hello, World!" to the screen, showcasing the use of the `write` system call through the libc wrapper and the `print` method.


### Snake Game: 
A fully functional implementation of the classic Snake game. This module utilizes `malloc` for dynamic memory allocation, `write_char_at()` for screen output, and `get()` to read character input. The game serves as a comprehensive demonstration of kernel`s capabilities.

These modules illustrate how developers can build applications for C-OS.

## Development Goals
- Learn low-level programming concepts and hardware interaction.
- Implement a minimal, functional operating system.
- Understand kernel development and userland integration.

## Contributing
Contributions are welcome! Please fork the repository, make changes, and submit a pull request.

## Acknowledgments
- Resources like [OSDev Wiki](https://wiki.osdev.org/) and [JamesM's Kernel Development Tutorials](http://www.jamesmolloy.co.uk/tutorial_html/).
- Inspiration from other open-source operating system projects.





