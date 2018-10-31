# Easehand

A Hand-Assistive Device

## Directory Overview

Below is a listing of the directory contents:

```
.
├── firmware
│   ├── bleuart2/
│   └── test/
└── skill
    ├── easehand.py
    └── speech_assets/
```

First, the firmware directory contains the source code needed to control 
the [nRF52832 Adafruit Feather Bluefruit LE](https://www.adafruit.com/product/3406)
with the [8-Channel PWM Servo FeatherWing](https://www.adafruit.com/product/2928). 
Within the firmware directory, "bleuart2" contains the code used during the final demo, 
and "test" contains small programs that were useful during the development process.

The skill directory contains source code for Alexa Skill that compliments our device. 
The script "easehand.py" is the main entrypoint of the skill and requires Python 3.6 
runtime (or greater). The "speech_assets" directory contains additional configuration files
which are needed in the [Alexa Developer Console](https://developer.amazon.com/alexa).
At the time of writing, the only revelant file is "InteractionModel.json" which contains 
the majority of the skill configuration. 



## Contact

This document is by no means a comprehensive overview of the system we implemented here.
If you have any questions about the source code, or the hardware used, 
please do not hesitate to reach out by opening an issue for this
project! 



