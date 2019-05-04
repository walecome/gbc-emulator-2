

for i in range(256):
    byte_str = "0x{0:0{1}X}".format(i, 2)
    print("opcode_cb_functions[{}] = [&] () {{ this->OPCodeCB{}(); }};".format(byte_str, byte_str));