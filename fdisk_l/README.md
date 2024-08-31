
# fdskL

A command-line utility for displaying partition information on disk devices. This tool supports both MBR (Master Boot Record) and GPT (GUID Partition Table) partition schemes and provides detailed information about each partition.

## Features

- **MBR Info:**
  - Displays partition details including start and end sectors, total sectors, size, and type.

- **GPT Info:**
  - Displays partition details including start and end LBAs (Logical Block Addresses), size, and type.
  - Recognizes common partition types such as Linux filesystem, EFI System, and BIOS boot.

- **Device Iteration:**
  - Automatically iterates over all `/dev/sd*` devices and displays information for each.



## Usage

## **Compile the program:**

   ```sh
   gcc -o fd fdskL.c -luuid
   ```

### Inspect a Specific Device

provide the device path as an argument:

```sh
belal@Ubuntu:~/Desktop/st/fdisk$ sudo ./fd /dev/sda

Disk /dev/sda: 50.0 GiB, 53687091200 bytes, 104857600 sectors
Disk model: VBOX HARDDISK                           
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: gpt
Disk identifier: 8df3f76b-8e35-ef41-9894-f850064321e0

Device       Start       End   Sectors  Size Type
/dev/sda1         2048       4095       2048   0.0G BIOS boot
/dev/sda2         4096    1054719    1050624   0.5G EFI System
/dev/sda3      1054720  104855551  103800832  49.5G Linux filesystem
```

### Iterate Over All `/dev/sd*` Devices

To automatically inspect all devices matching `/dev/sd*`, run the utility without arguments:

```sh
belal@Ubuntu:~/Desktop/st/fdisk$ sudo ./fd

Disk /dev/sda: 50.0 GiB, 53687091200 bytes, 104857600 sectors
Disk model: VBOX HARDDISK                           
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: gpt
Disk identifier: 8df3f76b-8e35-ef41-9894-f850064321e0

Device       Start       End   Sectors  Size Type
/dev/sda1         2048       4095       2048   0.0G BIOS boot
/dev/sda2         4096    1054719    1050624   0.5G EFI System
/dev/sda3      1054720  104855551  103800832  49.5G Linux filesystem

Disk /dev/sdb: 25.0 GiB, 26843545600 bytes, 52428800 sectors
Disk model: VBOX HARDDISK                           
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: dos
Disk identifier: 0x3ac6739b

Device     Boot   Start      End  Sectors Size Id Type
/dev/sdb1        2048    2099199    2097152   1G 83 Linux
/dev/sdb4     2099200   52428799   50329600  23G  5 Extended
/dev/sdb5     2101248    4198399    2097152   1G 83 Linux
```





