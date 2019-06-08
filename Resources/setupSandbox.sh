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

def _read_write_config_file(src, dst, config):
    with open(src, 'r') as fin:
        tpl = fin.read()
    with open(dst, 'w') as fout:
        fout.write(tpl.format(**config))

def main():
    _check_sudo_permissions()
    librespot_service_keys = ()
    librespot_keys = ('spotify_username', 'spotify_password')
    sandbox_service_keys = ('username', )
    sandbox_keys = ('hostname', 'client_id', 'client_secret')
    config = {}
    print("--------------------------")
    print("     SandBox Audio")
    print("-------------------------")
    print("This script will perform the basic configuration of SandBox Audio as a service and will install it as a service. "\
          "After running it, removal of SandBox is only possible by running the script removeSandbox.py")
    print("\nStarting with configuration of SandBox")
    print("----------------------------------------")
    config['username'] = input("Please enter your username:")
    config['hostname'] = input("Please enter your ip address:")
    config['client_id'] = input("Please enter your spotify-developer client id:")
    config['client_secret'] = input("Please enter your spotify-developer client secret:")
    print("\nStarting with configuration of librespot")
    print("----------------------------------------")
    config['spotify_username'] = input("Please enter your spotify username:")
    config['spotify_password'] = input("Please enter your spotify password:")
    print("Writing config files")
    print("----------------------------------------")
    _read_write_config_file("/opt/sandbox/Resources/librespot.service", "/etc/systemd/system/librespot.service", {key: config[key] for key in librespot_service_keys})
    _read_write_config_file("/opt/sandbox/Resources/sandbox.service", "/etc/systemd/system/sandbox.service", {key: config[key] for key in sandbox_service_keys})
    _read_write_config_file("/opt/sandbox/Resources/config.ini", "/opt/sandbox/config.ini", {key: config[key] for key in sandbox_keys})
    _read_write_config_file("/opt/sandbox/Resources/startLibrespot.sh", "/opt/sandbox/startLibrespot.sh", {key: config[key] for key in librespot_keys})
    os.chmod("/opt/sandbox/startLibrespot.sh", 0o700)
    print("Enabling services sandbox.service and librespot.service")
    print("----------------------------------------")
    os.system("systemctl enable sandbox.service")
    os.system("systemctl enable librespot.service")
    print("Please add ")
    print("         http://{}:80/spotify/auth_receiver".format(config['hostname']))
    print("to your redirect URIs under your Spotify Developer Account")
    print("Success")

if __name__=='__main__':
    main()
