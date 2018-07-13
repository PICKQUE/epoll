# epoll
```
gcc epoll.c
mkdir tmp
cd tmp
mkfifo 1 2 3
cd ..
./a.out tmp/1 tmp/2 tmp/3 &
echo hello > tmp/1
Reason: 0x1
get data: hello

echo hello > tmp/2
Reason: 0x1
get data: hello

echo hello > tmp/3
Reason: 0x1
get data: hello
```
