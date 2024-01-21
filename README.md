# Warehouse

first project spl course

by Clil Argas & Gilad Gochman

How to add a remote worker:

1. git clone <url> // in the folder
2. git config --global user.name \<user>
3. git config --global user.email \<email>
4. press the branch icon in vscode
5. write message
6. press commit & push
7. you can set a keboard shortcut to perform this, for example shift+s to sync

Having conflicts while syncing? No problem!

1. git config pull.rebase false // to merge
2. Now a 'merge' tab will open in the 'Source control' section (Ctrl+Shift+G G)
3. Setlle the conflict after viewing the git output
   for example:
   git rm warehouse # to remove the file from the commit

Valgrind usage:

We will use the following Valgrind command:
valgrind --leak-check=full --show-reachable=yes ./warehouse "./configFileExample.txt"
The expected Valgrind output is:
All heap blocks were freed -- no leaks are possible
ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
•
Compiler commands must include the following flags:
-g -Wall -Weffc++ -std=c++11 -Iinclude
