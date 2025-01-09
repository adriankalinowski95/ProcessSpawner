#!/usr/bin/env bash

SOLUTION_FILE="./core.sln"
MAIN_PROJECT="./core/core.csproj"

echo "=== Step 1: Restore references (.NET restore) ==="
dotnet restore

echo "=== Step 2: Build whole solution Release (.NET build) ==="
dotnet build --configuration Release

echo "=== Step 3: Publish app (self-contained) ==="
dotnet publish

echo "=== Step 4: Create db ==="
(
	cd "core"
	"./reset_db.sh"
)