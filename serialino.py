from serial import Serial
import json
import requests

arduino = Serial()
arduino.baudrate = 9600
arduino.port = 'COM3'
arduino.open()

def getdata():
    count = 0
    data = ''
    while count < 5:
        data += str(arduino.readline())
        count += 1

    print(data)
    listof=data.split("\\r\\n")
    print(listof)
    realdata= listof[2].split(';')
    print(realdata)
    return realdata


if __name__ == "__main__":
    ipadress = 'http://neofinder.tech:5000/api/iot_data'
    iptoken = 'http://neofinder.tech:5000/api/iot_auth' 
    listinfo = getdata()
    iddevice=listinfo[0]
    lat=listinfo[1]
    lon=listinfo[2]
    alt = listinfo[3]
    topost = {'id': '0',
       'lat': '0',
       'long': '0',
       'alt': '0',
       'battery': '0'}

    topost['id'] = iddevice[3:]
    topost['lat'] = lat
    topost['long'] = lon
    topost['alt'] = alt
    print(topost)
    
    token={
        'name': '******$,
        'password' : '***********'
    }
    r = requests.post(iptoken, json=token)
    authtoken = r.json()["access_token"]
    headers= {"Authorization": 'Bearer ' + authtoken}
    requests.post(ipadress, json=topost, headers=headers)
