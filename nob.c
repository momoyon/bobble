#define NOB_IMPLEMENTATION
#include "include/nob.h"
#include <string.h>
#include <stdbool.h>

#define SRC_PATH "./src/"
#define BINARY_NAME "bobble"

const char *_include_paths[] = {
    "./include",
    "./thirdparty/raylib-5.5/include/"
};

const char *_lib_paths[] = {
    "./thirdparty/raylib-5.5/lib/"
};

const char *_libs[] = {
    "m",
};

const char *_linker_flags[] = {
#ifdef __linux__
    "./thirdparty/raylib-5.5/lib/libraylib.a"
#endif
};

const char *_warnings[] = {
    "-Wall",
    "-Wextra",
    "-Wswitch-enum",
    "-Werror=switch-enum",
    "-Wno-char-subscripts",
    "-Wno-sign-compare",
    "-Wno-type-limits",
    "-Wno-unused-variable",
    "-Wno-unused-but-set-variable",
};

void usage(FILE *f, const char *program);
void help(FILE *f, const char *program);

void compiler(Cmd *cmd);
void include_paths(Cmd *cmd);
void lib_paths(Cmd *cmd);
void libs(Cmd *cmd);

int main(int argc, char **argv) {
    GO_REBUILD_URSELF(argc, argv);
    const char *program = shift_args(&argc, &argv);
    Cmd cmd = {0};


    // :options
    bool debug = false;
    bool run = false;

    while (argc > 0) {
        const char *arg = shift_args(&argc, &argv);

        if (strcmp(arg, "debug") == 0) {
            debug = true;
        } else if (strcmp(arg, "help") == 0) {
            help(stdout, program);
            return 0;
        } else if (strcmp(arg, "clean") == 0) {
            delete_file(BINARY_NAME);
            delete_file(BINARY_NAME"-debug");
            return 0;
        } else if (strcmp(arg, "run") == 0) {
            run = true;
        } else {
            nob_log(ERROR, "Unrecognized subcommand `%s`", arg);
            return 1;
        }
    }

    const char *binary = debug ? BINARY_NAME"-debug" : BINARY_NAME;

    // NOTE: Collect source files
    File_Paths src_files = {0};
    if (!read_entire_dir(SRC_PATH, &src_files)) {
        nob_log(ERROR, "Failed to read directory %s", SRC_PATH);
        return 1;
    }

    compiler(&cmd);
    cmd_append(&cmd, "-o", binary);
    if (debug) {
        cmd_append(&cmd, "-DDEBUG");
    }
    for (int i = 0; i < src_files.count; ++i) {
        const char *src_file = src_files.items[i];

        if (get_file_type(temp_sprintf("%s%s", SRC_PATH, src_file)) == FILE_REGULAR) {
            cmd_append(&cmd, temp_sprintf("%s%s", SRC_PATH, src_file));
        }
    }
    include_paths(&cmd);
    lib_paths(&cmd);
    libs(&cmd);

    // cmd_append(&cmd, "cc", "-Wall", "-Wextra", "-o", "", "main.c");
    if (!cmd_run(&cmd)) return 1;

    if (run) {
        cmd_append(&cmd, temp_sprintf("./%s", binary));
        if (!cmd_run(&cmd)) return 1;

    }
    return 0;
}


void help(FILE *f, const char *program) {
    usage(f, program);
    fprintf(f, "\n");
    fprintf(f, "Subcommands: \n");
    fprintf(f, " help  - Prints this help message.\n");
    fprintf(f, " debug - Enables debug build.\n");
    fprintf(f, " clean - Cleans the build files.\n");
    fprintf(f, " run   - Runs the built binary.\n");
}

void usage(FILE *f, const char *program) {
    fprintf(f, "Usage: %s [subcmd]\n", program);
}


void compiler(Cmd *cmd) {
    cmd->count = 0;
    cmd_append(cmd, "cc", "-Wall", "-Wextra");

    for (int i = 0; i < ARRAY_LEN(_warnings); ++i) {
        cmd_append(cmd, _warnings[i]);
    }
}

void include_paths(Cmd *cmd) {
    for (int i = 0; i < ARRAY_LEN(_include_paths); ++i) {
        cmd_append(cmd, temp_sprintf("-I%s", _include_paths[i]));
    }
}

void lib_paths(Cmd *cmd) {
    for (int i = 0; i < ARRAY_LEN(_lib_paths); ++i) {
        cmd_append(cmd, temp_sprintf("-L%s", _lib_paths[i]));
    }
}

void libs(Cmd *cmd) {
    for (int i = 0; i < ARRAY_LEN(_libs); ++i) {
        cmd_append(cmd, temp_sprintf("-l%s", _libs[i]));
    }

    for (int i = 0; i < ARRAY_LEN(_linker_flags); ++i) {
        cmd_append(cmd, temp_sprintf("-Wl,%s", _linker_flags[i]));
    }
}
