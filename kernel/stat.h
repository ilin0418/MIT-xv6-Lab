#define T_DIR     1   // Directory
#define T_FILE    2   // File
#define T_DEVICE  3   // Device
#define T_SYMLINK  4   // Symbolic link

struct stat {
  int dev;     // File system's disk device 
  uint ino;    // Inode number (index number)
  short type;  // Type of file (T_FILE, T_DIR, T_DEVICE, T_SYMLINK)
  short nlink; // Number of links to file
  uint64 size; // Size of file in bytes
};
