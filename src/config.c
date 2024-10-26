#include "config.h"
#include <toml.h>
#include <string.h>
#include <stdlib.h>

Config *config_init(const char *filename) {
    char errbuf[200];

    Config *config = calloc(1, sizeof(Config));
    if (!config) {
        fprintf(stderr, "Allocation failed: config\n");
        return NULL;
    }
    
    toml_table *tbl = toml_parse_file(filename, errbuf, sizeof(errbuf));
    if (!tbl) {
        fprintf(stderr, "Error when parsing toml file: %s", errbuf);
        free(config);
        return NULL;
    }

    toml_value delimeter = toml_table_string(tbl, "delimeter");
    if (!delimeter.ok) {
        fprintf(stderr, "Error when reading the value of delimeter\n");
        toml_table_free(tbl);
        return NULL;
    }
    strncpy(config->delimeter, delimeter.value.String, MAX_DELIM_LEN);
    toml_string_free(delimeter.value.String);

    toml_array *arr = toml_table_array(tbl, "block");
    if (!arr) {
        fprintf(stderr, "There's no array of tables <block>.\n");
        toml_table_free(tbl);
    }

    config->blocks_len = toml_array_len(arr);
    if (config->blocks_len > MAX_BLOCKS_LEN) {
        fprintf(stderr, "There's %d blocks defined in the config file, but the maximum numbers of blocks allowed is %d\n", config->blocks_len, MAX_BLOCKS_LEN);
        toml_table_free(tbl);
        return NULL;
    }
    config->blocks = calloc(1, sizeof(Block)*config->blocks_len);
    if (!config->blocks) {
        fprintf(stderr, "Allocation failed: blocks\n");
        toml_table_free(tbl);
        return NULL;
    }

    toml_table *ta = {0};
    toml_value cmd = {0};
    toml_value interval = {0};
    toml_value sig = {0};

    for (u32 i = 0; i < config->blocks_len; i++) {
        ta = toml_array_table(arr, i);
        interval = toml_table_int(ta, "interval");
        if (!interval.ok) {
            fprintf(stderr, "Error when reading value from key <%s> at table of index <%d>\n", "interval" ,i);
            toml_table_free(tbl);
            config_deinit(config);
            return NULL;
        }
        sig = toml_table_int(ta, "signal");
        if (!sig.ok) {
            fprintf(stderr, "Error when reading value from key <%s> at table of index <%d>\n", "signal" ,i);
            toml_table_free(tbl);
            config_deinit(config);
            return NULL;
        }
        cmd = toml_table_string(ta, "command");
        if (!cmd.ok) {
            fprintf(stderr, "Error when reading value from key <%s> at table of index <%d>\n", "command" ,i);
            toml_table_free(tbl);
            config_deinit(config);
            return NULL;
        }
        const u32 cmdlen = strlen(cmd.value.String);
        config->blocks[i].command = calloc(1, cmdlen+1);
        if (!config->blocks[i].command) {
            fprintf(stderr, "Allocation failed: command\n");
            toml_table_free(tbl);
            config_deinit(config);
            return NULL;
        }
        
        strncpy(config->blocks[i].command, cmd.value.String, cmdlen);
        config->blocks[i].interval = interval.value.Integer;
        config->blocks[i].signal = sig.value.Integer;
        toml_string_free(cmd.value.String);
    }
    toml_table_free(tbl);
    return config;
}

void config_deinit(Config *config) {
    for (u32 i = 0; i < config->blocks_len; i++) {
        if (config->blocks[i].command) {
            free(config->blocks[i].command);
        }
    }
    free(config->blocks);
    free(config);
}
