import serial.tools.list_ports 
import pyautogui
import time

ports = serial.tools.list_ports.comports()
serialInst = serial.Serial()

portList=[]

for onePort in ports:
    portList.append(str(onePort))
    print(str(onePort))

val = input("Chon cong : COM")

for x in range (0,len(portList)):
    if portList[x].startswith("COM" + str(val)):
        portVar= "COM" + str(val)
        print(portList[x])

serialInst.baudrate = 9600
serialInst.port = portVar
serialInst.open()
width, height = pyautogui.size()
pyautogui.moveTo(960, 540)
xn = 960
yn = 540
f=l=0
start = input("nhan nut:'S' khoi dong: ")
time.sleep(2)
while ((start=="s")|(start =="S")):
    if serialInst.in_waiting:
        packet = serialInst.readline()
        k = packet.decode('utf_8')
        print(k)
        h = int(k)
        print(h)
        if h == 2 :
            yn = yn + 15
            if yn > 1080:
               yn = 1080
        elif h == 1 :
           yn = yn - 15
           if yn==0:
               yn = 0
        elif h == 3 :
           xn = xn + 20
           if xn == 1920:
            xn = 1920
        elif h == 4 :
           xn = xn - 20
           if xn == 0:
            xn = 0
        elif h == 5:
            xn = 960
            yn = 540

        elif h == 6 :
            pyautogui.keyDown('pageup')
            time.sleep(0.2)
            pyautogui.keyUp('pageup')
        elif h == 7 :
            pyautogui.keyDown('pagedown')
            time.sleep(0.2)
            pyautogui.keyUp('pagedown')
        elif h == 8 |h == 9 :
            pyautogui.keyDown(',')
            pyautogui.keyDown('.')
            time.sleep(1)
            pyautogui.keyUp(',')
            pyautogui.keyUp('.')

        elif h == 10 :
            pyautogui.press('f')
            time.sleep(0.2)
            #pyautogui.keyUp('f')
        elif h == 11 :
            pyautogui.hotkey('shift','f')
            #pyautogui.press('f')
            time.sleep(0.2)
            #pyautogui.keyUp(['shift','f'])

        h=0
        pyautogui.moveTo(xn, yn)
