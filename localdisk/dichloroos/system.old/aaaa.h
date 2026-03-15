BYTE identifyata(BYTE channel, BYTE drive);
BYTE readata(BYTE channel, BYTE drive, DWORD lba, WORD count, BYTE* buffer);
BYTE writeata(BYTE channel, BYTE drive, DWORD lba, WORD count, BYTE* buffer);