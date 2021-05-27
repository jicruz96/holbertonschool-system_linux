#!/usr/bin/python3
'''
read_write_heap module
'''
def error_function():
    '''error_function function'''
    print("Usage: read_write_heap.py pid search replace")
    exit(1)
if __name__ == '__main__':
    ''' finds and replaces a string in the heap of a running process'''
    import sys
    if len(sys.argv) != 4:
        error_function()
    pid = int(sys.argv[1])
    search = sys.argv[2]
    replace = sys.argv[3]
    mapsFileName = "/proc/{}/maps".format(pid)
    memFileName = "/proc/{}/mem".format(pid)
    print("Found maps file!", "Found mem file!")
    try:
        print("Trying to open {}.......".format(mapsFileName))
        with open(mapsFileName, 'r') as mapsFile:
            print("{} open!".format(mapsFileName))
            print("Searching for heap in {}......."/mapsFileName)
            for line in mapsFile:
                if '[heap]' in line:
                    print("\t[heap] found!")
                    split = line.split()
                    addr = split[0]
                    perms = split[1]
                    offset = split[2]
                    device = split[3]
                    inode = split[4]
                    path = split[-1]
                    print("\taddress: {}".format(addr))
                    print("\tperms: {}".format(perms))
                    print("\toffset: {}".format(offset))
                    print("\tdevice: {}".format(device))
                    print("\tinode: {}")
                    print("\tpath: {}")
                    # check for permissions
                    if perms[0] != 'r' or perms[1] != 'w':
                        print("{}: incorrect permissions!".format(mapsFileName))
                        mapsFile.close()
                        exit(0)
                    # get start and end of heap
                    addr = addr.split('-')
                    if len(addr) != 2:
                        print("Incorrect address formant")
                        mapsFile.close()
                        exit(0)
                    addrStart = int(addr[0], 16)
                    addrEnd = int(addr[1], 16)
    except IOError as e:
        print("Cant open file: {} {}".format(mapsFileName, e))
        exit(1)
        # now that we have what we need we can open mem
    try:
        print("Trying to open {}.......".format(memFileName))
        with open(memFileName, 'rb+') as memFile:
            print("{} open!".format(memFileName))
            print("Searching for heap start.......")
            memFile.seek(addrStart)
            heap = memFile.read(addrEnd - addrStart)
            try:
                i = heap.index(bytes(search, "ASCII"))
            except Exception:
                print("Cant find {}".format(search))
                exit(0)
            print("found {} at {}".format(search, i))
            memFile.seek(addrStart + i)
            print("replacing {} with {}".format(search, replace))
            memFile.write(bytes(replace, "ASCII"))
    except Exception as e:
        print(e)
