from sys import argv, exit
from cs50 import SQL

if len(argv) != 2:
    print("Correct Usage: python roster.py house")
    exit(1)

house = argv[1]

# Connect to database
db = SQL("sqlite:///students.db")

# SQL query
students = db.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first", house)

for student in students:
    # Extract dict values into variables
    birth = student["birth"]
    first = student["first"]
    middle = student["middle"]
    last = student["last"]

    # Initialise name array
    name = []
    name.append(first)

    # Check for middle name
    if middle is not None:
        name.append(middle)

    name.append(last)

    print(f"{' '.join(name)}, born {birth}")