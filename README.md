[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

# HaletasWeb

This is the source code for my personal website built using **C++**.

---

## Project Structure

The project is organised as follows:

- `Tag.h` – Custom class for building HTML elements programmatically  
- `index()` – Generates the homepage (`index.html`)  
- `links()` – Generates the links page (`links.html`)  
- `projects()` – Fetches repositories from GitHub and generates the projects page (`projects.html`)  
- `WriteHTML()` – Utility function for writing final HTML files  

---

## Technologies Used

- **C++** – Primary programming language  
- **[nlohmann/json](https://github.com/nlohmann/json)** – JSON parsing library for GitHub API responses  
- **[libcurl](https://curl.se/libcurl/)** – HTTP requests (used to fetch repositories and metadata from GitHub)  
- **Custom HTML Tag DSL** (`Tag.h`) – For generating semantic HTML in C++  
- **Picocss**: Minimal CSS framework for semantic HTML

---

## How to Build

Clone the repository:
```bash
git clone https://github.com/Haletas033/Personal-Website
cd Personal-Website
```
Build the project (example with `g++`):
```bash
g++ -std=c++17 main.cpp -lcurl -o website
```
Run the generator:
```bash
./website
```

## License

This project is licensed under the [MIT License](LICENSE).
