#!/bin/bash
#
# Creates a 
#
# From https://etbe.coker.com.au/2024/04/24/ubuntu-24-04-bubblewrap/
#
# Usage:
#
#   sudo create_apparmor_bwrap_file.sh
# 


echo "Modifying the file '/etc/apparmor.d/bwrap'"

cat >/etc/apparmor.d/bwrap <<EOL


abi <abi/4.0>,
include <tunables/global>

profile bwrap /usr/bin/bwrap flags=(unconfined) {
  userns,

  # Site-specific additions and overrides. See local/README for details.
  include if exists <local/bwrap>
}

EOL

echo "Content of '/etc/apparmor.d/bwrap':"

cat /etc/apparmor.d/bwrap

echo "Running systemctl"

systemctl reload apparmor

echo "Done"


