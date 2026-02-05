#ifndef WS_EPAPER_CLEANUP_H
#define WS_EPAPER_CLEANUP_H

void free_gpiod_pins(void);

void wsepaper_cleanup(void);

void free_epaper_device(void);

void unbind_device(void);

void unregister_device_file(void);

void delete_device_file(void);

void unregister_device(void);

void unregister_class(void);

#endif // WS_EPAPER_CLEANUP_H
