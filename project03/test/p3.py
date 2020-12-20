#!/usr/bin/python3

import os, sys, subprocess
import argparse


def read_from_file(compar_file):
    with open(compar_file, 'r') as f:
        content = f.read()
    return content


def compare(result, expect):
    if result == expect:
        return True
    else:
        print("Expected: [{}]".format(expect))
        print("Got     : [{}]".format(result))
        return False


class Test(object):
    def __init__(self,
                 description=None,
                 compile_cmd=None,
                 run_cmd=None,
                 stdin=None,
                 stdout_compar=None,
                 stderr_compar=None,
                 output_file=None,
                 output_compar=None,
                 cwd="./",
                 timeout=10):
        self.description = description
        self.compile_cmd = compile_cmd
        self.run_cmd = run_cmd
        self.stdin = stdin
        self.stdout_compar = stdout_compar
        self.stderr_compar = stderr_compar
        self.output_file = output_file
        self.output_compar = output_compar
        self.cwd = cwd
        self.timeout = timeout

    def run(self):
        # Test for compilation
        try:
            subprocess.run(self.compile_cmd, check=True, cwd=self.cwd)
        except subprocess.CalledProcessError:
            print("Compilation Failed!")
            return False

        if self.stdin is None:
            self.stdin = subprocess.PIPE

        proc = subprocess.Popen(self.run_cmd,
                                stdin=self.stdin,
                                stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE,
                                cwd=self.cwd)
        try:
            outs, errs = proc.communicate(timeout=self.timeout)
        except subprocess.TimeoutExpired:
            print("Program timed out after {} seconds.".format(self.timeout))
            return False

        stdout = outs.decode(encoding='ascii')
        if self.stdout_compar is not None:
            stdout_expected = read_from_file(self.stdout_compar)
            correct = compare(stdout, stdout_expected)
            if not correct:
                print("Test failed when comparing stdout")
                return False
        stderr = errs.decode(encoding='ascii')
        if self.stderr_compar is not None:
            stderr_expected = read_from_file(self.stderr_compar)
            correct = compare(stderr, stderr_expected)
            if not correct:
                print("Test failed when comparing stderr")
                return False

        if self.output_file is not None and self.output_compar is not None:
            output_result = read_from_file(self.output_file)
            output_expected = read_from_file(self.output_compar)
            correct = compare(output_result, output_expected)
            if not correct:
                print("Test failed when comparing output file")
                return False

        return True

    def __str__(self):
        ret = ""
        for c in self.run_cmd:
            ret += c + ' '
        return ret


def run_tests(cont, *tests):
    passed = 0
    total = len(tests)
    for i, test in enumerate(tests):
        print("RUNNING TEST {} - {}".format(i + 1, test.description))
        print("COMMAND: {}".format(test))
        success = test.run()
        if success:
            print("TEST {} PASSED.\n\n".format(i + 1))
            passed += 1
        else:
            print("TEST {} FAILED.".format(i + 1))
            if cont:
                continue
            else:
                print("To run the remaining tests, please use flag -c.\n\n")
                break
    points = passed / total
    print("\nTESTS PASSED: {}/{}\nTOTAL POINTS: {:.2f}%".format(passed, total, points * 100))


def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-f", "--filename",
                        help="Target c filename",
                        type=str,
                        default="mylist.c")
    parser.add_argument("-c", "--cont",
                        help="Continue test suit if error occurs",
                        action="store_true")
    args = parser.parse_args()
    return args


if __name__ == '__main__':
    args = get_args()
    compile_cmd = ['gcc', '-o', 'mylist', args.filename, '-m32', '-Wall', '-Werror']
    output_file = 'output.txt'
    # ctest = Test(description="Some beta tests",
    #              compile_cmd=['gcc', '-o', 'mylist', 'mylist.c', '-m32', '-Wall', '-Werror'],
    #              run_cmd=['./mylist', '-i', 'output.txt', '3-50.txt'],
    #              stdout_compar='stdout1',
    #              output_file='output.txt',
    #              output_compar='output1')
    # run_tests(True, ctest)

    t1 = Test(description="Usage 1: Simple test on usage",
              compile_cmd=compile_cmd,
              run_cmd=['./mylist', '-i'],
              stdout_compar='test/stdout1.test')

    t2 = Test(description="Badfile 1: Single bad input file",
              compile_cmd=compile_cmd,
              run_cmd=['./mylist', '-i', output_file, 'badfile'],
              stdout_compar='test/stdout2.test',
              output_file=output_file,
              output_compar='test/out2.test')

    t3 = Test(description="Simple 1: Simple test with 1 input file",
              compile_cmd=compile_cmd,
              run_cmd=['./mylist', '-d', output_file, 'test/100-1000.txt'],
              stdout_compar='test/stdout3.test',
              output_file=output_file,
              output_compar='test/out3.test')

    t4 = Test(description="Simple 2: Simple test with more input files",
              compile_cmd=compile_cmd,
              run_cmd=['./mylist', '-i', output_file, "test/2-10.txt", "test/3-50.txt",
                       "test/10-500.txt"],
              stdout_compar='test/stdout4.test',
              output_file=output_file,
              output_compar='test/out4.test')

    t5 = Test(description="Badfile 2: One bad file together with 2 other valid files",
              compile_cmd=compile_cmd,
              run_cmd=['./mylist', '-i', output_file, 'test/2-10.txt', 'badfile',
                       'test/10-500.txt'],
              stdout_compar='test/stdout5.test',
              output_file=output_file,
              output_compar='test/out5.test')

    run_tests(args.cont, t1, t2, t3, t4, t5)
