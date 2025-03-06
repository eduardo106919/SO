# Debugging processes

Debugging information about processes in [gdb](https://sourceware.org/gdb/current/onlinedocs/gdb.pdf).

Chose whether to follow the **parent** or **child** process:
```bash
>> set follow-fork-mode child
>> set follow-fork-mode parent
```

Chose whether to **follow** a process after it ends:
```bash
>> set detach-on-fork on
>> set detach-on-fork off
```

**Navigate** through the processes:
```bash
>> inferior 1
>> inferior 2
>> ...
```
