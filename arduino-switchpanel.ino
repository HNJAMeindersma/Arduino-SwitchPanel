/* ----------------------------------------------------------------

  Script:       Switch Panel
  Version:      1.0
  Date:         2018-05-22
  Author:       H.N.J.A. Meindersma <dev@hnjameindersma.net>
  Description:  Switch Panel is a controller for relays, IR
                commands and RF signals. It runs on basicly any
                Arduino with at least 32 kb flash memory. By
                default there are 16 relay profiles. This is the
                same for the IR commands and RF signals, each.
                However, the user decides which and in what
                order the profiles get used. It can be controlled
                by 3 button control, IR commands or RF signals.
                And the menu is displayed trough a Liquid Crystal
                LCD screen with I2C module. All widths and heights
                should be supported, however the 1602 and 2004 are
                tested and confirmed to work for sure.

  Supported:    - control by 3 button control (OK, NEXT, PREVIOUS)
                - output for I2C LCD screen (1602 and 2004 tested)
                - output for relays (16 profiles)

  Unsupported:  - control by IR commands (OK, NEXT, PREVIOUS, #)
                - output for IR commands (16 profiles)
                - control by RF signals (OK, NEXT, PREVIOUS, #)
                - output for RF signals (16 profiles)

---------------------------------------------------------------- */

// General settings
int     serial_baud        =  9600;              // Serial baud rate
int     pin_led            =  13;                // Pin # for LED
int     pin_control_ok     =  10;                // Pin # for 'OK' button control
int     pin_control_nxt    =  11;                // Pin # for 'NXT' button control
int     pin_control_prv    =  12;                // Pin # for 'PRV' button control
int     pin_ir_rx          =  2;                 // RX pin # for IR commands
int     pin_ir_tx          =  3;                 // TX pin # for IR commands
int     pin_rf_rx          =  4;                 // RX pin # for RF signals
int     pin_rf_tx          =  5;                 // TX pin # for RF signals
bool    control            =  true;              // Enable/disable 3 button control
bool    control_ir         =  false;             // Enable/disable control by IR commands
bool    control_rf         =  false;             // Enable/disable control by RF signals
int     delay_bootup       =  2000;              // Delay (miliseconds) at bootup
int     delay_loop         =  50;                // Delay (miliseconds) after empty loop
int     delay_control      =  1000;              // Delay (miliseconds) after 3 button control
int     delay_control_ir   =  1000;              // Delay (miliseconds) after IR command
int     delay_control_rf   =  1000;              // Delay (miliseconds) after RF signal
byte    lcd_address        =  0x27;              // LCD address (1602: '0x20', 2004: '0x27')
int     lcd_width          =  20;                // LCD screen width (colums)
int     lcd_height         =  4;                 // LCD screen height (rows)
int     lcd_bootup         =  3;                 // Seconds to display bootup message
int     lcd_blackout       =  30;                // Seconds to blackout the LCD
char    lcd_message1[]     =  "Switch Panel";    // LCD bootup message 1
char    lcd_message2[]     =  "v1.0";            // LCD bootup message 2
char    lcd_type_sw[]      =  "ON/OFF relay";    // LCD 'sp_sw' name
char    lcd_type_ir[]      =  "IR command";      // LCD 'sp_ir' name
char    lcd_type_rf[]      =  "RF signal";       // LCD 'sp_rf' name

// Active list
char    sp_active[][8]   =  {  "sp_sw0", "sp_sw1", "sp_sw2", "sp_sw3", "sp_ir0", "sp_ir1", "sp_rf0", "sp_rf1"  };

// Relay profile list          LABEL                 PIN         DEFAULT
char    sp_sw0[][32]     =  {  "SW label 0",         "22",       "OFF"  };
char    sp_sw1[][32]     =  {  "SW label 1",         "23",       "OFF"  };
char    sp_sw2[][32]     =  {  "SW label 2",         "24",       "OFF"  };
char    sp_sw3[][32]     =  {  "SW label 3",         "25",       "OFF"  };
char    sp_sw4[][32]     =  {  "SW label 4",         "26",       "OFF"  };
char    sp_sw5[][32]     =  {  "SW label 5",         "27",       "OFF"  };
char    sp_sw6[][32]     =  {  "SW label 6",         "28",       "OFF"  };
char    sp_sw7[][32]     =  {  "SW label 7",         "29",       "OFF"  };
char    sp_sw8[][32]     =  {  "SW label 8",         "30",       "OFF"  };
char    sp_sw9[][32]     =  {  "SW label 9",         "31",       "OFF"  };
char    sp_sw10[][32]    =  {  "SW label 10",        "32",       "OFF"  };
char    sp_sw11[][32]    =  {  "SW label 11",        "33",       "OFF"  };
char    sp_sw12[][32]    =  {  "SW label 12",        "34",       "OFF"  };
char    sp_sw13[][32]    =  {  "SW label 13",        "35",       "OFF"  };
char    sp_sw14[][32]    =  {  "SW label 14",        "36",       "OFF"  };
char    sp_sw15[][32]    =  {  "SW label 15",        "37",       "OFF"  };

