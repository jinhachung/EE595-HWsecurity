import sys

filename = sys.argv[1]

def main():
    print(f"Printing out parsing results for: {filename}")
    f = open(filename, "r")
    rl = f.readlines()
    non_debug_lines = []
    fail_list = []
    success_list = []
    for l in rl:
        if not l.startswith("DEBUG:root"):
            non_debug_lines.append(l.strip())
    # e.g., ignore lines:
    # "A door broke while slamming apartment[1][2]:    log#0014-02x02: affected 6 times
    # e.g., result lines:
    # The apartment has been infiltrated, after surviving 478 slams with 23 log resets
    # The apartment has not been infiltrated, surviving 500 slams with 25 log resets.
    for l in non_debug_lines:
        if l.startswith("A door broke while"):
            # currently this is meaningless
            pass
        elif l.startswith("The apartment has been infiltrated,"):
            # infiltrated
            d = dict()
            m = l.split()
            d["slams"] = int(m[7])
            d["resets"] = int(m[10])
            fail_list.append(d)
        elif l.startswith("The apartment has not been infiltrated,"):
            # not infiltrated
            d = dict()
            m = l.split()
            d["slams"] = int(m[7])
            d["resets"] = int(m[10])
            success_list.append(d)
        else:
            print(f"!!!!! Unknown line parsing {filename}: {l}")

    #print("success list:")
    #print(success_list)
    #print("fail list:")
    #print(fail_list)

    F = len(fail_list)
    S = len(success_list)
    print(f"Success: {S} / Total: {F + S}, success rate: {S / (F + S):.2f}")

    avg_success_reset = get_average(success_list, "resets")
    avg_fail_slam = get_average(fail_list, "slams")
    
    if avg_success_reset:
        print(f"In case of success, there was an average of {avg_success_reset:.2f} resets per run")
    else:
        print("No successful runs")

    if avg_fail_slam:
        print(f"In case of failure, the apartment managed to tank {avg_fail_slam:.2f} slams")
    else:
        print("No failed runs")

def get_average(l, key):
    if len(l) == 0:
        return None
    sum = 0
    for elem in l:
        sum += elem[key]
    return sum / len(l)


if __name__ == "__main__":
    main()
