import snap7

client = snap7.client.Client()

client.connect("127.0.0.1", 0, 0, 1102)

data = bytearray([ 2469 >> 8, 2469 & 0xFF])

client.db_write(0, 0, data)
