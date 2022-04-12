import logging
import random
import numpy

UNIFORM                 = "uniform"
TWO_QUADRANTS           = "two_quadrants"
FEW_HOT                 = "few_hot"
PSEUDOGAUSSIAN_CENTERED = "pseudogaussian_centered"
ACCESS_PATTERNS = [UNIFORM, TWO_QUADRANTS, FEW_HOT, PSEUDOGAUSSIAN_CENTERED]
'''
1) UNIFORM:                 chance at choosing floor and line are uniformly distributed
                            requires: num_floors, num_lines

2) TWO_QUADRANTS:           dividing the whole apartment into four quadrants,
                            only access two diagonally connected quadrants (uniform inside)
                            requires: num_floors, num_lines

3) FEW_HOT:                 choose A% active users who will take up (100 - A)% of the accesses
                            within the active/nonactive groups, distribution is uniform
                            A = 5, 10, 15?
                            requires: num_floors, num_lines, hot_percentage

4) PSEUDOGAUSSIAN_CENTERED: chance at choosing floor and line are gaussian,
                            with mean at the center (T/2, where T = F or L)
                            and std. dev so that P[0 <= x <= T] ~ 0.99
                            because P[x <= Z = 2.575] = 0.995, we choose
                            std. dev = x / 5.15 --> N(2/x, x/5.15)
                            requires: num_floors, num_lines
5) SEQUENTIAL:              accesses occur sequentially (floor, line):
                            (0, 0), (0, 1), (0, 2), ... (0, L - 1)
                            (1, 0), (1, 1), (1, 2), ... (1, L - 1)
'''

# THE ALL NEW RAG, stop ragging!
class RandomAccessGenerator:
    def __init__(self, access_pattern, params):
        #sanity check
        if not access_pattern in ACCESS_PATTERNS:
            logging.debug(f"RandomAccessGenerator(): access_pattern {access_pattern} invalid")
            exit(-1)
        self.access_pattern = access_pattern
        self.slams_generated = 0
        self.params = params
        if self.access_pattern == FEW_HOT:
            # choose the several hot households in advance
            self.params["hot_households"] = []
            self.params["cold_households"] = []
            for f in range(self.params["num_floors"]):
                for l in range(self.params["num_lines"]):
                    household = dict()
                    household["floor"] = f
                    household["line"] = l
                    if random.randint(1, 100) <= self.params["hot_percentage"]:
                        # inssa
                        self.params["hot_households"].append(household)
                    else:
                        # assa
                        self.params["cold_households"].append(household)
                # avoid hot households from being completely empty
                # cold households can also be completely empty,
                # but this is highly unlikely so just keep my fingers crossed
                if len(self.params["hot_households"]) == 0:
                    household = dict()
                    household["floor"] = 0
                    household["line"] = 0
                    self.params["hot_households"].append(household)
                    self.params["cold_households"].pop(0)
        if self.access_pattern == SEQUENTIAL:
            # initialize "next access"
            self.next_access = dict()
            self.next_access["floor"] = 0
            self.next_access["line"] = 0

    def get_pseudogaussian_centered_location(self):
        household = dict()
        
        x = self.params["num_floors"]
        f = max(0, min(x - 1, int(numpy.random.normal(loc = x / 2, scale = x / 5.15))))
        #f = int(random.normal(loc = x / 2, scale = x / 5.15))
        #f = max(0, f)
        #f = min(f, x - 1)
        household["floor"] = f

        x = self.params["num_lines"]
        l = max(0, min(x - 1, int(numpy.random.normal(loc = x / 2, scale = x / 5.15))))
        #l = int(random.normal(loc = x / 2, scale = x / 5.15))
        #l = max(0, l)
        #l = min(l, x - 1)
        household["line"] = l
        return household

    def generate_random_access(self):
        location = dict()
        
        if self.access_pattern == UNIFORM:
            # random.randint(a, b) returns value in CLOSED range [a, b]
            location["floor"] = random.randint(0, self.params["num_floors"] - 1)
            location["line"] = random.randint(0, self.params["num_lines"] - 1)
        elif self.access_pattern == TWO_QUADRANTS:
            # choose floor first uniformly
            floor = random.randint(0, self.params["num_floors"] - 1)
            # this is buggy if num_lines is 1, but what kind of a weird apartment is that?
            if floor < int(self.params["num_floors"] / 2):
                line = random.randint(int(self.params["num_lines"] / 2), self.params["num_lines"] - 1)
            else:
                line = random.randint(0, int(self.params["num_lines"] / 2) - 1)
            location["floor"] = floor
            location["line"] = line
        elif self.access_pattern == FEW_HOT:
            if random.randint(1, 100) > self.params["hot_percentage"]:
                # accessing inssa, typical inssa
                household = self.params["hot_households"][random.randint(0, len(self.params["hot_households"]) - 1)]
            else:
                # accessing assa, wow this rarely happens!
                household = self.params["cold_households"][random.randint(0, len(self.params["cold_households"]) - 1)]
            location["floor"] = household["floor"]
            location["line"] = household["line"]
        elif self.access_pattern == PSEUDOGAUSSIAN_CENTERED:
            household = self.get_pseudogaussian_centered_location()
            location["floor"] = household["floor"]
            location["line"] = household["line"]
        else:#self.access_pattern == SEQUENTIAL:
            # generate access for "next_access" state
            location["floor"] = self.next_access["floor"]
            location["line"] = self.next_access["line"]
            # set new "next_access" state
            self.next_access["line"] += 1
            if self.next_access["line"] >= self.params["num_lines"]:
                # wrap around
                self.next_access["line"] = 0
                self.next_access["floor"] += 1
                if self.next_access["floor"] >= self.params["num_floors"]:
                    # wrap around
                    self.next_access["floor"] = 0
        return location



