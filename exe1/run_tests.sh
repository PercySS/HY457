#!/bin/bash

echo "=== HY457 - Run All Tests ==="

echo ""
echo "--- Building and Running monitor/ ---"
cd monitor
make clean
make
if [ -f monitor ]; then
    echo "Running monitor with test_pha..."
    ./monitor ./test_pha.out
else
    echo "  [!] Monitor build failed!"
fi
cd ..

echo ""
echo "--- Building and Running analyze/ ---"
cd analyze
make clean
make
if [ -f analyze ]; then
    echo "Running analyze with test_traffic.pcap..."
    ./analyze ./test_traffic.pcap 192.168.1.10
else
    echo "  [!] Analyze build failed!"
fi
cd ..

echo ""
echo "--- Building and Running vault/ ---"
cd vault
make clean
make
if [ -f vault ]; then
    echo "Running vault split..."
    ./vault split 9
    echo "Running vault join with example points..."
    ./vault join "(1,16)" "(2,27)" "(3,42)"
else
    echo "  [!] Vault build failed!"
fi
cd ..

echo ""
echo "=== Done ==="
