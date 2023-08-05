#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char * install;
  char * remove;
  char * update;
} PackageManagerCommands;

typedef enum {
  PACKAGE_MANAGER_APT,
  PACKAGE_MANAGER_PACMAN,
  PACKAGE_MANAGER_DNF,
  PACKAGE_MANAGER_ZYPPER
} PackageManagerType;

PackageManagerCommands packagers[] = {
  [PACKAGE_MANAGER_APT] = { .install = "apt install %s", .remove = "apt remove %s", .update = "apt update && apt upgrade" },
  [PACKAGE_MANAGER_PACMAN] = { .install = "pacman -S %s", .remove = "pacman -R %s", .update = "pacman -Syu" },
  [PACKAGE_MANAGER_DNF] = { .install = "dnf install %s", .remove = "dnf remove %s", .update = "dnf upgrade --refresh"},
  [PACKAGE_MANAGER_ZYPPER] = { .install = "zypper install %s", .remove = "zypper rm %s", .update = "zypper update"},
};

void installPackage(PackageManagerCommands packager, char * packageName) {
  char command[strlen(packageName) + strlen(packager.install)];
  sprintf(command, packager.install, packageName);
  system(command);
}

void removePackage(PackageManagerCommands packager, char * packageName) {
  char command[strlen(packageName) + strlen(packager.remove)];
  sprintf(command, packager.remove, packageName);
  system(command);
}

void upgradeSystem(PackageManagerCommands packager) {
  system(packager.update);
}

PackageManagerCommands indentifyLinuxDistroPackager() {
  FILE * fd = fopen("/etc/os-release", "r");
  if (fd == NULL) {
    goto MISS_SYS;
  }

  fseek(fd, 0, SEEK_END);
  int contentLength = ftell(fd);
  rewind(fd);

  char * contentBuffer = (char *) calloc(contentLength, sizeof(char));
  fread(contentBuffer, contentLength, 1, fd);
  fclose(fd);

  char * status = strstr(contentBuffer, "manjaro");
  if (status != NULL) return packagers[PACKAGE_MANAGER_PACMAN];

  status = strstr(contentBuffer, "arch");
  if (status != NULL) return packagers[PACKAGE_MANAGER_PACMAN];

  status = strstr(contentBuffer, "ubuntu");
  if (status != NULL) return packagers[PACKAGE_MANAGER_APT];

  status = strstr(contentBuffer, "debian");
  if (status != NULL) return packagers[PACKAGE_MANAGER_APT];

  status = strstr(contentBuffer, "kali");
  if (status != NULL) return packagers[PACKAGE_MANAGER_APT];

  status = strstr(contentBuffer, "elementary");
  if (status != NULL) return packagers[PACKAGE_MANAGER_APT];

  status = strstr(contentBuffer, "zorin");
  if (status != NULL) return packagers[PACKAGE_MANAGER_APT];

  status = strstr(contentBuffer, "opensuse");
  if (status != NULL) return packagers[PACKAGE_MANAGER_ZYPPER];

  status = strstr(contentBuffer, "fedora");
  if (status != NULL) return packagers[PACKAGE_MANAGER_DNF];

  goto MISS_SYS;

MISS_SYS: 
  fprintf(stderr, "ERROR: distro not indentified.\n");
  exit(1);
}

int main(int argc, char ** argv) {

  if (argc != 3) {
    fprintf(stderr, "ERROR: arguments are wrong.\n");
    return 1;
  }

  PackageManagerCommands packager = indentifyLinuxDistroPackager();

  if (strcmp(argv[1], "get") == 0) {
    installPackage(packager, argv[2]);
  } else if (strcmp(argv[1], "unget") == 0) {
    removePackage(packager, argv[2]);
  } else if (strcmp(argv[1], "sync") == 0 && strcmp(argv[2], "sys") == 0) {
    upgradeSystem(packager);
  } else {
    fprintf(stderr, "ERROR: arguments are wrong.\n");
  }

  return 0;
}
