# SandBox
The SandBox is a plugin-based, lean RFID controlled music player developed for the RaspberryPi and written in C++. 
It is intended as an open-source alternative to devices such as the TonieBox, TigerBox, Jooki, Hörbert etc. 
In contrast to more advanced projects like e.g. the [PhonieBox](https://github.com/MiczFlor/RPi-Jukebox-RFID), the SandBox has a significantly reduced functionalty, worse optics and more bugs. 
However, it was designed with plugin-based structure in mind, allowing the easy integration e.g. local audio files, GooglePlay or Apple Music. Also, the SandBox is written in pure C++, which might come in handy if you want to port it to different platforms (see section Third Party Dependencies).
Up to now, available plugins are a controller for the MFRC522 RFID reader and a Spotify controller. 

## Plugin Descriptions
### Spotify
The Spotify plugin actually is Spotify-Controller: It uses the official Spotify API to send playback/pause/stop commands to existing Spotify playback devices. 
An easy way to turn a Raspberry-Pi into a standalone music player is e.g. to use [librespot](https://github.com/librespot-org/librespot) to create a Spotify playback device on the RaspberryPi and then to use the SandBox to control it.

## Setup Instuction on the Raspberry Pi
This section describes how the SandBox can be set up on a Raspberry Pi Zero running Raspbebian Lite. Note that the Pi-Model and the OS are mostly interchangable.

### Prerequisites
You will need to buy/setup
* A Raspberry Pi Zero (W) with Raspbebian Lite and configured WiFi connection
* An MFRC522 RFID Reader
* A nice box to fit everything in
* A Mini-HDMI to VGA converter
* Stereo Speakers
* Some wires and a soldering iron to connect the reader to the Raspberry Pi
* A Spotify developer account

### Setup
1. Download the latest Release and install it by running `apt install SandBoxWithLibrespot.deb`
2. Edit librespot.service in `/etc/system.d/librespot.servie` and replace all parts with curly braces with the correct setting
3. Edit the SandBox configuration file under `/etc/SandBox/config.ini` and 

## Build Instructions
The SandBox can be conveniently built using cmake. First install all dependencies
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
	* boost
	* cpprest
* ReaderMRC522
	* [RPi-RFID](https://github.com/paguz/RPi-RFID)
* PlayerSpotify
	* cpprest

## ToDo
### Top Priority
* Create a DEB installer package for the Raspberry-Pi Zero
### Medium Priority
* Make the plugin interfaces C-Interfaces
* Refactor workflow to load the plugin configurations.
* Integrate playback of local audio files
