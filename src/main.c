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

int main(void) {
  PackageManagerCommands packager = packagers[PACKAGE_MANAGER_PACMAN];
  installPackage(packager, "discord");
  return 0;
}
