#!/usr/bin/env bash

set -e

echo "=== Step 1. Install (npm install) ==="
npm install

echo "=== Step 2. Build angular project ==="
ng build
