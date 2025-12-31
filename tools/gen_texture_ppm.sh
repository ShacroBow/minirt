#!/bin/sh
# Generate a small 4x2 P6 texture for testing
cat > tools/texture_test.ppm <<'EOF'
P6
4 2
255
EOF
printf '\xFF\x00\x00\x00\xFF\x00\x00\x00\xFF\xFF\xFF\xFF\x00\x00\x00\x80\x80\x80\xFF\x00\xFF\xFF\xFF\x00' >> tools/texture_test.ppm
chmod 644 tools/texture_test.ppm
