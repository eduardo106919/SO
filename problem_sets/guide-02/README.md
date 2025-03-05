# Debugging processes

Chose whether to follow the **parent** or **child** process:
```bash
>> set follow-fork-mode child
>> set follow-fork-mode parent
```

Chose whether to **follow** a process after it ends:
```bash
>> set detach-on-fork mode on
>> set detach-on-fork mode off
```

**Navigate** through the processes:
```bash
>> inferior 1
>> inferior 2
>> ...
```
