#include <sys/inotify.h>
#include <unistd.h>
#include <iostream>

int main() {
    int fd = inotify_init();
    if (fd < 0) return 1;

    int wd = inotify_add_watch(fd, ".", IN_CREATE | IN_DELETE);
    if (wd < 0) return 1;

    char buffer[1024];
    while (true) {
        int length = read(fd, buffer, sizeof(buffer));
        int i = 0;
        while (i < length) {
            auto* event = (struct inotify_event*)&buffer[i];
            if (event->mask & IN_CREATE)
                std::cout << "Fichier créé : " << event->name << std::endl;
            if (event->mask & IN_DELETE)
                std::cout << "Fichier supprimé : " << event->name << std::endl;
            i += sizeof(struct inotify_event) + event->len;
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);
}