// IR profile list             LABEL                 CODE
char    sp_ir0[][32]     =  {  "IR label 0",         "00x1acb"  };
char    sp_ir1[][32]     =  {  "IR label 1",         "00x1acb"  };
char    sp_ir2[][32]     =  {  "IR label 2",         "00x1acb"  };
char    sp_ir3[][32]     =  {  "IR label 3",         "00x1acb"  };
char    sp_ir4[][32]     =  {  "IR label 4",         "00x1acb"  };
char    sp_ir5[][32]     =  {  "IR label 5",         "00x1acb"  };
char    sp_ir6[][32]     =  {  "IR label 6",         "00x1acb"  };
char    sp_ir7[][32]     =  {  "IR label 7",         "00x1acb"  };
char    sp_ir8[][32]     =  {  "IR label 8",         "00x1acb"  };
char    sp_ir9[][32]     =  {  "IR label 9",         "00x1acb"  };
char    sp_ir10[][32]    =  {  "IR label 10",        "00x1acb"  };
char    sp_ir11[][32]    =  {  "IR label 11",        "00x1acb"  };
char    sp_ir12[][32]    =  {  "IR label 12",        "00x1acb"  };
char    sp_ir13[][32]    =  {  "IR label 13",        "00x1acb"  };
char    sp_ir14[][32]    =  {  "IR label 14",        "00x1acb"  };
char    sp_ir15[][32]    =  {  "IR label 15",        "00x1acb"  };

// RF profile list             LABEL                 CODE
char    sp_rf0[][32]     =  {  "RF label 0",         "0123456"  };
char    sp_rf1[][32]     =  {  "RF label 1",         "0123456"  };
char    sp_rf2[][32]     =  {  "RF label 2",         "0123456"  };
char    sp_rf3[][32]     =  {  "RF label 3",         "0123456"  };
char    sp_rf4[][32]     =  {  "RF label 4",         "0123456"  };
char    sp_rf5[][32]     =  {  "RF label 5",         "0123456"  };
char    sp_rf6[][32]     =  {  "RF label 6",         "0123456"  };
char    sp_rf7[][32]     =  {  "RF label 7",         "0123456"  };
char    sp_rf8[][32]     =  {  "RF label 8",         "0123456"  };
char    sp_rf9[][32]     =  {  "RF label 9",         "0123456"  };
char    sp_rf10[][32]    =  {  "RF label 10",        "0123456"  };
char    sp_rf11[][32]    =  {  "RF label 11",        "0123456"  };
char    sp_rf12[][32]    =  {  "RF label 12",        "0123456"  };
char    sp_rf13[][32]    =  {  "RF label 13",        "0123456"  };
char    sp_rf14[][32]    =  {  "RF label 14",        "0123456"  };
char    sp_rf15[][32]    =  {  "RF label 15",        "0123456"  };

/* ---------------------------------------------------------------- */

#include <ArduinoBoardManager.h>
ArduinoBoardManager arduino = ArduinoBoardManager();

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(lcd_address,lcd_width,lcd_height);

#include <IRremote.h>
IRrecv irrecv(pin_ir_rx);
decode_results ir_results;
IRsend irsend;

/* ---------------------------------------------------------------- */

