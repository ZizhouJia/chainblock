import os
import sys

del_file_name=["cmake_install.cmake","CMakeCache.txt","Makefile",""]

def clean_current_dir(root):
    items=os.listdir(root);
    for item in items:
        path=os.path.join(root,item)
        if(item=="CMakeFiles"):
            os.popen("rm -rf "+path)
            continue;
        if(item in del_file_name):
            os.popen("rm "+path)
            continue;
        if(item.endswith(".so") or item.endswith(".a")):
            os.popen("rm "+path)
            continue;
        if(item.startswith("test_")):
            os.popen("rm "+path)
            continue
        if(item=="." or item==".."):
            continue
        if(os.path.isdir(path)):
            clean_current_dir(path)


clean_current_dir(".")
