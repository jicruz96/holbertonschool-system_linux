#!/usr/bin/python3
'''
read_write_heap module
'''


def error_function(program):
    '''error_function function'''

    print("Usage: {} pid search replace".format(program))
    exit(1)


if __name__ == '__main__':
    ''' finds and replaces a string in the heap of a running process'''

    import sys

    if len(sys.argv) != 4:
        error_function(sys.argv[0])

    pid = int(sys.argv[1])
    search = sys.argv[2]
    replace = sys.argv[3]

    mapsFileName = f"/proc/{pid}/maps"
    memFileName = f"/proc/{pid}/mem"


    try:
        with open(mapsFileName, 'r') as mapsFile:

            for line in mapsFile:
                if '[heap]' in line:

                    split = line.split()
                    addr = split[0]
                    perms = split[1]
                    offset = split[2]
                    device = split[3]
                    inode = split[4]
                    path = split[-1]


                    # check for permissions
                    if perms[0] != 'r' or perms[1] != 'w':
                        mapsFile.close()
                        exit(0)

                    # get start and end of heap
                    addr = addr.split('-')
                    if len(addr) != 2:
                        mapsFile.close()
                        exit(0)
                    addrStart = int(addr[0], 16)
                    addrEnd = int(addr[1], 16)
    except IOError as e:
        exit(1)

        # now that we have what we need we can open mem
    try:
        with open(memFileName, 'rb+') as memFile:

            memFile.seek(addrStart)
            heap = memFile.read(addrEnd - addrStart)

            try:
                i = heap.index(bytes(search, "ASCII"))
            except Exception:
                exit(0)

            memFile.seek(addrStart + i)
            memFile.write(bytes(replace, "ASCII"))
    except Exception as e:
        print(e)
