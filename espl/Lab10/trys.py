import os

if __name__ == "__main__":
    for (root, dirs, files) in os.walk('Lab10', topdown=True):
        print("hello")
        print(root)
        print(dirs)
        print(files)