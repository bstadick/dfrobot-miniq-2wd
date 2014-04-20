#ifndef Notes_h_
#define Notes_h_

//      Name     Period(ms) Frequency(Hz)
#define A0       36.36      //27.5
#define B0       32.40      //30.868

#define C1       30.58      //32.703
#define C1S      28.86      //34.648
#define D1B      28.86      //34.648
#define D1       27.24      //36.708
#define D1S      25.71      //38.891
#define E1B      25.71      //38.891
#define E1       24.27      //41.203
#define F1       22.91      //43.654
#define F1S      21.62      //46.249
#define G1B      21.62      //46.249
#define G1       20.41      //48.999
#define G1S      19.26      //51.913
#define A1B      19.26      //51.913
#define A1       18.18      //50.000
#define A1S      17.16      //58.270
#define B1B      17.16      //58.270
#define B1       16.20      //61.735

#define C2       15.29      //65.406
#define D2       13.62      //73.416
#define E2       12.13      //82.407
#define F2       11.45      //87.307
#define G2       10.20      //97.999
#define A2       9.091      //110.00
#define B2       8.099      //123.47

#define C3       7.645      //130.81
#define D3       6.811      //146.83
#define E3       6.068      //164.81
#define F3       5.727      //174.61
#define G3       5.102      //196.00
#define A3       4.545      //220.00
#define B3       4.050      //246.94

#define C4       3.822      //261.63
#define D4       3.405      //293.67
#define E4       3.034      //329.63
#define F4       2.863      //349.23
#define G4       2.551      //392.00
#define A4       2.273      //440.00
#define B4       2.025      //493.88

#define C5       1.910      //523.25
#define D5       1.703      //587.33
#define E5       1.517      //659.26
#define F5       1.432      //698.46
#define G5       1.276      //783.99
#define A5       1.136      //880.00
#define B5       1.012      //987.77

#define C6       0.9556     //1046.5
#define D6       0.8513     //1174.7
#define E6       0.7584     //1318.5
#define F6       0.7159     //1396.9
#define G6       0.6378     //1568.0
#define A6       0.5682     //1760.0
#define B6       0.5062     //1975.5

#define C7       0.4778     //2093.0
#define D7       0.4257     //2349.3
#define E7       0.3792     //2637.0
#define F7       0.3580     //2793.0
#define G7       0.3189     //3136.0
#define A7       0.2841     //3520.0
#define B7       0.2531     //3951.1

#define C8       0.2389     //4186.0
#define ZERO     0x00       //Off

//************** More General Notes **************

//      Name     Period(ms) Frequency(Hz)
#define DO_L     E2       //262
#define DOA_L    E4       //277
#define RE_L     E5       //294
#define REA_L    E7       //311
#define MI_L     E8       //330
#define FA_L     EA       //349
#define FAA_L    EB       //370
#define SO_L     EC       //392
#define SOA_L    ED       //415
#define LA_L     EE       //440
#define LAA_L    EF       //466
#define TI_L     F0       //494
#define DO       F1       //523
#define DOA      F2       //554
#define RE       F3       //587
#define REA      F3       //622
#define MI       F4       //659
#define FA       F5       //698
#define FAA      F5       //740
#define SO       F6       //784
#define SOA      F7       //831
#define LA       F7       //880
#define LAA      F8       //932
#define TI       F8       //988
#define DO_H     F9       //1046
#define DOA_H    F9       //1109
#define RE_H     F9       //1175
#define REA_H    FA       //1245
#define MI_H     FA       //1318
#define FA_H     FA       //1397
#define FAA_H    FB       //1480
#define SO_H     FB       //1568
#define SOA_H    FB       //1661
#define LA_H     FC       //1760
#define LAA_H    FC       //1865
#define TI_H     FC       //1976
#define ZERO     0        //Off

//************** Notes for the tone() function **************

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

#endif