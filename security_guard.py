import logging

PER_APARTMENT   = "per_apartment"
PER_FLOOR       = "per_floor"
PER_LINE        = "per_line"
PER_DOOR        = "per_door"
LOGGING_METHODS = [PER_APARTMENT, PER_FLOOR, PER_LINE, PER_DOOR]

class Log:
    def __init__(self, logging_method, name):
        # sanity check
        if not logging_method in LOGGING_METHODS:
            logging.debug(f"Log(): logging_method {logging_method} invalid")
            exit(-1)
        self.logging_method = logging_method
        ''' name is in form of "log#n-fxl" (n: log ID)
            where f is floor and l is line
            fx0: log for 100, 200, ..., f00
            2xl: log for 201, 202, 203, ..., 20l
        '''
        self.name = name
        self.count = 0
        #logging.debug(f"Created {self.name}")

    def reset(self):
        self.count = 0

    def get_count(self):
        return self.count

    def add_count(self, delta):
        self.count += delta
        return self.count

    def print_info(self, debug = True):
        if debug:
            logging.debug(f"{self.name}: affected {self.count} times")
        else:
            print(f"{self.name}: affected {self.count} times")

class SecurityGuard:
    def __init__(self,
                 num_floors,
                 num_lines,
                 threshold,
                 logging_method,
                 log_reset_timer,
                 debug = True):
        #sanity check
        if num_floors <= 0:
            logging.debug(f"SecurityGuard(): num_floors {num_floors} invalid")
            exit(-1)
        if num_lines <= 0:
            logging.debug(f"SecurityGuard(): num_lines {num_lines} invalid")
            exit(-1)
        if threshold <= 0:
            logging.debug(f"SecurityGuard(): threshold {num_threshold} invalid")
            exit(-1)
        if not logging_method in LOGGING_METHODS:
            logging.debug(f"SecurityGuard(): logging_method {logging_method} invalid")
            exit(-1)

        # number of floors in apartment
        # number of lines (rooms) in a floor
        self.num_floors = num_floors
        self.num_lines = num_lines
        self.threshold = threshold
        self.logging_method = logging_method
        self.log_reset_timer = log_reset_timer
        self.debug = debug
        self.infiltrated = False
        self.total_slams = 0
        self.total_resets = 0
        self.total_logs = 0
        # set self.logs depending on type of logging method
        self.logs = []
        for f in range(self.num_floors):
            self.logs.append([])
            for l in range(self.num_lines):
                self.logs[f].append(Log(self.logging_method, f"log#{self.total_logs:04}-{f:02}x{l:02}"))
                self.total_logs += 1
    
    def get_sum_floor(self, floor):
        total_count = 0
        for l in range(self.num_lines):
            total_count += self.logs[floor][l].get_count()
        return total_count

    def get_sum_line(self, line):
        total_count = 0
        for f in range(self.num_floors):
            total_count += self.logs[f][line].get_count()
        return total_count

    def get_sum_apartment(self):
        total_count = 0
        for f in range(self.num_floors):
            total_count += self.get_sum_floor(f)
        return total_count

    def print_all_logs(self):
        for f in range(self.num_floors):
            if self.debug:
                logging.debug("")
            else:
                print("")
            for l in range(self.num_lines):
                self.logs[f][l].print_info(self.debug)

    def print_status(self):
        if self.infiltrated:
            print(f"The apartment has been infiltrated, after surviving {self.total_slams} slams with {self.total_resets} log resets")
        else:
            print(f"The apartment has not been infiltrated, surviving {self.total_slams} slams with {self.total_resets} log resets.")

    def slam_door(self, floor, line):
        if (floor < 0) or (floor >= self.num_floors) or (line < 0) or (line >= self.num_lines):
            logging.debug(f"SecurityGuard.slam_door(): floor {floor}, line {line} invalid")
            exit(-1)
        floorlist = []
        if (floor - 1 > 0) and (floor - 1 < self.num_floors):
            floorlist.append(floor - 1)
        if (floor + 1 > 0) and (floor + 1 < self.num_floors):
            floorlist.append(floor + 1)

        for f in floorlist:
            # check if any of the doors have been opened
            if self.logs[f][line].add_count(1) > self.threshold:
                print(f"A door broke while slamming apartment[{floor}][{line}]:", end = "\t")
                self.logs[f][line].print_info(debug = False)
                self.infiltrated = True
                #self.print_status()
                return False
            # check if guard should reset any of the logs (screw them nails)
            self.check_for_reset(f, line)
            self.total_slams += 1
            return True

    def check_for_reset(self, floor, line):
        if self.logging_method == PER_APARTMENT:
            if self.get_sum_apartment() >= self.log_reset_timer:
                for f in range(self.num_floors):
                    for l in range(self.num_lines):
                        self.logs[f][l].reset()
                logging.debug(f"Reset #{self.total_resets} @ logs [*][*]")
                self.total_resets += 1
        elif self.logging_method == PER_FLOOR:
            if self.get_sum_floor(floor) >= self.log_reset_timer:
                for l in range(self.num_lines):
                    self.logs[floor][l].reset()
                logging.debug(f"Reset #{self.total_resets} @ logs [{floor}][*]")
                self.total_resets += 1
        elif self.logging_method == PER_LINE:
            if self.get_sum_line(line) >= self.log_reset_timer:
                for f in range(self.num_floors):
                    self.logs[f][line].reset()
                logging.debug(f"Reset #{self.total_resets} @ logs [*][{line}]")
                self.total_resets += 1
        else:#logging_method == PER_DOOR
            if self.logs[floor][line].get_count() >= self.log_reset_timer:
                # there's really no reason for the reset timer to be a different value to the threshold
                # but let's just keep it this way so that the code is kept simple, nimble, and humble
                self.logs[floor][line].reset()
                logging.debug(f"Reset #{self.total_resets} @ log [{floor}][{line}]")
                self.total_resets += 1
        
