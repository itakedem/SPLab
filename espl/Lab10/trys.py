import os

if __name__ == "__main__":
    for (root, dirs, files) in os.walk('~', topdown=True):
        print("hello")
        print(root)
        print(dirs)
        print(files)