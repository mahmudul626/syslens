# SysLens 🔍

SysLens is a lightweight, terminal-based system information utility written in C. It parses the Linux `/proc` filesystem to produce a real-time snapshot of system health, hardware specifications, and resource usage.

## 🚀 Features

- **System Overview:** Displays OS name, kernel version, CPU model, uptime, and the current user.
- **Resource Monitoring:** Provides visual progress bars for RAM and swap usage, with color-coded alerts (green/yellow/red) for critical levels.
- **Process Statistics:** Summarizes total tasks along with counts of running, sleeping, and zombie processes.
- **Performance Metrics:** Shows system load averages over 1, 5, and 15 minutes.
- **Lightweight & Fast:** Consumes minimal memory and runs without external dependencies.

## 📂 Project Structure

The repository is organized as follows:

```
syslens/
├── include/
│   └── main.h        # Header definitions
├── src/
│   ├── main.c        # Program entry point and argument parsing
│   ├── mem.c         # Memory and load average logic
│   ├── proc.c        # Process parsing logic (/proc handling)
│   └── sys.c         # OS and hardware info logic
├── LICENSE           # Project license
├── Makefile          # Build automation script
└── README.md         # This documentation
```

## 🛠 Installation

### Prerequisites

- Linux-based operating system
- `gcc` compiler
- `make` utility

### Build from source

1. Clone or download the repository.
2. Enter the project directory:
   ```bash
   cd syslens
   ```
3. Compile the program:
   ```bash
   make
   ```

### System-wide installation

To install `syslens` so it can be run from anywhere:

```bash
sudo make install
```

## 📖 Usage

Running without arguments displays all available information. Flags can be used to show specific sections:

| Command            | Description                                         |
|---------------------|-----------------------------------------------------|
| `syslens`           | Show all system, resource, and process info        |
| `syslens -s`        | Show only system info (OS, kernel, CPU, etc.)      |
| `syslens -m`        | Show resource usage (RAM, swap, load average)      |
| `syslens -p`        | Show active process statistics                     |
| `syslens --version` | Display the current version                        |
| `syslens --help`    | Display the help menu                              |

## ⚙️ How it works

SysLens reads data from the Linux kernel's virtual `/proc` filesystem:

- **CPU:** Parsed from `/proc/cpuinfo`.
- **Memory:** Information from `/proc/meminfo` is used for calculations.
- **Processes:** Iterates over PID directories under `/proc` and reads their `/status` files.
- **Uptime:** Parsed from `/proc/uptime` and converted from seconds to hours/minutes.

## 🧹 Uninstallation

To remove the installed binary from the system:

```bash
sudo make uninstall
```

