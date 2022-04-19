UINT_25_MAX = 33554431

def generate_div_tests(lib) -> list:

    tests = []

    for i in range(0, 10):
        for j in range(1, 100):
            tests.append(
                (f"div_{i}_{j}_test", [
                    lib.load_value(1, i),
                    lib.load_value(2, j),
                    lib.div(3, 1, 2),
                    lib.output(3),
                    lib.load_value(3, ord('\n')),
                    lib.output(3),
                    lib.halt(),
                ], None)
            )
    
    return tests

def generate_mul_tests(lib) -> list:

    tests = []
    for k in range(0, 1):
        for i in range(0, 10000000, 5000000):
            for j in range(10000000, 20000000, 5000000):
                tests.append(
                    (f"mul_{i}_{j}_test", [
                        lib.load_value(k, i),
                        lib.load_value(2, j),
                        lib.mul(3, k, 2),
                        lib.load_value(4, 255),
                        lib.nand(3, 3, 4),
                        lib.nand(4, 3, 3),
                        lib.output(4),
                        lib.load_value(3, ord('\n')),
                        lib.output(3),
                        lib.halt(),
                    ], None)
                )
    
    return tests

def generate_add_tests(lib) -> list:

    tests = []

    for i in range(0, 10000000, 5000000):
        for j in range(10000000, 20000000, 5000000):
            tests.append(
                (f"add_{i}_{j}_test", [
                    lib.load_value(1, i),
                    lib.load_value(2, j),
                    lib.add(3, 1, 2),
                    lib.load_value(4, 255),
                    lib.nand(3, 3, 4),
                    lib.nand(4, 3, 3),
                    lib.output(4),
                    lib.load_value(3, ord('\n')),
                    lib.output(3),
                    lib.halt(),
                ], None)
            )
    
    return tests

def generate_and_tests(lib) -> list:

    tests = []

    for i in range(0, UINT_25_MAX, 8000000):
        tests.append(
            (f"and_{i}_255_test", [
                lib.load_value(1, 255),
                lib.load_value(2, i),
                lib.nand(3, 1, 2),
                lib.nand(4, 3, 3),
                lib.output(4),
                lib.load_value(3, ord('\n')),
                lib.output(3),
                lib.halt(),
            ], None)
        )
    
    return tests

def generate_or_tests(lib) -> list:

    tests = []

    for i in range(0, 256, 64):
        for j in range(0, 10000000, 10000000):
            tests.append(
                (f"or_{i}_{j}_test", [
                    lib.load_value(1, i),
                    lib.load_value(2, j),
                    lib.nand(3, 1, 2),
                    lib.nand(4, 3, 3),
                    lib.output(4),
                    lib.load_value(3, ord('\n')),
                    lib.output(3),
                    lib.halt(),
                ], None)
            )
    
    return tests

def generate_cmov_tests(lib) -> list:

    tests = []

    for i in range(0, 256, 64):
        for k in range(0, 2):
            tests.append(
                (f"cmov_{i}_{k}test", [
                    lib.load_value(1, i),
                    lib.load_value(4, 0),
                    lib.load_value(3, k),
                    lib.conditional_move(1, 4, 3),
                    lib.output(4),
                    lib.load_value(3, ord('\n')),
                    lib.output(3),
                    lib.halt(),
                ], None)
            )
    
    return tests

def generate_seg_map_tests(lib) -> list:
    
    map_1000 = [lib.map_segment(0, 1)] * 100000

    map_1000.insert(0, lib.load_value(1, 1000))
    map_1000.append(lib.halt())

    instrs = [lib.load_value(0, 10000), lib.map_segment(5, 0)]
    
    for i in range(10000):
        instrs.extend([
            lib.load_value(1, i),
            lib.segmented_store(5, 1, 1),
        ])

    instrs.append(lib.load_value(2, 255))

    for i in range(10000):
        instrs.extend([
            lib.load_value(1, i),
            lib.segmented_load(6, 5, 1),
            lib.nand(3, 6, 2),
            lib.nand(4, 3, 3),
            lib.output(4),
        ])

    instrs.append(lib.halt())

    # pprint(instrs)

    return [("seg_map_stress_test", map_1000, None), 
            ("seg_big_map_load_test", instrs, None)]

def generate_seg_unmap_tests(lib) -> list:
    
    map_1000 = [
        lib.map_segment(1, 0),
        # lib.load_value(5, ord('0')),
        # lib.add(6, 5, 1),
        # lib.output(6),
        # lib.load_value(6, ord('\n')),
        # lib.output(6),
        lib.unmap_segment(1),
    ] * 1000000

    map_1000.insert(0, lib.load_value(1, 100))
    map_1000.append(lib.halt())

    return [("seg_unmap_stress_test", map_1000, None)]

def generate_load_seg_tests(lib) -> list:

    tests = []
    for i in range(0, 256, 64):
        tests.append(
            (f"load_seg_{i}_test", [
                lib.load_value(0, i),
                lib.load_value(2, 1),
                lib.load_value(3, 0),
                lib.map_segment(1, 2),
                lib.segmented_store(1, 3, 0),
                lib.segmented_load(6, 1, 3),
                lib.output(6),
                lib.load_value(6, ord('\n')),
                lib.output(6),
                lib.halt(),
            ], None)
        )
    
    return tests

def generate_combined_tests(lib) -> list:
    tests = []
    for i in range(0, 1):
        j = (i + 1) % 8
        k = (i + 2) % 8
        l = (i + 3) % 8
        for n in range(0, 128, 64):
            tests.append(
                (f"combined_{i}_{n}_test", [
                    lib.input(j),
                    lib.load_value(k, 139),
                    lib.add(l, j, k),
                    lib.load_value(k, 2),
                    lib.mul(j, l, k),
                    lib.load_value(l, 3),
                    lib.div(k, j, l),
                    lib.load_value(j, 255),
                    lib.nand(l, j, k),
                    lib.nand(k, l, l),
                    lib.output(k),
                    lib.load_value(6, ord('\n')),
                    lib.output(6),
                    lib.halt(),
                ], chr(n))
            )
    return tests
