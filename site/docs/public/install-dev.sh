#!/bin/bash
# Development version of install script for testing
# Points to localhost for development

set -euo pipefail

echo "⚠️  Development installer - using localhost URLs"
echo "For production, use: curl -fsSL https://zeno.run/install.sh | bash"
echo

# Use local development URLs
ZENOSCRIPT_SITE="http://localhost:5173"

# Rest of the script would be the same as install.sh
# but pointing to development URLs for testing

echo "This is a development script stub."
echo "In production, this would install from: $ZENOSCRIPT_SITE"