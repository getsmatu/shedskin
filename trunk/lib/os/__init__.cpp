#include "__init__.hpp"
#include "path.hpp"

#include <cstdlib>
#include <sstream>
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>

namespace std {
#include <unistd.h>
}

#ifdef WIN32
#include <windows.h>
#endif

#ifdef OSX
#include <crt_externs.h>
#define environ (*_NSGetEnviron())
#endif

namespace __os__ {

str *const_0;
str *linesep, *name;
dict<str *, str *> *__ss_environ;
struct stat sbuf;

str *altsep, *curdir, *defpath, *devnull, *extsep, *pardir, *pathsep, *sep;

list<str *> *listdir(str *path) {
    list<str *> *r = new list<str *>();
    DIR *dp;
    struct dirent *ep;
    int count = 0;

    dp = opendir(path->unit.c_str());

    while (ep = readdir(dp))
        if(strcmp(ep->d_name, ".") && strcmp(ep->d_name, ".."))
            r->append(new str(ep->d_name));

    closedir (dp);
    return r;
}

str *getcwd() { 
    str *r;
    char *d=::getcwd(0, 0);
    r = new str(d);
    free(d);
    return r;
}

int chdir(str *dir) { 
    if(::chdir(dir->unit.c_str()) == -1)
        throw new OSError(dir);
}

str *strerror(int i) {
    return new str(::strerror(i));
}

#ifndef WIN32
str *readlink(str *path) {
    int size = 255;
    str *r;

    while (1)
      {
        char *buffer = (char *) GC_malloc (size);
	    int nchars = ::readlink(path->unit.c_str(), buffer, size);
    	if (nchars == -1) {
            throw new OSError(path);
  	    }
        if (nchars < size) {
            buffer[nchars] = '\0';
            r = new str(buffer);
            return r;
        }
        size *= 2;
      }       
}

int getuid() { return ::getuid(); }
int getgid() { return ::getgid(); }

int fork() {
    int ret;
    if ((ret = ::fork()) == -1)
        throw new OSError();
    return ret;
}

int chown(str *path, int uid, int gid) {
    if (::chown(path->unit.c_str(), uid, gid) == -1) 
        throw new OSError(path);
}
#endif

int system(str *c) {
    return std::system(c->unit.c_str());
}

str *getenv(str *name, str *alternative) {
    const char *waba = name->unit.c_str();
    if(std::getenv(waba))
        return new str(std::getenv(waba));
    return alternative;
}

int rename(str *a, str *b) {
    if(std::rename(a->unit.c_str(), b->unit.c_str()) == -1)
        throw new OSError(a);
}

int remove(str *path) {
    if(std::remove(path->unit.c_str()) == -1)
        throw new OSError(path);
}

int rmdir(str *a) {
    if (::rmdir(a->unit.c_str()) == -1)
        throw new OSError(a);
}

int removedirs(str *name) {
    tuple2<str *, str *> *__0, *__1, *__5;
    str *__2, *__3, *head, *tail;

    rmdir(name);
    __0 = __path__::split(name);
    head = __0->__getfirst__();
    tail = __0->__getsecond__();
    if ((!__bool(tail))) {
        __1 = __path__::split(head);
        head = __1->__getfirst__();
        tail = __1->__getsecond__();
    }

    while(__bool(__AND(head, tail, 2))) {
        try {
            rmdir(head);
        } catch (OSError *) {
            break;
        }
        __5 = __path__::split(head);
        head = __5->__getfirst__();
        tail = __5->__getsecond__();
    }
    return 0;
}


int mkdir(str *path, int mode) {
#ifdef WIN32
    if (::mkdir(path->unit.c_str()) == -1)
#else
    if (::mkdir(path->unit.c_str(), mode) == -1)
#endif
        throw new OSError(path);
}

int makedirs(str *name, int mode) {
    tuple2<str *, str *> *__0, *__1;
    str *head, *tail;
    int __2, __3, __4;

    __0 = __path__::split(name);
    head = __0->__getfirst__();
    tail = __0->__getsecond__();
    if ((!__bool(tail))) {
        __1 = __path__::split(head);
        head = __1->__getfirst__();
        tail = __1->__getsecond__();
    }
    if ((__bool(head) && __bool(tail) && (!__path__::exists(head)))) {
        try {
            makedirs(head, mode);
        } catch (OSError *e) {
            if (e->__ss_errno != EEXIST) {
                throw (e);
            }
        }
        if (__eq(tail, __path__::curdir)) {
            return 0;
        }
    }
    mkdir(name, mode);
    return 0;
}

int abort() {
    std::abort();
}

/* class __cstat */

class_ *cl___cstat;

__cstat::__cstat(str *path, int t) {
    int hop;
    this->__class__ = cl___cstat;
    
    if(t==1) {
        if(stat(path->unit.c_str(), &sbuf) == -1)
            throw new OSError(path);
    } else if (t==2) {
#ifndef WIN32
        if(lstat(path->unit.c_str(), &sbuf) == -1)
#endif
            throw new OSError(path);
    }
 
    fill_er_up();
}
    
__cstat::__cstat(int fd) {
    int hop;
    this->__class__ = cl___cstat;

    if(fstat(fd, &sbuf) == -1)
        throw new OSError();

    fill_er_up();
}

void __cstat::fill_er_up() {
    this->st_mode = sbuf.st_mode;
    this->st_ino = sbuf.st_ino;
    this->st_dev = sbuf.st_dev;
    this->st_rdev = sbuf.st_rdev;
    this->st_nlink = sbuf.st_nlink;
    this->__ss_st_atime = sbuf.st_atime;
    this->__ss_st_mtime = sbuf.st_mtime;
    this->__ss_st_ctime = sbuf.st_ctime;
    this->st_uid = sbuf.st_uid;
    this->st_gid = sbuf.st_gid;
    this->st_size = sbuf.st_size;
#ifndef WIN32
    this->st_blksize = sbuf.st_blksize;
    this->st_blocks = sbuf.st_blocks;
#endif
}

str *__cstat::__repr__() {
    return __mod(const_0, this->st_mode, this->st_ino, this->st_dev, this->st_nlink, this->st_uid, this->st_gid, this->st_size, this->__ss_st_mtime, this->__ss_st_atime, this->__ss_st_ctime);
}

int __cstat::__len__() {
    return 10;
}

int __cstat::__getitem__(int i) {
    i = __wrap(this, i);
    switch(i) {
        case 0: return st_mode;
        case 1: return st_ino;
        case 2: return st_dev;
        case 3: return st_nlink;
        case 4: return st_uid;
        case 5: return st_gid;
        case 6: return st_size;
        case 7: return __ss_st_atime;
        case 8: return __ss_st_mtime;
        case 9: return __ss_st_ctime;

        default:  
            throw new IndexError(new str("tuple index out of range")); 
    } 

    return 0;
}

tuple2<int, int> *__cstat::__slice__(int x, int l, int u, int s) {
    tuple2<int, int> *c = new tuple2<int, int>();

    slicenr(x, l, u, s, 10);
 
    if(s > 0)
        for(int i=l; i<u; i += s)
            c->append(__getitem__(i));
    else
        for(int i=l; i>u; i += s)
            c->append(__getitem__(i));

    return c;
}

__cstat *stat(str *path) {
    return new __cstat(path, 1);
}
__cstat *lstat(str *path) {
#ifndef WIN32
    return new __cstat(path, 2);
#else
    return new __cstat(path, 1);
#endif
}
__cstat *fstat(int fd) {
    return new __cstat(fd);
}

int stat_float_times(int newvalue) {
    if(newvalue==0)
        throw new TypeError(new str("os.stat_float_times: cannot change type"));
    return 1;
}

int getpid() {
    return ::getpid();
}

int putenv(str* varname, str* value) {
    std::stringstream ss;
    ss << varname->unit.c_str() << '=' << value->unit.c_str();
    return ::putenv(const_cast<char*>(ss.str().c_str()));
}

int umask(int newmask)  {
    return ::umask(newmask);
}

int unsetenv (str* var) {
    ::unsetenv(var->unit.c_str());
    return 0;
}

int chmod (str* path, int val) {
#ifdef WIN32
    DWORD attr;
    int res;
    attr = GetFileAttributesA(var->unit.c_str());

    if (attr != 0xFFFFFFFF) {
        if (i & S_IWRITE)
            attr &= ~FILE_ATTRIBUTE_READONLY;
        else
            attr |= FILE_ATTRIBUTE_READONLY;
        res = SetFileAttributesA(var->unit.c_str(), attr);
    }
    else {
        res = 0;
    }
    if(!res) {
        throw new OSError("Chmod");
    }
    return 0;
#else
    return ::chmod(path->unit.c_str(), val);
#endif
}

int renames (str* old, str* _new) {
    tuple2<str *, str *> *__0, *__1, *__5;
    str *__2, *__3, *head, *tail;


    __0 = __path__::split(_new);
    head = __0->__getfirst__();
    tail = __0->__getsecond__();

    if ((!__bool(tail))) {
        __1 = __path__::split(head);
        head = __1->__getfirst__();
        tail = __1->__getsecond__();
    }
    
    while(__bool(__AND(head, tail, 2)) && !__path__::exists(head)) {
        try {
            makedirs(head);
        } catch (OSError *) {
            break;
        }
        __5 = __path__::split(head);
        head = __5->__getfirst__();
        tail = __5->__getsecond__();
    }
    rename(old, _new);

    __0 = __path__::split(old);
    head = __0->__getfirst__();
    tail = __0->__getsecond__();

    if ((!__bool(tail))) {
        __1 = __path__::split(head);
        head = __1->__getfirst__();
        tail = __1->__getsecond__();
    }

    if(__bool(__AND(head,tail,2))) {
        removedirs(head);
    }
}

popen_pipe* popen(str* cmd) {
    return popen(cmd, new str("r"), -1);
}

popen_pipe* popen(str* cmd, str* mode) {
    return popen(cmd, mode, -1);
}

popen_pipe* popen(str* cmd, str* mode, int bufsize) {
    FILE* fp = ::popen(cmd->unit.c_str(), mode->unit.c_str());

    if(!fp) throw new OSError(cmd);
    return new popen_pipe(fp);
}

popen_pipe::popen_pipe(str *cmd, str *mode) {
    FILE* fp;

    if(!mode)
        mode = new str("r");
    fp = ::popen(cmd->unit.c_str(), mode->unit.c_str());
    this->name = cmd;
    this->mode = mode;

    endoffile=print_space=0;
    print_lastchar='\n';
}

popen_pipe::popen_pipe(FILE* pipe) {
    f = pipe;
    endoffile=print_space=0;
    print_lastchar='\n';
}

int popen_pipe::close() {
    pclose(f);
    closed = 1;
    return 0;
}

void __init() {
    const_0 = new str("(%d, %d, %d, %d, %d, %d, %d, %d, %d, %d)");

    cl___cstat = new class_("__cstat", 4, 4);

    linesep = new str("\n");
#ifdef WIN32
    name = new str("nt");
#else
    name = new str("posix");
#endif
    
    __ss_environ = new dict<str *, str *>();

    for (int n = 0; environ[n]; n++) {
        str *line = new str(environ[n]);
        int pos = line->find(new str("="));
        __ss_environ->__setitem__(line->__slice__(2, 0, pos, 0), line->__slice__(1, (pos+1), 0, 0));
    }

    __path__::__init(); /* ugh */

    curdir = __path__::curdir;
    pardir = __path__::pardir;
    extsep = __path__::extsep;
    sep = __path__::sep;
    pathsep = __path__::pathsep;
    defpath = __path__::defpath;
    altsep = __path__::altsep;
    devnull = __path__::devnull;

}

} // module namespace

