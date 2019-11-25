# SandBox Audio
The SandBox is a modular, lean RFID controlled music player developed for the RaspberryPi and written in C++. 
It is intended as an open-source alternative to devices such as the TonieBox, TigerBox, Jooki, Hörbert etc. Note that the SandBox still is in an early stage of development and thus lacks features, comfort and design of those commercial products.
The SandBox was designed with a modular structure in mind, allowing the easy integration e.g. local audio files, GooglePlay or Apple Music in the future. Also, the SandBox is written in C++, which might come in handy if you want to port it to different platforms (see section Third Party Dependencies).
The current version comes with integrated Spotify support using a precompiled version of the [librespot](https://github.com/librespot-org/librespot).

![SandBox](/misc/SandBox.jpg)

## Player Plugin Descriptions

### Spotify Plugin
The Spotify plugin actually is Spotify-Controller: It uses the official Spotify API to send playback/pause/stop commands to your existing Spotify playback devices. 
*Note that this means that if you already have a Spotify Music Player, the SandBox can be trivially configured to trigger playback on this existing player.*
The current version of the SandBox uses the [librespot](https://github.com/librespot-org/librespot) to create a local Spotify playback device, which is controlled via the RFID commands.
Note that up to now, you will need to have a premium subscription, since the Spotify API is not available for free accounts.
#### Usage
To write Spotify-RFID cards, simply place a RFID card on the player and visit http://sandbox/spotify. Choose the correct playback type and enter the Spotify URI of the medium (Spotify desktop app -> right-click -> share -> Copy Spotify URI)

## Setup Instuction on the Raspberry Pi Zero
This section describes how the SandBox can be set up on a Raspberry Pi Zero running Raspbebian Lite. 

### Prerequisites
You will need to buy/setup
* A Raspberry Pi Zero (W) with Raspbebian Lite and configured WiFi connection
* An MFRC522 RFID Reader
* MIFARE Classic RFID Cards
* A Mini-HDMI to 3.5" audio adapter
* Stereo Speakers
* Some wires and a soldering iron to connect the reader to the Raspberry Pi
* A Spotify Premium and a Spotify Developer account (register the latter on free under https://developer.spotify.com
Note that a Spotify Developer account is necessary, since the SandBox is an app which has the capability to control your Spotify account. 
To make such actions tracable, Spotify requires all such apps to be linked to some developer account.
Next prepare your Spotify Developer account:
	* Got to https://developer.spotify.com -> Dashboard -> Login
	* Click "Create an App", add a new App called "SandBox" and fill in all settings.

### Setup
1. Login to you Raspberry Pi
2. Change your Raspberry Pie's to "SandBox" by editing /etc/hostname
3. Download the latest [Release](https://github.com/csandmann/SandBox/releases) and install it by running `apt install SandBoxWithLibrespot_{VERSION}.deb`
4. Run install script
```
    sudo /opt/sandbox/setupSandbox.sh
```
After completing, this script will tell you which redirect URI you need to whitelist in your spotify developer account.

5. Connect the RFID Reader as described in https://github.com/paguz/RPi-RFID
6. To uninstall, simply run `sudo /opt/sandbox/removeSandbox.sh` and after that `sudo apt remove sandbox`

This is it, you can now use your SandBox. To see the log visit http://sandbox, to configure spotify visit http://sandbox/spotify


## Build Instructions (Not necessary for deployment)
The SandBox can be conveniently built using cmake. First install all dependencies:
```
apt install git, cmake, build-essential, libcpprest-dev
```
Then clone the repository and init the 3rd Party Dependencies
```
git clone https://github.com/check0104/SandBox.git
cd SandBox
git submodule update --init --recursive
mkdir build
cd build
```

### Deployment Build
For deployment, [cpprest](https://github.com/microsoft/cpprestsdk) is linked statically. So you need to build and install cpprest yourself (see https://github.com/Microsoft/cpprestsdk/wiki/How-to-build-for-Linux). Also, install the [libbcm2835](https://www.airspayce.com/mikem/bcm2835/)
After installing both componentents, simply run
```
cmake ..
```
and then 
```
make
```

### Development Build
To enable development outside the RaspberryPi, the SandBox comes with a mock-reader, which implements the `ClReaderBase::read()` and `ClReaderBase::write(const std::vector<unsigned char> &vcData)` as simply reading/writing from/to a file. This allows convenient development on a different system. To build the SandBox for development, simply use
```
cmake -DDEVELOPMENT=ON ..
```

## Third Party Dependencies
* Main Application
	* [BOOST](https://www.boost.org)
	* [cpprest](https://github.com/microsoft/cpprestsdk)
* ReaderMFRC522
	* [RPi-RFID](https://github.com/paguz/RPi-RFID)
	* [libbcm2835](https://www.airspayce.com/mikem/bcm2835/)
* PlayerSpotify
	* [cpprest](https://github.com/microsoft/cpprestsdk)
* Optional component to emulate a Spotify device on the RaspberryPi
	* [librespot](https://github.com/librespot-org/librespot)

## ToDo
### Top Priorities
* Bugfixes (see Release section for known issues)
* Remove album/playlist options and parse them from the URL
* Continue playlists where it was stopped, add 'reset' button
### Medium Priority
* Integrate play/pause, next, previous buttons
* Integrate a speaker and a rotary encoder for volumen control
* Make the plugin interfaces C-Interfaces
* Refactor workflow to load the plugin configurations.
* Integrate playback of local audio files
* Integrate rotating log-files
 
## Licenses
This project is licensed under *Open Source Licensing GPL V3* (see LICENSE.txt for details). License texts of 3rd Party applications can be found in LICENSE_3rdParty.md These other licenses are
* [BOOST](https://www.boost.org) is licensed under *Boost Software License - Version 1.0 - August 17th, 2003*.
* [cpprest](https://github.com/microsoft/cpprestsdk) is licensed under the *MIT License*
* [libbcm2835](https://www.airspayce.com/mikem/bcm2835) is licensensed under *Open Source Licensing GPL V2*
* [librespot](https://github.com/librespot-org/librespot) is licensed under the *MIT License*
* [RPi-RFID](https://github.com/paguz/RPi-RFID): Author contacted, probably licensed under UNLICENSE as the parent project (https://github.com/miguelbalboa/rfid)

