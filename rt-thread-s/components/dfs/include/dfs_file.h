/*
+------------------------------------------------------------------------------
| Project   : Device Filesystem
+------------------------------------------------------------------------------
| Copyright 2004, 2005  www.fayfayspace.org.
| All rights reserved.
|------------------------------------------------------------------------------
| File      : dfs_raw.h, the raw APIs of Device FileSystem
|------------------------------------------------------------------------------
| Chang Logs:
| Date           Author       Notes
| 2005-01-26     ffxz		  The first version
+------------------------------------------------------------------------------
*/

#ifndef __DFS_RAW_H__
#define __DFS_RAW_H__

#include <dfs_def.h>
#include <dfs.h>
#include <dfs_fs.h>

int dfs_file_open(struct dfs_fd* fd, const char *path, int flags);
int dfs_file_close(struct dfs_fd* fd);
int dfs_file_ioctl(struct dfs_fd* fd, int cmd, void *args);
int dfs_file_read(struct dfs_fd* fd, void *buf, rt_size_t len);
int dfs_file_getdents(struct dfs_fd* fd, struct dirent* dirp, rt_size_t nbytes);
int dfs_file_unlink(const char *path);
int dfs_file_write(struct dfs_fd* fd, const void *buf, rt_size_t len);
int dfs_file_lseek(struct dfs_fd* fd, rt_off_t offset);
int dfs_file_stat(const char *path, struct stat *buf);
int dfs_file_rename(const char* oldpath, const char* newpath);

#endif

