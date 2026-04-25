BYTE identifyata(BYTE channel, BYTE drive);
void readfromata(BYTE channel, BYTE drive, DWORD lba, WORD* buffer);
void writebyte(BYTE channel, BYTE drive, DWORD lba, BYTE data);