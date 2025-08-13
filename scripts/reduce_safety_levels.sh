#!/bin/bash
#
# Reduces the safety levels, helpful for using bwrap

# From https://github.com/DevToys-app/DevToys/issues/1373#issuecomment-2599820594

echo "Reducing the safety levels, helpful for using bwrap"

sudo sysctl -w kernel.apparmor_restrict_unprivileged_unconfined=0
sudo sysctl -w kernel.apparmor_restrict_unprivileged_userns=0

echo "Run 'restore_safety_levels.sh' to restore these levels again"
