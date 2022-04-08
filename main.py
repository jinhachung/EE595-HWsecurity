import logging
import random
import sys

import security_guard as sg
import random_access_generator as rag

logger = logging.getLogger()
logger.setLevel(logging.DEBUG)
#logger.setLevel(logging.WARNING)

def main():
    num_slams = 100000
    nf = 20
    nl = 10
    th = 10
    lrt = 10
    tradeoff_var = 4 / 5
    db = True
    p = dict()
    p["num_floors"] = nf
    p["num_lines"] = nl
    p["hot_percentage"] = 10
    
    # PER_APARTMENT, PER_FLOOR, PER_LINE, PER_DOOR
    if sys.argv[1] == "per_apartment":
        lm = sg.PER_APARTMENT
        th = int(th * nf * nl * tradeoff_var * tradeoff_var)
    elif sys.argv[1] == "per_floor":
        lm = sg.PER_FLOOR
        th = int(th * nf * tradeoff_var)
    elif sys.argv[1] == "per_line":
        lm = sg.PER_LINE
        th = int(th * nl * tradeoff_var)
    elif sys.argv[1] == "per_door":
        lm = sg.PER_DOOR
        th = int(th)
    else:
        print(f"!!!!! Wrong logging method argument: {sys.argv[1]}")
        exit(-1)

    # UNIFORM, TWO_QUADRANTS, FEW_HOT, PSEUDOGAUSSIAN_CENTERED
    if sys.argv[2] == "uniform":
        ap = rag.UNIFORM
    elif sys.argv[2] == "two_quadrants":
        ap = rag.TWO_QUADRANTS
    elif sys.argv[2] == "few_hot":
        ap = rag.FEW_HOT
    elif sys.argv[2] == "gaussian":
        ap = rag.PSEUDOGAUSSIAN_CENTERED
    else:
        print(f"!!!!! Wrong access pattern argument: {sys.argv[2]}")
        exit(-1)

    guard = sg.SecurityGuard(num_floors = nf,
                             num_lines = nl,
                             threshold = th,
                             logging_method = lm,
                             log_reset_timer = lrt,
                             debug = db)
    gen = rag.RandomAccessGenerator(access_pattern = ap,
                                    params = p)
    #guard.print_all_logs()
    for i in range(num_slams):
        location = gen.generate_random_access()
        f, l = location["floor"], location["line"]
        logging.debug(f"Slamming #{i}: household[{f}][{l}]")
        if not guard.slam_door(floor = f, line = l):
            break
    guard.print_status()

if __name__ == '__main__':
    main()
