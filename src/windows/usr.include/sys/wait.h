#pragma once

#define WEXITSTATUS(x) 0
#define WIFEXITED(x) 0
#define WIFSIGNALED(x) 0
#define WTERMSIG(x) 0

#define WNOHANG 0

#define waitpid(x, y, z) 0
