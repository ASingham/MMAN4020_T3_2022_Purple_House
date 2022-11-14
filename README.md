<div align="center">
  <h3 align="center">Purple House Remote Telemetry Module Setup</h3>
  <p align="center">
    Code installation and Microcontroller integration
  </p>
</div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#about-the-project">About The Project</a></li>
    <li><a href="#getting-started">Getting Started</a></li>
    <ul>
      <li><a href="#ESP32-package-installation">ESP32 Package Installation</a></li>
      <li><a href="#Additional-Library-installation">Additional Library Installation</a></li>
      <li><a href="#Remote-Telemetry-Module-Code-Installation">Remote Telemetry Module Code Installation</a></li>
    </ul>
    <li><a href="#usage">Usage</a></li>
  </ol>
</details>


<!-- ABOUT THE PROJECT -->
## About The Project
Source Repo for remote telemetry module C++ code to be used with ESP32 microcontroller, 4 ds18b20 temperature sensors and AskSensor IoT subscription.


<!-- GETTING STARTED -->
## Getting Started
* Ensure the latest version of Arduino IDE software is installed (2.0.1), which can be downloaded from [Arduino.cc](https://www.arduino.cc/en/software/).
* Ensure the ESP32 Add-on is installed in the Arduino IDE. See below for steps to complete this. 

### ESP32 Package Installation
1. In the Arduino IDE, go to: 
    ```sh
    File>preferences
    ```
2. In the "Additional Board Manager" URLs field, enter:
    ```sh
    https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
    ```
    and then click "OK"

3. In the Arduino IDE, open the Boards Manager by clicking through:
    ```sh
    Tools>Board>Boards Manager
    ```
    If an error appears, install using
    ```sh
    npm install --legacy-peer-deps
    ```
    
4. In Boards Manager, search for ESP32 and install:
    ```sh
    "ESP32 by Espressif Systems"
    ```
5. Test the installation and compatibility of the ESP32 board package 
    
    Plug the physical device into the computer using the USB cable. 
    * In Arduino IDE, select the board by clicking through:
    ```sh
    Tools>Board>Board>Adafruit ESP32 Feather
    ```
    * Select the Port used to connect the board to the computer by clicking through:
    ```sh
    Tools>Port>"The_COM_Port_Used_by_You"
    ```
    If the COM port cannot be found, install a USB to UART Bridge using the following [tutorial](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers).

### Additional Library Installation
1. In Arduino IDE, navigate to the Library Manager:
#### `Sketch>Include Library>Manage Libraries`

2. Using the search box, install the following two library packages:
    ```sh
    "OneWire" library by Paul Stoffregen et al.
    ```
    ```sh
    "DallasTemperature" by Miles Burton et al. 
    ```
3. Restart Arduino IDE after installing library packages.

### Remote Telemetry Module Code Installation (2 methods)
#### Method 1: Using Git (preferred method to enable version control if changes are to be made)

If the user has not installed Git onto their system, use this [tutorial](https://github.com/git-guides/install-git) to do so before continuing.

Now the user has Git installed, clone the repo into the desired location using the following steps:
1. Open the command prompt if using windows, and navigate to the desired folder for storing the repo.
    To do this, use the commands `cd` and `dir` as required to navigate between folders. A tutorial on this can be found [here](https://www.digitalcitizen.life/command-prompt-how-use-basic-commands/)
2. Clone the repo by pasting this command into the command prompt once the desired folder location is reached.
    ```sh
    git clone https://github.com/ASingham/MMAN4020_T3_2022_Purple_House.git
    ```
3. To check if the clone was successful, navigate into the newly created repo using the following command:
    ```sh
    cd MMAN4020_T3_2022_Purple_House
    ```

#### Method 2: Downloading a zip file (Easiest method if version control isn't neccessary): 
1. On the repo homepage, click on the green code icon to bring up the following menu:
<img
  src="images/code-location.png"
  alt="Alt text"
  style="display: inline-block; margin: 0 auto; max-width: 300px">

2. Click on the `Download ZIP` icon in the drop down menu to download a copy of the repo folder to your local device.

3. In your document manager on your local device, navigate to downloads, unzip the folder, and move the unzipped contents to the desired location. 



## Usage
1. Launch Arduino IDE application and from the cloned/downloaded repo, open:
    #### `OneSensorOLEDAskSensorSetup.ino`

2. As before, plug the device into the computer using the USB cable, and select both the board and the port from the Tools menu.
    ```sh
    Tools>Board>Board>Adafruit ESP32 Feather
    ```
    ```sh
    Tools>Port>"The_COM_Port_Used_by_You"
    ```
3. Press the Upload button in the Arduino IDE to compile the code and upload it to the connected board. 
    
    After a few seconds the output should say:
    ```sh
    "Done uploading"
    ```

4. Open the Serial Monitor in the Arduino IDE by pressing the button highlighted by the red circle below

    To see the output from the board, set the baud rate to: 
    #### `115200`

