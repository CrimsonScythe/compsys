start
# TODO: Add debugging commands here.
p /x 192
p /t 192
p /t 0x80
p /d 0x80
p /x 0b110
p /d 0b110
p UTF8_2B(192) != 0
q
