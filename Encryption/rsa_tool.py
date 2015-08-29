#!/usr/bin/python

import sys, getopt, os
import rsa

try:
    opts, args = getopt.getopt(sys.argv[1:], "hm:edg")
except getopt.GetoptError:
    print "rsa.py -m [message] -e(ncryption) -d(ecryption) -g(enerate key pair)"
    sys.exit(-1)

message = ""
privkey = ""
pubkey = ""

mode = -1

for opt, arg in opts:
    if opt == "-h":
        print "rsa.py -m [message] -e(ncryption) -d(ecryption) -g(enerate key pair)"
        exit(0)
    if opt == "-g":
        print "Generating key pair"
        (pubkey, privkey) = rsa.newkeys(1024)
        privfile = open("priv", "w")
        privfile.write(privkey.save_pkcs1("PEM"))
        pubfile = open("pub", "w")
        pubfile.write(pubkey.save_pkcs1("PEM"))
        quit(0)

    if opt == "-d":
        with open('priv') as privatefile:
            keydata = privatefile.read()
        privkey = rsa.PrivateKey.load_pkcs1(keydata,"PEM")
        print "Private key loaded"
        mode = 100

    if opt == "-e":
        with open('pub') as pubfile:
            keydata = pubfile.read()
        pubkey = rsa.PublicKey.load_pkcs1(keydata,"PEM")
        print "Public key loaded"
        mode = 200

    if opt == "-m":
        message = arg

if mode == 100:
    if message=="":
        while True:
            print "Input:",
            message=raw_input()
            result = ""
            j=0
            for i in range(0,len(message),2):
                result += chr(int("0x"+message[i]+message[i+1],16))
                print "Decrypted:"
                print rsa.decrypt(result, privkey)
    else:
        result = ""
        j=0
        for i in range(0,len(message),2):
            result += chr(int("0x"+message[i]+message[i+1],16))
        print "Decrypted:"
        print rsa.decrypt(result, privkey)
elif mode == 200:
    if message=="":
        while True:
            print "Input:",
            message=raw_input()
            message = rsa.encrypt(message, pubkey)
            retString = ""
            for i in range(len(message)):
                cha = str(hex(ord(message[i])))[2:]
                if len(cha) == 1:
                    cha = "0"+cha
                retString += cha
            print "Encrypted:"
            print retString
            cmd = "osascript -e \"set the clipboard to \\\""+retString+"\\\"\""
            os.system(cmd)
            print "Copied"
    else:
        message = rsa.encrypt(message, pubkey)
        retString = ""
        for i in range(len(message)):
            cha = str(hex(ord(message[i])))[2:]
            if len(cha) == 1:
                cha = "0"+cha
            retString += cha
        print "Encrypted:"
        print retString
        cmd = "osascript -e \"set the clipboard to \\\""+retString+"\\\"\""
        os.system(cmd)
        print "Copied"
