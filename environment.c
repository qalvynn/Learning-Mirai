#define _GNU_SOURCE

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define DEBUG_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__) /* instead of 1 million ifdef debugs just use this for printing  */


// opens /proc/cpuinfo Reads it and searches for the string " hypervisor " 
uint32_t check_cpuinfo(void) {
    int fd = open("/proc/cpuinfo", O_RDONLY);
    if (fd == -1)
        return 0;

    char buffer[1024];
    ssize_t n;
    int is_virtual = 0;

    while ((n = read(fd, buffer, sizeof(buffer)-1)) > 0) {
        buffer[n] = '\0';
        if (strstr(buffer, "hypervisor")) {
            is_virtual = 1;
            break;
        }
    }

    close(fd);
    return is_virtual;
}

uint32_t check_virtio_devices(void) {
    int fd = open("/proc/devices", O_RDONLY);
    if (fd == -1)
        return 0;

    char buf[1024];
    ssize_t n;
    int is_virtio = 0;

    while ((n = read(fd, buf, sizeof(buf)-1)) > 0) {
        buf[n] = '\0';
        if (strstr(buf, "vfio") || strstr(buf, "virtio") || strstr(buf, "virtblk")) {
            is_virtio = 1;
            break;
        }
    }

    close(fd);
    return is_virtio;
}

uint32_t check_devices(void) {
    DEBUG_PRINT("[check_devices] started\n");

    if (check_cpuinfo()) {
        DEBUG_PRINT("[check_cpuinfo] device is dirty\n");
        return 1;
    }

    if (check_virtio_devices()) {
        DEBUG_PRINT("[check_virtio_devices] device is dirty\n");
        return 1;
    }

    DEBUG_PRINT("[check_devices] device is clean\n");
    return 0;
}

int main() {
    uint32_t result = check_devices();
    DEBUG_PRINT("Result: %u\n", result);
    return 0;
}
