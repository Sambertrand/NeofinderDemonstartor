# newdevice.py
# Author: Sam Bertrand, Martin Degeldt
# Version: March 28, 2020
'''
Script for Neofinder inc.

Add a device to the database
Create a new .ino file to flash on the device
'''

import json
import pymongo
import argparse
from string import Template

with open('config.json') as json_file:
    config = json.load(json_file)
    dbclient = pymongo.MongoClient(config["db_url"], username=config["db_username"], password=config["db_pwd"], authSource='neofinderdb', ssl=True)

mgdb = dbclient["neofinderdb"]
devicedb = mgdb["devices"]

def createNewDevice(email):
    mydevice = {"owner": email}
    x = devicedb.insert_one(mydevice)
    #auto generated id
    return str(x.inserted_id)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('file', help='the arduino file to flash')
    parser.add_argument('email', help='email of the owner of the device')
    args = parser.parse_args()

    identifier = createNewDevice(args.email)


    with open(args.file, encoding=('utf-8')) as inoFile:
        ino = inoFile.read()

    newino = Template(ino).safe_substitute({"ID" : identifier})

    fileName, fileExtention = args.file.split('.')
    with open (fileName +"_"+ identifier+"." + fileExtention, 'w') as toFlash:
        toFlash.write(newino)

    print("New device created for " + args.email)