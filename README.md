# Esp-netHunter
A portable ESP8266-based device for Evil Twin and deauthentication attacks with customizable phishing pages.

## Things you will need  
* NodeMCU
* 3.7V lithium rechargeable battery
* Lithium battery Charger board
* 4x6cm PCB 2sided prototype vero Board
* 4mm Slide Switch
* Push button (3)
* 1.3in or 0.96in I2C OLED 128X64
* Antenna

## Connections

### 1. OLED Display
- **GND** → GND on ESP8266  
- **VCC** → 3.3V on ESP8266  
- **SCL** → D1 on ESP8266  
- **SDA** → D2 on ESP8266  

### 2. Buttons
- **Left Button:**
  - One pin → D5 on ESP8266  
  - Other pin → GND  
- **Select Button:**
  - One pin → D6 on ESP8266  
  - Other pin → GND  
- **Right Button:**
  - One pin → D7 on ESP8266  
  - Other pin → GND  

### 3. Battery Charging Module
- **BAT+** → Positive terminal of the battery  
- **BAT-** → Negative terminal of the battery  
- **OUT+** → One pin of the switch  
- **OUT-** → GND on ESP8266  

### 4. Switch
- **First pin** → OUT+ on the charging module  
- **Second pin** → VIN on ESP8266  

### 5. Battery (3.7V)
- **Positive terminal** → BAT+ on the charging module  
- **Negative terminal** → BAT- on the charging module  

### 6. Antenna
- Connected to the antenna port on ESP8266 (optional).


![connections](/images/connections.jpg)


## Installation  
1. Install **Arduino IDE**.  
2. Open **Arduino IDE**, go to **File > Preferences > Additional Boards Manager URLs**, and add:  
   `https://raw.githubusercontent.com/SpacehuhnTech/arduino/main/package_spacehuhn_index.json`.  
3. Select **NodeMCU** as your board.  
4. Upload the code to your NodeMCU.  
* You will need these libraries (Adafruit_GFX | Adafruit_SH110X for 1.3in OLED | Adafruit_SSD1306 for 0.96in OLED).


Alternatively, use [esp.huhn.me](https://esp.huhn.me/) to upload a `.bin` file without using Arduino IDE.  

## How to use  
1. Navigate between networks using the **Left/Right** buttons and select a network by clicking the **SELECT** button.  
2. Choose the type of attack.  
3. Select a phishing page or upload your custom HTML page.  
4. Once the attack starts, press **SELECT** to return to the home screen.  

## Attacks  
1. **Handle**: Waits until the target network is gone, then starts the Evil Twin attack.  
2. **Evil Twin**: Creates an Evil Twin network with deauthentication running simultaneously.  
3. **Deauth**: Sends deauthentication packets to disconnect devices from the target network.  
4. **EarthLink**: Creates a fake public network named "EarthLink" with a captive portal and phishing page.  

## Phishing Pages  
1. **Stars**: A simple page displaying "NetHunter" with a falling stars animation.  
2. **TpLink**: Mimics the TP-Link login page.  
3. **Custom Page**: Allows you to upload your own HTML page.  

## Notes  
- The ESP8266 LED indicates that the deauthentication attack is active.  
- To upload a custom phishing page:  
  1. Select the **right option** and connect to the `server AP` network using the password `kizohi00`.  
  2. Open a browser and navigate to `192.168.4.1`.  
  3. Upload your custom page.  
- The device captures entered passwords.  
- The phishing network automatically stops once a password is captured.  

## Important  
- When uploading a custom HTML page, the device automatically edits it to enable password phishing. **Your page must contain an input field and a button.**  
- If your custom page does not include an input field and button, it will be displayed as is, without the ability to capture user passwords.  

## Thanks To  
- [M1z23R](https://github.com/M1z23R): For the original project, which this version modifies.
