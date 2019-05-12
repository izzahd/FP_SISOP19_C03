# FP_SISOP19_C03

05111740000044 - Izzah Dinillah

05111740000070 - Raja Permata Boy Mangatur Simarmata


Buatlah sebuah music player dengan bahasa C yang memiliki fitur play nama_lagu, pause, next, prev, list lagu. Selain music player juga terdapat FUSE untuk mengumpulkan semua jenis file yang berekstensi .mp3 kedalam FUSE yang tersebar pada direktori /home/user. Ketika FUSE dijalankan, direktori hasil FUSE hanya berisi file .mp3 tanpa ada direktori lain di dalamnya. Asal file tersebut bisa tersebar dari berbagai folder dan subfolder. program mp3 mengarah ke FUSE untuk memutar musik.


* Program mp3:
```
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <ao/ao.h>
#include <mpg123.h>
#include <dirent.h>
#include<signal.h>
#define BITS 8

pthread_t tid1, tid2, tid3, tid4, tid5, tid6 ;

int i=0;
int total=0;
int status=0;
int nomor;
int hold=0;
char listlagu[1000][1000];
int a=1;
int stop=0;

int play(char song[])
{
    mpg123_handle *mh;
    unsigned char *buffer;
    size_t buffer_size;
    size_t done;
    int err;

    int driver;
    ao_device *dev;

    ao_sample_format format;
    int channels, encoding;
    long rate;

    /* initializations */
    ao_initialize();
    driver = ao_default_driver_id();
    mpg123_init();
    mh = mpg123_new(NULL, &err);
    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

    /* open the file and get the decoding format */
    mpg123_open(mh, song);
    mpg123_getformat(mh, &rate, &channels, &encoding);

    /* set the output format and open the output device */
    format.bits = mpg123_encsize(encoding) * BITS;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    dev = ao_open_live(driver, &format, NULL);

    /* decode and play */
    while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK){
      printf("\r");
     while(status==2){
    printf("\r");
    if(status == 3) break;
    }
	if(stop == 1) break;
        ao_play(dev, buffer, done);
    }
    /* clean up */
    free(buffer);
    ao_close(dev);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    ao_shutdown();

    return 0;
}

void* playlist(void *arg)
 {
    DIR *d;
    char songpath[100] = "/home/boy/laguku";
    struct dirent *dir;
    d = opendir(songpath);
    if (d == NULL)
    {
    return 0;
    }
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
{
printf("\r");
char filename[100];
strcpy(filename, dir->d_name);
int panjang = strlen(filename);
int y=0;
    if(filename[panjang-1] == '3' && filename[panjang-2] == 'p' && filename[panjang-3] == 'm' && filename[panjang-4] == '.')
    {
    i++;
    total++;
    strcpy(listlagu[i],dir->d_name);
    printf("%d. %s\n",i, listlagu[i]);
	}
}


        closedir(d);
    }
    hold=1;
}


void* show(void *arg)
{
//while(hold!=1){printf("\r");}

while(1){
    printf("\r");
    printf(" 1.Play\n 2.Pause\n 3.Resume\n 4.Previous\n 5.Next\n 6.Exit\n");
    scanf("%d", &status);
    }
}

void* playmusic(void *arg)
{
   while(1){
    printf("\r");
    while(status == 1){
        printf("\r");
        char sicko[100] = "/home/boy/laguku/";
	strcat(sicko, listlagu[a]);
	printf("Now Playing : %s\n", listlagu[a]);
	stop = 0;
        play(sicko);
        }
    }
}


void* previous(void *arg){
	while(1){printf("\r");
	if(status==4){
	stop =1;
	if(a>1){a=a-1;}
	else a=total;
	status=1;
		}
	}
}

void* next(void *arg){
	while(1){printf("\r");
        if(status==5){
        stop =1;
	if(a<total){a=a+1;}
	else a=1;
	status=1;
        }
      }
}

void* keluar(void *arg){
        while(1){printf("\r");
	if(status == 6){
	pthread_kill(tid1, SIGKILL);
    	pthread_kill(tid2, SIGKILL);
    	pthread_kill(tid3, SIGKILL);
    	pthread_kill(tid4, SIGKILL);
    	pthread_kill(tid5, SIGKILL);
    	pthread_kill(tid6, SIGKILL);

		}
	}
	}
int main(void)
{

    pthread_create(&(tid1), NULL, show, NULL);
    pthread_create(&(tid2), NULL, playmusic, NULL);
    pthread_create(&(tid3), NULL, playlist, NULL);
    pthread_create(&(tid4), NULL, previous, NULL);
    pthread_create(&(tid5), NULL, next, NULL);
    pthread_create(&(tid6), NULL, keluar, NULL);


    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);
    pthread_join(tid5, NULL);
    pthread_join(tid6, NULL);

    return 0;
}

```

Penjelasan:

Kita membuat fitur2 MP3 yang terdiri dari Play, Resume, Pause, Previous, Next dan Exit. Setiap fitur yang ada perlu dibuat threadnya. MP3 ini akan mengambil lagu dari sebuah file dan memasukkannya ke dalam satu list. Lalu, MP3 ini bisa memainkan lagu serta memutar lagu.
Step by step :
1. Buat list lagu di dalam folder
2. Masukkan lagu yang ada di dalam array 2D agar bisa memainkan lagu saat memanggil fungsinya
3. Akan ada menu dan pilihan yang bisa dibuat
4. Membuat fitur resume dan pause
5. Membuat fitur previous, next dan exit.


* Fuse:
```
#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

static const char *dirpath = "/home/izzah/Music";

static int xmp_getattr(const char *path, struct stat *stbuf)
{
  int res;
	char fpath[1000];
	sprintf(fpath,"%s%s",dirpath,path);
	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
  char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);
	int res = 0;

	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		res = (filler(buf, de->d_name, &st, 0));
			if(res!=0) break;
	}

	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
  char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);
	int res = 0;
  int fd = 0 ;

	(void) fi;
	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
```
