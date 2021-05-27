#!/usr/bin/python3
""" replaces a string in the heap memory of a running process

    Usage: ./read_write_heap.py <pid> <target> <replacement>

    where <pid> is a pid, <target> is a string to search for in the heap, and
    <replacement> replaces target.
"""

from re import findall
from sys import argv


def read_write_heap(pid, target, replacement):
    """ finds and replaces target string in heap of a running process"""

    try:
        with open('/proc/{}/maps'.format(pid), 'r') as maps_file:
            address_map = maps_file.read()
            maps_file.close()
    except FileNotFoundError as e:
        error_out('{} does not represent a running process.\n'.format(pid))
  
    pattern = r'\b([\da-zA-Z]+)-([\da-zA-Z]+).*?\[heap]'
    # re.findall() returns a list of tuples of the captured groups
    heap_ranges = findall(pattern, address_map)
    with open('/proc/{}/mem'.format(pid), 'r+b', 0) as mem_file:

        for heap_range in heap_ranges:
            start = int(heap_range[0], 16)
            end = int(heap_range[1], 16)
            mem_file.seek(start)
            buffer = mem_file.read(end - start)
            target_location = buffer.find(bytes(target, 'ascii'))

            if target_location != -1:
                mem_file.seek(start + target_location)
                difference = max(len(target) - len(replacement), 0)
                mem_file.write(replacement + bytes(difference))

        mem_file.close()


def error_out(error_msg):
    """ prints error message and exits with error code 1 """
    print(error_msg)
    exit(1)


if __name__ == '__main__':

    # 4 arguments exactly!
    if len(argv) != 4:
        error_out('Usage: {} pid target_str replacement_str\n'.format(argv[0]))

    # First arg must be the pid of a running process
    try:
        pid = int(argv[1])
    except ValueError:
        error_out('{}: first arg must be valid proccess id\n'.format(argv[0]))

    # Second arg is target string
    target = argv[2]

    # Third arg is replacement string. Must be encoded into bytes first
    replacement = bytes(argv[3], 'ascii')

    # Let's do this!
    read_write_heap(pid, target, replacement)
    print('Complete.')
    exit(0)