// Setup
void setup() {

  // Setup serial
  Serial.begin(serial_baud);
  while(!Serial);
  Serial.print("SETUP: Serial communication initiated (");
  Serial.print(serial_baud);
  Serial.print(" baud).\n");

  // System info
  Serial.print("INFO: Arduino ");
  Serial.print(arduino.BOARD_NAME);
  Serial.print(" ");
  Serial.print(ArduinoBoardManager::NUM_BITS);
  Serial.print("-bit with SDK version ");
  Serial.print(ArduinoBoardManager::SDK_VERSION);
  Serial.print(".\n");
  Serial.print("INFO: Clock: ");
  Serial.print(ArduinoBoardManager::MAX_MHZ / 1000000);
  Serial.print(" MHz | SRAM: ");
  Serial.print(ArduinoBoardManager::SRAM_SIZE);
  Serial.print(" bytes | EEPROM: ");
  Serial.print(ArduinoBoardManager::EEPROM_SIZE);
  Serial.print(" bytes | Flash: ");
  Serial.print(ArduinoBoardManager::FLASH_SIZE / 1000);
  Serial.print(" KB.\n");

  // Setup LED
  pinMode(pin_led, OUTPUT);
  digitalWrite(pin_led, HIGH);
  Serial.print("SETUP: LED pin is declared on pin '");
  Serial.print(pin_led);
  Serial.print("'.\n");
  Serial.print("INFO: LED is ON during setup and goes OFF when completed.\n");

  // Setup LCD
  Serial.print("SETUP: LCD pins are declared on the 'SDA' and 'SCL' pins.\n");
  Serial.print("INFO: LCD might flash during setup.\n");
  lcd.begin();
  lcd.display();
  lcd.noBacklight();
  lcd.noCursor();
  lcd.noAutoscroll();
  lcd.clear();
  delay(delay_bootup);
  if (lcd_height >= 4) {
    lcd.setCursor(round((lcd_width - ((sizeof(lcd_message1)) - 1)) / 2), 1);
    lcd.print(lcd_message1);
    lcd.setCursor(round((lcd_width - ((sizeof(lcd_message2)) - 1)) / 2), 2);
    lcd.print(lcd_message2);
    lcd.backlight();
  } else if (lcd_height >= 2) {
    lcd.setCursor(round((lcd_width - ((sizeof(lcd_message1)) - 1)) / 2), 0);
    lcd.print(lcd_message1);
    lcd.setCursor(round((lcd_width - ((sizeof(lcd_message2)) - 1)) / 2), 1);
    lcd.print(lcd_message2);
    lcd.backlight();
  } else {
    lcd.setCursor(round((lcd_width - ((sizeof(lcd_message1)) - 1)) / 2), 0);
    lcd.print(lcd_message1);
    lcd.backlight();
    delay((lcd_bootup * 500));
    lcd.clear();
    lcd.setCursor(round((lcd_width - ((sizeof(lcd_message2)) - 1)) / 2), 0);
    lcd.print(lcd_message2);
    delay((lcd_bootup * 500));
    lcd.clear();
    lcd.setCursor(round((lcd_width - ((sizeof(lcd_message1)) - 1)) / 2), 0);
    lcd.print(lcd_message1);
  } 

  // Setup control
  if (control == true) {
    Serial.print("INFO: Control is 'enabled'.\n");
    pinMode(pin_control_ok, INPUT);
    pinMode(pin_control_nxt, INPUT);
    pinMode(pin_control_prv, INPUT);
    Serial.print("SETUP: Control pins are declared on pin '");
    Serial.print(pin_control_ok);
    Serial.print("' for 'OK' and pin '");
    Serial.print(pin_control_nxt);
    Serial.print("' for 'NXT' and pin '");
    Serial.print(pin_control_prv);
    Serial.print("' for 'PRV'.\n");
  } else {
    Serial.print("INFO: Control is 'disabled'.\n");
  }

  // Setup IR transciever
  if (control_ir == true) {
    Serial.print("INFO: Control via IR is 'enabled'.\n");
    irrecv.enableIRIn();
    Serial.print("SETUP: IR transciever pins are declared on pin '");
    Serial.print(pin_ir_rx);
    Serial.print("' for RX and pin '");
    Serial.print(pin_ir_tx);
    Serial.print("' for TX.\n");
  } else {
    Serial.print("INFO: Control via IR is 'disabled'.\n");
  }

  // Setup RF transciever
  if (control_rf ==  true) {
    Serial.print("INFO: Control via RF is 'enabled'.\n");
    pinMode(pin_rf_rx, INPUT);
    pinMode(pin_rf_tx, OUTPUT);
    Serial.print("SETUP: RF transciever pins are declared on pin '");
    Serial.print(pin_rf_rx);
    Serial.print("' for RX and pin '");
    Serial.print(pin_rf_tx);
    Serial.print("' for TX.\n");
  } else {
    Serial.print("INFO: Control via RF is 'disabled'.\n");
  }

  // Setup active ports
  Serial.print("INFO: ");
  Serial.print(sizeof(sp_active) / sizeof(sp_active[0]));
  Serial.print(" list items declared.\n");
  for (byte sp_idx = 0; sp_idx < sizeof(sp_active) / sizeof(sp_active[0]); sp_idx++) {
    String sp_check = sp_active[sp_idx];
    if (sp_check.startsWith("sp_sw")) {
      Serial.print("SETUP: List item '");
      Serial.print(sp_check);
      Serial.print("' is declared on pin '");
      if (sp_check == "sp_sw0") {
        String sp_sw0_pin = sp_sw0[1];
        String sp_sw0_default = sp_sw0[2];
        Serial.print(sp_sw0[1]);
        Serial.print("' with label '");
        Serial.print(sp_sw0[0]);
        if (sp_sw0_default == "ON") {
          pinMode(sp_sw0_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw0_pin.toInt(), LOW);
          Serial.print("' and default 'ON");
        } else {
          pinMode(sp_sw0_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw0_pin.toInt(), HIGH);
          Serial.print("' and default 'OFF");
        }
      } else if (sp_check == "sp_sw1") {
        String sp_sw1_pin = sp_sw1[1];
        String sp_sw1_default = sp_sw1[2];
        Serial.print(sp_sw1[1]);
        Serial.print("' with label '");
        Serial.print(sp_sw1[0]);
        if (sp_sw1_default == "ON") {
          pinMode(sp_sw1_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw1_pin.toInt(), LOW);
          Serial.print("' and default 'ON");
        } else {
          pinMode(sp_sw1_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw1_pin.toInt(), HIGH);
          Serial.print("' and default 'OFF");
        }
      } else if (sp_check == "sp_sw2") {
        String sp_sw2_pin = sp_sw2[1];
        String sp_sw2_default = sp_sw2[2];
        Serial.print(sp_sw2[1]);
        Serial.print("' with label '");
        Serial.print(sp_sw2[0]);
        if (sp_sw2_default == "ON") {
          pinMode(sp_sw2_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw2_pin.toInt(), LOW);
          Serial.print("' and default 'ON");
        } else {
          pinMode(sp_sw2_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw2_pin.toInt(), HIGH);
          Serial.print("' and default 'OFF");
        }
      } else if (sp_check == "sp_sw3") {
        String sp_sw3_pin = sp_sw3[1];
        String sp_sw3_default = sp_sw3[2];
        Serial.print(sp_sw3[1]);
        Serial.print("' with label '");
        Serial.print(sp_sw3[0]);
        if (sp_sw3_default == "ON") {
          pinMode(sp_sw3_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw3_pin.toInt(), LOW);
          Serial.print("' and default 'ON");
        } else {
          pinMode(sp_sw3_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw3_pin.toInt(), HIGH);
          Serial.print("' and default 'OFF");
        }
      } else if (sp_check == "sp_sw4") {
        String sp_sw4_pin = sp_sw4[1];
        String sp_sw4_default = sp_sw4[2];
        Serial.print(sp_sw4[1]);
        Serial.print("' with label '");
        Serial.print(sp_sw4[0]);
        if (sp_sw4_default == "ON") {
          pinMode(sp_sw4_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw4_pin.toInt(), LOW);
          Serial.print("' and default 'ON");
        } else {
          pinMode(sp_sw4_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw4_pin.toInt(), HIGH);
          Serial.print("' and default 'OFF");
        }
      } else if (sp_check == "sp_sw5") {
        String sp_sw5_pin = sp_sw5[1];
        String sp_sw5_default = sp_sw5[2];
        Serial.print(sp_sw5[1]);
        Serial.print("' with label '");
        Serial.print(sp_sw5[0]);
        if (sp_sw5_default == "ON") {
          pinMode(sp_sw5_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw5_pin.toInt(), LOW);
          Serial.print("' and default 'ON");
        } else {
          pinMode(sp_sw5_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw5_pin.toInt(), HIGH);
          Serial.print("' and default 'OFF");
        }
      } else if (sp_check == "sp_sw6") {
        String sp_sw6_pin = sp_sw6[1];
        String sp_sw6_default = sp_sw6[2];
        Serial.print(sp_sw6[1]);
        Serial.print("' with label '");
        Serial.print(sp_sw6[0]);
        if (sp_sw6_default == "ON") {
          pinMode(sp_sw6_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw6_pin.toInt(), LOW);
          Serial.print("' and default 'ON");
        } else {
          pinMode(sp_sw6_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw6_pin.toInt(), HIGH);
          Serial.print("' and default 'OFF");
        }
      } else if (sp_check == "sp_sw7") {
        String sp_sw7_pin = sp_sw7[1];
        String sp_sw7_default = sp_sw7[2];
        Serial.print(sp_sw7[1]);
        Serial.print("' with label '");
        Serial.print(sp_sw7[0]);
        if (sp_sw7_default == "ON") {
          pinMode(sp_sw7_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw7_pin.toInt(), LOW);
          Serial.print("' and default 'ON");
        } else {
          pinMode(sp_sw7_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw7_pin.toInt(), HIGH);
          Serial.print("' and default 'OFF");
        }
      } else if (sp_check == "sp_sw8") {
        String sp_sw8_pin = sp_sw8[1];
        String sp_sw8_default = sp_sw8[2];
        Serial.print(sp_sw8[1]);
        Serial.print("' with label '");
        Serial.print(sp_sw8[0]);
        if (sp_sw8_default == "ON") {
          pinMode(sp_sw8_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw8_pin.toInt(), LOW);
          Serial.print("' and default 'ON");
        } else {
          pinMode(sp_sw8_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw8_pin.toInt(), HIGH);
          Serial.print("' and default 'OFF");
        }
      } else if (sp_check == "sp_sw9") {
        String sp_sw9_pin = sp_sw9[1];
        String sp_sw9_default = sp_sw9[2];
        Serial.print(sp_sw9[1]);
        Serial.print("' with label '");
        Serial.print(sp_sw9[0]);
        if (sp_sw9_default == "ON") {
          pinMode(sp_sw9_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw9_pin.toInt(), LOW);
          Serial.print("' and default 'ON");
        } else {
          pinMode(sp_sw9_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw9_pin.toInt(), HIGH);
          Serial.print("' and default 'OFF");
        }
      } else if (sp_check == "sp_sw10") {
        String sp_sw10_pin = sp_sw10[1];
        String sp_sw10_default = sp_sw10[2];
        Serial.print(sp_sw10[1]);
        Serial.print("' with label '");
        Serial.print(sp_sw10[0]);
        if (sp_sw10_default == "ON") {
          pinMode(sp_sw10_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw10_pin.toInt(), LOW);
          Serial.print("' and default 'ON");
        } else {
          pinMode(sp_sw10_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw10_pin.toInt(), HIGH);
          Serial.print("' and default 'OFF");
        }
      } else if (sp_check == "sp_sw11") {
        String sp_sw11_pin = sp_sw11[1];
        String sp_sw11_default = sp_sw11[2];
        Serial.print(sp_sw11[1]);
        Serial.print("' with label '");
        Serial.print(sp_sw11[0]);
        if (sp_sw11_default == "ON") {
          pinMode(sp_sw11_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw11_pin.toInt(), LOW);
          Serial.print("' and default 'ON");
        } else {
          pinMode(sp_sw11_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw11_pin.toInt(), HIGH);
          Serial.print("' and default 'OFF");
        }
      } else if (sp_check == "sp_sw12") {
        String sp_sw12_pin = sp_sw12[1];
        String sp_sw12_default = sp_sw12[2];
        Serial.print(sp_sw12[1]);
        Serial.print("' with label '");
        Serial.print(sp_sw12[0]);
        if (sp_sw12_default == "ON") {
          pinMode(sp_sw12_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw12_pin.toInt(), LOW);
          Serial.print("' and default 'ON");
        } else {
          pinMode(sp_sw12_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw12_pin.toInt(), HIGH);
          Serial.print("' and default 'OFF");
        }
      } else if (sp_check == "sp_sw13") {
        String sp_sw13_pin = sp_sw13[1];
        String sp_sw13_default = sp_sw13[2];
        Serial.print(sp_sw13[1]);
        Serial.print("' with label '");
        Serial.print(sp_sw13[0]);
        if (sp_sw13_default == "ON") {
          pinMode(sp_sw13_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw13_pin.toInt(), LOW);
          Serial.print("' and default 'ON");
        } else {
          pinMode(sp_sw13_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw13_pin.toInt(), HIGH);
          Serial.print("' and default 'OFF");
        }
      } else if (sp_check == "sp_sw14") {
        String sp_sw14_pin = sp_sw14[1];
        String sp_sw14_default = sp_sw14[2];
        Serial.print(sp_sw14[1]);
        Serial.print("' with label '");
        Serial.print(sp_sw14[0]);
        if (sp_sw14_default == "ON") {
          pinMode(sp_sw14_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw14_pin.toInt(), LOW);
          Serial.print("' and default 'ON");
        } else {
          pinMode(sp_sw14_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw14_pin.toInt(), HIGH);
          Serial.print("' and default 'OFF");
        }
      } else if (sp_check == "sp_sw15") {
        String sp_sw15_pin = sp_sw15[1];
        String sp_sw15_default = sp_sw15[2];
        Serial.print(sp_sw15[1]);
        Serial.print("' with label '");
        Serial.print(sp_sw15[0]);
        if (sp_sw15_default == "ON") {
          pinMode(sp_sw15_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw15_pin.toInt(), LOW);
          Serial.print("' and default 'ON");
        } else {
          pinMode(sp_sw15_pin.toInt(), OUTPUT);
          digitalWrite(sp_sw15_pin.toInt(), HIGH);
          Serial.print("' and default 'OFF");
        }
      }
      Serial.print("'.\n");
    } else {
      Serial.print("SETUP: List item '");
      Serial.print(sp_check);
      Serial.print("' does not need to do anything during setup.\n");
    }
  }

  // Setup completed
  delay((lcd_bootup * 500));
  digitalWrite(pin_led, LOW);
  Serial.print("SETUP COMPLETED!\n\n");
  Serial.flush();
  delay((lcd_bootup * 500));

  // Set current LCD menu
  String lcd_check_name = sp_active[0];
  lcd.clear();
  if (lcd_height >= 4) {
    lcd.setCursor(1, 1);
    lcd.print(sp_screen_get_label(sp_active[0]));
    if (lcd_check_name.startsWith("sp_sw")) {
      lcd.setCursor(1, 2);
      lcd.print(lcd_type_sw);
    } else if (lcd_check_name.startsWith("sp_ir")) {
      lcd.setCursor(1, 2);
      lcd.print(lcd_type_ir);
    } else if (lcd_check_name.startsWith("sp_rf")) {
      lcd.setCursor(1, 2);
      lcd.print(lcd_type_rf);
    }
  } else if (lcd_height >= 2) {
    lcd.setCursor(0, 0);
    lcd.print(sp_screen_get_label(sp_active[0]));
    if (lcd_check_name.startsWith("sp_sw")) {
      lcd.setCursor(0, 1);
      lcd.print(lcd_type_sw);
    } else if (lcd_check_name.startsWith("sp_ir")) {
      lcd.setCursor(0, 1);
      lcd.print(lcd_type_ir);
    } else if (lcd_check_name.startsWith("sp_rf")) {
      lcd.setCursor(0, 1);
      lcd.print(lcd_type_rf);
    }
  } else {
    lcd.setCursor(0, 0);
    lcd.print(sp_screen_get_label(sp_active[0]));
  }

}

