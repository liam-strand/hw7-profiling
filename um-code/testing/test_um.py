import os.path
import subprocess as sp
import sys
from ctypes import *
import multiprocessing as mp
from tqdm import tqdm
from pprint import pprint

from tests import *

TESTDIR = "testing/output/"

def main():
    ul = CDLL("./testing/umlab.so")
    prepare_library(ul)

    tests = [
        ("add_test", [
            ul.load_value(1, ord('0')),
            ul.load_value(2, 6),
            ul.add(3, 1, 2),
            ul.output(3),
            ul.load_value(3, ord('\n')),
            ul.output(3),
            ul.halt(),
        ], None),
        ("halt_test", [
            ul.halt()
        ], None),
        ("input_test", [
            ul.input(1),
            ul.load_value(2, 1),
            ul.add(3, 1, 2),
            ul.output(3),
            ul.halt(),
        ], "5"),
        ("prog_load_test", [
            ul.load_value(1, 5),
            ul.load_program(0, 1),
            ul.load_value(5, ord('F')),
            ul.output(5),
            ul.halt(),
            ul.load_value(1, 3),
            ul.map_segment(0, 1),
            ul.load_value(4, 2),
            ul.load_value(5, 3),
            ul.segmented_load(6, 2, 4),
            ul.segmented_load(7, 2, 5),
            ul.load_value(1, 1),
            ul.add(5, 5, 1),
            ul.segmented_load(2, 2, 5),
            ul.segmented_store(0, 3, 6),
            ul.segmented_store(0, 1, 7),
            ul.segmented_store(0, 4, 2),
            ul.load_program(0, 3),
        ], None)
    ]

    run_tests(tests)

    # run_tests(generate_div_tests(ul), "div")
    run_tests(generate_add_tests(ul), "add")
    run_tests(generate_mul_tests(ul), "mul")
    run_tests(generate_and_tests(ul), "and")
    run_tests(generate_or_tests(ul), "or")
    run_tests(generate_cmov_tests(ul), "cmov")
    run_tests(generate_seg_map_tests(ul), "seq_map")
    run_tests(generate_seg_unmap_tests(ul), "seq_unmap")
    run_tests(generate_load_seg_tests(ul), "load_seq")
    run_tests(generate_combined_tests(ul), "combined")

def run_tests(tests: list, name=None) -> None:
    # with mp.Pool(10) as pool:
    #     list(tqdm(pool.imap(apply_test, tests), total=len(tests), desc=name))
    for test in tqdm(tests, desc=name):
       apply_test(test)
    

def apply_test(zipped_test) -> None:
    test_name, instrs, uinput = zipped_test
    print_test(instrs, test_name)
        
    if uinput:
        write_input(test_name, uinput)
        write_output(test_name, uinput)
    else:
        write_output(test_name)

    if uinput:
        run_test(test_name, infile=f"{test_name}.0")
    else:
        run_test(test_name)

def run_test(test_name, infile=None):
    # print("running test", file=sys.stderr)
    if infile:
        sp.run(f"cat {TESTDIR}{infile} | ./um {TESTDIR}{test_name}.um > {TESTDIR}{test_name}.out", shell=True)
    else:
        sp.run(f"./um {TESTDIR}{test_name}.um > {TESTDIR}{test_name}.out", shell=True)
    
    sp.run(f"diff -aqN {TESTDIR}{test_name}.1 {TESTDIR}{test_name}.out", shell=True)

def print_test(instructions: list, filename: str) -> None:
    # print("printing test", file=sys.stderr)
    with open(f"{TESTDIR}{filename}.um", "wb") as f:
        for inst in instructions:
            f.write(inst.to_bytes(4, "big"))

def write_input(name: str, provided_input: str) -> None:
    # print("printing input", file=sys.stderr)
    with open(f"{TESTDIR}{name}.0", "w", encoding="ascii") as f:
        f.write(provided_input)

def write_output(name: str, user_input=None) -> None:
    # print("printing output", file=sys.stderr)
    if user_input:
        sp.run(f"cat {TESTDIR}{name}.0 | um {TESTDIR}{name}.um > {TESTDIR}{name}.1", shell=True)
    else:
        sp.run(f"um {TESTDIR}{name}.um > {TESTDIR}{name}.1", shell=True)

def prepare_library(lib):
    lib.conditional_move.argtypes = [c_uint, c_uint, c_uint]
    lib.segmented_load.argtypes   = [c_uint, c_uint, c_uint]
    lib.segmented_store.argtypes  = [c_uint, c_uint, c_uint]
    lib.mul.argtypes              = [c_uint, c_uint, c_uint]
    lib.div.argtypes              = [c_uint, c_uint, c_uint]
    lib.add.argtypes              = [c_uint, c_uint, c_uint]
    lib.nand.argtypes             = [c_uint, c_uint, c_uint]
    lib.halt.argtypes             = []
    lib.map_segment.argtypes      = [c_uint, c_uint]
    lib.unmap_segment.argtypes    = [c_uint]
    lib.output.argtypes           = [c_uint]
    lib.input.argtypes            = [c_uint]
    lib.load_program.argtypes     = [c_uint, c_uint]
    lib.load_value.argtypes       = [c_uint, c_uint]

    lib.conditional_move.restype = c_uint
    lib.segmented_load.restype   = c_uint
    lib.segmented_store.restype  = c_uint
    lib.mul.restype              = c_uint
    lib.div.restype              = c_uint
    lib.add.restype              = c_uint
    lib.nand.restype             = c_uint
    lib.halt.restype             = c_uint
    lib.map_segment.restype      = c_uint
    lib.unmap_segment.restype    = c_uint
    lib.output.restype           = c_uint
    lib.input.restype            = c_uint
    lib.load_program.restype     = c_uint
    lib.load_value.restype       = c_uint


if __name__ == "__main__":
    main()
