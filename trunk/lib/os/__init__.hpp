#ifndef __OS_HPP
#define __OS_HPP

#include "builtin.hpp"

using namespace __shedskin__;
namespace __os__ {

extern str *linesep, *name;
extern dict<str *, str *> *__ss_environ;
extern str *altsep, *curdir, *defpath, *devnull, *extsep, *pardir, *pathsep, *sep;

typedef OSError error;

class popen_pipe;
class __cstat;

list<str *> *listdir(str *path);
str *getcwd();
int chdir(str *dir);
str *getenv(str *name, str *alternative=0);
int rename(str *a, str *b);
int remove(str *a);
int rmdir(str *a);
int removedirs(str *name);
int mkdir(str *path, int mode=0777);
int makedirs(str *name, int mode=0777);
int abort();
int system(str *c);

extern class_ *cl___cstat;
class __cstat : public pyobj {
public:
    int st_mode, st_ino, st_dev, st_rdev, st_nlink, st_uid, st_gid, st_size, st_blksize, st_blocks;
    int __ss_st_mtime, __ss_st_atime, __ss_st_ctime;

    __cstat(str *path, int t);
    __cstat(int fd);
    void fill_er_up();

    int __len__();
    int __getitem__(int i);
    tuple2<int, int> *__slice__(int x, int l, int u, int s);

    str *__repr__();
};

__cstat *stat(str *path);
__cstat *lstat(str *path);
__cstat *fstat(int fd);

#ifndef WIN32
str *readlink(str *path);

int getuid();
int getgid();
int chown(str *path, int uid, int gid);

int fork();
#endif

int stat_float_times(int newvalue=-1);
str *strerror(int i);

int getpid();
int putenv(str* varname, str* value);
int umask(int newmask);
int unsetenv(str* var);
int chmod(str* path, int val);
int renames(str* old, str* _new);

class popen_pipe : public file {
public:
    popen_pipe(str *name, str *mode=0);
    popen_pipe(FILE* pipe);
    int close();
};

popen_pipe* popen(str* cmd);
popen_pipe* popen(str* cmd, str* mode);
popen_pipe* popen(str* cmd, str* mode, int bufsize);

void __init();

} // module namespace
#endif
