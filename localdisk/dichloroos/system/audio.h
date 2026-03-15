//NOTE: THIS DRIVER EMULES THE SPEAKER Sound Blaster 16 DRIVER
void resetsoundblaster();
void drawdsp();
void soundblasterhandler();
void soundblasterinit();
void soundblasterdisable();
void setupdma(LPBYTE buffer, WORD size);
void playsb(LPBYTE buffer, WORD size);
void setupsoundblaster();
void playbytebeat(LPBYTE buffer, WORD size);