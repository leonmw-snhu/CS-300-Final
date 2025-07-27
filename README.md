
# CSV Parser Unit Testing Project

This project provides a C++ CSV parser and a suite of unit tests written using [Google Test](https://github.com/google/googletest). It is designed for validating CSV parsing functionality, including header extraction, row access, error handling, and file manipulation.

---

## 📁 Project Structure

```
.
├── src/
│   ├── CSVparser.cpp
│   └── CSVparser.hpp
│   └── Project Two.hpp
├── tests/
│   ├── test_csvparser.cpp
│   ├── test.csv
│   ├── empty.csv
│   ├── header_only.csv
│   └── semicolon.csv
│   └── test_csvparser.cpp
├── Makefile
└── README.md
```

---

## 🔧 Requirements

- C++17-compatible compiler (e.g., `g++`, `clang++`)
- Google Test installed via Homebrew:
  ```bash
  brew install googletest
  ```

---

## 🛠 Building and Running Tests

To compile the test runner:

```bash
make
```

To clean the build:

```bash
make clean
```

To run the tests:

```bash
./test_runner
```

You’ll see output from Google Test including passed/failed test cases.

---

## 🧪 Included Tests

- File loading
- Header parsing
- Row access
- Add/delete row functionality
- Custom delimiter support
- Graceful handling of malformed or empty files

---

## 🕒 Timestamped Output

Each test run prints the current date/time at the start for easier log tracking:
```
🕒 Test run started at: Sat Jul 26 14:19:41 2025
```

---

## 📄 Notes

- Ensure all test CSV files are present in the `tests/` folder.
- Modify the `Makefile` if your `gtest` install path differs.

---

## 📬 Questions?

Feel free to reach out or file an issue for help or suggestions!
