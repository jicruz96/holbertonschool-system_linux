#!/usr/bin/python3
'''read_write_heap module'''
import sys


def error_function():
    '''error_function function'''
    print("Usage: read_write_heap.py pid search replace")
    exit(1)


def foobar(pid, search, replace):
    '''stuff'''
    mapsFileName = f"/proc/{pid}/maps"
    memFileName = f"/proc/{pid}/mem"

    print("Found maps file!", "Found mem file!")

    try:
        print(f"Trying to open {mapsFileName}.......")
        with open(mapsFileName, 'r') as mapsFile:
            print(f"{mapsFileName} open!")
            print(f"Searching for heap in {mapsFileName}.......")

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

                    print(f"\taddress: {addr}")
                    print(f"\tperms: {perms}")
                    print(f"\toffset: {offset}")
                    print(f"\tdevice: {device}")
                    print(f"\tinode: {inode}")
                    print(f"\tpath: {path}")

                    # check for permissions
                    if perms[0] != 'r' or perms[1] != 'w':
                        print(f"{mapsFileName}: incorrect permissions!")
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
        print(f"Cant open file: {mapsFileName} {e}")
        exit(1)

        # now that we have what we need we can open mem
    try:
        print(f"Trying to open {memFileName}.......")
        with open(memFileName, 'rb+') as memFile:
            print(f"{memFileName} open!")

            print("Searching for heap start.......")
            memFile.seek(addrStart)
            heap = memFile.read(addrEnd - addrStart)

            try:
                i = heap.index(bytes(search, "ASCII"))
            except Exception:
                print(f"Cant find {search}")
                exit(0)
            print(f"found {search} at {i}")

            memFile.seek(addrStart + i)
            print(f"replacing {search} with {replace}")
            memFile.write(bytes(replace, "ASCII"))
    except Exception as e:
        print(e)


if __name__ == '__main__':

    if len(sys.argv) != 4:
        error_function()

    pid = int(sys.argv[1])
    search = sys.argv[2]
    replace = sys.argv[3]
    foobar(pid, search, replace)
