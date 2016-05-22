#define serialOBDCAN Serial2
#define serialDebug Serial

unsigned long tierOneCounter = 0, tierTwoCounter = 0, tierThreeCounter = 0;
unsigned long tierFourCounter = 0, tierFiveCounter = 0, timer = micros();
unsigned short timeDiff;
short maxMPH = 0;

void setup() {
    //serialDebug.begin(115200);
    if(!STN1110Setup()){
        // do error thing
    }
}

void loop() {
    //  timing tiers:    tierOneCounter - 10ms
    //                   tierTwoCounter - 40ms
    //                   tierThreeCounter - 75ms
    //                   tierFourCounter - 250ms
    //                   tierFiveCounter - 700ms
    if(micros()<timer){
        timer=micros();
    }
    timeDiff = micros()-timer;
    timer=micros();
    //serialDebug.println(timeDiff);
    tierOneCounter += timeDiff;
    tierTwoCounter += timeDiff;
    tierThreeCounter += timeDiff;
    tierFourCounter += timeDiff;
    tierFiveCounter += timeDiff;

    if(tierOneCounter>10000){
        tierOneCounter=0;
        // do teir one tasks
    }
    if(tierTwoCounter>40000){
        tierTwoCounter=0;
        // do teir two tasks
    }
    if(tierThreeCounter>75000){
        tierThreeCounter=0;
        // do teir three tasks
        byte MilesPerHour = 0, EngineTemp = 0, TransTemp = 0, ECUvoltage = 0;
        short engineRPM = 0;

        //MilesPerHour = VehicleSpeed();
        if(MilesPerHour>maxMPH){maxMPH=MilesPerHour;}
        //engineRPM = EngineRPM();
        //EngineTemp = EngineTemp();
        //TransTemp = TransTemp();
        //fakeECUinfo(&MilesPerHour,&EngineTemp,&TransTemp,&ECUvoltage,&engineRPM,&AirFuelRatio,&FuelRate);
        //short rand = random(65535);
        //short rand = time2 - time1;
        //byte packetBuffer[MAX_PACKET_SIZE]={EngineTemp,MilesPerHour,(engineRPM>>8),engineRPM,ECUvoltage,TransTemp,((byte)(rand>>8)),(byte)rand,0,0,0,0};
    }
    if(tierFourCounter>250000){
        tierFourCounter=0;
        // do teir four tasks
    }
    if(tierFiveCounter>700000){
        tierFiveCounter=0;
        // do teir five tasks
    }
    //delay(de_lay);
}
