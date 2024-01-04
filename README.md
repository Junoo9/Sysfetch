# System Information Retrieval

This C++ program retrieves and displays key information about the system's hardware components, providing insights into the CPU, RAM, storage, and GPU.

## Features

- **CPU Information:** Architecture, number of processors, physical core count, CPU name, and clock speed.
- **RAM Information:** Total physical memory, available physical memory, total virtual memory, and available virtual memory.
- **Storage Information:** Details about each available drive, including total storage and available storage.
- **GPU Information:** Display names, resolutions, and color depths for active GPUs.

## How to Use

1. Clone the repository:

    ```bash
    git clone https://github.com/your-username/system-information-retrieval.git
    cd system-information-retrieval
    ```

2. Build the executable (assuming you have a C++ compiler installed):

    ```bash
    g++ main.cpp -o SystemInfo
    ```

3. Run the program:

    ```bash
    ./SystemInfo
    ```

## Requirements

- C++ compiler (e.g., GCC, Visual C++)
- Windows operating system (for registry-based CPU information retrieval)

## License

This project is licensed under the [MIT License](LICENSE).
