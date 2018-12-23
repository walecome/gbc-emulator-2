"""
Generates C++ code for instrution instantiation
"""

OUTPUT_FILE = "case_out.txt"

OPCODE_MAX = 0xFF

opcode_lengths = {
    0: [],
    1: [],
    2: [],
    3: [],
    4: []
}


def write_cases(file, opcodes, length):

    for opcode in opcodes:
        case = "case {}:".format(opcode)

        file.write(case)
        file.write("\n")

    file.write(" " * 4 + "length = {};".format(length))
    file.write("\n")
    file.write(" " * 4 + "break;")
    file.write("\n\n")


def main(file):

    for opcode in range(OPCODE_MAX + 1):
        byte_str = "0x{0:0{1}X}".format(opcode, 2)
        length = int(input("Length ({}): ".format(byte_str)))
        opcode_lengths[length].append(byte_str)

    for key in opcode_lengths.keys():
        write_cases(file, opcode_lengths[key], key)


try:
    if __name__ == "__main__":
        clear_file = input("Clear output file? (yes) ")

        if clear_file == "yes":
            # Clear output file
            with open(OUTPUT_FILE, "w"):
                pass

        with open(OUTPUT_FILE, 'a') as out:
            main(out)

except KeyboardInterrupt:
    print("Done!")
