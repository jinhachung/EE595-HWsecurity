import logging
import security_guard as sg

logger = logging.getLogger()
logger.setLevel(logging.DEBUG)

def main():
    logger.setLevel(logging.WARNING)
    print("******************** [Begin secure apartment simulation] ********************")
    
    print("******************** [ End secure apartment simulation ] ********************")

def test():
    guard = sg.SecurityGuard(num_floors = 5, num_lines = 3, logging_method = sg.PER_APARTMENT)
    guard.print_all_logs(False)

if __name__ == '__main__':
    test()
    #main()
