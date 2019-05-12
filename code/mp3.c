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
	a=a-1;
	status=1;
		}
	}
}

void* next(void *arg){
	while(1){printf("\r");
        if(status==5){
        stop =1;
        a=a+1;
	status=1;
        }
      }
}

void* keluar(void *arg){
        while(1){printf("\r");
	if(status == 6){
	pthread_join(tid1, SIGKILL);
    	pthread_join(tid2, SIGKILL);
    	pthread_join(tid3, SIGKILL);
    	pthread_join(tid4, SIGKILL);
    	pthread_join(tid5, SIGKILL);
    	pthread_join(tid6, SIGKILL);

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
