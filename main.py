import logging
import random

import security_guard as sg
import random_access_generator as rag

logger = logging.getLogger()
logger.setLevel(logging.DEBUG)
#logger.setLevel(logging.WARNING)

def main():
    print("******************** [Begin secure apartment simulation] ********************")
    
    print("******************** [ End secure apartment simulation ] ********************")

def test():
    num_slams = 500
    nf = 6
    nl = 6
    th = 5
    lrt = 20
    db = True
    p = dict()
    p["num_floors"] = nf
    p["num_lines"] = nl
    p["hot_percentage"] = 5
    
    # PER_APARTMENT, PER_FLOOR, PER_LINE, PER_DOOR
    lm = sg.PER_APARTMENT
    # UNIFORM, TWO_QUADRANTS, FEW_HOT, PSEUDOGAUSSIAN_CENTERED
    ap = rag.PSEUDOGAUSSIAN_CENTERED

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
        #f, l = random.randint(0, nf - 1), random.randint(0, nl - 1)
        logging.debug(f"Slamming #{i}: household[{f}][{l}]")
        if not guard.slam_door(floor = f, line = l):
            break
    guard.print_status()

if __name__ == '__main__':
    test()
    #main()
