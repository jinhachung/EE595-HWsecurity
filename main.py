import logging
import security_guard as sg

logger = logging.getLogger()
logger.setLevel(logging.DEBUG)

def main():
    logger.setLevel(logging.WARNING)
    print("******************** [Begin secure apartment simulation] ********************")
    
    print("******************** [ End secure apartment simulation ] ********************")

def test():
    num_slams = 15
    nf = 5
    nl = 3
    th = 3
    lm = sg.PER_APARTMENT
    lrt = 3
    db = True
    guard = sg.SecurityGuard(num_floors = nf,
                             num_lines = nl,
                             threshold = th,
                             logging_method = lm,
                             log_reset_timer = lrt,
                             debug = db)
    #guard.print_all_logs()
    for i in range(num_slams):
        print(f"Slamming #{i}")
        if not guard.slam_door(floor = 2, line = 1):
            break

if __name__ == '__main__':
    test()
    #main()
