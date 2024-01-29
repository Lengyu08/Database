# 如果执行成功执行二进制文件

clang++ .\src\trie.cpp -o .\src\trie.exe -std=c++17 -g
# .\src\trie.exe

if ($LASTEXITCODE -eq 0) {
    .\src\trie.exe
}