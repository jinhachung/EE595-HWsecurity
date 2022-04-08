import sys

filename = sys.argv[1]

def main():
    print(filename)
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
            d["slams"] = m[7]
            d["resets"] = m[10]
            fail_list.append(d)
        elif l.startswith("The apartment has not been infiltrated,"):
            # not infiltrated
            d = dict()
            m = l.split()
            d["slams"] = m[8]
            d["resets"] = m[11]
            success_list.append(d)
        else:
            print(f"!!!!! Unknown line parsing {filename}: {l}")

    print("success list:")
    print(success_list)
    print("fail list:")
    print(fail_list)

if __name__ == "__main__":
    main()
