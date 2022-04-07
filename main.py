import logging
import security_guard as sg

logger = logging.getLogger()
logger.setLevel(logging.DEBUG)

def main():
    logger.setLevel(logging.WARNING)
    print("******************** [Begin secure apartment simulation] ********************")
    
    print("******************** [ End secure apartment simulation ] ********************")

def test():
    nf = 5
    nl = 3
    th = 10
    lm = sg.PER_APARTMENT
    db = True
    guard = sg.SecurityGuard(num_floors = nf,
                             num_lines = nl,
                             threshold = th,
                             logging_method = lm,
                             debug = db)
    #guard.print_all_logs()
    for i in range(th + 1):
        print(f"Slamming #{i}")
        if not guard.slam_door(floor = 2, line = 1):
            break

if __name__ == '__main__':
    test()
    #main()
