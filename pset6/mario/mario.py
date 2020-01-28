from cs50 import get_int

# Get integer between 1-8 from user
while True:
    height = get_int("Height: ")
    if (height > 0 and height < 9):
        break

for i in range(height):
    # Print empty spaces (w/out newline)
    print(" " * ((height - i) - 1), end="")
    # Print block(s) (w/out newline)
    print("#" * (i + 1), end="")
    # Print new line
    print()