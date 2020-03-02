/*  Implementation of the script was discussed on a higher level with 
	Prathamesh Deshpande, Aravind Parappil and Manjeet Kumar Pandey 

	References:
	(1) https://stackoverflow.com/questions/62814/difference-between-binary-semaphore-and-mutex
	(2) https://en.wikipedia.org/wiki/Producer%E2%80%93consumer_problem
*/
#include <xinu.h>
#include <tscdf-input1.h>
#include <stdlib.h>
#include <stddef.h>
#include <future.h>	

int32 num_streams = -1, work_queue = -1, time_window = -1, output_time = -1;
sid32 mutex;

struct stream_struct{
	int32 time, value;
	future **fut_array;
};

struct stream_struct stream_element[20];

// Phase 2
void queue_consumer(struct stream_struct obj[], int32 id) {
	int i, j, count = 0;
	int32 *qarray;
	wait(mutex);

	struct tscdf* tc = tscdf_init(time_window);

	for(j = 0; j < work_queue; j++){

		tscdf_update(tc,obj[j].time,obj[j].value);
		count++;

		if(count == output_time){
			qarray = tscdf_quartiles(tc);
			if(qarray == NULL)
				kprintf("tscdf_quartiles_returned NULL\n");
			else
			{
				for(i = 0; i < 5; i++)
					kprintf("%d ",qarray[i]);

				kprintf("\n");
				freemem((char *)qarray,(6*sizeof(int32)));
			}
		}

	}	
	signal(mutex);
}


//Producer Function
int stream_proc( int32 nargs, char* args[]) {
	mutex = semcreate(1);
    char usage[] = "Usage: -s num_streams -w work_queue -t time_window -o output_time\n", c;
    char* ch;
    int32 i, j;
    if ((nargs % 2) != 0) {
      printf("%s", usage);
      return(-1);
    }
    else {
      
     while (nargs > 0) {
        ch = args[nargs-1];
        c = *(++ch);
	
        switch(c) {
        case 's':
          num_streams = atoi(args[nargs]);
          break;

        case 'w':
          work_queue = atoi(args[nargs]);
          break;

        case 't':
          time_window = atoi(args[nargs]);
          break;

        case 'o':
          output_time = atoi(args[nargs]);
          break;

        default:
          printf("%s", usage);
          return(-1);
        }

        nargs -= 2;
      }
    }


int x, y, z = 0, count = 0;
int32 stream_done[5];
for(i = 0; i < 5; i++){
	stream_done[i] =- 1;
}

//Fills the stream with time and values according to the stream ids.

for(i = 0; i < n_input; i++){
	y = 0;
	char *a = (char *)stream_input[i];
	int sid1 = atoi(a);
	int is_exist;
//Following for loop compares unique stream ids.
	for(i = 0; i < 5; i++){
		if(stream_done[i] == sid1){
			is_exist = 1;
		}
		else{
			is_exist = 0;
		}
	}

	if(is_exist == 0 && count < num_streams){
		count++;
		for(j = i; j < n_input; j++){
			char *b = (char *)stream_input[j];
			int sid2 = atoi(b);

			  while (*b++ != '\t');
			  	int ts = atoi(b);
			  while (*b++ != '\t');
	  		  	int v = atoi(b);

			if(sid2 == sid1 && y < work_queue){
				stream_element[y].time = ts;
				stream_element[y].value = v;
				y = y + 1;
			}	
		}
		stream_done[z] = sid1;

		z = z + 1;
		resume (create((void *) queue_consumer, 4096, 20, "queue_consumer", 2, stream_element, sid1));
		for(x = 0; x < 20; x++){
			stream_element[x].time = NULL;
			stream_element[x].value = NULL;
		}	
		
		
	}

	}
	    return 0;
}


//Future Producer Function
int stream_proc_future( int32 nargs, char* args[]) {
    char usage[] = "Usage: -s num_streams -w work_queue -t time_window -o output_time\n", c;
    char* ch;
    int32 i, j;
    if ((nargs % 2) != 0) {
      printf("%s", usage);
      return(-1);
    }
    else {
      
     while (nargs > 0) {
        ch = args[nargs-1];
        c = *(++ch);
	
        switch(c) {
        case 's':
          num_streams = atoi(args[nargs]);
          break;

        case 'w':
          work_queue = atoi(args[nargs]);
          break;

        case 't':
          time_window = atoi(args[nargs]);
          break;

        case 'o':
          output_time = atoi(args[nargs]);
          break;

        default:
          printf("%s", usage);
          return(-1);
        }

        nargs -= 2;
      }
    }


int x, y, z = 0, count = 0;
int32 stream_done[5];
for(i = 0; i < 5; i++){
	stream_done[i] =- 1;
}

//Fills the stream with time and values according to the stream ids.

for(i = 0; i < n_input; i++){
	y = 0;
	char *a = (char *)stream_input[i];
	int sid1 = atoi(a);
	int is_exist;
//Following for loop compares unique stream ids.
	for(i = 0; i < 5; i++){
		if(stream_done[i] == sid1){
			is_exist = 1;
		}
		else{
			is_exist = 0;
		}
	}

	if(is_exist == 0 && count < num_streams){
		count++;
		for(j = i; j < n_input; j++){
			char *b = (char *)stream_input[j];
			int sid2 = atoi(b);

			  while (*b++ != '\t');
			  	int ts = atoi(b);
			  while (*b++ != '\t');
	  		  	int v = atoi(b);

			if(sid2 == sid1 && y < work_queue){

				struct stream_struct* data = (struct stream_struct*)getmem(sizeof(struct stream_struct));
				data->time = ts;
				data->value = v;

				while ((future_set(stream_element[y].fut_array, (char *) data)) == SYSERR);
				y = y + 1;
			}	
		}
		stream_done[z] = sid1;

		z = z + 1;
		
	}

	}
	    return 0;
}

//Future Consumer
void future_consumer(struct stream_struct obj[], int32 id) {
	int i, j, count = 0;
	int32 *qarray;
	wait(mutex);

	struct tscdf* tc = tscdf_init(time_window);

	for(j = 0; j < work_queue; j++){

		tscdf_update(tc,obj[j].time,obj[j].value);
		count++;

		if(count == output_time){
			qarray = tscdf_quartiles(tc);
			if(qarray == NULL)
				kprintf("tscdf_quartiles_returned NULL\n");
			else
			{
				for(i = 0; i < 5; i++)
					kprintf("%d ",qarray[i]);

				kprintf("\n");
				freemem((char *)qarray,(6*sizeof(int32)));
			}
		}

	}	
	signal(mutex);
}
