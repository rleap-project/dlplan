import logging
import os
import subprocess


def read_file(filename):
    """ Read a file, line by line, ignoring end-of-line characters"""
    with open(filename, "r") as f:
        for line in f:
            yield line.rstrip('\n')


def write_file(filename, content):
    """ """
    with open(filename, "w") as f:
        f.write(content)


def execute(command, **kwargs):
    timeout = kwargs["timeout"] if "timeout" in kwargs else None
    stdout = open(kwargs["stdout"], 'w') if "stdout" in kwargs else None
    stderr = open(kwargs["stderr"], 'w') if "stderr" in kwargs else None
    cwd = kwargs["cwd"] if "cwd" in kwargs else os.getcwd()

    logging.info(f'Executing "{" ".join(map(str, command))}" on directory "{cwd}"')
    if stdout:
        logging.info(f'Standard output redirected to "{stdout.name}"')
    if stderr:
        logging.info(f'Standard error redirected to "{stderr.name}"')

    retcode = subprocess.call(command, cwd=cwd, stdout=stdout, stderr=stderr, timeout=timeout)

    if stdout:
        stdout.close()

    if stderr:
        stderr.close()

    if stderr is not None and os.path.getsize(stderr.name) == 0:  # Delete error log if empty
        os.remove(stderr.name)

    return retcode

def filename_core(filename):
    return os.path.splitext(os.path.basename(filename))[0]
