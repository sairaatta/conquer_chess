#!/bin/bash
# From https://github.com/DevToys-app/DevToys/issues/1373#issuecomment-2599820594

# commands to restore the default
sudo sysctl -w kernel.apparmor_restrict_unprivileged_unconfined=1
sudo sysctl -w kernel.apparmor_restrict_unprivileged_userns=1

echo "Safety levels restored :-)"
