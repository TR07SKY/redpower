import binascii
import redpower

header = "02000000cef5b89f4ebf38a73cd440439edaca4afa7c7ad82be281473b9e5ee5f70900005df17cda32f206b3f27d9c3a262f4709a08164b05846532f0afd4a456bd550c59f5d5d54ffff3f1e0f000000" #block header
hashbin = binascii.unhexlify(header) #b'' of the header

yesposthash = binascii.hexlify(redpower.hash(hashbin)[::-1]) #redpower hash
print(yesposthash)
