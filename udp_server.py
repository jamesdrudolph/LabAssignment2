import os
import time
import socket

# HPS/FPGA Memory Addresses
HW_REGS_BASE = 0xFC000000
HW_REGS_SPAN = 0x04000000
HW_REGS_MASK = HW_REGS_SPAN - 1
ALT_LWFPGASLVS_OFST = 0xFF200000
SEGS = 0x140
SEGS2 = 0x150

# UDP Server Info
UDP_IP = "192.168.0.100"
UDP_PORT = 420

# Convert 1 digit into 7-seg bit pattern
numtable = {
    "0": 0x40,
    "1": 0x79,
    "2": 0x24,
    "3": 0x30,
    "4": 0x19,
    "5": 0x12,
    "6": 0x02,
    "7": 0x78,
    "8": 0x00,
    "9": 0x10,
    " ": 0x7F,
    "-": 0x3F
}

# Convert array of 3 numtable into mmap writable string
def cons(bins):
    s = ""
    f = 0
    for i in range(0, 3):
        f = f | bins[i] << (i * 7)

    for i in range(0, 3):
        t = f >> (i * 8)
        s = s + chr(t & 0xFF)

    return s

# Open memory as file descriptor
fd = os.open("/dev/mem", os.O_RDWR | os.O_SYNC)

# Check if opened sucessfully
if fd == -1:
    print "error opening /dev/mem!"
    exit()

# Map /dev/mem to writable block of memory
vb = mmap.mmap(
    fd,
    HW_REGS_SPAN,
    flags=mmap.MAP_SHARED,
    offset=HW_REGS_BASE
)

# 7-seg base address
pos = (ALT_LWFPGASLVS_OFST + SEGS) & HW_REGS_MASK
pos2 = (ALT_LWFPGASLVS_OFST + SEGS2) & HW_REGS_MASK

def display(a, b, c, position):
    vb.seek(position)
        numtable[str(c)],
        numtable[str(b)],
        numtable[str(a)]
    ])
    vb.write(s)

# Sanity check
for i in range(0, 10):
    display(i, i, i, pos)
    display(i, i, i, pos2)
    time.sleep(0.1)

# Reset
display(" ", "-", "-", pos)
display("-", "-", " ", pos2)

# UDP Setup
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

# UDP Server Listener
print "READY"
while True:
    # Receive 3 byte string (3 digit number)
    data, addr = sock.recvfrom(3)
    if data == "FF":
        # Cleanup
        display(" ", "-", "-", pos)
        display("-", "-", " ", pos2)
        vb.close()
        os.close(fd)
        break
    else:
        if data[0] == "B":
            display(" ", data[1], data[2], pos)
        elif data[0] == "C":
            display(data[1], data[2], " ", pos2)