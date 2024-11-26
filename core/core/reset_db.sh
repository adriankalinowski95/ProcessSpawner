#!/bin/bash

echo "Removing files from migrations"
rm -rf Migrations/*

echo "Db remove"
yes | dotnet ef database drop

echo "Migration InitialCreate..."
dotnet ef migrations add InitialCreate

echo "Db update"
dotnet ef database update

echo "Db upate finish!"