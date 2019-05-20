# SandBox
The SandBox is a plugin-based, lean RFID controlled music player developed for the RaspberryPi and written in C++. 
It is intended as an open-source alternative to devices such as the TonieBox, TigerBox, Jooki, Hörbert etc. 
In contrast to more advanced projects like e.g. the [PhonieBox](https://github.com/MiczFlor/RPi-Jukebox-RFID), the SandBox has a significantly reduced functionalty, worse optics and most definitely more bugs. 
However, it was designed with plugin-based structure in mind, allowing the easy integration e.g. local audio files, GooglePlay or Apple Music. Also, the SandBox is written in pure C++, which might come in handy if you want to port it to different platforms (see section Third Party Dependencies).
Up to now, available plugins are a controller for the MFRC522 RFID reader and for Spotify.

## Plugin Descriptions

### Spotify
The Spotify plugin actually is Spotify-Controller: It uses the official Spotify API to send playback/pause/stop commands to your existing Spotify playback devices. 
*Note that this means that if you already have a Spotify Music Player, the SandBox can be trivially configured to trigger playback on this existing player.*
An easy way to turn a Raspberry-Pi into a standalone music player is e.g. to use [librespot](https://github.com/librespot-org/librespot) to create a local Spotify playback device and then to use the SandBox to control it. 
Note that up to now, you will need to have a premium subscription, since the Spotify API is not available for free accounts.

## Setup Instuction on the Raspberry Pi
This section describes how the SandBox can be set up on a Raspberry Pi Zero running Raspbebian Lite. Note that the Pi-Model and the OS are mostly interchangable.

### Prerequisites
You will need to buy/setup
* A Raspberry Pi Zero (W) with Raspbebian Lite and configured WiFi connection
* An MFRC522 RFID Reader
* A nice box to fit everything in
* A Mini-HDMI to 3.5" audio adapter
* Stereo Speakers
* Some wires and a soldering iron to connect the reader to the Raspberry Pi
* A Spotify Premium and a Spotify Developer account (register the latter on free under https://developer.spotify.com
Note that a Spotify Developer account is necessary, since the SandBox is an app which has the capability to control your Spotify account. 
To make such actions tracable, Spotify requires all such apps to be linked to some developer account.

Next prepare your Spotify Developer account:
1. Got to https://developer.spotify.com -> Dashboard -> Login
2. Click "Create an App", add a new App called "SandBox" and fill in all settings
3. Under "Redirect URIs" fill in *http://sandbox/spotify/auth_receiver*

### Setup
1. Login to you Raspberry Pi
1. Download the latest Release and install it by running `apt install SandBoxWithLibrespot_19.6.0.deb`
2. Change your Raspberry Pie's to "SandBox" by editing /etc/hostname
2. Edit librespot.service under *etc/system.d/librespot.service* and replace all parts with curly braces with the correct settings.
3. Edit the SandBox configuration file */etc/SandBox/config.ini* an replace all parts with curly braces with the correct settings.ClientId and ClientSecret are the ones from your Spotify Developer Account.
4. Restart to make all changes effective
5. Connect the RFID Reader as described in https://github.com/paguz/RPi-RFID

This is it, you can now use your SandBox.
To see the log visit http://sandbox
To write Spotify-RFID cards, simply place a blank card on the player and visit http://sandbox/spotify


## Build Instructions
The SandBox can be conveniently built using cmake. First install all dependencies:
```
apt install git, cmake, build-essential, libcpprest-dev
```
Then clone the repository and init the 3rd Party Dependencies
```
git clone https://github.com/check0104/SandBox.git
git submodule --init --recursive
cd SandBox
mkdir build
```

### Deployment
To build the SandBox for deployment, simply use
```
cmake ..
```

### Development
To enable development outside the RaspberryPi, the SandBox comes with a mock-reader, which implements the `ClReaderBase::read()` and `ClReaderBase::write(std::vector<unsigned char> &vcData)` as simply reading/writing from/to a file. This allows convenient development on a different system. To build the SandBox for development, simply use
```
cmake -DDEVELOPMENT=1 ..
```

## Third Party Dependencies
* Main Application
	* [BOOST](https://www.boost.org)
	* [cpprest](https://github.com/microsoft/cpprestsdk)
* ReaderMRC522
	* [RPi-RFID](https://github.com/paguz/RPi-RFID)
	* [libbcm2835](https://www.airspayce.com/mikem/bcm2835/)
* PlayerSpotify
	* [cpprest](https://github.com/microsoft/cpprestsdk)

## ToDo
### Top Priority
* Create a DEB installer package for the Raspberry-Pi Zero
### Medium Priority
* Make the plugin interfaces C-Interfaces
* Refactor workflow to load the plugin configurations.
* Integrate playback of local audio files

## Licenses
This project is licensed under *Open Source Licensing GPL V3* (see LICENSE.txt for details). License texts of 3rd Party applications can be found in LICENSE_3rdParty.md These other licenses are
* [BOOST](https://www.boost.org) is licensed under *Boost Software License - Version 1.0 - August 17th, 2003*.
* [cpprest](https://github.com/microsoft/cpprestsdk) is licensed under the *MIT License*
* [libbcm2835](https://www.airspayce.com/mikem/bcm2835) is licensensed under *Open Source Licensing GPL V2*
* RPi-RFID: -
