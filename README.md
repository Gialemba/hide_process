# hide_process

A simple Linux kernel module designed to hide a process from the system's process list.

## Overview

This kernel module allows you to conceal a running process by sending it a signal. Upon receiving this signal, the module modifies the kernel's process list to hide the specified process, making it invisible to commands like `ps`, `top`, and `htop`.

## Features

- **Process Hiding**: Hide a process from standard system monitoring tools.
- **Signal-Based Activation**: Trigger the hiding mechanism by sending a signal to the target process.
- **Lightweight**: Minimal codebase focused on the specific functionality of process hiding.

##  Installation

### Prerequisites

- A Linux system with kernel headers installed.

### Steps

1. Clone the repository:

   ```bash
   git clone https://github.com/Gialemba/hide_process.git
   cd hide_process
   ```
2. Compile the kernel module:

  ```bash
  make
  ```
3. Insert the module into the kernel:

  ```bash
  sudo make load
  ```
4. To remove the module:

  ```bash
  sudo make unload
  ```

## USAGE

1. Identify the PID of the process you want to hide.

2. Send the signal to the target process:

  ```bash
  kill -64 <pid>
  ```
  (replace `<pid>` with the actual process pid)

3. Verify that the process is hidden:

  ```bash
  ps -e | grep <pid>
  ```
  The process should no longer appear in the list.

### WARNINGS

- **Kernel Compatibility**: This module is designed for educational purposes and may not be compatible with all Linux kernel versions.

- **System Stability**: Improper use of kernel modules can lead to system instability. Ensure you understand the implications before loading this module.

- **Security Implications**: Hiding processes can be used for malicious purposes. Use responsibly and only on systems where you have explicit permission.
