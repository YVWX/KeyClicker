import serial
import time


class ClickerSerial():
    def __init__(self, port):
        self._valid = True
        
        try:
            self._ser = serial.Serial(port, baudrate = 9600, dsrdtr = True)
        except:
            print('Device can not be found or can not be configured')

    def connect(self, port):
        self._ser = serial.Serial(port, baudrate = 9600, dsrdtr = True)

        self._valid = True
        
    def info(self):
        if not self._valid:
            print("ClickerSerial invalid")
            return
        
        lines = self.silent_info()
        print(lines)

    def silent_info(self):
        if not self._valid:
            print("ClickerSerial invalid")
            return
        
        command = 'i'
        self._ser.write(command.encode('UTF-8'))
        lines = ''
        lines += self._ser.readline().decode('UTF-8')
        lines += self._ser.readline().decode('UTF-8')

        return lines

    def set_threshold(self, value):
        if not self._valid:
            print("ClickerSerial invalid")
            return
        
        command = 'st' + str(value)
        self._ser.write(command.encode('UTF-8'))

    def set_cycle_time(self, value):
        if not self._valid:
            print("ClickerSerial invalid")
            return
        
        command = 'sc' + str(value)
        self._ser.write(command.encode('UTF-8'))

    def press(self, index):
        if not self._valid:
            print("ClickerSerial invalid")
            return

        command = 'p' + str(index)
        self._ser.write(command.encode('UTF-8'))

    def press_seq(self, index_seq):
        if not self._valid:
            print("ClickerSerial invalid")
            return

        for index in index_seq:
            self.press(index)

    def close(self):
        if not self._valid:
            print("ClickerSerial invalid")
            return
        
        self._ser.close()
        self._valid = False

if __name__ == '__main__':
    clicker = ClickerSerial('COM3')

    key_seq = ['var', 'r', 'd', 'ok', 'ok', 'var', 'd', 'd', 'ok', 'ok', 'var', 'ok', 'ok', 'var', 'r', 'd', 'ok', 'ok', 'var', 'r', 'ok', 'ok', 'var', 'd', 'd', 'ok', 'ok', 'var', 'd', 'd', 'ok', 'ok', 'var', 'r', 'd', 'd', 'ok', 'ok']
    key_index = {
        'var': 0,
        'r': 3,
        'd': 1,
        'ok': 2
        }
    index_seq = []
    for key in key_seq:
        index_seq.append(key_index[key])

    clicker.set_threshold(96)

    for index in index_seq:
        clicker.press(index)
        time.sleep(1)

    clicker.close()
