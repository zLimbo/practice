
import os, shutil

dirs = os.listdir()

print(dirs)

def handle(path:str):
    print(path)
    if os.path.isdir(path):
        for sub in os.listdir(path):
            new_path = path + '/' + sub
            handle(new_path)
    elif os.path.isfile(path):
        if path.endswith('go'):
            new_dir = path[:-3]
            os.mkdir(new_dir)
            shutil.move(path, new_dir)
    else:
        print("false:", path)

def rmexe(path:str):
    # print(path)
    if os.path.isdir(path):
        for sub in os.listdir(path):
            new_path = path + '/' + sub
            handle(new_path)
    elif os.path.isfile(path):
        if path.endswith('exe'):
            print(path)
            os.remove(path)

rmexe('.')