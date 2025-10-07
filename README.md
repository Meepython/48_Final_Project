# Employee Payroll Management (C)

Simple terminal app to manage monthly salary records stored in a CSV file.  
Good for practicing C strings, file I/O, and menu-driven programs. Includes unit tests and an end-to-end (E2E) test.

---

## Features
- Create `employees.csv` (auto if missing) with header
- List all records in a fixed-width table
- Create a record (name, position, salary, date)
- Search by name or position
- Update position / salary / payment date for an exact name
- Delete a record (exact name + confirmation)
- Tests: 2 unit tests + 1 E2E test

---

## Repository Layout
```
.
├─ src/                # optional workspace folder
├─ csv/               # optional folder to keep sample CSVs
│  └─ employees.csv
├─ payroll.c          # Main app (menu) + internal test runner
├─ unit_test.c        # Standalone unit test (optional)
├─ e2e_test.c         # Standalone end-to-end test (optional)
└─ README.md
```
> The app uses `employees.csv` in the current working directory unless you override `CSV_PATH` at compile time.

---

## Build & Run

### Linux / macOS
```
# build main app
gcc -std=c11 -Wall -Wextra -Wpedantic payroll.c -o payroll

# run
./payroll
```

### Windows (MinGW)
```
gcc -std=c11 -Wall -Wextra -Wpedantic payroll.c -o payroll.exe
payroll.exe
```

---

## Using the Program

Menu:
```
1) List all record
2) Create salary record
3) Search salary record
4) Update salary record
5) Delete salary record
6) Unit test [2] + E2E
7) Exit
```

Input formats:
- Name: John Doe  
- Position: Developer  
- Salary: 42000.5  
- Payment Date: YYYY-MM-DD

---

## CSV Format

Header:
```
EmployeeName,Position,Salary,PaymentDate
```

Row example:
```
John Doe,Developer,42000.50,2025-08-01
```

Notes:
- No spaces after commas in the file
- Salary is written with 2 decimals

---

## Testing

You can run tests inside the app (menu option 6) or build the standalone test binaries.

### A) Built-in tests (menu option 6)
1. Run `./payroll`
2. Choose `6) Unit test [2] + E2E`

This backs up your current `employees.csv`, runs tests, then restores it.

### B) Standalone Unit Tests
```
# build
gcc -std=c11 -Wall -Wextra -Wpedantic -DTESTING unit_test.c -o unit_test

# run
./unit_test
```

Override the test CSV if you want:
```
gcc -std=c11 -Wall -Wextra -Wpedantic -DTESTING -DCSV_PATH="test_employees.csv" unit_test.c -o unit_test
./unit_test
```

### C) Standalone E2E Test
```
# build
gcc -std=c11 -Wall -Wextra -Wpedantic -DTESTING -DCSV_PATH="e2e_employees.csv" e2e_test.c -o e2e_test

# run
./e2e_test
```

---

## Troubleshooting

- “macro redefined” warning: don’t define `CSV_PATH` both in code and via `-DCSV_PATH=...`. Pick one.
- Line endings: if you edited CSV on Windows (CRLF), the app trims `\r\n`, but raw string checks in tests may fail. Keep files using LF or use provided tests.
- Input issues: the app uses `fgets` + `sscanf` to avoid `scanf` newline problems. If you add raw `scanf`, clear the input buffer.

---