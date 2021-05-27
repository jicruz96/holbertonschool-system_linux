#!/usr/bin/python3
""" replaces a string in the heap memory of a running process

    Usage: ./read_write_heap.py <pid> <target> <replacement>

    where <pid> is a pid, <target> is a string to search for in the heap, and
    <replacement> replaces target.
"""

from re import findall
from sys import argv, stdout


def find_target_location(buffer, target):
    """ finds and returns buffer index where target first appears in buffer

    Args:
        * buffer (bytes): bytes buffer
        * target (str)  : target string
    """
    print(buffer)
    input()
    print(buffer.find(bytes(target)))
    input()
    for i in range(len(buffer) - len(target) + 1):
        j = 0
        while j < len(target) and int(buffer[i + j]) == ord(target[j]):
            j += 1
        if j == len(target):
            input('returning {}'.format(i))
            return i
    return None


def read_write_heap(pid, target, replacement):
    """ finds and replaces target string in heap of a running process

    Args:
        * pid (int)          : pid of a running process
        * target (str)       : string to search and replace
        * replacement (bytes): replacement string
    """

    # We'll use a regular expression to match and extract the addresses.
    pattern_sub_strings = [
        r'\b',             # start of line
        r'([\da-zA-Z]+)',  # use parens to match/capture start of address range
        r'-',              # delimiter of address range
        r'([\da-zA-Z]+)',  # use parens to match/capture end of address range
        r'.*?',            # rest of string leading up to '[heap]'
        r'\[heap]'
    ]
    pattern = ''.join(pattern_sub_strings)

    # Get address_map from /proc/PID/maps file
    try:
        with open('/proc/{}/maps'.format(pid), 'r') as maps_file:
            address_map = maps_file.read()
            maps_file.close()
    except FileNotFoundError as e:
        error_out('{} does not represent a running process.\n'.format(pid))

    # Open mem file
    with open('/proc/{}/mem'.format(pid), 'r+b', 0) as mem_file:

        # Find all ranges of heap memory
        # Note: re.findall() will return a list of tuples containing the
        #       matched address ranges
        heap_ranges = findall(pattern, address_map)

        # Inspect heap ranges and replace string
        for heap_range in heap_ranges:

            # convert addresses from hex strings to decimal integers
            start = int(heap_range[0], 16)
            end = int(heap_range[1], 16)

            # go to starting address
            mem_file.seek(start)

            # read memory segment
            buffer = mem_file.read(end - start)

            # find location of target string
            target_location = find_target_location(buffer, target)

            if target_location:  # if we found a target

                # go to target string location in heap (mem file)
                mem_file.seek(start + target_location)

                # overwrite target with replacement
                difference = max(len(target) - len(replacement), 0)
                mem_file.write(replacement + bytes(difference))

    mem_file.close()


def error_out(error_msg):
    """ prints error message and exits with error code 1 """
    stdout.write(error_msg)
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
