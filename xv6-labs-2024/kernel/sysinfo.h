struct sysinfo {
  uint64 freemem;   // amount of free memory (bytes)
  uint64 nproc;     // number of process
  uint64 avgproc;   // load average 
};

int
systeminfo(uint64);
