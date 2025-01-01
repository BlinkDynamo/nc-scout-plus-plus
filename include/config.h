#ifndef CONFIG_H
#define CONFIG_H

/* config structure */
struct config {
    bool allowSpaces;
};

struct config load_config(const char *pathToConfigFile);

#endif
