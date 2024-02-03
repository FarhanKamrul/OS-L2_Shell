# Basic Shell Implementation

This is a basic implementation of a shell, similar to Windows Subsystem for Linux (WSL) but simpler.

**Note**: This has been made for my Operating Systems course; please use your own code especially if you are working on school assignments.

## Implementation Rules:

- Use the basic API for most `exec()` system calls: `exec(const char *pathname, char *const argv[])`
- Assume a max size of **20** for the argument vector.
- Parse the arguments to place them in the vector: `ls -a -l` is to be parsed into three parts, the first being the command. That is, `argv[0]` must be the command, and it must end in `NULL`. For example, if I am running the command `/bin/ls -l -a` then `argv` will be equal to `{"/bin/ls", "-l", "-a", NULL}`
- Spawn a child process to execute the command. The shell will either wait or not, depending on whether the user wants to push the process to the background.


### For absolute and relative paths:

- If the command starts with a `/` character, it is an absolute path to the binary.
- If it doesn't, it's a relative path and should be run in the `/bin/` directory.
