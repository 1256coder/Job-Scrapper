# Job Scraper & Category Search System

A console-based **Job Scraper** developed in **C++** as an **Operating Systems (OS) project** using **Fedora Linux**. The application fetches live remote job listings from the RemoteOK API, allows users to browse and search jobs by category, automatically generates an HTML page containing clickable application links, and opens selected job postings directly in the default web browser.

---

## Features

- Fetches live remote job listings using the RemoteOK API.
- Displays up to 50 latest job opportunities.
- Search jobs by title or category.
- Supports category matching for:
  - Python
  - Software Engineering
  - Arts
  - Electrical Engineering
  - Remote Jobs
- Automatically creates a **jobs.html** file containing clickable application links.
- Open selected job postings directly in the default web browser.
- Clear the generated HTML file from the program menu.
- Input validation for a smoother user experience.
- Colored console interface using ANSI escape codes.

---

## Technologies Used

- C++
- Fedora Linux
- libcurl
- nlohmann/json
- STL (Vector, String, Algorithm)
- HTML

---

## Project Structure

```
main.cpp      // Main source code
jobs.html     // Automatically generated after running the program
```

---

## How It Works

1. The program connects to the RemoteOK API.
2. Live job data is fetched in JSON format.
3. The JSON data is parsed using the nlohmann/json library.
4. Job details such as title, company, category, and application link are extracted.
5. The latest jobs are displayed in the console.
6. A **jobs.html** file is automatically generated with clickable application links.
7. Users can search jobs using categories or keywords.
8. Users can enter a job number to open the corresponding application page in their default web browser.

---

## Menu

```
========== JOB SCRAPER ==========

1. Display Jobs
2. Search By Category
3. Clear Saved Jobs File
4. Exit
```

---

## Requirements

Before running the project, install:

- Fedora Linux
- GCC/G++ Compiler
- libcurl
- nlohmann/json library

Compile the program:

```bash
g++ main.cpp -o JobScraper -lcurl
```

Run the program:

```bash
./JobScraper
```

---

## Output

The program displays job listings in the terminal and automatically creates a **jobs.html** file containing all fetched jobs with clickable application links.

---

## Learning Outcomes

This project demonstrates:

- REST API Integration
- JSON Parsing
- File Handling in C++
- HTML File Generation
- Searching and Filtering Algorithms
- Browser Automation
- Linux-based Development using Fedora
- Standard Template Library (STL)

---

## Future Improvements

- Save favorite jobs
- Filter jobs by company
- Filter jobs by salary
- Export job listings to CSV
- Add pagination support
- Improve cross-platform browser compatibility

---

## Author

Developed as an **Operating Systems (OS) Project** using **Fedora Linux**.

```

This version is suitable for a professional GitHub repository while clearly indicating that it was developed as an **Operating Systems project in Fedora Linux**.
