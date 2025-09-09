#!/bin/bash

# This is the post-install script for the run-guit package
# It will run after the package is installed.

echo "Running post-installation steps for run-guit..."

# Ensure the run-guit directory exists in /var/lib
mkdir -p /var/lib/run-guit

# Make sure the binary is executable
chmod +x /usr/local/bin/run-guit

# Create symlink for easier access
ln -sf /usr/local/bin/run-guit /usr/bin/run-guit

# Additional setup if necessary
# E.g., start a service or setup configs
# (Uncomment if needed)
# ln -sf /etc/xdg/run-guit/config.ini /var/lib/run-guit/config.ini

# Set permissions for the application directory
chown -R root:root /var/lib/run-guit

# Remove any old polkit rules
if [ -f /etc/polkit-1/rules.d/50-run-guit.rules ]; then
    rm /etc/polkit-1/rules.d/50-run-guit.rules
fi

# Install polkit policy file
POLICY_FILE="com.yourname.runguit.policy"
POLICY_SRC_PATH="./$POLICY_FILE"
POLICY_DEST_PATH="/usr/share/polkit-1/actions/$POLICY_FILE"

if [ -f "$POLICY_SRC_PATH" ]; then
    cp "$POLICY_SRC_PATH" "$POLICY_DEST_PATH"
    chmod 644 "$POLICY_DEST_PATH"
    echo "Installed polkit policy file to $POLICY_DEST_PATH"
else
    echo "Warning: Polkit policy file $POLICY_SRC_PATH not found!"
fi

echo "Post-installation complete."

# Optional: Add sudoers entry (uncomment if you want to allow sudo without password)
# echo "%wheel ALL=(ALL) NOPASSWD: /usr/bin/sv" >> /etc/sudoers
