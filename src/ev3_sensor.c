/**ev3_sensor.c here there are all functions' bodies
**author:soylisK
**/



#include "../include/ev3_sensor.h"


/****-----------------|functions|------------------------------------------------------------------------**/
int8_t  countSensors()
{
  int8_t file_count = 0;
  DIR *d;
  struct dirent *dir;
  /**sensor_driver_path is define in header(.h) file**/
  d = opendir(sensor_driver_path);
  if (d)
  {


     while ((dir = readdir(d)) != NULL)
     {
	      if (strncmp(dir->d_name, filesName ,strlen(filesName)) == 0)  //filesName is define in (.h)
	         file_count++;
      }
      closedir(d);
      //printf("rats#files=%d\n",count);
   }

    return file_count;

}

char *chpath(char *init_path,int8_t length_dn,int8_t lenght_up,char *str_add)
{
    int8_t str_size = strlen(init_path)-length_dn+lenght_up+1;//set the right size of string
    char *strR =(char*)malloc(str_size);
    strcpy(strR,init_path);
    strcat(strR,str_add);

    return strR;
}

sensor *load_sensor(uint8_t fn)
{
    sensor *sensor_node=(sensor*)malloc(sizeof(sensor));
		if(sensor_node==NULL)
			printf("error memory allocation..load_sensor");

    int8_t name_s=strlen(fullpath)+2;
    char *sensor_name=(char*)malloc(name_s);
    char *itoa=(char*)malloc(sizeof(int8_t));
    sprintf(itoa,"%d",fn);          //int to string
    strcpy(sensor_name,fullpath);  //copy fullpath string to a new variable
    strcat(sensor_name,itoa);     //concat fullpath with sensor name
    char *filep=NULL;
    /*read driver_name*/
    filep=chpath(sensor_name,0,strlen(driverf),driverf);
    sensor_node->driver =readData(filep);
    /*set_identifier*/
    /*filep=chpath(sensor_name,stlen(driverf),strlen(),);
    sensor_node->id=get_sensor_id(readData(filep));*/
    /*set port*/
    filep=chpath(sensor_name,strlen(sensor_node->driver),strlen(portf),portf);
    sensor_node->port=readData(filep);
    /*set fd*/
		filep=chpath(sensor_name,strlen(portf),strlen(dataf),dataf);
		sensor_node->fd=filep;
    /*set data*/
    filep=chpath(sensor_name,strlen(portf),strlen(dataf),dataf);
    sensor_node->data=atoi(readData(filep));
    /*set poll time*/
    filep=chpath(sensor_name,strlen(dataf),strlen(pollf),pollf);
    sensor_node->poll_time=atoi(readData(filep));
    /*set mode*/
    filep=chpath(sensor_name,strlen(pollf),strlen(modef),modef);
    sensor_node->mode= readData(filep);

    /*set next node's pointer*/
    sensor_node->next=NULL; //init state

		return sensor_node;

}

void append2list(sensor **slist)
{
		numbOfSen=countSensors(); //number of coonected sensors
		uint8_t i;
		for(i=0;i<numbOfSen;i++)
		{
				sensor *tmp=NULL,*node=NULL;
				// sensor *node=(sensor*)malloc(sizeof(sensor)); //alocate mem for a sensor node
				node=load_sensor(i);
				if(i==0)
				{
						(*slist)=node;	//head of list
						tmp=node;
				}
				else
				{
						tmp->next=node;
						tmp=node;
				}
		}
}

void update_sensor_value(sensor *s)
{
		s->data=atoi(readData(s->fd));
}

sensor *search4sensor(sensor *slist,char *port_name)
{
		sensor *node=NULL,*tmp=slist;
		uint8_t i;
		for( i=0;i<numbOfSen;i++)
		{
				if(strcmp(tmp->port,port_name)==0 )
				{
						node=tmp;
						break;
				}
				else
						tmp=tmp->next;
		}
		return node;
}

void deleteAsensor(sensor *slist,sensor *s)
{
		sensor *tmp1,*tmp2;
		tmp1=tmp2=slist; //set tmp1,2 to the head of list
		while( (tmp2!=NULL) ) //look in list until hit the node
		{
				if((tmp2->next)==s)
				{
						tmp1=tmp2;
						tmp2=tmp2->next;
						tmp1->next=tmp2;
						free(s);
						break;
				}
				else
						tmp2=tmp2->next;

		}
}


void removeAll(sensor *slist)
{
		sensor *tmp=slist;
		uint8_t i;
		for( i=0;i<numbOfSen;i++)
		{
				tmp=tmp->next;
				free(slist);
				slist=tmp;
		}
}
