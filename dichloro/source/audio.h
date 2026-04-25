void resetaudio();
void writedsp(BYTE cmd);
void audiohandler();
void initaudioboard();
void desableaudio();
void setupdma(LPBYTE buffer, WORD size);
void audioplay(LPBYTE buffer, WORD size);
void setupaudioboard();
void playbytebeat(LPBYTE buffer, WORD size);