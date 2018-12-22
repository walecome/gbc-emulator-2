"""
Generates C++ code for instrution instantiation
"""

OUTPUT_FILE = "out.txt"


def main(file):

    opcode = input("OPcode: ")
    length = input("Length: ")
    name = input("Name: ")

    class_instantiation = "{{{}, {}, {}}}".format(opcode, length, name.upper())

    cpp_class = "Instruction {} = {};".format(
        name.lower(), class_instantiation)

    print("Writing opcode class {} to file".format(class_instantiation))
    file.write(cpp_class + "\n")


try:
    if __name__ == "__main__":
        # Clear output file
        with open(OUTPUT_FILE, "w"):
            pass

        with open(OUTPUT_FILE, 'a') as out:
            while True:
                main(out)
except KeyboardInterrupt:
    print("Done!")
