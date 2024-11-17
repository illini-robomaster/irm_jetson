# Motor CAN Protocol

Each CAN frame has 8 data bytes. 

CAN 2.0: 1 Mbps

## M3508 (C620 Controller)

### CAN Frame: control board -> C620

**Frame ID** : 0x200
| Byte #   | Description              | For Controller ID: |
|  ------- | ------------------------ | -------------------|
|    0     | Higher 8 bits of current |      1             |
|    1     | Lower 8 bits of current  |      1             |
|    2     | Higher 8 bits of current |      2             |
|    3     | Lower 8 bits of current  |      2             |
|    4     | Higher 8 bits of current |      3             |
|    5     | Lower 8 bits of current  |      3             |
|    6     | Higher 8 bits of current |      4             |
|    7     | Lower 8 bits of current  |      4             |

**Frame ID** : 0x1FF
| Byte #   | Description              | For Controller ID: |
|  ------- | -------------------------| -------------------|
|    0     | Higher 8 bits of current |      5             |
|    1     | Lower 8 bits of current  |      5             |
|    2     | Higher 8 bits of current |      6             |
|    3     | Lower 8 bits of current  |      6             |
|    4     | Higher 8 bits of current |      7             |
|    5     | Lower 8 bits of current  |      7             |
|    6     | Higher 8 bits of current |      8             |
|    7     | Lower 8 bits of current  |      8             |

*Note*: Current actually means torque current. It is a signed 16bit int value: -16384 ~ 16384, which corresponds to approximately -20A ~ 20A.

### CAN Frame: C620 -> control board

**Frame ID** : 0x200 + Controller ID
| Byte #   | Description                             | 
| -------- | ----------------------------------------| 
|    0     | Higher 8 bits of rotor mechanical angle | 
|    1     | Lower 8 Bits of rotor mechanical angle  |
|    2     | Higher 8 bits of rotational speed       |
|    3     | Lower 8 Bits of rotational speed        |
|    4     | Higher 8 bits of actual torque current  |
|    5     | Lower 8 Bits of actual torque current   |
|    6     | Higher 8 bits motor temperature         |
|    7     | Not used                                |

*Note*: Feedback frequency can be changed by RoboMaster Assistant by plugging in to PWM port of the C620 controller (but actuallu runs UART)

*Rotor mechanical angle*: range 0 ~ 8191 (13 bit precision), which corresponds to 0 ~ 360 $\degree$

*Rotor speed value*: has unit rpm
*Motor temperature*: has unit $\degree C$



## GM6020 

### CAN Frame: control board -> GM6020
**Frame ID** : 0x1FF
| Byte #   | Description              | For Controller ID: |
|  ------- | ------------------------ | -------------------|
|    0     | Higher 8 bits of voltage |      1             |
|    1     | Lower 8 bits of voltage  |      1             |
|    2     | Higher 8 bits of voltage |      2             |
|    3     | Lower 8 bits of voltage  |      2             |
|    4     | Higher 8 bits of voltage |      3             |
|    5     | Lower 8 bits of voltage  |      3             |
|    6     | Higher 8 bits of voltage |      4             |
|    7     | Lower 8 bits of voltage  |      4             |

**Frame ID** : 0x2FF
| Byte #   | Description              | For Controller ID: |
|  ------- | ------------------------ | -------------------|
|    0     | Higher 8 bits of voltage |      5             |
|    1     | Lower 8 bits of voltage  |      5             |
|    2     | Higher 8 bits of voltage |      6             |
|    3     | Lower 8 bits of voltage  |      6             |
|    4     | Higher 8 bits of voltage |      7             |
|    5     | Lower 8 bits of voltage  |      7             |
|    6     | Not used                 |      N/A           |
|    7     | Not used                 |      N/A           |

*Voltage*: range -30000~30000

### CAN Frame: GM6020 -> control board

**Frame ID** : 0x204 + Controller ID
| Byte #   | Description                              |
|  ------- | ---------------------------------------- | 
|    0     | Higher 8 bits of rotor mechanical angle  |
|    1     | Lower 8 bits of rotor mechanical angle   |
|    2     | Higher 8 bits of rotational speed        | 
|    3     | Lower 8 bits of rotational speed         | 
|    4     | Higher 8 bits of actual torque current   |  
|    5     | Lower 8 bits of actual torque current    | 
|    6     | Higher 8 bits of motor temperature       | 
|    7     | Not used                                 | 

*Note*: Feedback frequency can be changed by RoboMaster Assistant by plugging in to PWM port of the C620 controller (but actuallu runs UART)

*Rotor mechanical angle*: range 0 ~ 8191
*Rotational speed*: has unit rpm
