#include "core.h"
#include <stdio.h>

void core_init(void) {
    core_log("Core initialized", 1);
}

void core_run(void) {
    while (1) {
        core_dispatch_events();
        // game update + render
        break; // temporaire
    }
}

void core_dispatch_events(void) {
    // gestion des événements (clavier / souris)
}

void core_log(const char *msg, int level) {
    printf("[LOG %d] %s\n", level, msg);
}
