import os
import sys

def main():
    print(count_lines_recursive(sys.argv[1]))

def count_lines_recursive(start_dir_name):
    lines = 0

    for entry in os.listdir(start_dir_name):
        try:
            file_lines = count_lines(start_dir_name + "\\" + entry)
            
            file_extensions = [".cpp", ".cc", ".h", ".hh"]
            for ext in file_extensions:
                if entry.endswith(ext):
                    lines += file_lines

        except:
            if "." in entry:
                continue
            lines += count_lines_recursive(start_dir_name + "\\" + entry)

    return lines


def entries_in_dir(dir):
    return [name for name in os.listdir(dir)]    


def get_files(entry_list):
    files = []
    for entry in entry_list:
        if os.path.isfile(entry):
            files.append(entry)
    return files

def get_dirs(entry_list):
    dirs = []
    for entry in entry_list:
        if os.path.isdir(entry):
            dirs.append(entry)
    return dirs

def count_lines(filename):
    with open(filename) as file:
        lines = file.readlines()
        return len(lines)


main()
