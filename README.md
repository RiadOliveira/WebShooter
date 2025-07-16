<h1 align="center">🕸 WebShooter</h1>

<p align="center">
  A high-performance CLI-based archiving tool developed in C as part of an undergraduate thesis. It leverages multithreading with the Producer-Consumer pattern to optimize I/O operations on Unix-like systems, offering superior performance over traditional archivers such as <code>tar</code> and <code>cpio</code>.
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
* [⚙️ Features](#features)
* [📈 Performance](#performance)
* [📝 License](#license)
* [👨‍💻 Author](#author)
* [🌐 Socials](#socials)
<!--te-->
<br/>

<h2 id="technologies">🛠️ Technologies</h2>

Built with:

* C (with POSIX Threads)
* [GCC (GNU Compiler Collection)](https://gcc.gnu.org/)
* [Make](https://www.gnu.org/software/make/) & [CMake](https://cmake.org/) <br/><br/>

<br/>

<h2 id="getting-started">🚀 Getting Started</h2>

<h3 id="prerequisites">Prerequisites</h3>

Before running the application, ensure the following tools are installed:
* Unix-like OS (Linux, BSD, macOS, etc.)
* GCC (GNU Compiler Collection)
* Make and CMake

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

<br/>

<h2 id="features">⚙️ Features</h2>

- **Multithreaded Architecture** - Parallel execution of I/O operations using the Producer-Consumer pattern.
- **CLI Utility** - User-friendly and flexible command-line interface with simple usage syntax.
- **Library Support** - Modular library for easy integration in other C applications.
- **Efficient I/O** - Operates using shared memory buffers, mutexes, and condition variables for synchronization.
- **Portable and Lightweight** - Pure C codebase with POSIX compliance for wide system compatibility.
- **Performance-Oriented** - Designed to minimize bottlenecks, reduce context-switching delays, and take advantage of multi-core CPUs. <br/><br/>

<h2 id="performance">📈 Performance</h2>

Performance tests were conducted against standard Unix archivers (`tar`, `cpio`) using the `hyperfine` benchmarking tool. The results show:

| Tool              | Archiving Speed | Unarchiving Speed |
|-------------------|-----------------|-------------------|
| WebShooter        | 1546.89 MB/s    | 1419.61 MB/s      |
| GNU tar           | 1106.21 MB/s    | 918.55 MB/s       |
| GNU cpio          | 448.61 MB/s     | 331.51 MB/s       |

> Performance gain of up to **55%** over GNU tar on decompression.

<br/>

<h2 id="license">📝 License</h2>
This project is MIT Licensed. See the LICENSE file for more details.

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
