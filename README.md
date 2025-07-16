<h1 align="center">🕸 WebShooter</h1>

<p align="center">
  A high-performance archiving utility designed for Unix-like systems that leverages multi-threading with the Producer-Consumer pattern to optimize file archiving and extraction operations. Developed as part of my Computer Science final project, this tool significantly outperforms traditional archivers like <code>tar</code> and <code>cpio</code> through parallel processing.
</p>

![image](https://img.shields.io/github/license/RiadOliveira/WebShooter)

<br/>

Contents
=================
<!--ts-->
* [🛠️ Technologies](#technologies)
* [🚀 Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Installation & Setup](#setup)
  * [CLI Usage](#usage)
* [📐 Architecture & Logic](#architecture)
  * [Multithreaded Strategy](#strategy)
  * [Archiving Process](#archiving)
    * [Producer Thread](#archiving-producer)
    * [Consumer Thread](#archiving-consumer)
  * [Unarchiving Process](#unarchiving)
    * [Producer Thread](#unarchiving-producer)
    * [Consumer Thread](#unarchiving-consumer)
* [📂 Project Structure](#structure)
* [⚙️ Features](#features)
* [🔧 Usage Examples](#examples)
  * [📦 Web (Archiving) Examples](#example-web)
  * [📤 Unweb (Unarchiving) Examples](#example-unweb)
* [📊 Performance Benchmarks](#performance)
* [📚 Academic Article](#article)
* [📝 License](#license)
* [👨‍💻 Author](#author)
* [🌐 Socials](#socials)
<!--te-->
<br/>

<h2 id="technologies">🛠️ Technologies</h2>

Built with:

* [C](https://en.cppreference.com/w/c/language.html)
  * [POSIX Threads](https://pubs.opengroup.org/onlinepubs/7908799/xsh/threads.html) 
* [GCC (GNU Compiler Collection)](https://gcc.gnu.org/)
* [CMake](https://cmake.org/)
* [GNU Make](https://www.gnu.org/software/make/) <br/><br/>

<h2 id="getting-started">🚀 Getting Started</h2>

<h3 id="prerequisites">Prerequisites</h3>

Before running the application, ensure the following tools are installed:
* Unix-like OS (Linux, BSD, macOS, etc.)
* GCC (C Compiler)
* Make and CMake
* Git

<h3 id="setup">Installation & Setup</h3>

```bash
# Clone the repository
git clone https://github.com/RiadOliveira/WebShooter.git

# Navigate to the project directory
cd WebShooter/release

# Build with CMake and Make
cmake -DCMAKE_BUILD_TYPE=Release ../
make

# Run the executable
./wst -h
```

<h3 id="usage">CLI Usage</h3>

The CLI offers three main options: archiving (`--web`), unarchiving (`--unweb`), and help.  
It was designed to be simple and intuitive for both users and developers. <br/><br/>

| Option | Alias | Required | Description |
|--------|-------|----------|-------------|
| `--web` | `-w` | Yes | Archives the specified files/folders into a single output file. Requires the output archive path followed by one or more input paths. |
| `--unweb` | `-u` | Yes | Extracts contents from a previously created archive. Requires the archive path and optionally a destination path (defaults to current directory). |
| `--help` | `-h` | No | Displays the full help screen with usage examples and descriptions. |

<h2 id="architecture">📐 Architecture & Logic</h2>

<h3 id="strategy">Multithreaded Strategy</h3>

The archiver uses the classic Producer-Consumer pattern:
- **Producer Thread**: Reads input files/directories and pushes data into shared buffers.
- **Consumer Thread**: Writes buffer data into an output file.
- Synchronization is done with **mutexes and condition variables** from `pthread`.
- **Double-buffering** prevents thread contention.

Each buffer has a fixed size of **1 MB** and is managed via status flags:
- `unset`: Ready for Producer.
- `consumable`: Ready for Consumer.
- `finished`: End of transmission.

<h3 id="archiving">Archiving Process</h3>

<h4 id="archiving-producer">Producer Thread</h4>

* Reads files/directories recursively.
* Store metadata and file contents into buffers.
* Fills buffers with 1MB chunks.
* Signals consumer when buffers are ready.

<h4 id="archiving-consumer">Consumer Thread</h4>

* Writes buffer contents to output file.
* Signals producer when buffers are empty.
* Handles final archive structure.

<h3 id="unarchiving">Unarchiving Process</h3>

<h4 id="unarchiving-producer">Producer Thread</h4>

* Reads archive file in 1MB chunks.
* Fills buffers sequentially
* Signals consumer when data is available.

<h4 id="unarchiving-consumer">Consumer Thread</h4>

* Reconstructs original directory structure.
* Creates files with original metadata.
* Handles special directory markers.

<h2 id="structure">📂 Project Structure</h2>

```text
WebShooter/
├── .vscode/         # VSCode tasks and debug settings
├── debug/           # Debug build output
├── release/         # Release build output
├── src/
│   ├── cli/         # CLI argument handling and dispatch
│   ├── lib/         # Archiving/unarchiving logic
│   ├── utils/       # Shared helpers (buffers, I/O)
└── CMakeLists.txt   # CMake configuration
```

<br/>

<h2 id="features">⚙️ Features</h2>

- **Multi-threaded Architecture**: Implements producer-consumer pattern with POSIX threads for parallel archiving/extraction.
- **CLI Utility** - User-friendly and flexible command-line interface with simple usage syntax.
- **Library Support** - Modular library for easy integration in other C applications.
- **Efficient I/O Handling** - Operates using shared memory buffers, mutexes, and condition variables for synchronization.
- **Directory Preservation**: Maintains complete directory structure and file metadata.
- **Portable and Lightweight** - Pure C codebase with POSIX compliance for wide system compatibility.
- **Performance-Oriented** - Designed to minimize bottlenecks, reduce context-switching delays, and take advantage of multi-core CPUs. <br/><br/>

<h2 id="examples">🔧 Usage Examples</h2>

<h4 id="example-web">📦 Web (Archiving) Examples</h4>

```bash
# Archive a single folder into output.wst
./wst -w output.wst folder/

# Archive multiple contents (files and directories)
./wst --web archive.wst file1.txt file2.txt folder1/
```

<h4 id="example-unweb">📤 Unweb (Unarchiving) Examples</h4>

```bash
# Unarchive into the current directory
./wst -u archive.wst

# Unarchive into a specific output folder
./wst --unweb archive.wst ./output-folder/
```

<br/>

<h2 id="performance">📊 Performance Benchmarks</h2>

Benchmarked using [hyperfine](https://github.com/sharkdp/hyperfine) against standard Unix tools:

| Tool              | Archiving Speed | Unarchiving Speed |
|-------------------|-----------------|-------------------|
| **This Project**  | **1546.89 MB/s**| **1419.61 MB/s**  |
| GNU tar           | 1106.21 MB/s    | 918.55 MB/s       |
| GNU cpio          | 448.61 MB/s     | 331.51 MB/s       |

> 🚀 Achieved **40% faster** archiving and **55% faster** unarchiving than `tar`.

*Tested on Dell G15 (Intel i5-10500H, 6 cores/12 threads, NVMe SSD)*

<br/>

<h2 id="article">📚 Academic Article</h2>
This project was developed as part of a Computer Science undergraduate thesis. For full methodology and benchmarking details: <a href="https://drive.google.com/file/d/1WLR53rfo7fqLqL0qZlNOb7oieLnb9AQq/view?usp=sharing">📖 Read the full article</a> <i>(PDF, 17 pages - in Brazilian Portuguese).</i>

<h2 id="license">📝 License</h2>
This project is MIT Licensed. See <a href="https://github.com/RiadOliveira/WebShooter/blob/main/LICENSE">LICENSE</a> file for more details.

<br/>

<h2 id="author">👨‍💻 Author</h2>

<kbd>
  <a href="https://github.com/RiadOliveira">
    <img src="https://avatars.githubusercontent.com/u/69125013?v=4" width="100" alt="Ríad Oliveira"/>
    <br/><br/>
    <p align="center"><b>Ríad Oliveira</b></p>
  </a>
</kbd>

## 🌐 Socials

<div id="socials">
  <a href = "mailto:riad.oliveira@hotmail.com"><img class="badge" src="https://img.shields.io/badge/Microsoft_Outlook-0078D4?style=for-the-badge&logo=microsoft-outlook&logoColor=white" target="_blank"/></a>
  <a href = "mailto:riad.oliveira@gmail.com"><img class="badge" src="https://img.shields.io/badge/Gmail-D14836?style=for-the-badge&logo=gmail&logoColor=white" target="_blank"/></a>
  <a href="https://www.linkedin.com/in/ríad-oliveira" target="_blank"><img class="badge" src="https://img.shields.io/badge/-LinkedIn-%230077B5?style=for-the-badge&logo=linkedin&logoColor=white" target="_blank"/></a>
</div>
