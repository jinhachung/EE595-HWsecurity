import logging

PER_APARTMENT = "per_apartment"
PER_FLOOR = "per_floor"
PER_LINE = "per_line"
PER_DOOR = "per_door"
LOGGING_METHODS = [PER_APARTMENT, PER_FLOOR, PER_LINE, PER_DOOR]

class Log:
    def __init__(self, logging_method, name):
        # sanity check
        if not logging_method in LOGGING_METHODS:
            exit(-1)
        self.logging_method = logging_method
        ''' name is in form of "log#n-fxl" (n: log ID)
            where f is floor and l is line
            fx0: log for 100, 200, ..., f00
            2xl: log for 201, 202, 203, ..., 20l
        '''
        self.name = name
        self.count = 0
        logging.debug(f"Created {self.name}")

    def reset(self):
        self.count = 0

    def get_count(self):
        return self.count

    def add_count(self, delta):
        self.count += delta

    def print_info(self, debug = True):
        if debug:
            logging.debug(f"{self.name}: affected {self.count} times")
        else:
            print(f"{self.name}: affected {self.count} times")

class SecurityGuard:
    def __init__(self, num_floors, num_lines, logging_method):
        # sanity check
        if num_floors <= 0:
            exit(-1)
        if num_lines <= 0:
            exit(-1)
        if not logging_method in LOGGING_METHODS:
            exit(-1)

        # number of floors in apartment
        # number of lines (rooms) in a floor
        self.num_floors = num_floors
        self.num_lines = num_lines
        self.total_cost = 0
        self.total_logs = 0
        self.logging_method = logging_method
        '''
        if self.logging_method == PER_APARTMENT:
            pass
        elif self.logging_method == PER_FLOOR:
            pass
        elif self.logging_method == PER_LINE:
            pass
        else:#logging_method == PER_DOOR
            pass
       '''
        # set self.logs depending on type of logging method
        self.logs = []
        for f in range(self.num_floors):
            self.logs.append([])
            for l in range(self.num_lines):
                self.logs[f].append(Log(self.logging_method, f"log#{self.total_logs:04}-{f:02}x{l:02}"))
                self.total_logs += 1
    
    def get_sum_floor(floor):
        total_count = 0
        for l in range(self.num_lines):
            total_count += self.logs[f][l].get_count()
        return total_count

    def get_sum_line(line):
        total_count = 0
        for f in range(self.num_floors):
            total_count += self.logs[f][l].get_count()
        return total_count

    def print_all_logs(self, debug = True):
        for f in range(self.num_floors):
            if debug:
                logging.debug("")
            else:
                print("")
            for l in range(self.num_lines):
                self.logs[f][l].print_info(debug)

