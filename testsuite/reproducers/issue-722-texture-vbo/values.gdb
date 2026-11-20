set pagination off
set confirm off
set print thread-events off
set breakpoint pending on
python
import gdb, os, struct
samples = 0
errors = 0
output = open(os.environ['COIN_UV_DUMP'], 'wb')
class Values(gdb.Breakpoint):
    def __init__(self, dimension):
        super().__init__('SoMultiTextureCoordinateElement::set%d' % dimension)
        self.dimension = dimension
    def stop(self):
        global samples, errors
        frame = gdb.newest_frame()
        count = int(frame.read_var('numCoords'))
        # The cache fixture uses 26*26 points, growing to 26*28 in phase 4.
        if count not in (676,728) or int(frame.read_var('unit')) != 0:
            return False
        pointer = frame.read_var('coords')
        stride = pointer.type.target().sizeof
        if stride != self.dimension * struct.calcsize('=f'):
            errors += 1
            return False
        data = bytes(gdb.selected_inferior().read_memory(int(pointer), count*stride))
        if self.dimension == 4:
            if any(data[i+8:i+16] != struct.pack('=ff',0,1) for i in range(0,len(data),16)):
                errors += 1
            # Keep the original S/T bytes, including signed zero/NaN payloads.
            data = b''.join(data[i:i+8] for i in range(0,len(data),16))
        output.write(struct.pack('=i',count)+data)
        samples += 1
        return False
Values(2)
Values(4)
end
run
python
output.close()
print('VALUES_SAMPLES=%d VALUES_ERRORS=%d' % (samples,errors))
if errors or samples != int(os.environ['COIN_UV_EXPECTED']):
    gdb.execute('quit 1')
end
