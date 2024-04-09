 #include "CryptoEntry.h"   
    
CryptoEntry::CryptoEntry(std::string _timestamp, double _open, double _high, double _low, double _close)
  : timestamp(_timestamp), open(_open), high(_high), low(_low), close(_close) {}