/* ---------------------------------------------------------------- */

int sp_blackout_timer_last = millis();
int sp_screen_total = sizeof(sp_active) / sizeof(sp_active[0]);
int sp_screen_current_idx = 0;
String sp_screen_current_name = sp_active[0];
String sp_screen_current_label = sp_screen_get_label(sp_screen_current_name);
String sp_screen_get_label(String get_name) {
  String get_name_new = get_name;
  if (get_name_new == "sp_sw0") {\
    return sp_sw0[0];
  } else if (get_name_new == "sp_sw1") {
    return sp_sw1[0];
  } else if (get_name_new == "sp_sw2") {
    return sp_sw2[0];
  } else if (get_name_new == "sp_sw3") {
    return sp_sw3[0];
  } else if (get_name_new == "sp_sw4") {
    return sp_sw4[0];
  } else if (get_name_new == "sp_sw5") {
    return sp_sw5[0];
  } else if (get_name_new == "sp_sw6") {
    return sp_sw6[0];
  } else if (get_name_new == "sp_sw7") {
    return sp_sw7[0];
  } else if (get_name_new == "sp_sw8") {
    return sp_sw8[0];
  } else if (get_name_new == "sp_sw9") {
    return sp_sw9[0];
  } else if (get_name_new == "sp_sw10") {
    return sp_sw10[0];
  } else if (get_name_new == "sp_sw11") {
    return sp_sw11[0];
  } else if (get_name_new == "sp_sw12") {
    return sp_sw12[0];
  } else if (get_name_new == "sp_sw13") {
    return sp_sw13[0];
  } else if (get_name_new == "sp_sw14") {
    return sp_sw14[0];
  } else if (get_name_new == "sp_sw15") {
    return sp_sw15[0];
  } else if (get_name_new == "sp_ir0") {
    return sp_ir0[0];
  } else if (get_name_new == "sp_ir1") {
    return sp_ir1[0];
  } else if (get_name_new == "sp_ir2") {
    return sp_ir2[0];
  } else if (get_name_new == "sp_ir3") {
    return sp_ir3[0];
  } else if (get_name_new == "sp_ir4") {
    return sp_ir4[0];
  } else if (get_name_new == "sp_ir5") {
    return sp_ir5[0];
  } else if (get_name_new == "sp_ir6") {
    return sp_ir6[0];
  } else if (get_name_new == "sp_ir7") {
    return sp_ir7[0];
  } else if (get_name_new == "sp_rf0") {
    return sp_rf0[0];
  } else if (get_name_new == "sp_rf1") {
    return sp_rf1[0];
  } else if (get_name_new == "sp_rf2") {
    return sp_rf2[0];
  } else if (get_name_new == "sp_rf3") {
    return sp_rf3[0];
  } else if (get_name_new == "sp_rf4") {
    return sp_rf4[0];
  } else if (get_name_new == "sp_rf5") {
    return sp_rf5[0];
  } else if (get_name_new == "sp_rf6") {
    return sp_rf6[0];
  } else if (get_name_new == "sp_rf7") {
    return sp_rf7[0];
  }
}

