#define TIMEOUT 30000

boolean CheckAndDoWebServer(){
    File httpRequestFile;
    File daFile;  //  file reference for reading files
    File scriptFile;
    unsigned long time1;
    EthernetClient client = webServer.available();
    if(client){
        boolean currentLineIsBlank = true;
        boolean requestIsComplete = false;
        boolean isGETreq = false;
        boolean currentLineIsContentLengthBOOL = false;
        byte counter = 0;
        byte counter2 = 0;
        char contentLength[] = {' ',' ',' ',' ',' ',' ',' ','\0'};
        unsigned long contentLengthInt = 0;
        unsigned long fullRequestSize = 0;
        char contentLengthCompareChars[] = {"Content-Length: "};
        if(SD.exists("httpREQ.txt")){
            SD.remove("httpREQ.txt");
        }
        httpRequestFile = SD.open(("httpREQ.txt"),FILE_WRITE);
        time1 = millis();
        while(client.connected()){
            if(client.available()){
                char c = client.read();
                httpRequestFile.write(c);
                if(httpRequestFile.size()==1){
                    isGETreq=(c=='G');
                }
                if(isGETreq){
                    requestIsComplete = ((c == '\n') && currentLineIsBlank);
                }else{
                    if((c==contentLengthCompareChars[counter])&&(fullRequestSize==0)){
                        counter++;
                        currentLineIsContentLengthBOOL = (counter==16);
                    }
                    if(currentLineIsContentLengthBOOL){
                        if((c != '\r')&&(counter2<8)){
                            contentLength[counter2++] = c;
                            contentLength[counter2] = '\0';
                        }else{
                            contentLength[counter2] = '\0';
                            contentLengthInt = atoi(contentLength);
                            fullRequestSize = httpRequestFile.size();
                            fullRequestSize = fullRequestSize + contentLengthInt+2;
                            currentLineIsContentLengthBOOL = false;
                        }
                    }
                    unsigned long tempSize = httpRequestFile.size();
                    requestIsComplete = ((tempSize>fullRequestSize)&&(fullRequestSize>0));
                }
                if(requestIsComplete||((millis()-time1)>TIMEOUT)){
                    httpRequestFile.seek(0);
                    int lengthOfFirstLine = 0;
                    if(httpRequestFile.peek()=='P'){
                        isGETreq = false;
                    }
                    while(httpRequestFile.read()!='\r'){
                        lengthOfFirstLine++;
                    }
                    int filenameLength = lengthOfFirstLine-(isGETreq?14:15);
                    char requestedFile[filenameLength+1];
                    if(isGETreq){
                        httpRequestFile.seek(5);
                    }else{
                        httpRequestFile.seek(6);
                    }
                    for(int i = 0; i <filenameLength; i++){
                        requestedFile[i]=httpRequestFile.read();
                        requestedFile[i+1]='\0';
                    }
                    for(int i = 0; i < filenameLength; i++){
                        if((requestedFile[i]>='a')&&(requestedFile[i]<='z')){
                            requestedFile[i]&=0b11011111;
                        }
                    }
                    char fileEXT[4] = {requestedFile[filenameLength-3],requestedFile[filenameLength-2],requestedFile[filenameLength-1],'\0'};
                    String contentType;
                    if((fileEXT[0]=='I')&&(fileEXT[1]=='C')&&(fileEXT[2]=='O')){contentType="image/x-icon";}
                    else if((fileEXT[0]=='J')&&(fileEXT[1]=='P')&&(fileEXT[2]=='G')){contentType="image/jpeg";}
                    else if((fileEXT[0]=='G')&&(fileEXT[1]=='I')&&(fileEXT[2]=='F')){contentType="image/gif";}
                    else if((fileEXT[0]=='P')&&(fileEXT[1]=='N')&&(fileEXT[2]=='G')){contentType="image/png";}
                    else if((fileEXT[0]=='H')&&(fileEXT[1]=='T')&&(fileEXT[2]=='M')){contentType="text/html";}
                    else if((fileEXT[0]=='T')&&(fileEXT[1]=='X')&&(fileEXT[2]=='T')){contentType="text/plain";}
                    else if((fileEXT[0]=='.')&&(fileEXT[1]=='J')&&(fileEXT[2]=='S')){contentType="application/javascript";}
                    else if((fileEXT[0]=='A')&&(fileEXT[1]=='S')&&(fileEXT[2]=='L')){contentType="text/json";}
                    else if(filenameLength == 0){contentType="text/html";}
                    client.println(F("HTTP/1.1 200 OK"));
                    client.print(F("Content-Type: "));
                    client.println(contentType);
                    client.println(F("Connection: close"));
                    client.println(F("X-Dinosaur-Says: RAWR"));
                    client.println();
                    httpRequestFile.close();
                    if((fileEXT[0]=='A')&&(fileEXT[1]=='S')&&(fileEXT[2]=='L')){
                        if((requestedFile[0]=='I')&&(requestedFile[1]=='N')&&(filenameLength==6)){
                            // send json of inputs. Both analog and digital.
                            client.print(F("{\"AnalogInputs\":{\"A0\":"));
                            client.print(analogRead(0));
                            client.print(F(",\"A1\":"));
                            client.print(analogRead(1));
                            client.print(F(",\"A2\":"));
                            client.print(analogRead(2));
                            client.print(F(",\"A3\":"));
                            client.print(analogRead(3));
                            client.print(F(",\"A4\":"));
                            client.print(analogRead(4));
                            client.print(F(",\"A5\":"));
                            client.print(analogRead(5));
                            client.print(F("},\"DigitalInputs\":{\"D5\":"));
                            client.print(digitalRead(5));
                            client.print(F(",\"D6\":"));
                            client.print(digitalRead(6));
                            client.print(F(",\"D7\":"));
                            client.print(digitalRead(7));
                            client.print(F(",\"D8\":"));
                            client.print(digitalRead(8));
                            client.print(F("}}"));
                        }else{
                            scriptFile = SD.open(isGETreq?requestedFile:"httpreq.txt",FILE_READ);
                            if(!scriptFile){
                                // this would be an error condition
                                client.println(F("error"));
                            }
                        }
                    }else{
                        daFile = SD.open((filenameLength==0)?"index.htm":requestedFile);
                        if(daFile){
                            while(daFile.available()){
                                client.write(daFile.read());
                            }
                        }
                        daFile.close();
                    }
                    break;
                }
                if(c == '\n') {
                    // you're starting a new line
                    currentLineIsBlank = true;
                }else if(c != '\r'){
                    // you've gotten a character on the current line
                    currentLineIsBlank = false;
                }
            }
        }
        client.stop();
        daFile.close();
    }else{return false;}
    return true;
}
