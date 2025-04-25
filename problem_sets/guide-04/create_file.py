file_name = "data.txt"
file_size = 1000000

with open(file_name, "w") as f:
    text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n"

    while f.tell() < (file_size - 100):
        f.write(text)

    # add 'X' for padding
    remaining_bytes = file_size - f.tell()
    f.write("X" * remaining_bytes)