/* ---------------------------------------------------------------- */

// Loop
void loop() {

  // Check input
  if (control == true && (digitalRead(pin_control_ok) == HIGH)) {

    lcd.backlight();
    if (sp_screen_current_name.startsWith("sp_sw")) {
      Serial.print("List item activated: (");
      Serial.print(sp_screen_current_idx);
      Serial.print(") '");
      Serial.print(sp_screen_current_name);
      Serial.print("' with label '");
      Serial.print(sp_screen_current_label);
      Serial.print("' turn '");
      if (sp_screen_current_name == "sp_sw0") {
        String sp_sw0_pin = sp_sw0[1];
        if (digitalRead(sp_sw0_pin.toInt()) == HIGH) {
          digitalWrite(sp_sw0_pin.toInt(), LOW);
          Serial.print("ON");
        } else {
          digitalWrite(sp_sw0_pin.toInt(), HIGH);
          Serial.print("OFF");
        }
      } else if (sp_screen_current_name == "sp_sw1") {
        String sp_sw1_pin = sp_sw1[1];
        if (digitalRead(sp_sw1_pin.toInt()) == HIGH) {
          digitalWrite(sp_sw1_pin.toInt(), LOW);
          Serial.print("ON");
        } else {
          digitalWrite(sp_sw1_pin.toInt(), HIGH);
          Serial.print("OFF");
        }
      } else if (sp_screen_current_name == "sp_sw2") {
        String sp_sw2_pin = sp_sw2[1];
        if (digitalRead(sp_sw2_pin.toInt()) == HIGH) {
          digitalWrite(sp_sw2_pin.toInt(), LOW);
          Serial.print("ON");
        } else {
          digitalWrite(sp_sw2_pin.toInt(), HIGH);
          Serial.print("OFF");
        }
      } else if (sp_screen_current_name == "sp_sw3") {
        String sp_sw3_pin = sp_sw3[1];
        if (digitalRead(sp_sw3_pin.toInt()) == HIGH) {
          digitalWrite(sp_sw3_pin.toInt(), LOW);
          Serial.print("ON");
        } else {
          digitalWrite(sp_sw3_pin.toInt(), HIGH);
          Serial.print("OFF");
        }
      } else if (sp_screen_current_name == "sp_sw4") {
        String sp_sw4_pin = sp_sw4[1];
        if (digitalRead(sp_sw4_pin.toInt()) == HIGH) {
          digitalWrite(sp_sw4_pin.toInt(), LOW);
          Serial.print("ON");
        } else {
          digitalWrite(sp_sw4_pin.toInt(), HIGH);
          Serial.print("OFF");
        }
      } else if (sp_screen_current_name == "sp_sw5") {
        String sp_sw5_pin = sp_sw5[1];
        if (digitalRead(sp_sw5_pin.toInt()) == HIGH) {
          digitalWrite(sp_sw5_pin.toInt(), LOW);
          Serial.print("ON");
        } else {
          digitalWrite(sp_sw5_pin.toInt(), HIGH);
          Serial.print("OFF");
        }
      } else if (sp_screen_current_name == "sp_sw6") {
        String sp_sw6_pin = sp_sw6[1];
        if (digitalRead(sp_sw6_pin.toInt()) == HIGH) {
          digitalWrite(sp_sw6_pin.toInt(), LOW);
          Serial.print("ON");
        } else {
          digitalWrite(sp_sw6_pin.toInt(), HIGH);
          Serial.print("OFF");
        }
      } else if (sp_screen_current_name == "sp_sw7") {
        String sp_sw7_pin = sp_sw7[1];
        if (digitalRead(sp_sw7_pin.toInt()) == HIGH) {
          digitalWrite(sp_sw7_pin.toInt(), LOW);
          Serial.print("ON");
        } else {
          digitalWrite(sp_sw7_pin.toInt(), HIGH);
          Serial.print("OFF");
        }
      } else if (sp_screen_current_name == "sp_sw8") {
        String sp_sw8_pin = sp_sw8[1];
        if (digitalRead(sp_sw8_pin.toInt()) == HIGH) {
          digitalWrite(sp_sw8_pin.toInt(), LOW);
          Serial.print("ON");
        } else {
          digitalWrite(sp_sw8_pin.toInt(), HIGH);
          Serial.print("OFF");
        }
      } else if (sp_screen_current_name == "sp_sw9") {
        String sp_sw9_pin = sp_sw9[1];
        if (digitalRead(sp_sw9_pin.toInt()) == HIGH) {
          digitalWrite(sp_sw9_pin.toInt(), LOW);
          Serial.print("ON");
        } else {
          digitalWrite(sp_sw9_pin.toInt(), HIGH);
          Serial.print("OFF");
        }
      } else if (sp_screen_current_name == "sp_sw10") {
        String sp_sw10_pin = sp_sw10[1];
        if (digitalRead(sp_sw10_pin.toInt()) == HIGH) {
          digitalWrite(sp_sw10_pin.toInt(), LOW);
          Serial.print("ON");
        } else {
          digitalWrite(sp_sw10_pin.toInt(), HIGH);
          Serial.print("OFF");
        }
      } else if (sp_screen_current_name == "sp_sw11") {
        String sp_sw11_pin = sp_sw11[1];
        if (digitalRead(sp_sw11_pin.toInt()) == HIGH) {
          digitalWrite(sp_sw11_pin.toInt(), LOW);
          Serial.print("ON");
        } else {
          digitalWrite(sp_sw11_pin.toInt(), HIGH);
          Serial.print("OFF");
        }
      } else if (sp_screen_current_name == "sp_sw12") {
        String sp_sw12_pin = sp_sw12[1];
        if (digitalRead(sp_sw12_pin.toInt()) == HIGH) {
          digitalWrite(sp_sw12_pin.toInt(), LOW);
          Serial.print("ON");
        } else {
          digitalWrite(sp_sw12_pin.toInt(), HIGH);
          Serial.print("OFF");
        }
      } else if (sp_screen_current_name == "sp_sw13") {
        String sp_sw13_pin = sp_sw13[1];
        if (digitalRead(sp_sw13_pin.toInt()) == HIGH) {
          digitalWrite(sp_sw13_pin.toInt(), LOW);
          Serial.print("ON");
        } else {
          digitalWrite(sp_sw13_pin.toInt(), HIGH);
          Serial.print("OFF");
        }
      } else if (sp_screen_current_name == "sp_sw14") {
        String sp_sw14_pin = sp_sw14[1];
        if (digitalRead(sp_sw14_pin.toInt()) == HIGH) {
          digitalWrite(sp_sw14_pin.toInt(), LOW);
          Serial.print("ON");
        } else {
          digitalWrite(sp_sw14_pin.toInt(), HIGH);
          Serial.print("OFF");
        }
      } else if (sp_screen_current_name == "sp_sw15") {
        String sp_sw15_pin = sp_sw15[1];
        if (digitalRead(sp_sw15_pin.toInt()) == HIGH) {
          digitalWrite(sp_sw15_pin.toInt(), LOW);
          Serial.print("ON");
        } else {
          digitalWrite(sp_sw15_pin.toInt(), HIGH);
          Serial.print("OFF");
        }
      }
      Serial.print("'.\n");
    } else if (sp_screen_current_name.startsWith("sp_ir")) {
      // IR output here (some day)
    } else if (sp_screen_current_name.startsWith("sp_rf")) {
      // RF output here (some day)
    }
    digitalWrite(pin_led, HIGH);
    delay(delay_control);
    digitalWrite(pin_led, LOW);
    sp_blackout_timer_last = millis();

  } else if (control == true && (digitalRead(pin_control_nxt) == HIGH || digitalRead(pin_control_prv) == HIGH)) {

    lcd.backlight();
    if (digitalRead(pin_control_nxt) == HIGH) {
      if (sp_screen_current_idx < (sp_screen_total - 1)) {
        sp_screen_current_idx++;
      } else {
        sp_screen_current_idx = 0;
      }
    } else if (digitalRead(pin_control_prv) == HIGH) {
      if (sp_screen_current_idx == 0) {
        sp_screen_current_idx = (sp_screen_total - 1);
      } else {
        sp_screen_current_idx--;
      }
    }
    sp_screen_current_name = sp_active[sp_screen_current_idx];
    sp_screen_current_label = sp_screen_get_label(sp_screen_current_name);
    if (lcd_height >= 4) {
      lcd.clear();
      lcd.setCursor(1, 1);
      lcd.print(sp_screen_current_label);
      if (sp_screen_current_name.startsWith("sp_sw")) {
        lcd.setCursor(1, 2);
        lcd.print(lcd_type_sw);
      } else if (sp_screen_current_name.startsWith("sp_ir")) {
        lcd.setCursor(1, 2);
        lcd.print(lcd_type_ir);
      } else if (sp_screen_current_name.startsWith("sp_rf")) {
        lcd.setCursor(1, 2);
        lcd.print(lcd_type_rf);
      }
    } else if (lcd_height >= 2) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(sp_screen_current_label);
      if (sp_screen_current_name.startsWith("sp_sw")) {
        lcd.setCursor(0, 1);
        lcd.print(lcd_type_sw);
      } else if (sp_screen_current_name.startsWith("sp_ir")) {
        lcd.setCursor(0, 1);
        lcd.print(lcd_type_ir);
      } else if (sp_screen_current_name.startsWith("sp_rf")) {
        lcd.setCursor(0, 1);
        lcd.print(lcd_type_rf);
      }
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(sp_screen_current_label);
    }
    Serial.print("New list item set: (");
    Serial.print(sp_screen_current_idx);
    Serial.print(") '");
    Serial.print(sp_screen_current_name);
    Serial.print("' with label '");
    Serial.print(sp_screen_current_label);
    Serial.print("'.\n");
    digitalWrite(pin_led, HIGH);
    delay(delay_control);
    digitalWrite(pin_led, LOW);
    sp_blackout_timer_last = millis();

  } else if (control_ir == true && ("IR input" == "not yet implemented")) {

    lcd.backlight();
    // IR input here (some day)
    digitalWrite(pin_led, HIGH);
    delay(delay_control_ir);
    digitalWrite(pin_led, LOW);
    sp_blackout_timer_last = millis();

  } else if (control_rf == true && ("RF input" == "not yet implemented")) {

    lcd.backlight();
    // RF input here (some day)
    digitalWrite(pin_led, HIGH);
    delay(delay_control_rf);
    digitalWrite(pin_led, LOW);
    sp_blackout_timer_last = millis();

  } else {

    delay(delay_loop);

  }

  // LCD blackout
  int sp_blackout_timer_allow = lcd_blackout * 1000;
  int sp_blackout_timer_total = millis() - sp_blackout_timer_last;
  if (sp_blackout_timer_total > sp_blackout_timer_allow) {
    lcd.noBacklight();
  }

}

