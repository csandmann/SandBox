#!/usr/bin/python3

import os
import sys

def _check_sudo_permissions():
    try:
        test_file_path = "/etc/sbxtest"
        with open(test_file_path, 'w') as fout:
            pass
        os.remove(test_file_path)
    except PermissionError:
        sys.stderr.write("Please run this script as sudo!\n")
        quit(1)


def _safe_remove(file_path):
    try:
        os.remove(file_path)
    except FileNotFoundError:
        print("File {} not found".format(file_path))

def main():
    _check_sudo_permissions()
    ans = input("Do you really want to remove all configuration files of SandBox (yes/no)?")
    if ans != "Yes" and ans != 'y' and ans != 'yes':
        print("Aborting")
        quit(0)
    _safe_remove("/etc/systemd/system/librespot.service")
    _safe_remove("/etc/systemd/system/sandbox.service")
    _safe_remove("/opt/sandbox/config.ini")
    print("success!")

if __name__=='__main__':
    main()
