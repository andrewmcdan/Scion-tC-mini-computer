void PinsSetup(){
    pinMode(DueInt, INPUT);
    pinMode(DueInt, OUTPUT);
    digitalWrite(DueInt, LOW);
    pinMode(DueRTS, INPUT);
    pinMode(DueRTS, OUTPUT);
    digitalWrite(DueRTS, HIGH);
    pinMode(44, INPUT);
    pinMode(44, OUTPUT);
    digitalWrite(44, HIGH);
    pinMode(RasPitoDueDataReq, INPUT);
    analogWriteResolution(12);
    analogReadResolution(12);
    analogWrite(DAC0, 4095);
    analogWrite(DAC1, 0);
    digitalWrite(DueInt, HIGH);
}
