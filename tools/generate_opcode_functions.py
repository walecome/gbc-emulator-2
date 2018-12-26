"""
Generates C++ code for instrution instantiation
"""

CB_PREFIX = True


OUTPUT_FILE_DEF = "def_out.txt"
OUTPUT_FILE_IMPL = "impl_out.txt"

if CB_PREFIX:
    OUTPUT_FILE_DEF = "cb_" + OUTPUT_FILE_DEF
    OUTPUT_FILE_IMPL = "cb_" + OUTPUT_FILE_IMPL

OPCODE_MAX = 0xFF

OPERAND_ORDER = ["B", "C", "D", "E", "H", "L", "(HL)", "A"]
current_bit = 0
current_operand = 0

current_func = ""


def write_def(file, opcode_str):

    if CB_PREFIX:
        definition_string = "void OPCodeCB{}();".format(opcode_str)
    else:
        definition_string = "void OPCode{}();".format(opcode_str)

    file.write(definition_string)
    file.write("\n")


def write_impl(file, opcode_str):

    if CB_PREFIX:
        definition_string = "void Processor::OPCodeCB{}()".format(opcode_str)
    else:
        definition_string = "void Processor::OPCode{}()".format(opcode_str)

    global current_func

    if opcode_str == "0x80":
        current_func = "RES"
    elif opcode_str == "0xC0":
        current_func = "SET"

    file.write(definition_string)
    file.write("\n")
    file.write("{")
    file.write("\n")

    if current_func != "":
        global current_operand
        global current_bit
        func_str = "{} {}, {}".format(
            current_func, current_bit, OPERAND_ORDER[current_operand])

        comment_str = "\t// {}\n".format(func_str)

        file.write(comment_str)

        if current_func == "RES":
            exe_str = "\tresetBit({}, {});\n".format(
                current_bit, OPERAND_ORDER[current_operand])
            file.write(exe_str)

        elif current_func == "SET":
            exe_str = "\tsetBit({}, {});\n".format(
                current_bit, OPERAND_ORDER[current_operand])
            file.write(exe_str)

        if current_operand == len(OPERAND_ORDER) - 1:
            current_bit = (current_bit + 1) % 8

        current_operand = (current_operand + 1) % len(OPERAND_ORDER)

    file.write("}")
    file.write("\n\n")


def main(file, ver):

    for opcode in range(OPCODE_MAX + 1):
        byte_str = "0x{0:0{1}X}".format(opcode, 2)
        if ver == 0:
            write_def(file, byte_str)

        elif ver == 1:
            write_impl(file, byte_str)


try:
    if __name__ == "__main__":
        clear_file = input("Clear output files? (yes) ")

        if clear_file == "yes":
            # Clear output file
            with open(OUTPUT_FILE_DEF, "w"):
                pass

            with open(OUTPUT_FILE_IMPL, "w"):
                pass

        with open(OUTPUT_FILE_DEF, 'a') as out:
            main(out, 0)

        with open(OUTPUT_FILE_IMPL, 'a') as out:
            main(out, 1)


except KeyboardInterrupt:
    print("Done!")
